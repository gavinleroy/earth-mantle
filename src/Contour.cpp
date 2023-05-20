#include "Contour.h"

Contour::Contour()
{
    this->contourFilter = vtkSmartPointer<vtkContourFilter>::New();
    this->mapper        = vtkSmartPointer<vtkPolyDataMapper>::New();

    MantleIO::MantleAttr property = MantleIO::MantleAttr::TempAnom;

    contourFilter->SetInputArrayToProcess(
        0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, property.c_str());
    //    contourFilter->GenerateValues(10, -1100, 1100);

    contourFilter->SetValue(0, -150);
    contourFilter->SetValue(1, 0);
    contourFilter->SetValue(2, 200);

    vtkNew<vtkLookupTable> lut;
    lut->SetVectorMode(vtkScalarsToColors::MAGNITUDE);
    lut->SetHueRange(.6, 1.);
    lut->SetValueRange(1, 1);
    lut->SetSaturationRange(.8, .8);

    // Save the mapper internally so we can attach to the scene later.
    this->mapper->SetInputConnection(contourFilter->GetOutputPort());
    this->mapper->SelectColorArray(property.c_str());
    this->mapper->SetLookupTable(lut);
}


void Contour::SetInputConnection(vtkAlgorithmOutput *cin)
{
    // NOTE: expects property assigned attributes.
    contourFilter->SetInputConnection(cin);
}

void Contour::ConnectToActor(vtkSmartPointer<vtkActor> actor)
{
    actor->SetMapper(this->mapper);
    actor->GetProperty()->SetLineWidth(5);
    actor->VisibilityOn();
}
