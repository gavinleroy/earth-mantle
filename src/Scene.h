#pragma once


#include <memory>
#include <string>
#include <iostream>

#include <vtkLight.h>
#include <vtkRenderer.h>
#include <vtkSmartPointer.h>
#include <vtkRenderWindowInteractor.h>

#include "Tube.h"
#include "Volumes.h"
#include "LIConvolution.h"
#include "Pipeline.h"
#include "Contour.h"
#include "IsoVolume.h"

// TODO: add everything else
enum class EarthView {
    Contour = 0,
    LIC,
};

// TODO: add everything else
enum class VolumeView {
    Volume = 0,
    IsoVolume,
    Tubes,
    // ...
};

using EarthMappings = std::unordered_map<EarthView, std::shared_ptr<Pipe::ActorMapped>>;
using VolumeMappings = std::unordered_map<VolumeView, std::shared_ptr<Pipe::VolumeMapped>>;

class Scene {
private:
    Scene(const Scene &)          = delete;  // Delete the copy-constructor.
    void operator=(const Scene &) = delete;  // Delete the assignment operator.

    // The current shown mappings (if any)
    std::optional<EarthView>  currentEarthMapper;
    std::optional<VolumeView> currentEarthVolume;
    std::vector<VolumeView> currentEarthVolumes;

    std::shared_ptr<Pipe::AllInput> inputPipelines;

    EarthMappings                earthMappers;
    VolumeMappings               earthVolumes;
    vtkSmartPointer<vtkRenderer> renderer;

    void SetMapping(EarthView idx);
    void SetVolume(VolumeView idx);
    void SwitchMapping(EarthView idx);
    void ToggleVolume(VolumeView idx);

public:
    Scene();
    void InitRenderer(vtkSmartPointer<vtkRenderer> renderer);
    void InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor);
    void ProcessInput(std::string input);
    void Update(double dt, double t);
};
