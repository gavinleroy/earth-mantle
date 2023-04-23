#pragma once

// VTK includes
#include "vtkCellDataToPointData.h"
#include "vtkStructuredGrid.h"
#include "vtkArrayCalculator.h"
#include "vtkGeometryFilter.h"
#include "vtkSurfaceLICMapper.h"
#include "vtkSurfaceLICInterface.h"

#include "Mantle.h"

class LineIntegralConvolution : private Mantle {
public:
    LineIntegralConvolution();
    vtkSmartPointer<vtkActor> GetActor();
private:
    vtkSmartPointer<vtkActor> mActor;
};