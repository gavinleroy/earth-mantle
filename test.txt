#include "Vorticity.h"

Vorticity::Vorticity()
    : Resample()
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    // Compute the velocity field
    vtkNew<vtkArrayCalculator> velocityCalculator;
    velocityCalculator->SetInputConnection(GetResampled()->GetOutputPort());
    velocityCalculator->SetResultArrayName("velocity");
    velocityCalculator->AddScalarVariable("vx", "vx");
    velocityCalculator->AddScalarVariable("vy", "vy");
    velocityCalculator->AddScalarVariable("vz", "vz");
    velocityCalculator->SetFunction("(iHat * vx + jHat * vy + kHat * vz) * 1e9");

    // Detect the vortex cores
    vtkNew<vtkVortexCore> vortexCore;
    vortexCore->SetInputConnection(velocityCalculator->GetOutputPort());
    vortexCore->SetInputArrayToProcess(
            0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "velocity");
    vortexCore->FasterApproximationOn();

    // Assign the velocity attribute
    vtkNew<vtkAssignAttribute> assignAttribute;
    assignAttribute->SetInputConnection(velocityCalculator->GetOutputPort());
    assignAttribute->Assign("velocity", vtkDataSetAttributes::VECTORS,
                            vtkAssignAttribute::POINT_DATA);

    // Trace the streamlines
    vtkNew<vtkStreamTracer> streamTracer;
    streamTracer->SetInputConnection(assignAttribute->GetOutputPort());
    streamTracer->SetSourceConnection(vortexCore->GetOutputPort());
    streamTracer->SetInterpolatorType(vtkStreamTracer::INTERPOLATOR_WITH_DATASET_POINT_LOCATOR);
    streamTracer->SetIntegrationDirectionToBoth();
    streamTracer->SetIntegratorTypeToRungeKutta45();
    streamTracer->SetIntegrationStepUnit(vtkStreamTracer::CELL_LENGTH_UNIT);
    streamTracer->SetTerminalSpeed(1e-12);
    streamTracer->SetMaximumError(1e-6);
    streamTracer->SetMaximumPropagation(1000);
    streamTracer->SetMaximumNumberOfSteps(2000);

    // Visualize the streamlines as tubes
    vtkNew<vtkTubeFilter> tubeFilter;
    tubeFilter->SetInputConnection(streamTracer->GetOutputPort());
    tubeFilter->SetRadius(50);
    tubeFilter->SetNumberOfSides(6);
    tubeFilter->CappingOn();

    // Map the poly data
    vtkNew<vtkPolyDataMapper> polyDataMapper;
    polyDataMapper->SetInputConnection(tubeFilter->GetOutputPort());
    mActor->SetMapper(polyDataMapper);
}

void Vorticity::ConnectToScene(vtkSmartPointer <vtkRenderer> renderer) {
    renderer->AddActor(mActor);
}

void Vorticity::RemoveFromScene(vtkSmartPointer <vtkRenderer> renderer) {
    renderer->RemoveActor(mActor);
}