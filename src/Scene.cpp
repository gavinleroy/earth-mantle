#include "Scene.h"

Scene::Scene()
{
    // TODO: initialize scene with a default pipeline (e.g. wireframe)
}

void Scene::InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
{
    // TODO: lights sources, camera, etc.
    this->renderer = renderer;
}

void Scene::InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor)
{
    // print avilable input options
    std::cout << "Available input options:" << std::endl;
    std::cout << "l: switch to LIConvolution pipeline" << std::endl;
    std::cout << "t: switch to Tube pipeline" << std::endl;
    std::cout << "v: switch to Volumes pipeline" << std::endl;
    std::cout << "c: switch to Contour pipeline" << std::endl;
    std::cout << "i: switch to IsoVolumes pipeline" << std::endl;
}

void Scene::ProcessInput(std::string input)
{
    if (input.compare("l") == 0) ConnectPipeline(liconvolution);
    if (input.compare("t") == 0) ConnectPipeline(tube);
    if (input.compare("v") == 0) ConnectPipeline(volumes);
    if (input.compare("c") == 0) ConnectPipeline(contour);
    if (input.compare("i") == 0) ConnectPipeline(isovolume);

    // add more controls here if needed ...
}

std::unique_ptr<Pipe::Pipeline> Scene::NewPipeline(pipelineType pipeType) 
{
    switch(pipeType) {
        case volumes:
            return std::make_unique<Volumes>(); 
        case tube:
            return std::make_unique<Tube>();
        case liconvolution:
            return std::make_unique<LIConvolution>();
        case contour:
            return std::make_unique<Contour>();
        case isovolume:
            return std::make_unique<IsoVolume>();
        default:
            std::cout << "Invalid pipeline type." << std::endl;
            return nullptr;
    }
}

void Scene::ConnectPipeline(pipelineType pipeType)
{
    // Disconnect all pipelines
    for (const auto & pipeline : pipelines) {
        if (pipeline != nullptr) {
            pipeline->RemoveFromScene(this->renderer);
        }
    }

    // Find the selected pipeline
    auto it = std::find(pipeTypes.begin(), pipeTypes.end(), pipeType);
    int idx = it - pipeTypes.begin();

    // If the selected pipeline is not found, create a new one
    if (it == pipeTypes.end()) {
        pipelines.push_back(NewPipeline(pipeType));
        pipeTypes.push_back(pipeType);
        idx = pipelines.size() - 1;
    }

    // Connect the selected pipeline
    pipelines[idx]->ConnectToScene(this->renderer);

    std::string pipeTypeStr = typeid(*pipelines[idx]).name();
    pipeTypeStr = pipeTypeStr.erase(0, pipeTypeStr.find_first_not_of("0123456789"));
    std::cout << pipeTypeStr << " pipeline connected." << std::endl;
}

void Scene::Update(double dt, double t)
{
    /* TODO */
}
