
#include "IsoVolume.h"

IsoVolume::IsoVolume()
{
    this->input   = vtkSmartPointer<vtkAssignAttribute>::New();
    this->mVolume = vtkSmartPointer<vtkVolume>::New();

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

    MantleIO::MantleAttr property = MantleIO::MantleAttr::TempAnom;
    this->input->Assign(property.c_str(), vtkDataSetAttributes::SCALARS,
                        vtkAssignAttribute::POINT_DATA);

    vtkNew<vtkGPUVolumeRayCastMapper> volumeRayMapper;
    volumeRayMapper->SetBlendModeToComposite();
    volumeRayMapper->SetInputConnection(this->input->GetOutputPort());
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
    throw "NYI";
    // this->input->SetInputConnection(cin);
}

void IsoVolume::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddVolume(mVolume);
}

void IsoVolume::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveVolume(mVolume);
}
