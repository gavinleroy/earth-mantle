#pragma once

#include "earth.h"
#include "mantle.h"

#include <memory>

#include <vtkLight.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>

/// <summary>
/// Class that stores all content of the 3D scene.
/// </summary>
class Scene {
public:
    /// <summary>
    /// Constructor. Allocates the scene content.
    /// </summary>
    Scene()
        : mEarth(std::make_unique<Earth>())
        , mMantle(std::make_unique<Mantle>())
    {
    }

    /// <summary>
    /// Initialization function that lets all scene elements add their props to the renderer.
    /// </summary>
    /// <param name="renderer">Renderer to add the props to.</param>
    void InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
    {
        mEarth->InitRenderer(renderer);


        // mMantle->InitRenderer(renderer);

        // HACK: just trying to get things working.
        //       XXX: this currently doesn't work, we probably
        //       should start more basic and use a director volume
        //       rendering rather than going through the actor.
        // for (auto actor : m.GetActors())
        //     mRenderer->AddActor(actor);

        for (auto volume : mMantle->GetVolumes())
            renderer->AddVolume(volume);


        // create a light source
        //		vtkNew<vtkLight> light;
        //		light->SetPosition(0, 0, 0);
        //		light->SetIntensity(10);
        //		light->SetConeAngle(90);
        //		light->PositionalOn();
        //		renderer->AddLight(light);
    }

    /// <summary>
    /// Initialization function that allows scene elements to add UI elements to the
    /// interactor.
    /// </summary>
    /// <param name="renderWindowInteractor">Interactor to add UI elements to.</param>
    void InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor)
    {
        //		mJacobiConstant->InitUI(renderWindowInteractor);
    }

    /// <summary>
    /// Updates the content of the scene.
    /// </summary>
    /// <param name="dt">Time passed since the last Update in milliseconds.</param>
    /// <param name="t">Total time passed since start of the application in
    /// milliseconds.</param>
    void Update(double dt, double t)
    {
        mEarth->Update(dt, t * 0.001);
        mMantle->Update();
    }

private:
    Scene(const Scene &)          = delete;  // Delete the copy-constructor.
    void operator=(const Scene &) = delete;  // Delete the assignment operator.

    std::unique_ptr<Earth>  mEarth;  // Second massive body: Earth
    std::unique_ptr<Mantle> mMantle;
};
