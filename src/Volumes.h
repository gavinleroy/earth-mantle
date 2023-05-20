#pragma once

#include <vtkAssignAttribute.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkPlane.h>
#include <vtkSmartVolumeMapper.h>
#include "Pipeline.h"
#include "Resample.h"

class Volumes : public Pipe::Pipeline, private Resample::Resample {
private:
    Volumes(const Volumes &)        = delete;  // Delete the copy-constructor.
    void operator=(const Volumes &) = delete;  // Delete the assignment operator.

    vtkSmartPointer<vtkVolume> mVolume;

public:
    Volumes();
    ~Volumes() { }

    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer);
};
