#include "Contour.h"

void AddSegmentToCTF(
        const vtkSmartPointer<vtkColorTransferFunction>& ctf,
        double from, double r1, double g1, double b1,
        double to, double r2, double g2, double b2,
        int nr_contours = 2, double thickness = 25,
        bool start = false, bool end = false
) {
    double r_step = (r2-r1)/nr_contours; // 0
    double g_step = (g2-g1)/nr_contours; // 1
    double b_step = (b2-b1)/nr_contours; // 0

    double eps;
    if (from > 0) {
        eps = 1e-5;
    } else {
        eps = 1e-5;
    }

    double step_size = (to - from)/nr_contours;
    if (start) {
        ctf->AddRGBPoint(
                from + thickness / 2,
                0,
                0,
                0
        );
        ctf->AddRGBPoint(
                from + thickness / 2 + eps,
                r1,
                g1,
                b1
        );
    } else {
        ctf->AddRGBPoint(
                from,
                r1,
                g1,
                b1
        );
    }

    for (int i = 1; i < nr_contours; i++) {
        ctf->AddRGBPoint(
                from + i * step_size - thickness/2 - eps,
                r1 + r_step * i,
                g1 + g_step * i,
                b1 + b_step * i
        );
        ctf->AddRGBPoint(
                from + i * step_size - thickness/2,
                0,
                0,
                0
        );
        ctf->AddRGBPoint(
                from + i * step_size + thickness/2,
                0,
                0,
                0
        );
        ctf->AddRGBPoint(
                from + i * step_size + thickness/2 + eps,
                r1 + r_step * i,
                g1 + g_step * i,
                b1 + b_step * i
        );
    }

    if (end) {
        ctf->AddRGBPoint(
                to - thickness / 2,
                r2,
                g2,
                b2
        );
        ctf->AddRGBPoint(
                to - thickness / 2 + eps,
                0,
                0,
                0
        );
    } else {
        ctf->AddRGBPoint(
                to,
                r2,
                g2,
                b2
        );
    }
}


Contour::Contour()
{
    contourFilter = vtkSmartPointer<vtkContourFilter>::New();
    mapper        = vtkSmartPointer<vtkPolyDataMapper>::New();
    mask          = vtkSmartPointer<vtkMaskPoints>::New();

    ConstructInternal();
}

void Contour::ConstructInternal()
{
    MantleIO::MantleAttr property = MantleIO::MantleAttr::TempAnom;

//    vtkNew<vtkLookupTable> lut;
//    lut->SetVectorMode(vtkScalarsToColors::MAGNITUDE);
//    lut->SetValueRange(0, 0);
//
//    contourFilter->SetInputArrayToProcess(
//        0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, property.c_str());
//
//    contourFilter->SetValue(0, -800);
//    contourFilter->SetValue(0, -300);
//    contourFilter->SetValue(1, -150);
//    contourFilter->SetValue(2, 0);
//    contourFilter->SetValue(3, 200);
//    contourFilter->SetValue(4, 300);
//    contourFilter->SetValue(0, -800);

    // Save the mapper internally so we can attach to the scene later.
//    mapper->SetInputConnection(contourFilter->GetOutputPort());

    double range[] = {-1100.,1100.};

    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->SetColorSpaceToRGB();
    colorTransferFunction->SetScaleToLinear();
    colorTransferFunction->AdjustRange(range);
    AddSegmentToCTF(
            colorTransferFunction,
            -1100,0, 0, 1,
            -150, 0, 1, 1,
            2, 25,
            false, true
    );
    AddSegmentToCTF(
            colorTransferFunction,
            -150,0, 1, 1,
            0, 1, 1, 1,
            3, 25,
            true, true
    );
    AddSegmentToCTF(
            colorTransferFunction,
            0,1, 1, 1,
            150, 1, 1, 0,
            3, 25,
            true, true
    );
    AddSegmentToCTF(
            colorTransferFunction,
            150,1, 1, 0,
            1100, 1, 0, 0,
            2, 25,
            true, false
    );

    mapper->SelectColorArray(property.c_str());
    mapper->SetScalarVisibility(true);
    mapper->SetScalarModeToUsePointFieldData();
    mapper->SetLookupTable(colorTransferFunction);

}

void Contour::SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines)
{
    mapper->SetInputConnection(pipelines->geometry->GetOutputPort());
//    contourFilter->SetInputConnection(pipelines->geometry->GetOutputPort());
}

void Contour::ConnectToActor(vtkSmartPointer<vtkActor> actor)
{
    actor->SetMapper(this->mapper);
    actor->GetProperty()->SetLineWidth(5);
    actor->VisibilityOn();
}
