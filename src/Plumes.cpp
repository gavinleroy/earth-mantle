#include "Plumes.h"

Plumes::Plumes()
{
    isosurface = vtkSmartPointer<vtkMarchingCubes>::New();
    mActor     = vtkSmartPointer<vtkActor>::New();
    ConstructInternal();
}

void Plumes::ConstructInternal()
{
    double                           range[] = { -1100., 1100. };
    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->SetNanOpacity(1.0);
    colorTransferFunction->SetColorSpaceToRGB();
    colorTransferFunction->SetScaleToLinear();
    colorTransferFunction->AdjustRange(range);
    colorTransferFunction->AddRGBPoint(-1100, 0., 0., 1.);
    colorTransferFunction->AddRGBPoint(-150, 0., 1., 1.);
    colorTransferFunction->AddRGBPoint(0, 1., 1., 1.);
    colorTransferFunction->AddRGBPoint(150, 1., 1., 0.);
    colorTransferFunction->AddRGBPoint(1100, 1.0, 0., 0.);

    // create a scalar bar
    vtkNew<vtkScalarBarActor> scalarBar;
    scalarBar->SetTitle("Plumes");
    scalarBar->SetLookupTable(colorTransferFunction);
    SetScalarBar(scalarBar);

    isosurface->GenerateValues(numContours, lowerBound, upperBound);

    vtkNew<vtkSmoothPolyDataFilter> smoothie;
    smoothie->SetInputConnection(isosurface->GetOutputPort());
    smoothie->SetNumberOfIterations(15);
    smoothie->SetRelaxationFactor(0.1);
    smoothie->FeatureEdgeSmoothingOff();
    smoothie->BoundarySmoothingOn();

    vtkNew<vtkPolyDataNormals> normalGenerator;
    normalGenerator->SetInputConnection(smoothie->GetOutputPort());
    normalGenerator->ComputePointNormalsOn();
    normalGenerator->ComputeCellNormalsOn();

    vtkNew<vtkPolyDataMapper> mapper;
    mapper->SetInputConnection(normalGenerator->GetOutputPort());
    mapper->SetLookupTable(colorTransferFunction);

    mActor->SetMapper(mapper);
}

void Plumes::SetLowerBound(int32_t lb)
{
    lowerBound = lb;
    isosurface->GenerateValues(numContours, lowerBound, upperBound);
}

void Plumes::SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines)
{
    isosurface->SetInputConnection(pipelines->assignAttr->GetOutputPort());
}

void Plumes::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddActor(mActor);
}

void Plumes::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveActor(mActor);
}
