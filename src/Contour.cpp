
#include "Contour.h"

Contour::Contour()
        : Resample::Resample()
{
    if (mContour != nullptr) {
        return;
    }
    mContour = vtkSmartPointer<vtkActor>::New();
    MantleIO::MantleAttr property = MantleIO::MantleAttr::TempAnom;
    auto resampler = Resample::GetResampled();

    vtkNew<vtkAssignAttribute> assignAttribute;
    assignAttribute->SetInputConnection(resampler->GetOutputPort());
    assignAttribute->Assign(property.c_str(), vtkDataSetAttributes::SCALARS,
                            vtkAssignAttribute::POINT_DATA);

    vtkNew<vtkContourFilter> contourFilter;
    contourFilter->SetInputConnection(assignAttribute->GetOutputPort());
    contourFilter->SetInputArrayToProcess(0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, property.c_str());
    contourFilter->GenerateValues(10, -1100, 1100);

    vtkNew<vtkPolyDataMapper> polyMapper;

    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(0., 0., 0.);
    plane->SetNormal(0., 1.0, 0);

    vtkNew<vtkLookupTable> lut;
    lut->SetVectorMode(vtkScalarsToColors::MAGNITUDE);
    lut->SetHueRange(.6, 1.);
    lut->SetValueRange(1, 1);
    lut->SetSaturationRange(.8, .8);

    polyMapper->SetInputConnection(contourFilter->GetOutputPort());
    polyMapper->SelectColorArray(property.c_str());
    polyMapper->AddClippingPlane(plane);
    polyMapper->SetLookupTable(lut);

    mContour->SetMapper(polyMapper);
    mContour->GetProperty()->SetLineWidth(5);
    mContour->VisibilityOn();
}

void Contour::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddActor(mContour);
}

void Contour::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveActor(mContour);
}

void Contour::Update() { }

