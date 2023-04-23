#pragma once


#include <memory>

#include <vtkLight.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>

#include "Tube.h"
#include "Volumes.h"
#include "LineIntegralConvolution.h"


class Scene {
private:
    Scene(const Scene &) = delete;           // Delete the copy-constructor.
    void operator=(const Scene &) = delete;  // Delete the assignment operator.

    std::unique_ptr<Tube>    tube;
    std::unique_ptr<Volumes> volumes;
    std::unique_ptr<LineIntegralConvolution> line_integral_convolution;

public:
    Scene()
        : tube(std::make_unique<Tube>())
        , volumes(std::make_unique<Volumes>())
        , line_integral_convolution(std::make_unique<LineIntegralConvolution>())
    {
    }

    void InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
    {
        // for (auto actor : mMantle->GetActors())
        //     renderer->AddActor(actor);

        for (auto volume : volumes->GetVolumes())
            renderer->AddVolume(volume);

        for (auto actor : tube->GetActors())
            renderer->AddActor(actor);

        for (auto volume : tube->GetVolumes())
            renderer->AddActor(volume);

        renderer->AddActor(line_integral_convolution->GetActor());

        // TODO: do we need a light source?
    }

    void InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor)
    {
        /* TODO */
    }

    /// <summary>
    /// Updates the content of the scene.
    /// </summary>
    /// <param name="dt">Time passed since the last Update in milliseconds.</param>
    /// <param name="t">Total time passed since start of the application in
    /// milliseconds.</param>
    void Update(double dt, double t)
    {
        /* TODO */
    }
};
