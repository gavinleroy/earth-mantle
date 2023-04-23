#include "mantle.h"

Mantle::Mantle()
    : mVolume(vtkSmartPointer<vtkVolume>::New())
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    LoadMantleSet("/dev/null");
}

vtkSmartPointer<vtkDataObject> Mantle::LoadFromFile(
    const std::string fn,
    const std::string variable /* define polymorphism over computation */)
{
#ifndef NDEBUG
    std::cout << "Loading variable: " << variable << " from file: " << fn << std::endl;
#endif

    vtkNew<vtkNetCDFCFReader> reader;

    reader->SetFileName(fn.c_str());
    reader->UpdateMetaData();
    reader->SetSphericalCoordinates(true);
    reader->SetOutputTypeToStructured();

    // Unset all available variables
    std::for_each(
        std::rbegin(this->variables), std::rend(this->variables),
        [&](auto const &value) { reader->SetVariableArrayStatus(value.c_str(), 0); });

    // Explicitly set the variables we want
    reader->SetVariableArrayStatus(variable.c_str(), 1);

    reader->Update();

#ifndef NDEBUG
    reader->GetOutput()->Print(std::cout);
#endif

    return reader->GetOutput();
}

void Mantle::LoadMantleSet(std::filesystem::path data_dir)
{
    // TODO: load series from a specified directory (specify root via env var)
    const std::string fn = "../data/FullMantle/spherical001.nc";

    std::string variable = "temperature";

    auto loaded_from_file = LoadFromFile(fn, variable);

    vtkNew<vtkCellDataToPointData> cellToPoint;
    cellToPoint->SetInputData(loaded_from_file);
    cellToPoint->Update();

    auto structured_grid = cellToPoint->GetStructuredGridOutput();

    vtkNew<vtkResampleToImage> resampler;
    resampler->SetInputDataObject(structured_grid);
    resampler->SetSamplingDimensions(100, 100, 100);
    resampler->Update();

#ifndef NDEBUG
    resampler->GetOutput()->Print(std::cout);
#endif

    vtkNew<vtkAssignAttribute> assignAttribute;
    assignAttribute->SetInputConnection(resampler->GetOutputPort());
    assignAttribute->Assign(variable.c_str(), "SCALARS", "POINT_DATA");
    assignAttribute->Update();

#ifndef NDEBUG
    assignAttribute->GetOutput()->Print(std::cout);
#endif

    // Create a color transfer function
    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->AddRGBPoint(293, 0.0, 0.0, 1.0);
    colorTransferFunction->AddRGBPoint(1200, 1.0, 1.0, 1.0);
    colorTransferFunction->AddRGBPoint(3608, 1.0, 0.0, 0.0);

    // Create a piecewise function
     vtkNew<vtkPiecewiseFunction> opacityTransferFunction;
     opacityTransferFunction->AddPoint(293, 1.0);
     opacityTransferFunction->AddPoint(3608, 1.0);

    vtkNew<vtkVolumeProperty> volumeProperty;
    volumeProperty->ShadeOn();
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);

    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(0., 0., 0.);
    plane->SetNormal(0., 1.0, -1.);


    // Add a mapper to create graphic primitives from the data
    vtkNew<vtkSmartVolumeMapper> volumeMapper;
    volumeMapper->SetInputConnection(assignAttribute->GetOutputPort());
    volumeMapper->SelectScalarArray(variable.c_str());
    volumeMapper->AddClippingPlane(plane);
    // volumeMapper->SetBlendModeToIsoSurface();
    // volumeMapper->SetSampleDistance(4);

    // HACK: this seems very specific to a single pipeline, I(gavin) don't
    // know how this will interact with the other things we'll need.

    this->mVolume->SetMapper(volumeMapper);
    this->mVolume->SetProperty(volumeProperty);
}

void Mantle::Update()
{ /* UNIMPLEMENTED */
}

// TODO:
std::vector<vtkSmartPointer<vtkActor>> Mantle::GetActors()
{
    auto v = std::vector<vtkSmartPointer<vtkActor>>({ this->mActor });
    return v;
}

// TODO:
std::vector<vtkSmartPointer<vtkVolume>> Mantle::GetVolumes()
{
    auto v = std::vector<vtkSmartPointer<vtkVolume>>({ this->mVolume });
    return v;
}
