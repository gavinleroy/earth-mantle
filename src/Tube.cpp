#include "Tube.h"
#include <vtkColorTransferFunction.h>

Tube::Tube()
    : Mantle()
    , streamlineActor(vtkSmartPointer<vtkActor>::New())
{
    auto variables = std::vector<std::string>({
        "vx",
        "vy",
        "vz",
    });

    auto structuredGrid = LoadFromFile("spherical001.nc", variables);

    vtkNew<vtkLookupTable> ctf;
    ctf->SetVectorMode(vtkScalarsToColors::MAGNITUDE);
    ctf->SetHueRange(0.667, 0.0);
    ctf->SetValueRange(1, 1);
    ctf->SetSaturationRange(1, 1);

    vtkNew<vtkCellDataToPointData> cellToPoint;
    cellToPoint->SetInputData(structuredGrid);
    cellToPoint->Update();

    vtkNew<vtkArrayCalculator> calculator;
    calculator->SetInputConnection(cellToPoint->GetOutputPort());
    calculator->SetResultArrayName("velocity");
    calculator->AddScalarVariable("vx", "vx");
    calculator->AddScalarVariable("vy", "vy");
    calculator->AddScalarVariable("vz", "vz");
    calculator->SetFunction("(iHat * vx + jHat * vy + kHat * vz) * 1e9");

    vtkNew<vtkLineSource> line;
    line->SetResolution(1000);
    line->SetPoint1(-6378., -6378., -6378.);
    line->SetPoint2(6378., 6378., 6378.);

    vtkNew<vtkAssignAttribute> assignAttribute;
    assignAttribute->SetInputConnection(calculator->GetOutputPort());
    assignAttribute->Assign("velocity", vtkDataSetAttributes::SCALARS,
                            vtkAssignAttribute::POINT_DATA);
    assignAttribute->Update();

    vtkNew<vtkStreamTracer> tracer;
    tracer->SetInputConnection(assignAttribute->GetOutputPort());
    tracer->SetSourceConnection(line->GetOutputPort());

    tracer->SetInterpolatorType(vtkStreamTracer::INTERPOLATOR_WITH_DATASET_POINT_LOCATOR);
    tracer->SetIntegrationDirection(vtkStreamTracer::BOTH);
    tracer->SetIntegratorType(vtkStreamTracer::RUNGE_KUTTA45);
    tracer->SetIntegrationStepUnit(vtkStreamTracer::CELL_LENGTH_UNIT);

    tracer->SetTerminalSpeed(1e-12);
    tracer->SetMaximumError(1e-6);
    tracer->SetMaximumPropagation(12760);
    tracer->SetMaximumNumberOfSteps(2000);

#ifndef NDEBUG
    tracer->GetOutput()->Print(std::cout);
#endif

    vtkNew<vtkTubeFilter> tubes;
    tubes->SetInputConnection(tracer->GetOutputPort());
    tubes->SetRadius(127.1);
    tubes->SetNumberOfSides(6);
    tubes->CappingOn();

    vtkNew<vtkPolyDataMapper> streamlineMapper;
    streamlineMapper->SetInputConnection(tubes->GetOutputPort());
    streamlineMapper->SetLookupTable(ctf);

    streamlineActor->SetMapper(streamlineMapper);
    streamlineActor->VisibilityOn();
}


void Tube::Update()
{ /* UNIMPLEMENTED */
}

// TODO:
std::vector<vtkSmartPointer<vtkActor>> Tube::GetActors()
{
    return std::vector<vtkSmartPointer<vtkActor>>({ this->streamlineActor });
}


std::vector<vtkSmartPointer<vtkVolume>> Tube::GetVolumes()
{
    return std::vector<vtkSmartPointer<vtkVolume>>();
}
