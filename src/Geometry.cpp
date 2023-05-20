#include "Geometry.h"

namespace Geometry {
    Geometry::Geometry()
    {
        // ----------
        // HULL STUFF

        // Initialize the outer sphere
        this->hull = vtkSmartPointer<vtkSphereSource>::New();
        hull->SetCenter(0, 0, 0);
        hull->SetThetaResolution(100);
        hull->SetPhiResolution(100);
        hull->SetRadius(18400000.0);

        // Initialize the cutting plan, XXX: only affects outer sphere.
        this->cuttingPlane = vtkSmartPointer<vtkPlane>::New();
        cuttingPlane->SetOrigin(0., 0., 0.);
        cuttingPlane->SetNormal(0., 1.0, 0);

        vtkNew<vtkClipPolyData> hullClipper;
        // hullClipper->InsideOutOn();
        hullClipper->SetClipFunction(cuttingPlane);
        hullClipper->SetInputConnection(hull->GetOutputPort());

        vtkNew<vtkPolyDataMapper> hullMapper;
        hullMapper->SetInputConnection(hullClipper->GetOutputPort());
        hullMapper->AddClippingPlane(cuttingPlane);

        // Initialize hull actor
        this->hullActor = vtkSmartPointer<vtkActor>::New();
        hullActor->SetMapper(hullMapper);
        hullActor->GetProperty()->SetOpacity(0.25);

        // ----------
        // CORE STUFF

        // Initialize the inner sphere (core)
        this->core = vtkSmartPointer<vtkSphereSource>::New();
        core->SetCenter(0, 0, 0);
        core->SetRadius(10100000);
        core->SetThetaResolution(100);
        core->SetPhiResolution(100);

        vtkNew<vtkPolyDataMapper> coreMapper;
        coreMapper->SetInputConnection(core->GetOutputPort());

        // Initialize core actor
        this->coreActor = vtkSmartPointer<vtkActor>::New();
        coreActor->SetMapper(coreMapper);
    }

    void Geometry::SetActiveMapper(std::shared_ptr<Pipe::ActorMapped> mapper)
    {
#ifndef NDEBUG
        std::cout << "GEOMETRY: Setting new mapper" << std::endl;
#endif
        // FIXME: this isn't sufficient.
        mapper->ConnectToActor(coreActor);
    }

    void Geometry::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
    {
        renderer->AddActor(this->hullActor);
        renderer->AddActor(this->coreActor);
    }

    void Geometry::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
    {
        renderer->RemoveActor(this->hullActor);
        renderer->RemoveActor(this->coreActor);
    }
};
