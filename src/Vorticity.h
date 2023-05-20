#pragma once

#include <vtkGradientFilter.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataMapper.h>

#include "Mantle.h"
#include "Pipeline.h"

class Vorticity : private MantleIO::Mantle, public Pipe::Pipeline {
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
