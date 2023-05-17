#include "Geometry.h"

Geometry::Geometry()
{
    // Initialize the outer sphere
    this->hull = vtkSmartPointer<vtkSphereSource>::New();

    // Initialize the inner sphere (core)
    this->core = vtkSmartPointer<vtkSphereSource>::New();
    core->SetCenter(0, 0, 0);
    core->SetRadius(10100000);
    core->SetThetaResolution(100);
    core->SetPhiResolution(100);

    // Initialize the cutting plan, XXX: only affects outer sphere.
    this->cuttingPlane = vtkSmartPointer<vtkPlane>::New();


    // Initialize actors
    this->coreActor = vtkSmartPointer<vtkActor>::New();

    this->hullActor = vtkSmartPointer<vtkActor>::New();

    // FIXME: not ready yet.
    assert(false);
}
