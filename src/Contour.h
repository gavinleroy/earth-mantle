#pragma once

#include <vtkContourFilter.h>
#include <vtkAssignAttribute.h>
#include <vtkActor.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataMapper.h>

#include "Mantle.h"
#include "Pipeline.h"

class Contour : public Pipe::ActorMapped {
private:
    Contour(const Contour &)        = delete;
    void operator=(const Contour &) = delete;

    vtkSmartPointer<vtkContourFilter>  contourFilter;
    vtkSmartPointer<vtkPolyDataMapper> mapper;

public:
    Contour();

    void SetInputConnection(vtkAlgorithmOutput *input);
    void ConnectToActor(vtkSmartPointer<vtkActor> actor);
};
