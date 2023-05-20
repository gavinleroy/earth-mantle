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

class LIConvolution : public Pipe::ActorMapped {
private:
    LIConvolution(const LIConvolution &)  = delete;  // Delete the copy-constructor.
    void operator=(const LIConvolution &) = delete;  // Delete the assignment operator.

    vtkSmartPointer<vtkGeometryFilter>   geometry;
    vtkSmartPointer<vtkSurfaceLICMapper> mapper;

    void ConstructInternal();

public:
    LIConvolution();
    LIConvolution(vtkAlgorithmOutput *input);
    ~LIConvolution() { }

    void ConnectToActor(vtkSmartPointer<vtkActor> actor);
    void SetInputConnection(vtkAlgorithmOutput *input);
};
