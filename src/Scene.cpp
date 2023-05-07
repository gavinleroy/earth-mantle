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

    // add more controls here if needed ...
}

void Scene::SwitchPipeline(int index)
{
    if (pipelines[index] != nullptr) {
        std::cout << "Pipeline already exists." << std::endl;
        return;
    }
    std::string pipelineType = "None";
    switch(index) {
        case 0:
            pipelineType = "LIConvolution";
            pipelines[0] = std::make_unique<LIConvolution>();
            break;
        case 1:
            pipelineType = "Tube";
            pipelines[1] = std::make_unique<Tube>();
            break;
        case 2:
            pipelineType = "Volumes";
            std::cout << pipelineType << " pipeline created." << std::endl;
            pipelines[2] = std::move(std::make_unique<Volumes>());
            break;
        default:
            std::cout << "Invalid pipeline index." << std::endl;
            return;
    }
    std::cout << pipelineType << " pipeline created." << std::endl;
    pipelines[index]->ConnectToScene(this->renderer);
    std::cout << "Pipeline connected to Scene." << std::endl;
}

void Scene::Update(double dt, double t)
{
    /* TODO */
}
