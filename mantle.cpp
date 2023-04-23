#include "mantle.h"

Mantle::Mantle()
    : mVolume(vtkSmartPointer<vtkVolume>::New())
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    LoadMantleSet("/dev/null");
}

vtkSmartPointer<vtkDataObject> Mantle::LoadFromFile(
    const std::string              fn,
    const std::vector<std::string> to_load /* define polymorphism over computation */)
{
#ifndef NDEBUG
    std::cout << "Loading variables from file (" << fn << ")" << std::endl;
    std::for_each(to_load.begin(), to_load.end(),
                  [](auto const &value) { std::cout << "  - " << value << std::endl; });
#endif

    vtkNew<vtkNetCDFCFReader> reader;
    reader->SetFileName(fn.c_str());
    reader->UpdateMetaData();
    reader->SetSphericalCoordinates(true);
    reader->SetOutputTypeToStructured();

    auto set_to = [&reader](int v) {
        return [&reader, v](auto const &value) {
            reader->SetVariableArrayStatus(value.c_str(), v);
        };
    };

    // Unset all available variables
    std::for_each(this->variables.begin(), this->variables.end(), set_to(0));

    // Explicitly set the variables we want
    std::for_each(to_load.begin(), to_load.end(), set_to(1));

    reader->Update();

#ifndef NDEBUG
    reader->GetOutput()->Print(std::cout);
#endif

    return reader->GetOutput();
}

void Mantle::LoadMantleSet(std::filesystem::path data_dir)
{
    // TODO: load series from a specified directory (specify root via env var)
    const std::string fn = "../data/FullMantle/spherical001.nc";

    auto variables = std::vector<std::string>({
        "temperature",
        "vx",
        "vy",
        "vz",
    });

    auto loaded_from_file = LoadFromFile(fn, variables);

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

    std::for_each(variables.begin(), variables.end(),
                  [&assignAttribute](auto const &variable) {
                      assignAttribute->Assign(variable.c_str(), "SCALARS", "POINT_DATA");
                  });

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
    volumeMapper->SelectScalarArray("temperature");
    volumeMapper->AddClippingPlane(plane);
    // volumeMapper->SetBlendModeToIsoSurface();
    // volumeMapper->SetSampleDistance(4);

    // HACK: this seems very specific to a single pipeline, I(gavin) don't
    // know how this will interact with the other things we'll need.

    this->mVolume->SetMapper(volumeMapper);
    this->mVolume->SetProperty(volumeProperty);
}

void Mantle::Update()
{ /* UNIMPLEMENTED */
}

// TODO:
std::vector<vtkSmartPointer<vtkActor>> Mantle::GetActors()
{
    auto v = std::vector<vtkSmartPointer<vtkActor>>({ this->mActor });
    return v;
}

// TODO:
std::vector<vtkSmartPointer<vtkVolume>> Mantle::GetVolumes()
{
    auto v = std::vector<vtkSmartPointer<vtkVolume>>({ this->mVolume });
    return v;
}
