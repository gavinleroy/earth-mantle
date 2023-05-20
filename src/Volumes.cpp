#include "Volumes.h"

Volumes::Volumes()
{
    this->mVolume         = vtkSmartPointer<vtkVolume>::New();
    this->assignAttribute = vtkSmartPointer<vtkAssignAttribute>::New();

    MantleIO::MantleAttr property = MantleIO::MantleAttr::TempAnom;

    assignAttribute->Assign(property.c_str(), vtkDataSetAttributes::SCALARS,
                            vtkAssignAttribute::POINT_DATA);

#ifndef NDEBUG
    assignAttribute->GetOutput()->Print(std::cout);
#endif

    double range[] = { -1100., 1100. };

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
    vtkNew<vtkSmartVolumeMapper> volumeMapper;
    volumeMapper->SetInputConnection(assignAttribute->GetOutputPort());
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
    // TODO adjust this offset so the scene looks nice
    // this->mVolume->SetPosition(14000, 0, 14000);
}

void Volumes::SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines)
{
    throw "NYI";
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
