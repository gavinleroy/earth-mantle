#include "Scene.h"

Scene::Scene()
{
    pipelines = std::vector<std::unique_ptr<Pipe::Pipeline>>();

    // Insert all objects (currently no dynamic toggle).
    // pipelines.push_back(std::move(std::make_unique<Tube>()));
    // pipelines.push_back(std::move(std::make_unique<Volumes>()));
    // pipelines.push_back(std::move(std::make_unique<LIConvolution>()));
}

void Scene::InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
{
    // TODO: do we need a light source?
    // -- probably, Gavin

    this->renderer = renderer;
    for (auto &pipe : pipelines)
        pipe->ConnectToScene(renderer);
}

void Scene::InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor)
{
    /* TODO */
}

void Scene::ProcessInput(char input)
{
    if (input == 'v') {
        pipelines.push_back(std::move(std::make_unique<Volumes>()));
        pipelines.back()->ConnectToScene(this->renderer);
    }
    if (input == 't') {
        pipelines.push_back(std::move(std::make_unique<Tube>()));
        pipelines.back()->ConnectToScene(this->renderer);
    }
    if (input == 'l') {
        pipelines.push_back(std::move(std::make_unique<LIConvolution>()));
        pipelines.back()->ConnectToScene(this->renderer);
    }
}

void Scene::Update(double dt, double t)
{
    /* TODO */
}
