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

    // Extract the geometry
    vtkNew<vtkGeometryFilter> geometry;
    geometry->SetInputConnection(calculator->GetOutputPort());

    // Line integral convolution (LIC) mapper
    vtkNew<vtkSurfaceLICMapper> licMapper;
    licMapper->SetInputConnection(geometry->GetOutputPort());

    // Overlay the temperature anomaly
    licMapper->SelectColorArray("temperature anomaly");
    licMapper->SetScalarVisibility(true);
    licMapper->SetScalarModeToUsePointFieldData();

    // Create a color transfer function
    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->SetNanOpacity(1.0);
    colorTransferFunction->SetColorSpaceToRGB();
    colorTransferFunction->SetScaleToLinear();
    double range[] = { -1100., 1100. };
    colorTransferFunction->AdjustRange(range);
    colorTransferFunction->AddRGBPoint(-1100, 0., 0., 1.);
    colorTransferFunction->AddRGBPoint(-150, 0., 1., 1.);
    colorTransferFunction->AddRGBPoint(0, 1., 1., 1.);
    colorTransferFunction->AddRGBPoint(150, 1., 1., 0.);
    colorTransferFunction->AddRGBPoint(1100, 1.0, 0., 0.);
    licMapper->SetLookupTable(colorTransferFunction);

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

void LIConvolution::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveActor(mActor);
}

void LIConvolution::Update() { }
