#include "Vorticity.h"

Vorticity::Vorticity()
    : Mantle()
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    auto fromFile = GetCurrentStream();

    // Convert the cell data to point data
    vtkNew<vtkCellDataToPointData> cellDataToPointData;
    cellDataToPointData->SetInputConnection(fromFile->GetOutputPort());

    // Compute the velocity field
    vtkNew<vtkArrayCalculator> velocityCalculator;
    velocityCalculator->SetInputConnection(cellDataToPointData->GetOutputPort());
    velocityCalculator->SetResultArrayName("velocity");
    velocityCalculator->AddScalarVariable("vx", "vx");
    velocityCalculator->AddScalarVariable("vy", "vy");
    velocityCalculator->AddScalarVariable("vz", "vz");
    velocityCalculator->SetFunction("(iHat * vx + jHat * vy + kHat * vz) * 1e9");

    // Compute the jacobian from the velocity field
    vtkNew<vtkGradientFilter> gradient;
    gradient->SetInputConnection(velocityCalculator->GetOutputPort());
    gradient->SetFasterApproximation(true);
    gradient->SetResultArrayName("jacobian");
    gradient->ComputeVorticityOn();
    gradient->SetVorticityArrayName("vorticity");
    gradient->SetInputArrayToProcess(
            0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "velocity");

    // Compute the vorticity magnitude
    vtkNew<vtkArrayCalculator> vorticityCalculator;
    vorticityCalculator->SetInputConnection(gradient->GetOutputPort());
    vorticityCalculator->SetResultArrayName("vorticity magnitude");
    vorticityCalculator->AddVectorVariable("vorticity", "vorticity");
    vorticityCalculator->SetFunction("sqrt(dot(vorticity, vorticity))");

    // Filter by the vorticity magnitude
    vtkNew<vtkThresholdPoints> thresholdPoints;
    thresholdPoints->SetInputConnection(vorticityCalculator->GetOutputPort());
    thresholdPoints->SetInputArrayToProcess(
            0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "vorticity magnitude");
    thresholdPoints->ThresholdBetween(0.035, 0.06);
    vtkNew<vtkMaskPoints> maskPoints;
    maskPoints->SetInputConnection(thresholdPoints->GetOutputPort());
    maskPoints->SetOnRatio(1);

    // Assign the velocity attribute
    vtkNew<vtkAssignAttribute> assignAttribute;
    assignAttribute->SetInputConnection(vorticityCalculator->GetOutputPort());
    assignAttribute->Assign("velocity", vtkDataSetAttributes::VECTORS,
                            vtkAssignAttribute::POINT_DATA);

    // Trace the streamlines
    vtkNew<vtkStreamTracer> streamTracer;
    streamTracer->SetInputConnection(assignAttribute->GetOutputPort());
    streamTracer->SetSourceConnection(maskPoints->GetOutputPort());
    streamTracer->SetInterpolatorType(vtkStreamTracer::INTERPOLATOR_WITH_DATASET_POINT_LOCATOR);
    streamTracer->SetIntegrationDirection(vtkStreamTracer::BOTH);
    streamTracer->SetIntegratorType(vtkStreamTracer::RUNGE_KUTTA45);
    streamTracer->SetIntegrationStepUnit(vtkStreamTracer::CELL_LENGTH_UNIT);
    streamTracer->SetTerminalSpeed(1e-12);
    streamTracer->SetMaximumError(1e-6);
    streamTracer->SetMaximumPropagation(12000);
    streamTracer->SetMaximumNumberOfSteps(2000);

    // Visualize the streamlines as tubes
    vtkNew<vtkTubeFilter> tubeFilter;
    tubeFilter->SetInputConnection(streamTracer->GetOutputPort());
    tubeFilter->SetRadius(60);
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