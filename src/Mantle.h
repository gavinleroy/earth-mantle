#pragma once

// TODO: these are horrible, they should really
// only be in a single place.
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

#include <iterator>
#include <sstream>
#include <filesystem>
#include <optional>


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


    class Mantle {
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

    protected:
        static vtkSmartPointer<vtkAlgorithm> GetCurrentStream();

    public:
        Mantle();
        static void Step();
    };

}
