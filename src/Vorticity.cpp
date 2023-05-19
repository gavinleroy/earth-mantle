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
    vorticityCalculator->SetFunction("vorticity . iHat + vorticity . jHat + vorticity. kHat");

    vtkNew<vtkGeometryFilter> geometry;
    geometry->SetInputConnection(gradient->GetOutputPort());

    vtkNew<vtkPolyDataMapper> polyDataMapper;
    polyDataMapper->SetInputConnection(geometry->GetOutputPort());
    polyDataMapper->SetScalarVisibility(true);
    polyDataMapper->SetArrayName("vorticity magnitude");
    polyDataMapper->SetScalarModeToUsePointData();

    vtkNew<vtkPlane> clippingPlane;
    clippingPlane->SetOrigin(0.0, 0.0, 0.0);
    clippingPlane->SetNormal(0.0, 1.0, 0.0);
    polyDataMapper->AddClippingPlane(clippingPlane);

    mActor->SetMapper(polyDataMapper);
}

void Vorticity::ConnectToScene(vtkSmartPointer <vtkRenderer> renderer) {
    renderer->AddActor(mActor);
}

void Vorticity::RemoveFromScene(vtkSmartPointer <vtkRenderer> renderer) {
    renderer->RemoveActor(mActor);
}