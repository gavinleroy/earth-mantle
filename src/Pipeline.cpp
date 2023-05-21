#include "Pipeline.h"

namespace MantleIO {

    // TODO: one performance bottleneck is reading. We need to make the reader
    // happen on a background worker so we don't block the window interaction
    // while something is getting rendered.
    Mantle::Mantle()
    {
        // XXX: only initialize global reader once.
        if (globalReader != nullptr)
            return;

        globalReader = vtkSmartPointer<vtkNetCDFCFReader>::New();
        globalReader->SetFileName(LocateFile(current_timestep).c_str());
        globalReader->UpdateMetaData();
        globalReader->SetSphericalCoordinates(true);
        globalReader->SetOutputTypeToStructured();

        for (auto &value : MantleAttr::values())
            globalReader->SetVariableArrayStatus(value.c_str(), 1);

#ifndef NDEBUG
        std::cout << std::endl
                  << "--- reading data file [" << current_timestep << "] --- "
                  << std::endl;
        globalReader->GetOutput()->Print(std::cout);
#endif
    }

    std::filesystem::path Mantle::LocateFile(size_t time_step)
    {
        std::stringstream ss = std::stringstream();
        ss << "spherical" << std::setfill('0') << std::setw(3) << current_timestep
           << ".nc";

        std::filesystem::path stem     = std::filesystem::path(data_directory);
        std::filesystem::path resolved = stem.append(ss.str());

#ifndef NDEBUG
        std::cout << "Loading file: " << resolved << std::endl;
        assert(std::filesystem::exists(resolved));
#endif

        return resolved;
    }

    vtkAlgorithmOutput *Mantle::GetOutputPort()
    {
        return globalReader->GetOutputPort();
    }

    void Mantle::Step()
    {
#ifndef NDEBUG
        std::cout << "Stepping Mantle state" << std::endl;
#endif
        current_timestep += 1;

        // Update where the reader is pointing.
        globalReader->SetFileName(LocateFile(current_timestep).c_str());
    }

    // Dataset attributes

    std::vector<MantleAttr> MantleAttr::values()
    {
        auto elems = std::vector<MantleAttr>();
        for (int i = Value::Vx; i <= Value::TempAnom; ++i)
            elems.push_back(MantleAttr(static_cast<Value>(i)));
        return elems;
    }


}

namespace Pipe {
    // FIXME: I don't think using a vtkAlgorithm will work with
    // the lazy updates. We need to look into this.
    using pipe_output = vtkSmartPointer<vtkAlgorithm>;
    using pipe_input  = vtkSmartPointer<vtkAlgorithm>;

    // What I imagine here is a series of generic pipelines
    // E.G., cell to point data, computing velocity, etc ...
    // XXX: these pipelines may need to store some state
    // to get lazily updated with the rest of the pipeline.

    Geometry::Geometry()
    {
        // Initialize fields
        cuttingPlane = vtkSmartPointer<vtkPlane>::New();
        core         = vtkSmartPointer<vtkSphereSource>::New();
        hull         = vtkSmartPointer<vtkSphereSource>::New();

        MantleIO::MantleAttr tempAnom = MantleIO::MantleAttr::TempAnom;

        // Create a color transfer function
        vtkNew<vtkColorTransferFunction> colorTransferFunction;
        double                           range[] = { -1100., 1100. };

        colorTransferFunction->SetColorSpaceToRGB();
        colorTransferFunction->SetScaleToLinear();
        colorTransferFunction->AdjustRange(range);
        colorTransferFunction->AddRGBPoint(-1100, 0., 0., 1.);
        colorTransferFunction->AddRGBPoint(-150, 0., 1., 1.);
        colorTransferFunction->AddRGBPoint(0, 1., 1., 1.);
        colorTransferFunction->AddRGBPoint(150, 1., 1., 0.);
        colorTransferFunction->AddRGBPoint(1100, 1.0, 0., 0.);

        // Initialize the cutting plane
        cuttingPlane->SetOrigin(cx, cy, cz);
        cuttingPlane->SetNormal(nx, ny, nz);

        // Initialize the hull
        hull->SetCenter(cx, cy, cz);
        hull->SetRadius(outerRadius);
        hull->SetThetaResolution(resolution);
        hull->SetPhiResolution(resolution);

        // Clip the hull
        vtkNew<vtkClipPolyData> hullClipper;
        hullClipper->SetInputConnection(hull->GetOutputPort());
        hullClipper->SetClipFunction(cuttingPlane);
        hullClipper->GenerateClipScalarsOn();

        // Initialize the core
        core->SetCenter(cx, cy, cz);
        core->SetRadius(innerRadius);
        core->SetThetaResolution(resolution);
        core->SetPhiResolution(resolution);

        // Clip the core
        vtkNew<vtkClipPolyData> coreClipper;
        coreClipper->SetInputConnection(core->GetOutputPort());
        coreClipper->SetClipFunction(cuttingPlane);
        coreClipper->GenerateClipScalarsOn();
        coreClipper->SetInsideOut(true);

        // Initialize the disk
        vtkNew<vtkDiskSource> slice;
        slice->SetCenter(cx, cy, cz);
        slice->SetNormal(nx, ny, nz);
        slice->SetInnerRadius(innerRadius);
        slice->SetOuterRadius(outerRadius);
        slice->SetCircumferentialResolution(resolution);
        slice->SetRadialResolution(resolution);

        vtkNew<vtkAppendPolyData> appendPolyData;
        appendPolyData->AddInputConnection(hullClipper->GetOutputPort());
        appendPolyData->AddInputConnection(coreClipper->GetOutputPort());
        appendPolyData->AddInputConnection(slice->GetOutputPort());

        // Gaussian kernel
        vtkNew<vtkGaussianKernel> gaussianKernel;
        gaussianKernel->SetSharpness(2.0);
        gaussianKernel->SetRadius(100.0);

        // Point interpolator
        pointInterpolator = vtkSmartPointer<vtkPointInterpolator>::New();
        // pointInterpolator->SetSourceConnection(data->GetOutputPort());
        pointInterpolator->SetInputConnection(appendPolyData->GetOutputPort());
        pointInterpolator->SetKernel(gaussianKernel);
    }

    void Geometry::IncreaseInnerRadius()
    {
        innerRadius += 100;
        core->SetRadius(innerRadius);
    }

    void Geometry::SetInputConnection(vtkAlgorithmOutput *cin)
    {
        pointInterpolator->SetSourceConnection(cin);
    }

    vtkAlgorithmOutput *Geometry::GetOutputPort()
    {
        return pointInterpolator->GetOutputPort();
    }


}
