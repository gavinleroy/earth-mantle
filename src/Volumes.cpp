#include "Volumes.h"
#include <vtkAssignAttribute.h>
#include <vtkDataSetAttributes.h>

Volumes::Volumes()
    : Mantle()
    , mVolume(vtkSmartPointer<vtkVolume>::New())
{
    auto variables = std::vector<std::string>({
        "temperature",
        "vx",
        "vy",
        "vz",
    });

    std::string property = "temperature";

    auto loaded_from_file = LoadFromFile("spherical001.nc", variables);

    vtkNew<vtkCellDataToPointData> cellToPoint;
    cellToPoint->SetInputData(loaded_from_file);
    cellToPoint->Update();

    auto structured_grid = cellToPoint->GetStructuredGridOutput();

    vtkNew<vtkResampleToImage> resampler;
    resampler->SetInputDataObject(structured_grid);
    resampler->SetSamplingDimensions(100, 100, 100);
    resampler->Update();

#ifndef NDEBUG
    resampler->GetOutput()->Print(std::cout);
#endif

    vtkNew<vtkAssignAttribute> assignAttribute;
    assignAttribute->SetInputConnection(resampler->GetOutputPort());
    assignAttribute->Assign(property.c_str(), vtkDataSetAttributes::SCALARS,
                            vtkAssignAttribute::POINT_DATA);
    assignAttribute->Update();

#ifndef NDEBUG
    assignAttribute->GetOutput()->Print(std::cout);
#endif

    // Create a color transfer function
    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->AddRGBPoint(293, 0.0, 0.0, 1.0);
    colorTransferFunction->AddRGBPoint(1200, 1.0, 1.0, 1.0);
    colorTransferFunction->AddRGBPoint(3608, 1.0, 0.0, 0.0);

    // Create a piecewise function
    vtkNew<vtkPiecewiseFunction> opacityTransferFunction;
    opacityTransferFunction->AddPoint(293, 1.0);
    opacityTransferFunction->AddPoint(3608, 1.0);

    vtkNew<vtkVolumeProperty> volumeProperty;
    volumeProperty->ShadeOn();
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);

    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(0., 0., 0.);
    plane->SetNormal(0., 1.0, -1.);


    // Add a mapper to create graphic primitives from the data
    vtkNew<vtkSmartVolumeMapper> volumeMapper;
    volumeMapper->SetInputConnection(assignAttribute->GetOutputPort());
    volumeMapper->SelectScalarArray(property.c_str());
    volumeMapper->AddClippingPlane(plane);
    // volumeMapper->SetBlendModeToIsoSurface();
    // volumeMapper->SetSampleDistance(4);

    // HACK: this seems very specific to a single pipeline, I(gavin) don't
    // know how this will interact with the other things we'll need.

    this->mVolume->SetMapper(volumeMapper);
    this->mVolume->SetProperty(volumeProperty);
}


std::vector<vtkSmartPointer<vtkVolume>> Volumes::GetVolumes()
{
    return std::vector<vtkSmartPointer<vtkVolume>>({ this->mVolume });
}

void Volumes::Update()
{
    /* TODO */
}
