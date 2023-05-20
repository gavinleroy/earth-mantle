#pragma once

#include "Mantle.h"
#include "Pipeline.h"

// Utility pipelines

namespace Pipe {
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

        vtkAlgorithmOutput *GetOutputPort()
        {
            return cellToPoint->GetOutputPort();
            // return resampler->GetOutputPort();
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
            MantleIO::MantleAttr tempanom = MantleIO::MantleAttr::TempAnom;
            assignAttribute               = vtkSmartPointer<vtkAssignAttribute>::New();
            assignAttribute->Assign(tempanom.c_str(), vtkDataSetAttributes::SCALARS,
                                    vtkAssignAttribute::POINT_DATA);
        }

        void SetInputConnection(vtkAlgorithmOutput *cin)
        {
            assignAttribute->SetInputConnection(cin);
        }

        vtkAlgorithmOutput *GetOutputPort()
        {
            return assignAttribute->GetOutputPort();
        }
    };
};
