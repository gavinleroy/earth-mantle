#include "Slabs.h"

Slabs::Slabs()
{
    isosurface = vtkSmartPointer<vtkMarchingCubes>::New();
    mActor     = vtkSmartPointer<vtkActor>::New();
    ConstructInternal();
}

void Slabs::ConstructInternal()
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

void Slabs::SetUpperBound(int32_t ub)
{
    upperBound = ub;
    isosurface->GenerateValues(numContours, lowerBound, upperBound);
}

void Slabs::SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines)
{
    isosurface->SetInputConnection(pipelines->assignAttr->GetOutputPort());
    // this->volumeRayMapper->SetInputConnection(pipelines->assignAttr->GetOutputPort());
}

void Slabs::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddActor(mActor);
    // renderer->AddVolume(mVolume);
}

void Slabs::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveActor(mActor);
    // renderer->RemoveVolume(mVolume);
}
