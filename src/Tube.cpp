#include "Tube.h"

Tube::Tube()
    : Mantle()
    , streamlineActor(vtkSmartPointer<vtkActor>::New())
{
    auto structuredGrid = GetCurrentStream();

    vtkNew<vtkLookupTable> ctf;
    ctf->SetVectorMode(vtkScalarsToColors::MAGNITUDE);
    ctf->SetHueRange(0.0, 0.2);
    ctf->SetValueRange(1, 1);
    ctf->SetSaturationRange(0.5, 0.5);

    vtkNew<vtkCellDataToPointData> cellToPoint;
    cellToPoint->SetInputConnection(structuredGrid->GetOutputPort());

    vtkNew<vtkArrayCalculator> calculator;
    calculator->SetInputConnection(cellToPoint->GetOutputPort());
    calculator->SetResultArrayName("velocity");
    calculator->AddScalarVariable("vx", "vx");
    calculator->AddScalarVariable("vy", "vy");
    calculator->AddScalarVariable("vz", "vz");
    calculator->SetFunction("(iHat * vx + jHat * vy + kHat * vz) * 1e9");

    // Line Seed
    vtkNew<vtkLineSource> line;
    line->SetResolution(1000);
    line->SetPoint1(-6378., -6378., -6378.);
    line->SetPoint2(6378., 6378., 6378.);

    // Spherical Seed
    vtkSmartPointer<vtkPointSource> seeds = vtkSmartPointer<vtkPointSource>::New();
    seeds->SetCenter(0, 0, 0);
    seeds->SetRadius(6000);
    seeds->SetNumberOfPoints(1000);

//    Seed based on filtered points TempAnomaly
    MantleIO::MantleAttr anom = MantleIO::MantleAttr::TempAnom;
    vtkSmartPointer<vtkThresholdPoints> thresholdFilter = vtkSmartPointer<vtkThresholdPoints>::New();
    thresholdFilter->SetInputConnection(cellToPoint->GetOutputPort());
    thresholdFilter->ThresholdBetween(900, 1100);
    thresholdFilter->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, anom.c_str());
    vtkSmartPointer<vtkMaskPoints> sampler = vtkSmartPointer<vtkMaskPoints>::New();
    sampler->SetInputConnection(thresholdFilter->GetOutputPort());
    sampler->SetOnRatio(1);

    vtkNew<vtkAssignAttribute> assignAttribute;
    assignAttribute->SetInputConnection(calculator->GetOutputPort());
    assignAttribute->Assign("velocity", vtkDataSetAttributes::SCALARS,
                            vtkAssignAttribute::POINT_DATA);

    vtkNew<vtkStreamTracer> tracer;
    tracer->SetInputConnection(assignAttribute->GetOutputPort());
//    tracer->SetSourceConnection(line->GetOutputPort());
    tracer->SetSourceConnection(seeds->GetOutputPort());
//    tracer->SetSourceConnection(sampler->GetOutputPort());

    tracer->SetInterpolatorType(vtkStreamTracer::INTERPOLATOR_WITH_DATASET_POINT_LOCATOR);
    tracer->SetIntegrationDirection(vtkStreamTracer::BOTH);
    tracer->SetIntegratorType(vtkStreamTracer::RUNGE_KUTTA45);
    tracer->SetIntegrationStepUnit(vtkStreamTracer::CELL_LENGTH_UNIT);

    tracer->SetTerminalSpeed(1e-12);
    tracer->SetMaximumError(1e-6);
    tracer->SetMaximumPropagation(12000);
    tracer->SetMaximumNumberOfSteps(2000);

#ifndef NDEBUG
    tracer->GetOutput()->Print(std::cout);
#endif

    vtkNew<vtkTubeFilter> tubes;
    tubes->SetInputConnection(tracer->GetOutputPort());
    tubes->SetRadius(60);
    tubes->SetNumberOfSides(6);
    tubes->CappingOn();

    vtkNew<vtkPolyDataMapper> streamlineMapper;
    streamlineMapper->SetInputConnection(tubes->GetOutputPort());
    streamlineMapper->SetLookupTable(ctf);

    streamlineActor->SetMapper(streamlineMapper);
    streamlineActor->VisibilityOn();
}


void Tube::Update() { }


void Tube::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddActor(streamlineActor);
}

void Tube::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveActor(streamlineActor);
}