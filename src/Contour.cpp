
#include "Contour.h"

Contour::Contour()
    : Resample::Resample()
{
    if (mContour != nullptr) {
        return;
    }
    mContour                       = vtkSmartPointer<vtkActor>::New();
    MantleIO::MantleAttr property  = MantleIO::MantleAttr::TempAnom;
    auto                 resampler = Resample::GetResampled();

    vtkNew<vtkGeometryFilter> geometryFilter;
    geometryFilter->SetInputConnection(Resample::GetPointDataBeforeResample()->GetOutputPort());

    // Create a color transfer function
    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->SetNanOpacity(1.0);
    colorTransferFunction->SetColorSpaceToRGB();
    colorTransferFunction->SetScaleToLinear();
    double range[] = {-1100, 1100};
    colorTransferFunction->AdjustRange(range);
    colorTransferFunction->AddRGBPoint(-1100, 0., 0., 1.);
    colorTransferFunction->AddRGBPoint(-150, 0., 1., 1.);
    colorTransferFunction->AddRGBPoint(0, 1., 1., 1.);
    colorTransferFunction->AddRGBPoint(150, 1., 1., 0.);
    colorTransferFunction->AddRGBPoint(1100, 1.0, 0., 0.);

    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(0., 0., 0.);
    plane->SetNormal(0., 1.0, 0);

    vtkNew<vtkPolyDataMapper> polyMapper;
    polyMapper->SetInputConnection(geometryFilter->GetOutputPort());
    polyMapper->SelectColorArray(property.c_str());
    polyMapper->SetScalarVisibility(true);
    polyMapper->SetScalarModeToUsePointFieldData();
    polyMapper->SetLookupTable(colorTransferFunction);
    polyMapper->AddClippingPlane(plane);

    mContour->SetMapper(polyMapper);
}

void Contour::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddActor(mContour);
}

void Contour::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveActor(mContour);
}
