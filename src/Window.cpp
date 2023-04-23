#include "Window.h"

vtkStandardNewMacro(PickingInteractorStyle);

void PickingInteractorStyle::SetScene(std::shared_ptr<Scene> scene)
{
    mScene = scene;
}

// ------------------------
// Window class

// TODO: there seems to be a lot of functionality in the
//       constructor, can we factor this out?
Window::Window()
{
    // create renderer
    mRenderer = vtkSmartPointer<vtkRenderer>::New();
    mRenderer->SetBackground(.2, .2, .2);
    mRenderer->GetActiveCamera()->SetViewUp(0, 0, 1);
    mRenderer->GetActiveCamera()->SetPosition(0, -50000, 2);
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
    mRenderer->ResetCamera();

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
void Window::Loop()
{
    auto start = std::chrono::high_resolution_clock::now();
    auto last  = start;

    while (mRenderWindow->GetGenericWindowId()) {
        // compute the timing deltas
        auto   now = std::chrono::high_resolution_clock::now();
        double dt  = std::chrono::duration<double, std::milli>(now - last).count();
        double t   = std::chrono::duration<double, std::milli>(now - start).count();
        last       = now;

        // update the scene
        mScene->Update(dt, t);

        // render the frame anew
        mRenderWindow->Render();
        mRenderWindowInteractor->ProcessEvents();
    }

    mRenderWindow->Finalize();
    mRenderWindow->GetInteractor()->TerminateApp();
}
