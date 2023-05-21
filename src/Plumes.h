#pragma once

#include <vtkGPUVolumeRayCastMapper.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkSmoothPolyDataFilter.h>
#include <vtkPolyDataNormals.h>

#include "Pipeline.h"

class Plumes : public Pipe::VolumeMapped {
private:
    Plumes(const Plumes &)         = delete;
    void operator=(const Plumes &) = delete;

    vtkSmartPointer<vtkMarchingCubes> isosurface;
    vtkSmartPointer<vtkActor>         mActor;

    uint32_t numContours = 4;
    int32_t  lowerBound = 400, upperBound = 1100;

    void ConstructInternal();

public:
    Plumes();

    void SetLowerBound(int32_t lb);
    void SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines);
    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer);
};
