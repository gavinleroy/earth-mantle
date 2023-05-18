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
#include "IsoVolume.h"

class Scene {
private:
    Scene(const Scene &) = delete;           // Delete the copy-constructor.
    void operator=(const Scene &) = delete;  // Delete the assignment operator.
    vtkSmartPointer<vtkRenderer> renderer;

    enum pipelineType
    {
        volumes,
        tube,
        liconvolution,
        contour,
        isovolume,
    };
    std::vector<pipelineType> pipeTypes;
    std::vector<std::unique_ptr<Pipe::Pipeline>> pipelines;
    
    std::unique_ptr<Pipe::Pipeline> NewPipeline(pipelineType pipeType);
    void ConnectPipeline(pipelineType pipeType);
    
public:
    Scene();
    void InitRenderer(vtkSmartPointer<vtkRenderer> renderer);
    void InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor);
    void ProcessInput(std::string input);
    void Update(double dt, double t);
};
