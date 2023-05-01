#pragma once

#include "Mantle.h"

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkTexture.h>
#include <vtkRenderer.h>
#include <vtkNetCDFCFReader.h>
#include <vtkNetCDFReader.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkDataArraySelection.h>
#include <vtkAssignAttribute.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkStructuredGrid.h>
#include <vtkResampleToImage.h>
#include <vtkDataArrayCollection.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkStringArray.h>
#include <vtkAssignAttribute.h>
#include <vtkCellData.h>
#include <vtkStructuredGrid.h>
#include <vtkPlane.h>
#include <vtkCutter.h>
#include <vtkClipVolume.h>
#include <vtkClipDataSet.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkPolyData.h>
#include <vtkRectilinearGrid.h>
#include <vtkCellDataToPointData.h>
#include <vtkFloatArray.h>
#include <vtkFixedPointVolumeRayCastMapper.h>
#include <vtkPlanes.h>
#include <vtkTubeFilter.h>
#include <vtkMarchingCubes.h>
#include <vtkImageDataGeometryFilter.h>
#include <vtkArrayCalculator.h>
#include <vtkStreamTracer.h>
#include <vtkLineSource.h>
#include <vtkStructuredGridOutlineFilter.h>
#include <vtkNamedColors.h>

#include "Mantle.h"
#include "Pipeline.h"

class Tube : private MantleIO::Mantle, public Pipe::Pipeline {
private:
    Tube(const Tube &) = delete;            // Delete the copy-constructor.
    void operator=(const Tube &) = delete;  // Delete the assignment operator.

    vtkSmartPointer<vtkActor> streamlineActor;

public:
    Tube();
    ~Tube() { }
    void Update();
    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
};
