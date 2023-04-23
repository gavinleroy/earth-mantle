#include "Tube.h"

Tube::Tube()
    : Mantle()
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    auto variables = std::vector<std::string>({
                                                      "vx",
                                                      "vy",
                                                      "vz",
                                              });

    auto loaded_from_file = LoadFromFile("spherical001.nc", variables);

//    vtkNew<vtkCellDataToPointData> cellToPoint;
//    cellToPoint->SetInputData(loaded_from_file);
//    cellToPoint->Update();
//
//    auto structured_grid = cellToPoint->GetStructuredGridOutput();

    vtkNew<vtkResampleToImage> resampler;
    resampler->SetInputDataObject(loaded_from_file);
    resampler->SetSamplingDimensions(100, 100, 100);
    resampler->Update();

#ifndef NDEBUG
    resampler->GetOutput()->Print(std::cout);
#endif

    vtkSmartPointer<vtkArrayCalculator> calculator
            = vtkSmartPointer<vtkArrayCalculator>::New();
    calculator->SetInputConnection(resampler->GetOutputPort());
    calculator->SetResultArrayName("velocity");
    calculator->AddScalarVariable("vx", "vx");
    calculator->AddScalarVariable("vy", "vy");
    calculator->AddScalarVariable("vz", "vz");
    calculator->SetFunction("(iHat * vx + jHat * vy + kHat * vz) * 1e9");
    calculator->Update();
//
//    calculator->GetOutput()->Print(std::cout);

    vtkSmartPointer<vtkLineSource> line = vtkSmartPointer<vtkLineSource>::New();
    line->SetResolution(1000);
    line->SetPoint1(-6378., -6378., -6378.);
    line->SetPoint2(6435, 5683, 6414);


//    vtkNew<vtkAssignAttribute> assignAttribute;
//    assignAttribute->SetInputConnection(calculator->GetOutputPort());
//    assignAttribute->Assign("velocity", "VECTORS", "CELL_DATA");
//    assignAttribute->Update();

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
//
//    vtkSmartPointer<vtkPolyData> streamlines = vtkSmartPointer<vtkPolyData>::New();
//    streamlines->ShallowCopy(tracer->GetOutput());
//
//    std::cout << streamlines->GetNumberOfPoints() << std::endl;

    vtkSmartPointer<vtkTubeFilter> tubeFilter = vtkSmartPointer<vtkTubeFilter>::New();
    tubeFilter->SetInputConnection(tracer->GetOutputPort());
    tubeFilter->SetRadius(127.1);
    tubeFilter->SetNumberOfSides(6);
    tubeFilter->CappingOn();
    tubeFilter->Update();

    vtkSmartPointer<vtkPolyDataMapper> mapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    mapper->SetInputConnection(tubeFilter->GetOutputPort());
    mapper->ScalarVisibilityOn();
//    mapper->SetScalarRange(tubeFilter->GetOutput()->GetScalarRange());

    mActor->SetMapper(mapper);
    mActor->VisibilityOn();
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
