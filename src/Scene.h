#pragma once


#include <memory>
#include <string>
#include <iostream>

#include <vtkLight.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>

#include "Tube.h"
#include "Volumes.h"
#include "LIConvolution.h"
#include "Pipeline.h"
#include "Contour.h"

class Scene {
private:
    Scene(const Scene &) = delete;           // Delete the copy-constructor.
    void operator=(const Scene &) = delete;  // Delete the assignment operator.
    /* Pipeline pointers:
        * 0: LIConvolution
        * 1: Tube
        * 2: Volumes
    */
    std::unique_ptr<Pipe::Pipeline> pipelines[4];
    vtkSmartPointer<vtkRenderer> renderer;

public:
    Scene();
    void InitRenderer(vtkSmartPointer<vtkRenderer> renderer);
    void InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor);
    void ProcessInput(char* input);
    void SwitchPipeline(int index);
    void Update(double dt, double t);
};
