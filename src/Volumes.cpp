#include "Volumes.h"

Volumes::Volumes()
{
#ifndef NDEBUG
    std::cout << "VOLUMES: building with no args" << std::endl;
#endif

    mVolume      = vtkSmartPointer<vtkVolume>::New();
    volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    ConstructInternal();
}

Volumes::Volumes(std::shared_ptr<Pipe::AllInput> pipelines)
{
#ifndef NDEBUG
    std::cout << "VOLUMES: building with input pipelines" << std::endl;
#endif

    mVolume      = vtkSmartPointer<vtkVolume>::New();
    volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    SetInputConnection(pipelines);

    ConstructInternal();
}

void Volumes::ConstructInternal()
{
#ifndef NDEBUG
    std::cout << "VOLUMES: constructing internal objects" << std::endl;
#endif

    double range[] = { -1100., 1100. };

    MantleIO::MantleAttr property = MantleIO::MantleAttr::TempAnom;

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
    std::cout << "VOLUMES: loading scalar array: " << property.c_str()
              << " for volume render" << std::endl;
#endif
    // volumeMapper->SetInputConnection(assignAttribute->GetOutputPort());
    volumeMapper->SelectScalarArray(property.c_str());
    volumeMapper->AddClippingPlane(plane);
    // volumeMapper->SetBlendModeToIsoSurface();
    // volumeMapper->SetSampleDistance(4);

    this->mVolume->SetMapper(volumeMapper);
    this->mVolume->SetProperty(volumeProperty);
}

void Volumes::SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines)
{
#ifndef NDEBUG
    std::cout << "VOLUMES: setting input connection" << std::endl;
#endif
    volumeMapper->SetInputConnection(pipelines->assignAttr->GetOutputPort());
}

void Volumes::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddVolume(mVolume);
}

void Volumes::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveVolume(mVolume);
}
