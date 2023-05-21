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
    currentEarthMapper = EarthView::Contour;
    earthMappers       = EarthMappings({
        {
            EarthView::LIC,
            std::make_shared<LIConvolution>(),
        },
        {
            EarthView::Contour,
            std::make_shared<Contour>(),
        },
        // TODO: add the rest of the mappings
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
    std::cout << "Switch between mappings:" << std::endl;
    std::cout << "c: Contour" << std::endl << std::endl;
    std::cout << "l: LIConvolution" << std::endl;

    std::cout << "Toggle volume views:" << std::endl;
    std::cout << "v: Volumes" << std::endl;
    std::cout << "i: IsoVolumes" << std::endl;
    std::cout << "t: Tubes" << std::endl << std::endl;
}

void Scene::ProcessInput(std::string input)
{
    if (input.compare("c") == 0) SwitchMapping(EarthView::Contour);
    // CRASHES IF PRESSED TWICE
    if (input.compare("l") == 0) SwitchMapping(EarthView::LIC);

    if (input.compare("v") == 0) ToggleVolume(VolumeView::Volume);
    if (input.compare("i") == 0) ToggleVolume(VolumeView::IsoVolume);
    if (input.compare("t") == 0) ToggleVolume(VolumeView::Tubes);

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
}

void Scene::ToggleVolume(VolumeView idx)
{
#ifndef NDEBUG
    std::cout << "SCENE: Toggling volume" << std::endl;
#endif

    SetVolume(idx);
}

void Scene::Update(double dt, double t)
{
    // inputPipelines->geometry->IncreaseInnerRadius();
    // TODO
}
