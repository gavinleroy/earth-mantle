#pragma once

#include <vtkRenderer.h>
#include <vtkSmartPointer.h>

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
#include <vtkArrayCalculator.h>

namespace Pipe {
    // A class that can take an input stream
    class InputPipeline {
    public:
        virtual ~InputPipeline() { }
        virtual void SetInputConnection(vtkAlgorithmOutput *cin) = 0;
    };

    // A class that produces an output stream
    class OutputPipeline {
    public:
        virtual ~OutputPipeline() { }
        virtual vtkAlgorithmOutput *GetOutputPort() = 0;
    };

    // Input/Output streams
    class Pipeline : public OutputPipeline, public InputPipeline {
    public:
        virtual ~Pipeline() { }
    };

    // ------------------------------
    // Used for outputing visual data

    // Represents classes that work on the earth itself. LIC, contours, etc ...
    class ActorMapped : public InputPipeline {
    public:
        virtual ~ActorMapped() { }

        virtual void ConnectToActor(vtkSmartPointer<vtkActor> actor) = 0;
    };

    // Represents classes that
    class VolumeMapped : public InputPipeline {
    public:
        virtual ~VolumeMapped() { }

        virtual void ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)  = 0;
        virtual void RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer) = 0;
    };
}
