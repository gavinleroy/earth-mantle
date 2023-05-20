#include "Contour.h"

Contour::Contour()
{
    contourFilter = vtkSmartPointer<vtkContourFilter>::New();
    mapper        = vtkSmartPointer<vtkPolyDataMapper>::New();

    ConstructInternal();
}

void Contour::ConstructInternal()
{
    MantleIO::MantleAttr property = MantleIO::MantleAttr::TempAnom;

    vtkNew<vtkLookupTable> lut;
    lut->SetVectorMode(vtkScalarsToColors::MAGNITUDE);
    lut->SetHueRange(.6, 1.);
    lut->SetValueRange(1, 1);
    lut->SetSaturationRange(.8, .8);

    contourFilter->SetInputArrayToProcess(
            0, 0, 0, vtkDataObject::FIELD_ASSOCIATION_POINTS, property.c_str());
    //    contourFilter->GenerateValues(10, -1100, 1100);

    contourFilter->SetValue(0, -150);
    contourFilter->SetValue(1, 0);
    contourFilter->SetValue(2, 200);

    // Save the mapper internally so we can attach to the scene later.
    mapper->SetInputConnection(contourFilter->GetOutputPort());
    mapper->SelectColorArray(property.c_str());
    mapper->SetLookupTable(lut);
}

void Contour::SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines)
{
    contourFilter->SetInputConnection(pipelines->assignAttr->GetOutputPort());
}

void Contour::ConnectToActor(vtkSmartPointer<vtkActor> actor)
{
    actor->SetMapper(this->mapper);
    actor->GetProperty()->SetLineWidth(5);
    actor->VisibilityOn();
}
