
#include "IsoVolume.h"

IsoVolume::IsoVolume()
{
    volumeRayMapper = vtkSmartPointer<vtkGPUVolumeRayCastMapper>::New();
    mVolume         = vtkSmartPointer<vtkVolume>::New();
    ConstructInternal();
}

void IsoVolume::ConstructInternal()
{
    double                           range[] = { -1100., 1100. };
    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->SetNanOpacity(1.0);
    colorTransferFunction->SetColorSpaceToRGB();
    colorTransferFunction->SetScaleToLinear();
    colorTransferFunction->AdjustRange(range);
    colorTransferFunction->AddRGBPoint(-1100, 0., 0., 1.);
    colorTransferFunction->AddRGBPoint(-150, 0., 1., 1.);
    colorTransferFunction->AddRGBPoint(0, 1., 1., 1.);
    colorTransferFunction->AddRGBPoint(150, 1., 1., 0.);
    colorTransferFunction->AddRGBPoint(1100, 1.0, 0., 0.);

    vtkNew<vtkPiecewiseFunction> opacityTransferFunction;
    opacityTransferFunction->AddPoint(-1100, 1.0);
    opacityTransferFunction->AddPoint(-151, 1.0);
    opacityTransferFunction->AddPoint(-150, 0.0);
    opacityTransferFunction->AddPoint(150, 0.0);
    opacityTransferFunction->AddPoint(151, 1.);
    opacityTransferFunction->AddPoint(1100, 1.);

    volumeRayMapper->SetBlendModeToComposite();
    volumeRayMapper->SetAutoAdjustSampleDistances(true);
    volumeRayMapper->SetUseJittering(true);

    vtkNew<vtkVolumeProperty> volumeProperty;
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->ShadeOn();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);
    volumeProperty->SetAmbient(.3);
    volumeProperty->SetDiffuse(.75);
    volumeProperty->SetSpecular(.5);

    mVolume->SetMapper(volumeRayMapper);
    mVolume->SetProperty(volumeProperty);
}

void IsoVolume::SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines)
{
    this->volumeRayMapper->SetInputConnection(pipelines->assignAttr->GetOutputPort());
}

void IsoVolume::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddVolume(mVolume);
}

void IsoVolume::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveVolume(mVolume);
}
