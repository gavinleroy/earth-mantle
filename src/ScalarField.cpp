#include "ScalarField.h"

ScalarField::ScalarField()
    : Resample()
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    MantleIO::MantleAttr tempAnom = MantleIO::MantleAttr::TempAnom;
    auto data = GetPointDataBeforeResample();

    // Plane source
    vtkNew<vtkPlaneSource> planeSource;
    double radius = 7000;
    planeSource->SetOrigin(-radius, -radius, 0);
    planeSource->SetPoint1(2*radius,-radius, 0);
    planeSource->SetPoint2(-radius, 2*radius, 0);
    planeSource->SetResolution(100, 100);

    // Core source
    vtkNew<vtkSphereSource> coreSource;
    coreSource->SetCenter(0, 0, 0);
    coreSource->SetRadius(5000);
    coreSource->SetThetaResolution(100);
    coreSource->SetPhiResolution(100);

    vtkNew<vtkAppendPolyData> appendPolyData;
    appendPolyData->AddInputConnection(planeSource->GetOutputPort());
    appendPolyData->AddInputConnection(coreSource->GetOutputPort());

    // Gaussian kernel
    vtkNew<vtkGaussianKernel> gaussianKernel;
    gaussianKernel->SetSharpness(2.0);
    gaussianKernel->SetRadius(100.0);

    // Point interpolator
    vtkNew<vtkPointInterpolator> pointInterpolator;
    pointInterpolator->SetSourceConnection(data->GetOutputPort());
    pointInterpolator->SetInputConnection(appendPolyData->GetOutputPort());
    pointInterpolator->SetKernel(gaussianKernel);

    // Create a color transfer function
    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->SetColorSpaceToRGB();
    colorTransferFunction->SetScaleToLinear();
    double range[] = {-1100.,1100.};
    colorTransferFunction->AdjustRange(range);
    colorTransferFunction->AddRGBPoint(-1100, 0., 0., 1.);
    colorTransferFunction->AddRGBPoint(-150, 0., 1., 1.);
    colorTransferFunction->AddRGBPoint(0, 1., 1., 1.);
    colorTransferFunction->AddRGBPoint(150, 1., 1., 0.);
    colorTransferFunction->AddRGBPoint(1100, 1.0, 0., 0.);

    // Map the poly data
    vtkNew<vtkPolyDataMapper> polyDataMapper;
    polyDataMapper->SetInputConnection(pointInterpolator->GetOutputPort());
    polyDataMapper->SetScalarVisibility(true);
    polyDataMapper->SelectColorArray(tempAnom.c_str());
    polyDataMapper->SetScalarModeToUsePointFieldData();
    polyDataMapper->SetLookupTable(colorTransferFunction);
    mActor->SetMapper(polyDataMapper);
}

void ScalarField::ConnectToScene(vtkSmartPointer <vtkRenderer> renderer) {
    renderer->AddActor(mActor);
}

void ScalarField::RemoveFromScene(vtkSmartPointer <vtkRenderer> renderer) {
    renderer->RemoveActor(mActor);
}