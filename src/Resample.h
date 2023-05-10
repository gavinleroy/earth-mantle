#pragma once

#include <vtkCellDataToPointData.h>
#include <vtkResampleToImage.h>
#include <vtkCellDataToPointData.h>

#include "Mantle.h"


namespace Resample {
    class Resample: private MantleIO::Mantle {
    private:
        inline static vtkSmartPointer<vtkResampleToImage> resampler = nullptr;
        inline static vtkSmartPointer<vtkCellDataToPointData> cellToPoint = nullptr;

    public:
        Resample();

    protected:
        static vtkSmartPointer<vtkAlgorithm> GetResampled();
        static vtkSmartPointer<vtkAlgorithm> GetPointDataBeforeResample();
    };
}
