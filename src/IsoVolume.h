#pragma once

#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>

#include "Pipeline.h"

class IsoVolume : public Pipe::VolumeMapped {
private:
    IsoVolume(const IsoVolume &)      = delete;
    void operator=(const IsoVolume &) = delete;

    vtkSmartPointer<vtkAssignAttribute> input;
    vtkSmartPointer<vtkVolume>          mVolume;

public:
    IsoVolume();

    void SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines);
    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer);
};
