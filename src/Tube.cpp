#include "Tube.h"

Tube::Tube()
{
#ifndef NDEBUG
    std::cout << "TUBE: creating with default" << std::endl;
#endif

    thresholdFilter = vtkSmartPointer<vtkThresholdPoints>::New();
    assignAttribute = vtkSmartPointer<vtkAssignAttribute>::New();

    // cellToPoint     = vtkSmartPointer<vtkCellDataToPointData>::New();
    streamlineActor = vtkSmartPointer<vtkActor>::New();

    ConstructInternal();
}

Tube::Tube(std::shared_ptr<Pipe::AllInput> pipelines)
{
#ifndef NDEBUG
    std::cout << "TUBE: creating with pipelines" << std::endl;
#endif

    thresholdFilter = vtkSmartPointer<vtkThresholdPoints>::New();
    assignAttribute = vtkSmartPointer<vtkAssignAttribute>::New();
    // cellToPoint     = vtkSmartPointer<vtkCellDataToPointData>::New();
    streamlineActor = vtkSmartPointer<vtkActor>::New();

    SetInputConnection(pipelines);
    ConstructInternal();
}


void Tube::ConstructInternal()
{
    //    Seed based on filtered points TempAnomaly
    MantleIO::MantleAttr anom = MantleIO::MantleAttr::TempAnom;

    vtkNew<vtkLookupTable> ctf;
    ctf->SetVectorMode(vtkScalarsToColors::MAGNITUDE);
    ctf->SetHueRange(0.0, 0.2);
    ctf->SetValueRange(1, 1);
    ctf->SetSaturationRange(0.5, 0.5);

    // create a scalar bar
    vtkNew<vtkScalarBarActor> scalarBar;
    scalarBar->SetTitle("Tubes");
    scalarBar->SetLookupTable(ctf);
    SetScalarBar(scalarBar);

    // Line Seed
    // vtkNew<vtkLineSource> line;
    // line->SetResolution(1000);
    // line->SetPoint1(-6378., -6378., -6378.);
    // line->SetPoint2(6378., 6378., 6378.);

    // Spherical Seed
    vtkNew<vtkPointSource> seeds;
    seeds->SetCenter(0, 0, 0);
    seeds->SetRadius(6000);
    seeds->SetNumberOfPoints(1000);

    thresholdFilter->ThresholdBetween(500, 1100);
    thresholdFilter->SetInputArrayToProcess(
        0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, anom.c_str());

    vtkNew<vtkMaskPoints> sampler;
    sampler->SetInputConnection(thresholdFilter->GetOutputPort());
    sampler->SetOnRatio(1);

    assignAttribute->Assign("velocity", vtkDataSetAttributes::SCALARS,
                            vtkAssignAttribute::POINT_DATA);

    vtkNew<vtkStreamTracer> tracer;
    tracer->SetInputConnection(assignAttribute->GetOutputPort());
    tracer->SetSourceConnection(sampler->GetOutputPort());
    tracer->SetIntegratorTypeToRungeKutta45();
    tracer->SetInterpolatorTypeToDataSetPointLocator();
    tracer->SetIntegrationDirectionToForward();
    // tracer->SetIntegrationDirectionToBoth();
    tracer->SetIntegrationStepUnit(vtkStreamTracer::CELL_LENGTH_UNIT);
    tracer->SetTerminalSpeed(1e-12);
    tracer->SetMaximumError(1e-6);
    tracer->SetMaximumPropagation(5000);
    tracer->SetMaximumNumberOfSteps(2000);
    tracer->SetComputeVorticity(true);
    // tracer->SetMaximumPropagation(12000);
    // tracer->SetMaximumPropagation(6000);
    // tracer->SetMaximumNumberOfSteps(2000);

    vtkNew<vtkTubeFilter> tubes;
    tubes->SetInputConnection(tracer->GetOutputPort());
    tubes->SetRadius(10);
    tubes->SetNumberOfSides(6);
    tubes->CappingOn();

    vtkNew<vtkPolyDataMapper> streamlineMapper;
    streamlineMapper->SetInputConnection(tubes->GetOutputPort());
    streamlineMapper->SetLookupTable(ctf);
    streamlineActor->SetMapper(streamlineMapper);
    streamlineActor->VisibilityOn();
}

void Tube::SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines)
{
#ifndef NDEBUG
    std::cout << "TUBE: setting input connection" << std::endl;
#endif
    thresholdFilter->SetInputConnection(pipelines->cellToPoint->GetOutputPort());
    assignAttribute->SetInputConnection(
        pipelines->otherVelocityCalculator->GetOutputPort());

    // cellToPoint->SetInputConnection(pipelines->reader->GetOutputPort());
}

void Tube::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddActor(this->streamlineActor);
}

void Tube::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveActor(this->streamlineActor);
}
