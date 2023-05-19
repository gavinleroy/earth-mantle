#include "Vorticity.h"

Vorticity::Vorticity()
    : Mantle()
    , mActor(vtkSmartPointer<vtkActor>::New())
{
    std::cout << "Vorticity pipeline" << std::endl;
}

void Vorticity::ConnectToScene(vtkSmartPointer <vtkRenderer> renderer) {
    renderer->AddActor(mActor);
}

void Vorticity::RemoveFromScene(vtkSmartPointer <vtkRenderer> renderer) {
    renderer->RemoveActor(mActor);
}