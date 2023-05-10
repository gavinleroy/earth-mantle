#include "LIConvolution.h"

LIConvolution::LIConvolution()
    : Mantle()
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    auto fromFile = GetCurrentStream();

    // Convert the cell data to point data
    vtkNew<vtkCellDataToPointData> cellDataToPointData;
    cellDataToPointData->SetInputConnection(fromFile->GetOutputPort());

    // Aggregate the velocity into one array
    vtkNew<vtkArrayCalculator> calculator;
    calculator->SetInputConnection(cellDataToPointData->GetOutputPort());
    calculator->SetResultArrayName("velocity");
    calculator->AddScalarVariable("vx", "vx");
    calculator->AddScalarVariable("vy", "vy");
    calculator->AddScalarVariable("vz", "vz");
    calculator->SetFunction("(iHat * vx + jHat * vy + kHat * vz) * 1e9");

    // TODO do we need this? or can we do this without the intermediate geometry filter?
    vtkNew<vtkGeometryFilter> geometry;
    geometry->SetInputConnection(calculator->GetOutputPort());

    // Line integral convolution (LIC) mapper
    vtkNew<vtkSurfaceLICMapper> licMapper;
    licMapper->SetInputConnection(geometry->GetOutputPort());
    licMapper->SetInputArrayToProcess(0, 0, 0, vtkDataObject::POINT, "velocity");

    // Disable enhanced LIC (for now)
    licMapper->GetLICInterface()->SetEnhancedLIC(0);

    // Slice the geometry
    vtkNew<vtkPlane> clippingPlane;
    clippingPlane->SetOrigin(0., 0., 0);
    clippingPlane->SetNormal(0., 1.0, 0);

    licMapper->AddClippingPlane(clippingPlane);
    this->mActor->SetMapper(licMapper);
}

void LIConvolution::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddActor(mActor);
}

void LIConvolution::Update() { }
