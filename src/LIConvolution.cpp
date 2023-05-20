#include "LIConvolution.h"


LIConvolution::LIConvolution()
{
    geometry = vtkSmartPointer<vtkGeometryFilter>::New();
    mapper   = vtkSmartPointer<vtkSurfaceLICMapper>::New();
    ConstructInternal();
}

LIConvolution::LIConvolution(vtkAlgorithmOutput *cin)
{
    geometry = vtkSmartPointer<vtkGeometryFilter>::New();
    mapper   = vtkSmartPointer<vtkSurfaceLICMapper>::New();
    geometry->SetInputConnection(cin);
    ConstructInternal();
}

void LIConvolution::ConstructInternal()
{
#ifndef NDEBUG
    std::cout << "LIC: Constructing internal, current geometry:" << std::endl;
    geometry->Update();
    geometry->GetOutput()->Print(std::cout);
#endif

    MantleIO::MantleAttr temp_anom = MantleIO::MantleAttr::TempAnom;

    double                 range[] = { -1100., 1100. };
    vtkNew<vtkLookupTable> lookupTable;
    lookupTable->SetHueRange(0.75, 0.);
    lookupTable->SetValueRange(1., 1.);
    lookupTable->SetSaturationRange(1., 1.);
    lookupTable->SetRange(range);
    lookupTable->Build();

    mapper->SetInputConnection(geometry->GetOutputPort());
    mapper->SetInputArrayToProcess(0, 0, 0, vtkDataObject::POINT, "velocity");
    mapper->GetLICInterface()->SetEnhancedLIC(1);  // Disable enhanced LIC (for now)

    mapper->SelectColorArray(temp_anom.c_str());
    mapper->SetScalarVisibility(true);
    mapper->SetScalarModeToUsePointFieldData();
    mapper->SetLookupTable(lookupTable);
}

void LIConvolution::SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines)
{
#ifndef NDEBUG
    std::cout << "LIC: Setting assignAttribute connection" << std::endl;
#endif
    this->geometry->SetInputConnection(pipelines->velocityCalculator->GetOutputPort());
}

void LIConvolution::ConnectToActor(vtkSmartPointer<vtkActor> actor)
{
#ifndef NDEBUG
    std::cout << "LIC: Connecting to new actor" << std::endl;
#endif
    actor->SetMapper(mapper);
}
