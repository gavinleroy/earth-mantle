#include "Window.h"

vtkStandardNewMacro(PickingInteractorStyle);

void PickingInteractorStyle::SetScene(std::shared_ptr<Scene> scene)
{
    mScene = scene;
}

// TODO: there seems to be a lot of functionality in the
//       constructor, can we factor this out?
Window::Window()
{
    // create renderer
    this->mRenderer = vtkSmartPointer<vtkRenderer>::New();
    vtkNew<vtkNamedColors> namedColors;
    mRenderer->SetBackground(namedColors->GetColor3d("AliceBlue").GetData());
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
    vtkNew<PickingInteractorStyle> interactorStyle;
    interactorStyle->SetDefaultRenderer(mRenderer);
    interactorStyle->SetScene(mScene);
    mRenderer->ResetCamera();

    // create key press observer
    vtkNew<KeyPressObserver> keyPressObserver;
    keyPressObserver->SetScene(mScene);

    // create interactor
    this->mRenderWindowInteractor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
    mRenderWindowInteractor->SetInteractorStyle(interactorStyle);
    mRenderWindowInteractor->AddObserver(vtkCommand::KeyPressEvent, keyPressObserver);
    mRenderWindowInteractor->SetRenderWindow(mRenderWindow);
    mRenderWindowInteractor->EnableRenderOff();
    mRenderWindowInteractor->Initialize();
    mScene->InitUI(mRenderWindowInteractor);

    // finally set the window title
    mRenderWindow->SetWindowName("Earth Mantle");
}

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
            // MantleIO::Mantle::Step();

            // update the scene
            mScene->Update(dt.count(), t.count());
            last = now;
        }

        // render the frame anew
        mScene->Update(dt.count(), t.count());
        mRenderWindow->Render();
        mRenderWindowInteractor->ProcessEvents();
    }

    mRenderWindow->Finalize();
    mRenderWindow->GetInteractor()->TerminateApp();
}
