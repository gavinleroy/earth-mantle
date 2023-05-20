#include "Scene.h"
#include "LIConvolution.h"
#include <memory>

Scene::Scene()
{
#ifndef NDEBUG
    std::cout << "SCENE: Initializing new scene" << std::endl;
#endif
    geometry = std::make_unique<Geometry::Geometry>();

    reader             = std::make_shared<MantleIO::Mantle>();
    resampled          = std::make_shared<Pipe::Resample>();
    velocityCalculator = std::make_shared<Pipe::VelocityCalculator>();
    assignAttr         = std::make_shared<Pipe::TempAnomAttribute>();

    // Initialize default pipeline starts

    // mantle -> resampled
    resampled->SetInputConnection(reader->GetOutputPort());

    // mantle -> resampled -> attribute assigned
    assignAttr->SetInputConnection(resampled->GetOutputPort());

    // mantle -> resampled -> velocity
    velocityCalculator->SetInputConnection(resampled->GetOutputPort());

    // Initialize earth mappings
    currentEarthMapper = EarthView::LIC;
    earthMappers       = EarthMappings({
        { EarthView::LIC,
                { .mapper = std::make_shared<LIConvolution>(  // velocityCalculator->GetOutputPort()
                ),
                  .input = velocityCalculator } }
        // TODO: add the rest of the mappings
    });

    currentEarthVolume = {};
    earthVolumes       = VolumeMappings();
}

void Scene::InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
{
#ifndef NDEBUG
    std::cout << "SCENE: Initializing renderer" << std::endl;
#endif

    // TODO: lights sources, camera, etc.
    this->renderer = renderer;

    this->geometry->ConnectToScene(this->renderer);

    //     if (this->currentEarthMapper.has_value())
    //         SetMapping(this->currentEarthMapper.value());
    //     else {
    // #ifndef NDEBUG
    //         std::cout << "SCENE: No active mapping set" << std::endl;
    // #else
    //         ;
    // #endif
    //     }
}

void Scene::SetMapping(EarthView idx)
{
#ifndef NDEBUG
    std::cout << "SCENE: Setting mapping" << std::endl;
#endif
    auto data = this->earthMappers[idx];
    data.mapper->SetInputConnection(data.input->GetOutputPort());

    vtkNew<vtkActor> act;
    data.mapper->ConnectToActor(act);
    renderer->AddActor(act);

    // this->geometry->SetActiveMapper(data.mapper);
}

void Scene::InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor)
{
    // print avilable input options
    std::cout << "Available input options:" << std::endl;
    // std::cout << "l: switch to LIConvolution pipeline" << std::endl;
    // std::cout << "t: switch to Tube pipeline" << std::endl;
    // std::cout << "v: switch to Volumes pipeline" << std::endl;
    // std::cout << "c: switch to Contour pipeline" << std::endl;
    // std::cout << "i: switch to IsoVolumes pipeline" << std::endl;
    // std::cout << "w: toggle wireframe" << std::endl;
    // std::cout << "s: step forward" << std::endl;
    // std::cout << "b: step backward" << std::endl;
}

void Scene::ProcessInput(char *input)
{
    // TODO: only render one pipeline at a time
    std::cout << "Input: " << input << std::endl;
    if (strcmp(input, "l") == 0)
        SwitchPipeline(0);
    if (strcmp(input, "t") == 0)
        SwitchPipeline(1);
    if (strcmp(input, "v") == 0)
        SwitchPipeline(2);
    if (strcmp(input, "c") == 0)
        SwitchPipeline(3);
    if (strcmp(input, "i") == 0)
        SwitchPipeline(4);

    std::cout << "Ignoring for now >:)" << std::endl;

    // add more controls here if needed ... (yes, pls)
}

void Scene::SwitchPipeline(int index)
{
    // // remove old pipeline
    // for (const auto &pipeline : pipelines) {
    //     if (pipeline != nullptr) {
    //         std::cout << "Remove pipeline from Scene." << std::endl;
    //         pipeline->RemoveFromScene(this->renderer);
    //     }
    // }

    // if (pipelines[index] == nullptr) {
    //     switch (index) {
    //     case 0:
    //         pipelines[0] = std::make_unique<LIConvolution>();
    //         break;
    //     case 1:
    //         pipelines[1] = std::make_unique<Tube>();
    //         break;
    //     case 2:
    //         pipelines[2] = std::move(std::make_unique<Volumes>());
    //         break;
    //     case 3:
    //         pipelines[3] = std::make_unique<Contour>();
    //         break;
    //     case 4:
    //         pipelines[4] = std::make_unique<IsoVolume>();
    //         break;
    //     default:
    //         std::cout << "Invalid pipeline index." << std::endl;
    //         return;
    //     }
    // }
    // std::cout << "Connect pipeline to Scene." << std::endl;
    // pipelines[index]->ConnectToScene(this->renderer);
}

void Scene::Update(double dt, double t)
{
    // TODO
}
