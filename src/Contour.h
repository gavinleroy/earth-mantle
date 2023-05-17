#pragma once

#include "Resample.h"
#include "Pipeline.h"
#include <vtkContourFilter.h>
#include <vtkAssignAttribute.h>
#include <vtkActor.h>
#include <vtkContourFilter.h>
#include <vtkPolyDataMapper.h>

class Contour : public Pipe::Pipeline, private Resample::Resample {
private:
    Contour(const Contour &)        = delete;
    void operator=(const Contour &) = delete;

    inline static vtkSmartPointer<vtkActor> mContour = nullptr;

public:
    Contour();
    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer) override;
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer) override;
};
