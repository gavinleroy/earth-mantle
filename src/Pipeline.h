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
#include <vtkPointInterpolator.h>
#include <vtkSphereSource.h>
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
#include <vtkDiskSource.h>
#include <vtkAppendPolyData.h>
#include <vtkGaussianKernel.h>
#include <vtkPointInterpolator.h>

#include <sstream>
#include <optional>
#include <filesystem>

// Forward decls
namespace Pipe {

    inline const uint32_t SAMPLING_DIM = 100;

    struct AllInput;
    class Resample;
    class VelocityCalculator;
    class TempAnomAttribute;

    class InputPipeline {
    public:
        virtual ~InputPipeline() { }
        virtual void SetInputConnection(std::shared_ptr<AllInput> pipelines) = 0;
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

namespace MantleIO {
    using cached_data                 = std::optional<vtkSmartPointer<vtkAlgorithm>>;
    inline const size_t MAX_TIMESTEPS = 999;

    // NOTE: you shouldn't use auto when using a MantleAttr
    class MantleAttr {
    public:
        enum Value : uint8_t {
            // ----------------------------------------
            // The next three are the axis definitions
            Lon,
            Lat,
            Radius,

            // ----------------------------------------
            // Below this line are readable attributes
            Vx,
            Vy,
            Vz,
            Temp,
            ThermCond,
            ThermExp,
            SpinAnom,
            TempAnom
        };

        MantleAttr() = default;
        constexpr MantleAttr(Value a)
            : value(a)
        {
        }

        explicit operator bool() const = delete;

        constexpr bool operator==(MantleAttr a) const
        {
            return value == a.value;
        }

        constexpr bool operator!=(MantleAttr a) const
        {
            return value != a.value;
        }

        const std::string str() const
        {
            return std::string(this->c_str());
        }

        const char *c_str() const
        {
            switch (this->value) {
            case Value::Lon:
                return "lon";
            case Value::Lat:
                return "lat";
            case Value::Radius:
                return "r";
            case Value::Vx:
                return "vx";
            case Value::Vy:
                return "vy";
            case Value::Vz:
                return "vz";
            case Value::ThermCond:
                return "thermal conductivity";
            case Value::ThermExp:
                return "thermal expansivity";
            case Value::SpinAnom:
                return "spin transition-induced density anomaly";
            case Value::TempAnom:
                return "temperature anomaly";
            case Value::Temp:
                return "temperature";
            default:
                return "unknown";
            }
        }

        static std::vector<MantleAttr> values();

    private:
        Value value;
    };


    class Mantle : public Pipe::OutputPipeline {
    private:
        inline static const std::filesystem::path data_directory   = "../data/FullMantle";
        inline static size_t                      current_timestep = 1;

        // Using a global reader will require us to reread the data from file,
        // if we switch to preprocessed textures (or images) we could get away with
        // loading the files in the background (or caching them if someone has a HUGE ram).
        inline static vtkSmartPointer<vtkNetCDFCFReader> globalReader = nullptr;
        // inline static std::array<cached_data, MAX_TIMESTEPS> cache
        //     = std::array<cached_data, MAX_TIMESTEPS>();

        static std::filesystem::path LocateFile(size_t time_step);

    public:
        Mantle();
        static void         Step();
        vtkAlgorithmOutput *GetOutputPort();
    };
}

namespace Pipe {

    // A class that can take an assignAttribute stream
    // -----------------
    // Utility pipelines

    class CellToPoint : public Pipeline {
    public:
        vtkSmartPointer<vtkCellDataToPointData> cellToPoint;

        CellToPoint()
        {
            cellToPoint = vtkSmartPointer<vtkCellDataToPointData>::New();
        }

        void SetInputConnection(vtkAlgorithmOutput *cin)
        {
            cellToPoint->SetInputConnection(cin);
        }

        void Print(std::ostream &out)
        {
            cellToPoint->Update();
            cellToPoint->Print(out);
        }

        void SetInputConnection(std::shared_ptr<AllInput> pipelines)
        {
            throw "NYI";
        }

        vtkAlgorithmOutput *GetOutputPort()
        {
            return cellToPoint->GetOutputPort();
        }
    };

    class Resample : public Pipeline {
    private:
        vtkSmartPointer<vtkResampleToImage> resampler;

    public:
        Resample()
        {
            resampler = vtkSmartPointer<vtkResampleToImage>::New();
            resampler->SetSamplingDimensions(SAMPLING_DIM, SAMPLING_DIM, SAMPLING_DIM);
        }

        void SetInputConnection(vtkAlgorithmOutput *cin)
        {
            resampler->SetInputConnection(cin);
        }

        void Print(std::ostream &out)
        {
            resampler->Update();
            resampler->Print(out);
        }

        void SetInputConnection(std::shared_ptr<AllInput> pipelines)
        {
            throw "NYI";
        }

        vtkAlgorithmOutput *GetOutputPort()
        {
            return resampler->GetOutputPort();
        }
    };

    class VelocityCalculator : public Pipeline {
    private:
        vtkSmartPointer<vtkArrayCalculator> calculator;

    public:
        VelocityCalculator()
        {
            calculator = vtkSmartPointer<vtkArrayCalculator>::New();
            calculator->SetResultArrayName("velocity");
            calculator->AddScalarVariable("vx", "vx");
            calculator->AddScalarVariable("vy", "vy");
            calculator->AddScalarVariable("vz", "vz");
            calculator->SetFunction("(iHat * vx + jHat * vy + kHat * vz) * 1e9");
        }

        void Print(std::ostream &out)
        {
            calculator->Update();
            calculator->Print(out);
        }

        void SetInputConnection(vtkAlgorithmOutput *cin)
        {
            calculator->SetInputConnection(cin);
        }

        void SetInputConnection(std::shared_ptr<AllInput> pipelines)
        {
            throw "NYI";
        }

        vtkAlgorithmOutput *GetOutputPort()
        {
            return calculator->GetOutputPort();
        }
    };

    // TODO we just template over the attribute.
    class TempAnomAttribute : public Pipeline {
    private:
        vtkSmartPointer<vtkAssignAttribute> assignAttribute;

    public:
        TempAnomAttribute()
        {
            MantleIO::MantleAttr property = MantleIO::MantleAttr::TempAnom;
            assignAttribute               = vtkSmartPointer<vtkAssignAttribute>::New();
            assignAttribute->Assign(property.c_str(), vtkDataSetAttributes::SCALARS,
                                    vtkAssignAttribute::POINT_DATA);
        }

        void SetInputConnection(vtkAlgorithmOutput *cin)
        {
            assignAttribute->SetInputConnection(cin);
        }

        void SetInputConnection(std::shared_ptr<AllInput> pipelines)
        {
            throw "NYI";
        }

        void Print(std::ostream &out)
        {
            assignAttribute->Update();
            assignAttribute->Print(out);
        }

        vtkAlgorithmOutput *GetOutputPort()
        {
            return assignAttribute->GetOutputPort();
        }
    };


    class Geometry : public Pipe::OutputPipeline {
    public:
        // I don't think we need these
        vtkSmartPointer<vtkActor> coreActor;
        vtkSmartPointer<vtkActor> hullActor;

        // Tweaking geometry visuals
        uint32_t resolution  = 200;
        double   outerRadius = 6377;
        double   innerRadius = 3486;
        double   cx = 0, cy = 0, cz = 0;
        double   nx = 0, ny = 1, nz = 0;

        vtkSmartPointer<vtkSphereSource>      core;
        vtkSmartPointer<vtkSphereSource>      hull;
        vtkSmartPointer<vtkPlane>             cuttingPlane;
        vtkSmartPointer<vtkPointInterpolator> pointInterpolator;

        Geometry();

        void IncreaseInnerRadius();

        void SetInputConnection(vtkAlgorithmOutput *cin);

        vtkAlgorithmOutput *GetOutputPort();
    };


    // -------------------------------------------

    struct AllInput {
        std::shared_ptr<MantleIO::Mantle>   reader;
        std::shared_ptr<CellToPoint>        cellToPoint;
        std::shared_ptr<Resample>           resampled;
        std::shared_ptr<VelocityCalculator> imageVelocityCalculator;
        std::shared_ptr<VelocityCalculator> otherVelocityCalculator;
        std::shared_ptr<TempAnomAttribute>  assignAttr;
        std::shared_ptr<Geometry>           geometry;

        AllInput()
        {
            reader                  = std::make_shared<MantleIO::Mantle>();
            cellToPoint             = std::make_shared<CellToPoint>();
            resampled               = std::make_shared<Resample>();
            imageVelocityCalculator = std::make_shared<VelocityCalculator>();
            otherVelocityCalculator = std::make_shared<VelocityCalculator>();
            assignAttr              = std::make_shared<TempAnomAttribute>();
            geometry                = std::make_shared<Geometry>();

            // NOTE: this is super fragile!
            //       Be very careful during initialization.

            // mantle -> cell to poin
            cellToPoint->SetInputConnection(reader->GetOutputPort());

            // mantle -> cell to point -> resampled
            resampled->SetInputConnection(cellToPoint->GetOutputPort());

            // mantle -> cell to point -> velocity
            otherVelocityCalculator->SetInputConnection(cellToPoint->GetOutputPort());

            // mantle -> cell to point -> resampled -> velocity
            imageVelocityCalculator->SetInputConnection(resampled->GetOutputPort());

            // mantle -> resampled -> attribute assigned
            assignAttr->SetInputConnection(resampled->GetOutputPort());

            // mantle -> cell to point -> velocity
            geometry->SetInputConnection(otherVelocityCalculator->GetOutputPort());
        }
    };
}
