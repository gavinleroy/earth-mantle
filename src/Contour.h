#pragma once

#include <vtkContourFilter.h>
#include <vtkAssignAttribute.h>
#include <vtkActor.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkIntArray.h>
#include <vtkMaskPoints.h>

#include "Pipeline.h"

class Contour : public Pipe::ActorMapped {
private:
    Contour(const Contour &) = delete;
    void operator=(const Contour &) = delete;

    vtkSmartPointer<vtkContourFilter>  contourFilter;
    vtkSmartPointer<vtkPolyDataMapper> mapper;
    vtkSmartPointer<vtkMaskPoints> mask;

    void ConstructInternal();

public:
    Contour();

    void SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines);
    void ConnectToActor(vtkSmartPointer<vtkActor> actor);
};
