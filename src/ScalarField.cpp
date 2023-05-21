#include "ScalarField.h"

ScalarField::ScalarField()
    : Resample()
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    MantleIO::MantleAttr tempAnom = MantleIO::MantleAttr::TempAnom;
    auto data = GetPointDataBeforeResample();

    // Geometry parameters
    int resolution = 200;
    double outerRadius = 6377;
    double innerRadius = 3486;
    double cx = 0, cy = 0, cz = 0;
    double nx = 0, ny = 1, nz = 0;


    // Initialize the cutting plane
    vtkNew<vtkPlane> cuttingPlane;
    cuttingPlane->SetOrigin(cx, cy, cz);
    cuttingPlane->SetNormal(nx, ny, nz);

    // Initialize the hull
    vtkNew<vtkSphereSource> hull;
    hull->SetCenter(cx, cy, cz);
    hull->SetRadius(outerRadius);
    hull->SetThetaResolution(resolution);
    hull->SetPhiResolution(resolution);

    // Clip the hull
    vtkNew<vtkClipPolyData> hullClipper;
    hullClipper->SetInputConnection(hull->GetOutputPort());
    hullClipper->SetClipFunction(cuttingPlane);
    hullClipper->GenerateClipScalarsOn();

    // Initialize the core
    vtkNew<vtkSphereSource> core;
    core->SetCenter(cx, cy, cz);
    core->SetRadius(innerRadius);
    core->SetThetaResolution(resolution);
    core->SetPhiResolution(resolution);

    // Clip the core
    vtkNew<vtkClipPolyData> coreClipper;
    coreClipper->SetInputConnection(core->GetOutputPort());
    coreClipper->SetClipFunction(cuttingPlane);
    coreClipper->GenerateClipScalarsOn();
    coreClipper->SetInsideOut(true);

    // Initialize the disk
    vtkNew<vtkDiskSource> slice;
    slice->SetCenter(cx, cy, cz);
    slice->SetNormal(nx, ny, nz);
    slice->SetInnerRadius(innerRadius);
    slice->SetOuterRadius(outerRadius);
    slice->SetCircumferentialResolution(resolution);
    slice->SetRadialResolution(resolution);

    vtkNew<vtkAppendPolyData> appendPolyData;
    appendPolyData->AddInputConnection(hullClipper->GetOutputPort());
    appendPolyData->AddInputConnection(coreClipper->GetOutputPort());
    appendPolyData->AddInputConnection(slice->GetOutputPort());
    //appendPolyData->AddInputConnection(planeSource->GetOutputPort());
    //appendPolyData->AddInputConnection(coreSource->GetOutputPort());

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