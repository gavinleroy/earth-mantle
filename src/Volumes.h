#pragma once

#include <vtkAssignAttribute.h>
#include <vtkColorTransferFunction.h>
#include <vtkPiecewiseFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkPlane.h>
#include <vtkSmartVolumeMapper.h>
//#include <vtkPolyDataMapper.h>
//#include <vtkActor.h>
//#include <vtkProperty.h>
//#include <vtkTexture.h>
//#include <vtkRenderer.h>
//#include <vtkNetCDFCFReader.h>
//#include <vtkNetCDFReader.h>
//#include <vtkPiecewiseFunction.h>
//#include <vtkColorTransferFunction.h>
//#include <vtkVolumeProperty.h>
//#include <vtkDataArraySelection.h>
//#include <vtkAssignAttribute.h>
//#include <vtkSmartVolumeMapper.h>
//#include <vtkVolumeMapper.h>
//#include <vtkVolume.h>
//#include <vtkStructuredGrid.h>
//#include <vtkResampleToImage.h>
//#include <vtkDataArrayCollection.h>
//#include <vtkImageData.h>
//#include <vtkPointData.h>
//#include <vtkDoubleArray.h>
//#include <vtkDataSetMapper.h>
//#include <vtkLookupTable.h>
//#include <vtkStringArray.h>
//#include <vtkAssignAttribute.h>
//#include <vtkCellData.h>
//#include <vtkStructuredGrid.h>
//#include <vtkPlane.h>
//#include <vtkCutter.h>
//#include <vtkClipPolyData.h>
//#include <vtkClipDataSet.h>
//#include <vtkStructuredGridGeometryFilter.h>
//#include <vtkPolyData.h>
//#include <vtkRectilinearGrid.h>
//#include <vtkCellDataToPointData.h>
//#include <vtkFloatArray.h>
//#include <vtkFixedPointVolumeRayCastMapper.h>
//#include <vtkThreshold.h>
//#include <vtkUnstructuredGrid.h>

#include "Pipeline.h"
#include "Resample.h"

class Volumes : public Pipe::Pipeline, private Resample::Resample {
private:
    Volumes(const Volumes &) = delete;         // Delete the copy-constructor.
    void operator=(const Volumes &) = delete;  // Delete the assignment operator.

    vtkSmartPointer<vtkVolume> mVolume;

public:
    Volumes();
    ~Volumes() { }

    void Update();
    void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
    void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer);
};
