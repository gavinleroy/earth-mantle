#include "Scene.h"
#include "LIConvolution.h"
#include <memory>

Scene::Scene()
{
#ifndef NDEBUG
    std::cout << "SCENE: Initializing new scene" << std::endl;
#endif
    inputPipelines = std::make_shared<Pipe::AllInput>();

#ifndef NDEBUG
    std::cout << "SCENE: building new mappings" << std::endl;
#endif

    // Initialize earth mappings
    currentEarthMapper = {};
    earthMappers       = EarthMappings({
        {
            EarthView::LIC,
            std::make_shared<LIConvolution>(),
        },
        {
            EarthView::Contour,
            std::make_shared<Contour>(),
        },
    });

#ifndef NDEBUG
    std::cout << "SCENE: building new volumes" << std::endl;
#endif

    currentEarthVolume = {};
    earthVolumes       = VolumeMappings({
        {
            VolumeView::Volume,
            std::make_shared<Volumes>(),
        },
        {
            VolumeView::Slabs,
            std::make_shared<Slabs>(),
        },
        {
            VolumeView::Plumes,
            std::make_shared<Plumes>(),
        },
        {
            VolumeView::IsoVolume,
            std::make_shared<IsoVolume>(),
        },
        {
            VolumeView::Tubes,
            std::make_shared<Tube>(),
        },
    });
}

void Scene::InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
{
#ifndef NDEBUG
    std::cout << "SCENE: Initializing renderer" << std::endl;
#endif

    // TODO: lights sources, camera, etc.
    this->renderer = renderer;

    if (this->currentEarthVolume.has_value())
        SetVolume(this->currentEarthVolume.value());

    if (this->currentEarthMapper.has_value())
        SetMapping(this->currentEarthMapper.value());
    else {
#ifndef NDEBUG
        std::cout << "SCENE: No active mapping set" << std::endl;
#else
        ;
#endif
    }
}

void Scene::SetMapping(EarthView idx)
{
#ifndef NDEBUG
    std::cout << "SCENE: Setting mapping" << std::endl;
#endif
    auto mapper = this->earthMappers[idx];
    mapper->SetInputConnection(this->inputPipelines);

    vtkNew<vtkActor> act;
    mapper->ConnectToActor(act);
    renderer->AddActor(act);
}

void Scene::SetVolume(VolumeView idx)
{
#ifndef NDEBUG
    std::cout << "SCENE: Setting mapping" << std::endl;
#endif
    auto volume = this->earthVolumes[idx];
    volume->SetInputConnection(this->inputPipelines);
    volume->ConnectToScene(this->renderer);
}

void Scene::InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor)
{
    std::cout << std::endl
              << "Switch between mappings:" << std::endl
              << "c: Contour" << std::endl
              << "l: LIConvolution (don't press twice)" << std::endl;

    std::cout << std::endl
              << "Toggle volume views:" << std::endl
              << "v: Volumes" << std::endl
              << "s: Cold slabs" << std::endl
              << "p: Hot plumes" << std::endl
              << "i: IsoVolumes" << std::endl
              << "t: Tubes" << std::endl;
}

void Scene::ProcessInput(std::string input)
{
    // FIXME: CRASHES IF "l" is PRESSED TWICE (segfault)
    if (input.compare("c") == 0)
        SwitchMapping(EarthView::Contour);
    if (input.compare("l") == 0)
        SwitchMapping(EarthView::LIC);

    if (input.compare("v") == 0)
        ToggleVolume(VolumeView::Volume);
    if (input.compare("s") == 0)
        ToggleVolume(VolumeView::Slabs);
    if (input.compare("p") == 0)
        ToggleVolume(VolumeView::Plumes);
    if (input.compare("i") == 0)
        ToggleVolume(VolumeView::IsoVolume);
    if (input.compare("t") == 0)
        ToggleVolume(VolumeView::Tubes);

    if (input.compare("a") == 0) {
        int nrOfActors = renderer->GetActors()->GetNumberOfItems();
        std::cout << "Number of actors: " << nrOfActors << std::endl;
    }
}

void Scene::SwitchMapping(EarthView idx)
{
#ifndef NDEBUG
    std::cout << "SCENE: Switching mapping" << std::endl;
#endif
    renderer->RemoveAllViewProps();
    SetMapping(idx);
    for (auto volumeIdx : this->currentEarthVolumes) {
        SetVolume(volumeIdx);
    }
}

void Scene::ToggleVolume(VolumeView idx)
{
#ifndef NDEBUG
    std::cout << "SCENE: Toggling volume" << std::endl;
#endif
    std::vector<VolumeView> remainingEarthVolumes;
    std::remove_copy(this->currentEarthVolumes.begin(), this->currentEarthVolumes.end(),
                     std::back_inserter(remainingEarthVolumes), idx);
    if (remainingEarthVolumes.size() == this->currentEarthVolumes.size()) {
        remainingEarthVolumes.push_back(idx);
        SetVolume(idx);
    } else
        this->earthVolumes[idx]->RemoveFromScene(this->renderer);

    this->currentEarthVolumes = remainingEarthVolumes;
}

void Scene::Update(double dt, double t)
{
    // inputPipelines->geometry->IncreaseInnerRadius();
    // TODO
}
