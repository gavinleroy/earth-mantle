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
#include "Geometry.h"
#include "Utils.h"

// TODO: add everything else
enum class EarthView {
    Contour = 0,
    LIC,
};

// TODO: add everything else
enum class VolumeView {
    // ...
};

template <typename T> struct Mapping {
    std::shared_ptr<Pipe::OutputPipeline> input;
    std::shared_ptr<T>                    mapper;
};

using EarthMappings  = std::unordered_map<EarthView, Mapping<Pipe::ActorMapped>>;
using VolumeMappings = std::unordered_map<VolumeView, Mapping<Pipe::VolumeMapped>>;

class Scene {
private:
    Scene(const Scene &)          = delete;  // Delete the copy-constructor.
    void operator=(const Scene &) = delete;  // Delete the assignment operator.

    // The current shown mappings (if any)
    std::optional<EarthView>  currentEarthMapper;
    std::optional<VolumeView> currentEarthVolume;

    // Input data sources, and common transformations.
    std::shared_ptr<MantleIO::Mantle>         reader;
    std::shared_ptr<Pipe::Resample>           resampled;
    std::shared_ptr<Pipe::VelocityCalculator> velocityCalculator;
    std::shared_ptr<Pipe::TempAnomAttribute>  assignAttr;


    std::unique_ptr<Geometry::Geometry> geometry;

    EarthMappings                earthMappers;
    VolumeMappings               earthVolumes;
    vtkSmartPointer<vtkRenderer> renderer;

    void SetMapping(EarthView idx);

public:
    Scene();
    void InitRenderer(vtkSmartPointer<vtkRenderer> renderer);
    void InitUI(vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor);
    void ProcessInput(char *input);
    void SwitchPipeline(int index);
    void Update(double dt, double t);
};
