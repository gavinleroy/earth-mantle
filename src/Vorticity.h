#pragma once

#include <vtkArrayCalculator.h>
#include <vtkGradientFilter.h>
#include <vtkVortexCore.h>
#include <vtkThresholdPoints.h>
#include <vtkMaskPoints.h>
#include <vtkAssignAttribute.h>
#include <vtkStreamTracer.h>
#include <vtkTubeFilter.h>
#include <vtkPolyDataMapper.h>

#include "Resample.h"
#include "Pipeline.h"

class Vorticity : private Resample::Resample, public Pipe::Pipeline {
private:
    // Delete the copy-constructor and assignment operators
    Vorticity(const Vorticity &) = delete;
    void operator=(const Vorticity &) = delete;

    vtkSmartPointer<vtkActor> mActor;

public:
    Vorticity();
    ~Vorticity() {};

    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer);
};
