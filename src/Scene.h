#pragma once


#include <memory>

#include <vtkLight.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>

#include "Tube.h"
#include "Volumes.h"
#include "LIConvolution.h"
#include "Pipeline.h"

class Scene {
private:
    Scene(const Scene &) = delete;           // Delete the copy-constructor.
    void operator=(const Scene &) = delete;  // Delete the assignment operator.

    std::vector<std::unique_ptr<Pipe::Pipeline>> pipelines;

public:
    Scene();
    void InitRenderer(vtkSmartPointer<vtkRenderer> renderer);
    void InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor);
    void Update(double dt, double t);
};
