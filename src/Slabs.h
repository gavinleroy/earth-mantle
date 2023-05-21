#pragma once

#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataNormals.h>

#include "Pipeline.h"

class Slabs : public Pipe::VolumeMapped {
private:
    Slabs(const Slabs &)          = delete;
    void operator=(const Slabs &) = delete;

    vtkSmartPointer<vtkMarchingCubes> isosurface;
    vtkSmartPointer<vtkActor>         mActor;

    uint32_t numContours = 4;
    int32_t  lowerBound = -1100, upperBound = -200;

    void ConstructInternal();

public:
    Slabs();

    void SetUpperBound(int32_t ub);
    void SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines);
    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer);
};
