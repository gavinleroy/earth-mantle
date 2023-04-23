#pragma once

// VTK includes
#include "vtkCellDataToPointData.h"
#include "vtkStructuredGrid.h"
#include "vtkArrayCalculator.h"
#include "vtkDataSetSurfaceFilter.h"
#include "vtkSurfaceLICMapper.h"

#include "Mantle.h"

class LineIntegralConvolution : private Mantle {
public:
    LineIntegralConvolution();
    vtkSmartPointer<vtkActor> GetActor();
private:
    vtkSmartPointer<vtkActor> mActor;
};