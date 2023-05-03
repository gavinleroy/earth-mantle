#include "Window.h"

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

    // Handle an arrow key
    if (key == "Up")
    {
        std::cout << "The up arrow was pressed." << std::endl;
    }

    // Handle a "normal" key
    if (key == "a")
    {
        std::cout << "The a key was pressed." << std::endl;
    }

    // Forward events
    vtkInteractorStyleTrackballCamera::OnKeyPress();
    }

    //void SetScene(std::shared_ptr<Scene> scene)
    //{
    //    mScene = scene;
    //}
};
vtkStandardNewMacro(KeyPressInteractorStyle);


// TODO: there seems to be a lot of functionality in the
//       constructor, can we factor this out?
Window::Window()
{
    // create renderer
    this->mRenderer = vtkSmartPointer<vtkRenderer>::New();
    mRenderer->SetBackground(.2, .2, .2);
    mRenderer->GetActiveCamera()->SetViewUp(0, 0, 1);
    mRenderer->GetActiveCamera()->SetPosition(0, -50000, 2);
    mRenderer->UseDepthPeelingOn();

    // create scene
    this->mScene = std::make_shared<Scene>();
    mScene->InitRenderer(mRenderer);

    // create render window
    this->mRenderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    mRenderWindow->SetSize(800, 600);
    mRenderWindow->AddRenderer(mRenderer);

    // create picking style
    vtkNew<KeyPressInteractorStyle> interactorStyle;
    interactorStyle->SetDefaultRenderer(mRenderer);
    //interactorStyle->SetScene(mScene);
    mRenderer->ResetCamera();

    // create interactor
    this->mRenderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
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
void Window::Loop()
{
    auto start = std::chrono::high_resolution_clock::now();
    auto last  = start;

    std::chrono::duration<double, std::milli> delay = std::chrono::minutes(2);

    while (mRenderWindow->GetGenericWindowId()) {
        // compute the timing deltas
        auto now = std::chrono::high_resolution_clock::now();
        auto dt  = std::chrono::duration<double, std::milli>(now - last);
        auto t   = std::chrono::duration<double, std::milli>(now - start);

        if (dt > delay) {
            MantleIO::Mantle::Step();

            // update the scene
            mScene->Update(dt.count(), t.count());
            last = now;
        }

        // render the frame anew
        mRenderWindow->Render();
        mRenderWindowInteractor->ProcessEvents();
    }

    mRenderWindow->Finalize();
    mRenderWindow->GetInteractor()->TerminateApp();
}
