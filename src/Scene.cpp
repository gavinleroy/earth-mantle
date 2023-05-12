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
    //std::cout << "w: toggle wireframe" << std::endl;
    //std::cout << "s: step forward" << std::endl;
    //std::cout << "b: step backward" << std::endl;
}

void Scene::ProcessInput(char* input)
{
    // TODO: only render one pipeline at a time
    std::cout << "Input: " << input << std::endl;
    if (strcmp(input, "l") == 0) SwitchPipeline(0);
    if (strcmp(input, "t") == 0) SwitchPipeline(1);
    if (strcmp(input, "v") == 0) SwitchPipeline(2);
    if (strcmp(input, "c") == 0) SwitchPipeline(3);

    // add more controls here if needed ...
}

void Scene::SwitchPipeline(int index)
{
    //remove old pipeline
    for (int i = 0; i < 3; i++) {
        if (pipelines[i] != nullptr) {
            std::cout << "Remove pipeline from Scene." << std::endl;
            pipelines[i]->RemoveFromScene(this->renderer);
        }
    }
    if (pipelines[index] == nullptr) {
        switch(index) {
            case 0:
                pipelines[0] = std::make_unique<LIConvolution>();
                break;
            case 1:
                pipelines[1] = std::make_unique<Tube>();
                break;
            case 2:
                pipelines[2] = std::move(std::make_unique<Volumes>());
                break;
            case 3:
                pipelines[3] = std::move(std::make_unique<Contour>());
                break;
            default:
                std::cout << "Invalid pipeline index." << std::endl;
                return;
        }
    }
    std::cout << "Connect pipeline to Scene." << std::endl;
    pipelines[index]->ConnectToScene(this->renderer);
}


void Scene::Update(double dt, double t)
{
    /* TODO */
}
