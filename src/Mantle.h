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
#include <vtkScalarsToColors.h>
#include <vtkLookupTable.h>

#include <filesystem>

class Mantle {
private:
    Mantle(const Mantle &) = delete;          // Delete the copy-constructor.
    void operator=(const Mantle &) = delete;  // Delete the assignment operator.

    inline const static std::vector<std::string> variables = std::vector<std::string>(
        { "lon", "lat", "r", "vx", "vy", "vz", "thermal conductivity",
          "thermal expansivity", "spin transition-induced density anomaly",
          "temperature anomaly" });

    std::filesystem::path data_directory;

    vtkSmartPointer<vtkVolume> mVolume;
    vtkSmartPointer<vtkActor>  mActor;

protected:
    vtkSmartPointer<vtkDataObject> LoadFromFile(const std::string              fn,
                                                const std::vector<std::string> variables);

public:
    Mantle(std::filesystem::path data_directory);
    Mantle();

    /// Getters for retrieving internal objects.
    /// TODO: not sure if we actually want to have both
    ///       actors and volumes at play, but I(gavin)
    ///       don't know when we need one vs the other.
    std::vector<vtkSmartPointer<vtkActor>>  GetActors();
    std::vector<vtkSmartPointer<vtkVolume>> GetVolumes();

    void LoadMantleSeries();

    void LoadMantleFiles(std::filesystem::path data_dir);

    void Update();
};
