#pragma once

#include "Pipeline.h"
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
#include <vtkAlgorithm.h>
#include <vtkSphereSource.h>
#include <vtkClipPolyData.h>

namespace Geometry {
    // XXX: I have absolutely no idea how to do this. I've looked up on a lot of
    //      resources and the only thing I can find on proxy geometry is from Unity.
    //      From the VTK discussions it seems that *every* visualization must have its
    //      own actor. Which is basically what we're already doing.
    //
    //      A few things we can do:
    //      1. Limit the number of active visualizations to 1. When active, the current
    //         vis will map itself onto these actors.
    //
    //      2. Figure out how to do real proxy geometry (I'll need help though).
    //      3. If we need multiple active visualizations at a time we'll need a way
    //         to orchestrate the cutting planes so that the geometry looks like one.
    class Geometry {
    private:
        vtkSmartPointer<vtkSphereSource> core;
        vtkSmartPointer<vtkSphereSource> hull;
        // TODO: how they hell can we get the wedge view?
        //       This would be something with 3 cutting planes iirc.
        vtkSmartPointer<vtkPlane> cuttingPlane;

        vtkSmartPointer<vtkActor> coreActor;
        vtkSmartPointer<vtkActor> hullActor;

    public:
        Geometry();

        void SetActiveMapper(std::shared_ptr<Pipe::ActorMapped> mapper);
        void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer);
        void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer);
    };
};
