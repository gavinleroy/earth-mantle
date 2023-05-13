#include "Resample.h"
namespace Resample {
    Resample::Resample()
            : MantleIO::Mantle()
    {
        // Only initialize once
        if (resampler != nullptr) {
            return;
        }

        auto fromFile = GetCurrentStream();

        cellToPoint = vtkSmartPointer<vtkCellDataToPointData>::New();
        cellToPoint->SetInputConnection(fromFile->GetOutputPort());

        resampler = vtkSmartPointer<vtkResampleToImage>::New();
        resampler->SetInputConnection(cellToPoint->GetOutputPort());
        resampler->SetSamplingDimensions(200, 200, 200);

#ifndef NDEBUG
        resampler->Update();
        resampler->GetOutput()->Print(std::cout);
#endif
    }

    vtkSmartPointer<vtkAlgorithm> Resample::GetResampled() {
        return resampler;
    }

    vtkSmartPointer<vtkAlgorithm> Resample::GetPointDataBeforeResample()
    {
        return cellToPoint;
    }
}
