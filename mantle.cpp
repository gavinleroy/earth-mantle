#include "mantle.h"

Mantle::Mantle()
    : mVolume(vtkSmartPointer<vtkVolume>::New())
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    LoadMantleSet("/dev/null");
}

void Mantle::LoadFromFile(
    const std::string fn,
    const std::string variable /* define polymorphism over computation */)
{
#ifndef NDEBUG
    std::cout << "Loading variable: " << variable << " from file: " << fn << std::endl;
#endif

    vtkNew<vtkNetCDFCFReader> reader;

    reader->SetFileName(fn.c_str());
    reader->UpdateMetaData();

    // Unset all available variables
    std::for_each(
        std::rbegin(this->variables), std::rend(this->variables),
        [&](auto const &value) { reader->SetVariableArrayStatus(value.c_str(), 0); });

    // Explicitly set the variables we want
    reader->SetVariableArrayStatus(variable.c_str(), 1);

    reader->Update();

#ifndef NDEBUG
    reader->GetOutput()->Print(std::cout);
#endif

    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkStructuredGrid::SafeDownCast(
        reader->GetOutput());


#ifndef NDEBUG
    structuredGrid->Print(std::cout);
#endif

    vtkNew<vtkDataSetMapper> mapper;
    mapper->SetInputData(structuredGrid);

    double range[2];
    structuredGrid->GetCellData()->GetArray(variable.c_str())->GetRange(range);

#ifndef NDEBUG
    std::cout << range[0] << " " << range[1] << std::endl;
#endif

    mapper->SetScalarRange(range);
    mapper->SelectColorArray(variable.c_str());

    // HACK: this seems very specific to a single pipeline, I(gavin) don't
    // know how this will interact with the other things we'll need.
    this->mActor->SetMapper(mapper);
    this->mActor->GetProperty()->SetRepresentationToWireframe();
}

void Mantle::LoadMantleSet(std::filesystem::path data_dir)
{
    // TODO: load series from a specified directory (specify root via env var)
    const std::string fn_001 = "../data/FullMantle/spherical001.nc";

    LoadFromFile(fn_001, "temperature");
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
