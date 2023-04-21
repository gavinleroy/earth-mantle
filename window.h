//
// Created by Raphael Winkler on 01.04.23.
//
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
/// Class for the listening to mouse events and inherits its interaction from VTK's
/// "Terrain" style.
/// </summary>
class PickingInteractorStyle : public vtkInteractorStyleTerrain {
private:
    std::shared_ptr<Scene> mScene;  // Scene containing all elements of the 3D scene.

public:
    static PickingInteractorStyle *New();
    vtkTypeMacro(PickingInteractorStyle, vtkInteractorStyleTerrain);


    /// <summary>
    /// Sets the scene that the picking is forwarded to.
    /// </summary>
    /// <param name="scene">Scene that is picked.</param>
    void SetScene(std::shared_ptr<Scene> scene);
};

/// <summary>
/// Class that handles the window.
/// </summary>
class Window {
private:
    vtkSmartPointer<vtkRenderer>     mRenderer;      // Renderer that contains the scene.
    vtkSmartPointer<vtkRenderWindow> mRenderWindow;  // Class that creates the window.
    vtkSmartPointer<vtkRenderWindowInteractor>
        mRenderWindowInteractor;    // Interactor that handles user interactions.
    std::shared_ptr<Scene> mScene;  // Scene containing all elements of the 3D scene.

public:
    /// <summary>
    /// Constructor.
    /// </summary>
    Window();


    /// <summary>
    /// Render and update loop.
    /// </summary>
    void Loop();
};
