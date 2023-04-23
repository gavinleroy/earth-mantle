#include "Mantle.h"

Mantle::Mantle(std::filesystem::path data_dir)
    : data_directory(data_dir)
    , mVolume(vtkSmartPointer<vtkVolume>::New())
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    /* TODO */
}

Mantle::Mantle()
    : Mantle(
        "/Users/raphaelwinkler/CLionProjects/earth-mantle/data/FullMantle") /* FIXME we need this to
                                                                     be absolute from the
                                                                     ROOT_DIR */
{
    /* TODO */
}

vtkSmartPointer<vtkDataObject> Mantle::LoadFromFile(
    const std::string              fn,
    const std::vector<std::string> to_load /* define polymorphism over computation */)
{
    std::filesystem::path resolved = this->data_directory.append(fn);

    assert(std::filesystem::exists(resolved));

#ifndef NDEBUG
    std::cout << "Loading variables from absolute (" << resolved << ")" << std::endl;
    std::for_each(to_load.begin(), to_load.end(),
                  [](auto const &value) { std::cout << "  - " << value << std::endl; });
#endif

    vtkNew<vtkNetCDFCFReader> reader;
    reader->SetFileName(resolved.c_str());
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

void Mantle::LoadMantleSeries(/* uses current data directory */)
{
    /* TODO */
    throw "not yet implemented";
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
