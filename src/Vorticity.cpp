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

    // According to VTK, the range of vorticity magnitude is: [0, 0.539739]

    vtkNew<vtkAssignAttribute> assignAttribute;
    assignAttribute->SetInputConnection(vorticityCalculator->GetOutputPort());
    assignAttribute->Assign("vorticity magnitude", vtkDataSetAttributes::SCALARS,
                            vtkAssignAttribute::POINT_DATA);

    vtkNew<vtkContourFilter> contourFilter;
    contourFilter->SetInputConnection(assignAttribute->GetOutputPort());
    contourFilter->SetInputArrayToProcess(
            0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, "vorticity magnitude");
    contourFilter->SetValue(0, .03);

    vtkNew<vtkPolyDataMapper> polyDataMapper;
    polyDataMapper->SetInputConnection(contourFilter->GetOutputPort());

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