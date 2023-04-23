#include "tube.h"

Tube::Tube()
    : mVolume(vtkSmartPointer<vtkVolume>::New())
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    LoadTubeSet("/dev/null");
}

vtkSmartPointer<vtkDataObject> Tube::LoadFromFile(const std::string fn)
{
#ifndef NDEBUG
    // std::cout << "Loading variable: " << variable << " from file: " << fn << std::endl;
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
    std::for_each(
        std::rbegin(this->include), std::rend(this->include),
        [&](auto const &value) { reader->SetVariableArrayStatus(value.c_str(), 1); });


    reader->Update();

#ifndef NDEBUG
    reader->GetOutput()->Print(std::cout);
#endif

    return reader->GetOutput();
}

void Tube::LoadTubeSet(std::filesystem::path data_dir)
{
    // TODO: load series from a specified directory (specify root via env var)
    const std::string fn = "../data/FullMantle/spherical001.nc";

    auto loaded_from_file = LoadFromFile(fn);

    vtkNew<vtkCellDataToPointData> cellToPoint;
    cellToPoint->SetInputData(loaded_from_file);
    cellToPoint->Update();

    auto structured_grid = cellToPoint->GetStructuredGridOutput();

    vtkNew<vtkResampleToImage> resampler;
    resampler->SetInputDataObject(structured_grid);
    resampler->SetSamplingDimensions(100, 100, 100);
    resampler->Update();

    resampler->GetOutput()->Print(std::cout);
#ifndef NDEBUG
    resampler->GetOutput()->Print(std::cout);
#endif

    // Update the calculator filter
    //    vtkNew<vtkAssignAttribute> assignAttribute;
    //    assignAttribute->SetInputConnection(resampler->GetOutputPort());
    //    std::for_each(
    //            std::rbegin(this->include), std::rend(this->include),
    //            [&](auto const &value) { assignAttribute->Assign(value.c_str(),
    //            "SCALARS", "POINT_DATA"); });
    //    assignAttribute->Update();
    //
    //
    //    assignAttribute->Print(std::cout);
    vtkSmartPointer<vtkArrayCalculator> calculator
        = vtkSmartPointer<vtkArrayCalculator>::New();
    calculator->SetInputConnection(resampler->GetOutputPort());
    calculator->SetResultArrayName("velocity");
    calculator->AddScalarVariable("vx", "vx");
    calculator->AddScalarVariable("vy", "vy");
    calculator->AddScalarVariable("vz", "vz");
    calculator->SetFunction("(iHat * vx + jHat * vy + kHat * vz) * 1e9");
    calculator->Update();

    calculator->GetOutput()->Print(std::cout);

    vtkSmartPointer<vtkStreamTracer> tracer = vtkSmartPointer<vtkStreamTracer>::New();
    tracer->SetInputConnection(calculator->GetOutputPort());
    tracer->GetOutput()->Print(std::cout);
    //    resampler->GetOutput()->Print(std::cout);
}

void Tube::Update()
{ /* UNIMPLEMENTED */
}

// TODO:
std::vector<vtkSmartPointer<vtkActor>> Tube::GetActors()
{
    auto v = std::vector<vtkSmartPointer<vtkActor>>({ this->mActor });
    return v;
}

// TODO:
std::vector<vtkSmartPointer<vtkVolume>> Tube::GetVolumes()
{
    auto v = std::vector<vtkSmartPointer<vtkVolume>>({ this->mVolume });
    return v;
}
