#pragma once

#include <vtkAssignAttribute.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkPlane.h>
#include <vtkSmartVolumeMapper.h>

#include <vtkAssignAttribute.h>
#include <vtkDataSetAttributes.h>

#include "Pipeline.h"

class Volumes : public Pipe::VolumeMapped {
private:
    Volumes(const Volumes &)        = delete;  // Delete the copy-constructor.
    void operator=(const Volumes &) = delete;  // Delete the assignment operator.

    vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper;
    vtkSmartPointer<vtkVolume>          mVolume;

    void ConstructInternal();

public:
    Volumes();
    ~Volumes() { }

    void SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines);
    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer);
};
