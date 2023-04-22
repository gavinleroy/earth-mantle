#pragma once

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
#include <vtkClipPolyData.h>
#include <vtkStructuredGridGeometryFilter.h>
#include <vtkPolyData.h>
#include <vtkRectilinearGrid.h>
#include <vtkCellDataToPointData.h>
#include <vtkFloatArray.h>
#include <vtkFixedPointVolumeRayCastMapper.h>

/// <summary>
/// Class that represents the Earth.
/// </summary>
class Mantle {
private:
    Mantle(const Mantle &)         = delete;  // Delete the copy-constructor.
    void operator=(const Mantle &) = delete;  // Delete the assignment operator.

    vtkSmartPointer<vtkVolume> mVolume;
    vtkSmartPointer<vtkActor>  mActor;

public:
    /// <summary>
    /// Constructor.
    /// </summary>
    Mantle();

    /// Getters for retrieving internal objects.
    /// TODO: not sure if we actually want to have both
    ///       actors and volumes at play, but I(gavin)
    ///       don't know when we need one vs the other.
    std::vector<vtkSmartPointer<vtkActor>>  GetActors();
    std::vector<vtkSmartPointer<vtkVolume>> GetVolumes();

    void LoadInitialFile();

    /// <summary>
    /// Updates the properties of the Earth.
    /// </summary>
    /// <param name="dt">Time passed since the last Update in milliseconds.</param>
    /// <param name="t">Total time passed since start of the application in
    /// milliseconds.</param>
    void Update();
};
