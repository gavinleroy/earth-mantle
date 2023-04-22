//
// Created by Raphael Winkler on 12.04.23.
//

#ifndef EARTH_MANTLE_DIRECTVOLUME_H
#define EARTH_MANTLE_DIRECTVOLUME_H

#endif //EARTH_MANTLE_DIRECTVOLUME_H


#include <iostream>
#include <string>
#include <filesystem>

#include <vtkRenderWindow.h>
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkNetCDFCFReader.h>
#include <vtkNetCDFReader.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkDataArraySelection.h>
#include <vtkAssignAttribute.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkCamera.h>
#include <vtkStructuredGrid.h>
#include <vtkResampleToImage.h>
#include <vtkInteractorStyleTrackballActor.h>


class DirectVolumeRendering {
public:
    DirectVolumeRendering() {

        renderer = vtkSmartPointer<vtkRenderer>::New();

        renderer_window = vtkSmartPointer<vtkRenderWindow>::New();
        renderer_window->AddRenderer(renderer);

        interactor = vtkSmartPointer<vtkRenderWindowInteractor>::New();
        interactor->SetRenderWindow(renderer_window);

//        vtkSmartPointer<vtkInteractorStyleTrackballActor> style = vtkSmartPointer<vtkInteractorStyleTrackballActor>::New();
//        interactor->SetInteractorStyle(style);

        vtkSmartPointer<vtkNetCDFReader> reader = vtkSmartPointer<vtkNetCDFReader>::New();

        reader->SetFileName("../data/mantle01/spherical001.nc");
//        reader->SetVariableArrayStatus("temperature", 1);
        reader->SetVariableArrayStatus("temperature", 1);
        reader->Update();
        reader->GetOutput()->Print(std::cout);


        vtkSmartPointer<vtkResampleToImage> resample = vtkSmartPointer<vtkResampleToImage>::New();
        resample->SetInputConnection(reader->GetOutputPort());
        resample->SetSamplingDimensions(100, 100, 100);
        resample->Update();

//        resample->GetOutput()->Print(std::cout);

        vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
//        volumeMapper->SetInputConnection(reader->GetOutputPort());

        volumeMapper->SetInputConnection(resample->GetOutputPort());
        volumeMapper->SetScalarModeToUsePointFieldData();
        volumeMapper->SelectScalarArray("temperature");
        volumeMapper->SetRequestedRenderModeToGPU();
        volumeMapper->Update();

//        volumeMapper->Print(std::cout);
        // Create a transfer function
        vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
        opacityTransferFunction->AddPoint(0, -1e3);
        opacityTransferFunction->AddPoint(1, 1e3);

        vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
        colorTransferFunction->AddRGBPoint(0, 1.0, 0.0, 0.0);
        colorTransferFunction->AddRGBPoint(1, 0.0, 0.0, 1.0);

        // Create a volume property and attach the transfer function
        vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
        volumeProperty->SetColor(colorTransferFunction);
        volumeProperty->SetScalarOpacity(opacityTransferFunction);
        volumeProperty->ShadeOn();

        // Create a volume and add the mapper and volume property
        vtkSmartPointer<vtkVolume> volume = vtkSmartPointer<vtkVolume>::New();
        volume->SetMapper(volumeMapper);
        volume->SetProperty(volumeProperty);
        volume->Update();

        // Add the volume to the renderer and set the background color
        renderer->AddVolume(volume);
        renderer->SetBackground(0.2, 0.2, 0.2);

        // Set the camera position
        vtkSmartPointer<vtkCamera> camera = renderer->GetActiveCamera();
        camera->SetPosition(0, 0, -400);
        camera->SetFocalPoint(0, 0, 0);

        renderer_window->Render();
        interactor->Start();
    }

    void Loop()
    {
        auto start = std::chrono::high_resolution_clock::now();
        auto last = start;
        while (renderer_window->GetGenericWindowId())
        {
            // compute the timing deltas
            auto now = std::chrono::high_resolution_clock::now();
            double dt = std::chrono::duration<double, std::milli>(now - last).count();
            double t = std::chrono::duration<double, std::milli>(now - start).count();
            last = now;

//            mScene->Update(dt, t);

            renderer_window->Render();
            interactor->ProcessEvents();
        }
        renderer_window->Finalize();
        renderer_window->GetInteractor()->TerminateApp();
    }

private:
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkRenderWindow> renderer_window;
    vtkSmartPointer<vtkRenderWindowInteractor> interactor;
};
