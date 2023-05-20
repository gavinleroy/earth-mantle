#pragma once

#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>

#include "Mantle.h"
#include "Pipeline.h"

class IsoVolume : public Pipe::VolumeMapped {
private:
    IsoVolume(const IsoVolume &)      = delete;
    void operator=(const IsoVolume &) = delete;

    vtkSmartPointer<vtkAssignAttribute> input;
    vtkSmartPointer<vtkVolume>          mVolume;

public:
    IsoVolume();

    void SetInputConnection(vtkAlgorithmOutput *input);
    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer);
};
