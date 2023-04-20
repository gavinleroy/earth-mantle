//
// Created by Raphael Winkler on 01.04.23.
//

#ifndef EARTH_MANTLE_WINDOW_H
#define EARTH_MANTLE_WINDOW_H

#endif //EARTH_MANTLE_WINDOW_H


#pragma once

#include "scene.h"
#include "earth.h"

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTerrain.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>

#include <chrono>
#include <algorithm>

/// <summary>
/// Class for the listening to mouse events and inherits its interaction from VTK's "Terrain" style.
/// </summary>
class PickingInteractorStyle : public vtkInteractorStyleTerrain
{
public:
    static PickingInteractorStyle* New();
    vtkTypeMacro(PickingInteractorStyle, vtkInteractorStyleTerrain);


    /// <summary>
    /// Sets the scene that the picking is forwarded to.
    /// </summary>
    /// <param name="scene">Scene that is picked.</param>
    void SetScene(std::shared_ptr<Scene> scene) { mScene = scene; }

private:
    /// <summary>
    /// Computes the ground plane intersection of the ray through the picked pixel.
    /// </summary>
    /// <returns>3D world coordinate of the picked location.</returns>
//    Vector3d PickPlane()
//    {
//        int* clickPos = this->GetInteractor()->GetEventPosition();
//        int* size = this->GetDefaultRenderer()->GetVTKWindow()->GetSize();
//        double aspect = size[0] / (double)size[1];
//        vtkCamera* camera = this->GetDefaultRenderer()->GetActiveCamera();
//        auto matrix = camera->GetCompositeProjectionTransformMatrix(aspect, 0, 1);
//        Matrix4d m;
//        std::copy(matrix->GetData(), matrix->GetData() + 16, m.data());
//        m.transposeInPlace();
//        Matrix4d minv = m.inverse();
//        Vector4d npc0(clickPos[0] / (size[0] - 1.) * 2 - 1, clickPos[1] / (size[1] - 1.) * 2 - 1, 0, 1);
//        Vector4d npc1(clickPos[0] / (size[0] - 1.) * 2 - 1, clickPos[1] / (size[1] - 1.) * 2 - 1, 1, 1);
//        Vector4d world0 = minv * npc0;
//        world0 /= world0.w();
//        Vector4d world1 = minv * npc1;
//        world1 /= world1.w();
//        Vector3d l(world1.x() - world0.x(), world1.y() - world0.y(), world1.z() - world0.z());
//        l.normalize();
//        Vector3d l0(world0.x(), world0.y(), world0.z());
//        Vector3d n(0, 0, 1);
//        double t = -l0.dot(n) / l.dot(n);
//        Vector3d world = l0 + t * l;
//        return world;
//    }

    std::shared_ptr<Scene> mScene;		// Scene containing all elements of the 3D scene.
};
vtkStandardNewMacro(PickingInteractorStyle);

/// <summary>
/// Class that handles the window.
/// </summary>
class Window
{
public:
    /// <summary>
    /// Constructor.
    /// </summary>
    Window()
    {
        // create renderer
        mRenderer = vtkSmartPointer<vtkRenderer>::New();
        mRenderer->SetBackground(.2, .2, .2);
        mRenderer->GetActiveCamera()->SetViewUp(0, 0, 1);
        mRenderer->GetActiveCamera()->SetPosition(0, -100000, 2);
        mRenderer->UseDepthPeelingOn();

        // create scene
        mScene = std::make_shared<Scene>();
        mScene->InitRenderer(mRenderer);

        // create render window
        mRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
        mRenderWindow->SetSize(800, 600);
        mRenderWindow->AddRenderer(mRenderer);

        // create picking style
        vtkNew<PickingInteractorStyle> interactorStyle;
        interactorStyle->SetDefaultRenderer(mRenderer);
        interactorStyle->SetScene(mScene);

        // create interactor
        mRenderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        mRenderWindowInteractor->SetInteractorStyle(interactorStyle);
        mRenderWindowInteractor->SetRenderWindow(mRenderWindow);
        mRenderWindowInteractor->EnableRenderOff();
        mRenderWindowInteractor->Initialize();
        mScene->InitUI(mRenderWindowInteractor);

        // finally set the window title
        mRenderWindow->SetWindowName("Earth Mantle");
    }

    /// <summary>
    /// Render and update loop.
    /// </summary>
    void Loop()
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto last = start;
        while (mRenderWindow->GetGenericWindowId())
        {
            // compute the timing deltas
            auto now = std::chrono::high_resolution_clock::now();
            double dt = std::chrono::duration<double, std::milli>(now - last).count();
            double t = std::chrono::duration<double, std::milli>(now - start).count();
            last = now;

            // update the scene
            mScene->Update(dt, t);

            // render the frame anew
            mRenderWindow->Render();
            mRenderWindowInteractor->ProcessEvents();
        }
        mRenderWindow->Finalize();
        mRenderWindow->GetInteractor()->TerminateApp();
    }

private:
    vtkSmartPointer<vtkRenderer> mRenderer;								// Renderer that contains the scene.
    vtkSmartPointer<vtkRenderWindow> mRenderWindow;						// Class that creates the window.
    vtkSmartPointer<vtkRenderWindowInteractor> mRenderWindowInteractor;	// Interactor that handles user interactions.
    std::shared_ptr<Scene> mScene;										// Scene containing all elements of the 3D scene.
};
