#include "Scene.h"

Scene::Scene()
{
    pipelines = std::vector<std::unique_ptr<Pipe::Pipeline>>();

    // TODO: initialize scene with a default pipeline
    // pipelines.push_back(std::move(std::make_unique<Wireframe>()));
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
    // TODO: create GUI with elements like buttons, sliders, etc ...

    // print avilable input options
    std::cout << "Available input options:" << std::endl;
    std::cout << "l: load LIConvolution pipeline" << std::endl;
    std::cout << "t: load Tube pipeline" << std::endl;
    std::cout << "v: load Volumes pipeline" << std::endl;
    //std::cout << "w: toggle wireframe" << std::endl;
    //std::cout << "s: step forward" << std::endl;
    //std::cout << "b: step backward" << std::endl;
}

void Scene::ProcessInput(char input)
{
    // TODO: only load the pipeline if it is not already loaded
    // TODO: only render one pipeline at a time
    if (input == 'l') {
        pipelines.push_back(std::move(std::make_unique<LIConvolution>()));
        pipelines.back()->ConnectToScene(this->renderer);
    }
    if (input == 't') {
        pipelines.push_back(std::move(std::make_unique<Tube>()));
        pipelines.back()->ConnectToScene(this->renderer);
    }
    if (input == 'v') {
        pipelines.push_back(std::move(std::make_unique<Volumes>()));
        pipelines.back()->ConnectToScene(this->renderer);
    }
    if (input == 'w') {
        // toggle wireframe
        // -- I think this should be loaded when scene is initialized (Rasmus)
    }

    // TODO: add contols for time stepping
    if (input == 's') {
        // step forward
    }
    if (input == 'b') {
        // step backward
    }

    // add more controls here if needed ...
}

void Scene::SwitchPipeline(int index)
{
    /* TODO */
}

void Scene::Update(double dt, double t)
{
    /* TODO */
}
