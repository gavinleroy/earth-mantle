#include "Volumes.h"

Volumes::Volumes()
{
    mVolume         = vtkSmartPointer<vtkVolume>::New();
    volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    ConstructInternal();
}

void Volumes::ConstructInternal()
{
    double range[] = { -1100., 1100. };

    MantleIO::MantleAttr property  = MantleIO::MantleAttr::TempAnom;
    // Create a color transfer function
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

    // Create a piecewise function
    vtkNew<vtkPiecewiseFunction> opacityTransferFunction;
    opacityTransferFunction->AddPoint(-1100, 0.8);
    opacityTransferFunction->AddPoint(1100, 0.8);

    vtkNew<vtkVolumeProperty> volumeProperty;
    volumeProperty->ShadeOff();
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);

    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(0., 0., 0.);
    plane->SetNormal(0., 1.0, 0);


    // Add a mapper to create graphic primitives from the data
//    vtkNew<vtkSmartVolumeMapper> volumeMapper;
#ifndef NDEBUG
    std::cout << "Loading scalar array: " << property.c_str() << " for volume render"
              << std::endl;
#endif
    volumeMapper->SelectScalarArray(property.c_str());
    volumeMapper->AddClippingPlane(plane);
    // volumeMapper->SetBlendModeToIsoSurface();
    // volumeMapper->SetSampleDistance(4);

    this->mVolume->SetMapper(volumeMapper);
    this->mVolume->SetProperty(volumeProperty);
}

void Volumes::SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines)
{
    volumeMapper->SetInputConnection(pipelines->assignAttr->GetOutputPort());
    // this->assignAttribute->SetInputConnection(cin);
}

void Volumes::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddVolume(mVolume);
}

void Volumes::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveVolume(mVolume);
}
