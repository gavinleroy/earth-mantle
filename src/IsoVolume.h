#pragma once

#include "Pipeline.h"
#include "Resample.h"

#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>

class IsoVolume : public Pipe::Pipeline, private Resample::Resample {
private:
    IsoVolume(const IsoVolume &)      = delete;
    void operator=(const IsoVolume &) = delete;

    inline static vtkSmartPointer<vtkVolume> mVolume = nullptr;

public:
    IsoVolume();
    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer) override;
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer) override;
};
