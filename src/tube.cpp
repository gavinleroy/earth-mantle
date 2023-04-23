#include "Tube.h"

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

//    vtkSmartPointer<vtkImageData> data = vtkImageData::SafeDownCast(calculator->GetOutput());
//    data->Print(std::cout);

//
//    vtkNew<vtkAssignAttribute> assignAttribute;
//    assignAttribute->SetInputData(data);
//    assignAttribute->Assign("velocity", "VECTORS", "POINT_DATA");
//    assignAttribute->Update();

    vtkSmartPointer<vtkLineSource> line = vtkSmartPointer<vtkLineSource>::New();
    line->SetResolution(1000);
    line->SetPoint1(-6378., -6378., -6378.);
    line->SetPoint2(6435, 5683, 6414);

    vtkSmartPointer<vtkStreamTracer> tracer = vtkSmartPointer<vtkStreamTracer>::New();
    tracer->SetInputConnection(calculator->GetOutputPort());
    tracer->SetSourceConnection(line->GetOutputPort());
    tracer->SetTerminalSpeed(1e-12);
    tracer->SetInterpolatorTypeToDataSetPointLocator();
    tracer->SurfaceStreamlinesOff();
    tracer->SetIntegrationDirectionToBoth();
    tracer->SetIntegrationStepUnit(2);
    tracer->SetInitialIntegrationStep(0.2);
    tracer->SetMinimumIntegrationStep(0.01);
    tracer->SetMaximumIntegrationStep(0.5);
    tracer->SetMaximumNumberOfSteps(2000);
    tracer->SetMaximumError(1e-6);
    tracer->SetIntegratorTypeToRungeKutta45();
    tracer->Update();
    tracer->GetOutput()->Print(std::cout);


    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(tracer->GetOutputPort());

    mActor->SetMapper(mapper);

    vtkSmartPointer<vtkPolyData> streamlines = vtkSmartPointer<vtkPolyData>::New();
    streamlines->ShallowCopy(tracer->GetOutput());

    std::cout << streamlines->GetNumberOfPoints() << std::endl;

    vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(tracer->GetOutputPort());
    tubeFilter->SetRadius(127.1);
    tubeFilter->SetNumberOfSides(6);
    tubeFilter->CappingOn();
    tubeFilter->Update();


//    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//    mapper->SetInputConnection(tubeFilter->GetOutputPort());
//
//    mActor->SetMapper(mapper);

//
//    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
//    mapper->SetInputConnection(tubeFilter->GetOutputPort());
//
//    mActor->SetMapper(mapper);

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
