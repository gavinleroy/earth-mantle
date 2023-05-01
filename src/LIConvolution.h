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

public:
    LIConvolution();
    ~LIConvolution() { }

    void Update();
    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
};
