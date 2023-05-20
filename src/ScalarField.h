#pragma once

#include <vtkPlaneSource.h>
#include <vtkSphereSource.h>
#include <vtkClipPolyData.h>
#include <vtkAppendPolyData.h>
#include <vtkGaussianKernel.h>
#include <vtkPointInterpolator.h>

#include "Pipeline.h"
#include "Resample.h"

class ScalarField : public Pipe::Pipeline, private Resample::Resample {
private:
    ScalarField(const ScalarField &)        = delete;  // Delete the copy-constructor.
    void operator=(const ScalarField &) = delete;  // Delete the assignment operator.

    vtkSmartPointer<vtkActor> mActor;

public:
    ScalarField();
    ~ScalarField(){}

    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer);
};
