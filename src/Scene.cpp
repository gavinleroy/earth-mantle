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

    for (auto &pipe : pipelines)
        pipe->ConnectToScene(renderer);
}

void Scene::InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor)
{
    /* TODO */
}

void Scene::Update(double dt, double t)
{
    /* TODO */
}
