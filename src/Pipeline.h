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

#include <sstream>

// Forward decls
namespace Pipe {
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

    class Resample : public Pipeline {
    private:
        vtkSmartPointer<vtkResampleToImage>     resampler;
        vtkSmartPointer<vtkCellDataToPointData> cellToPoint;

    public:
        Resample()
        {
            cellToPoint = vtkSmartPointer<vtkCellDataToPointData>::New();

            resampler = vtkSmartPointer<vtkResampleToImage>::New();
            resampler->SetInputConnection(cellToPoint->GetOutputPort());
            resampler->SetSamplingDimensions(200, 200, 200);
        }

        void SetInputConnection(vtkAlgorithmOutput *cin)
        {
            cellToPoint->SetInputConnection(cin);
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
            // return cellToPoint->GetOutputPort();
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


    // -------------------------------------------

    struct AllInput {
        std::shared_ptr<MantleIO::Mantle>   reader;
        std::shared_ptr<Resample>           resampled;
        std::shared_ptr<VelocityCalculator> velocityCalculator;
        std::shared_ptr<TempAnomAttribute>  assignAttr;

        AllInput()
        {
            reader             = std::make_shared<MantleIO::Mantle>();
            resampled          = std::make_shared<Pipe::Resample>();
            velocityCalculator = std::make_shared<Pipe::VelocityCalculator>();
            assignAttr         = std::make_shared<Pipe::TempAnomAttribute>();

            // NOTE: this is super fragile!
            //       Be very careful during initialization.

            // mantle -> resampled
            resampled->SetInputConnection(reader->GetOutputPort());

            // mantle -> resampled -> attribute assigned
            assignAttr->SetInputConnection(resampled->GetOutputPort());

            // mantle -> resampled -> velocity
            velocityCalculator->SetInputConnection(resampled->GetOutputPort());
        }
    };
}
