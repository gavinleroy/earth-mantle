#include "Volumes.h"
#include <vtkAssignAttribute.h>
#include <vtkDataSetAttributes.h>

Volumes::Volumes()
    : Mantle()
    , mVolume(vtkSmartPointer<vtkVolume>::New())
{
    MantleIO::MantleAttr property = MantleIO::MantleAttr::Temp;
    auto                 fromFile = GetCurrentStream();

    vtkNew<vtkCellDataToPointData> cellToPoint;
    cellToPoint->SetInputConnection(fromFile->GetOutputPort());

    auto structured_grid = cellToPoint->GetStructuredGridOutput();

    vtkNew<vtkResampleToImage> resampler;
    resampler->SetInputConnection(cellToPoint->GetOutputPort());
    resampler->SetSamplingDimensions(100, 100, 100);

#ifndef NDEBUG
    resampler->GetOutput()->Print(std::cout);
#endif

    vtkNew<vtkAssignAttribute> assignAttribute;
    assignAttribute->SetInputConnection(resampler->GetOutputPort());
    assignAttribute->Assign(property.c_str(), vtkDataSetAttributes::SCALARS,
                            vtkAssignAttribute::POINT_DATA);

#ifndef NDEBUG
    assignAttribute->GetOutput()->Print(std::cout);
#endif

    double range[] = { 0., 3610. };

    // Create a color transfer function
    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->SetNanOpacity(1.0);
    colorTransferFunction->SetColorSpaceToRGB();
    colorTransferFunction->SetScaleToLinear();
    colorTransferFunction->AdjustRange(range);
    colorTransferFunction->AddRGBPoint(0, 0., 1., 1.);
    colorTransferFunction->AddRGBPoint(1623, 0., 0., 1.);
    colorTransferFunction->AddRGBPoint(1803, 0., 0., 0.);
    colorTransferFunction->AddRGBPoint(1995, 1.0, 0., 0.);
    colorTransferFunction->AddRGBPoint(3608, 1.0, 1.0, 0.0);

    // Create a piecewise function
    vtkNew<vtkPiecewiseFunction> opacityTransferFunction;
    opacityTransferFunction->AddPoint(0, 0.8);
    opacityTransferFunction->AddPoint(3608, 0.8);

    vtkNew<vtkVolumeProperty> volumeProperty;
    volumeProperty->ShadeOn();
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

void Volumes::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddVolume(mVolume);
}

void Volumes::Update() { }
