#pragma once

// VTK includes
#include "vtkCellDataToPointData.h"
#include "vtkStructuredGrid.h"
#include "vtkArrayCalculator.h"
#include "vtkGeometryFilter.h"
#include "vtkSurfaceLICMapper.h"
#include "vtkSurfaceLICInterface.h"

#include "Mantle.h"
#include "Pipeline.h"

class LIConvolution : private MantleIO::Mantle, public Pipe::Pipeline {
private:
    LIConvolution(const LIConvolution &) = delete;   // Delete the copy-constructor.
    void operator=(const LIConvolution &) = delete;  // Delete the assignment operator.

    vtkSmartPointer<vtkActor> mActor;
    vtkSmartPointer<vtkPlane> mClippingPlane;

public:
    LIConvolution();
    ~LIConvolution() { }

    void Update();
    void Update(double dt, double t);
    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer);
};
