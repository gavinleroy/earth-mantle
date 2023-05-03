#pragma once


#include <chrono>
#include <algorithm>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkInteractorStyleTerrain.h>
#include <vtkCamera.h>
#include <vtkMatrix4x4.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkNew.h>
#include <vtkObjectFactory.h>

#include "Scene.h"

class PickingInteractorStyle : public vtkInteractorStyleTerrain {
private:
    std::shared_ptr<Scene> mScene;  // Scene containing all elements of the 3D scene.

public:
    static PickingInteractorStyle *New();
    vtkTypeMacro(PickingInteractorStyle, vtkInteractorStyleTerrain);
    void SetScene(std::shared_ptr<Scene> scene);
};

class Window {
private:
    vtkSmartPointer<vtkRenderer>     mRenderer;      // Renderer that contains the scene.
    vtkSmartPointer<vtkRenderWindow> mRenderWindow;  // Class that creates the window.
    vtkSmartPointer<vtkRenderWindowInteractor>
        mRenderWindowInteractor;    // Interactor that handles user interactions.
    std::shared_ptr<Scene> mScene;  // Scene containing all elements of the 3D scene.

public:
    Window();
    void Loop();
};
