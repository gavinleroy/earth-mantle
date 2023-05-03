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

// Define interaction style
class KeyPressInteractorStyle : public vtkInteractorStyleTrackballCamera
{
public:
  static KeyPressInteractorStyle* New();
  vtkTypeMacro(KeyPressInteractorStyle, vtkInteractorStyleTrackballCamera);

  virtual void OnKeyPress() override
  {
    // Get the keypress
    vtkRenderWindowInteractor* rwi = this->Interactor;
    std::string key = rwi->GetKeySym();

    // Output the key that was pressed
    std::cout << "Pressed " << key << std::endl;

    // Handle a "normal" key
    if (key == "a")
    {
        std::cout << "The a key was pressed." << std::endl;
    }

    // Forward events
    vtkInteractorStyleTrackballCamera::OnKeyPress();
    }
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
