#include "LIConvolution.h"


LIConvolution::LIConvolution()
{
    // geometry = vtkSmartPointer<vtkGeometryFilter>::New();
    mapper = vtkSmartPointer<vtkSurfaceLICMapper>::New();
    ConstructInternal();
}

LIConvolution::LIConvolution(vtkAlgorithmOutput *cin)
{
    // geometry = vtkSmartPointer<vtkGeometryFilter>::New();
    mapper = vtkSmartPointer<vtkSurfaceLICMapper>::New();
    geometry->SetInputConnection(cin);
    ConstructInternal();
}

void LIConvolution::ConstructInternal()
{
#ifndef NDEBUG
    std::cout << "LIC: Constructing internal, current geometry:" << std::endl;
#endif

    MantleIO::MantleAttr temp_anom = MantleIO::MantleAttr::TempAnom;

    double                 range[] = { -2., 2. };
    vtkNew<vtkLookupTable> lookupTable;
    lookupTable->SetHueRange(0.75, 0.);
    lookupTable->SetValueRange(1., 1.);
    lookupTable->SetSaturationRange(1., 1.);
    lookupTable->SetRange(range);
    lookupTable->Build();

    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->SetNanOpacity(1.0);
    colorTransferFunction->SetColorSpaceToRGB();
    colorTransferFunction->SetScaleToLinear();
    colorTransferFunction->AdjustRange(range);
    colorTransferFunction->AddRGBPoint(-4, .6, .3, 0.);
    colorTransferFunction->AddRGBPoint(2, 1., 1., 0.);
    colorTransferFunction->AddRGBPoint(-1, 0., 1., 0.);
    colorTransferFunction->AddRGBPoint(0, 1., 1., 1.);
    colorTransferFunction->AddRGBPoint(1, 0., 1., 0.);
    colorTransferFunction->AddRGBPoint(2, 1., 1., 0.);
    colorTransferFunction->AddRGBPoint(4, .6, .3, 0.);
//    colorTransferFunction->AddRGBPoint(-2, 0.5, 0., 0.7);
//    colorTransferFunction->AddRGBPoint(-1.5, 0.5, 0.5, 1.);
//    colorTransferFunction->AddRGBPoint(-1, 0., 1., 0.2);
//    colorTransferFunction->AddRGBPoint(-0.5, 1., 1., 0.);
//    colorTransferFunction->AddRGBPoint(0, 1., 1., 1.);
//    colorTransferFunction->AddRGBPoint(0.5, 1., 1., 0.);
//    colorTransferFunction->AddRGBPoint(1, 0., 1., 0.2);
//    colorTransferFunction->AddRGBPoint(1.5, 0.5, 0.5, 1.);
//    colorTransferFunction->AddRGBPoint(2, 0.5, 0., 0.7);

    // mapper->SetInputConnection(geometry->GetOutputPort());
    mapper->SetInputArrayToProcess(0, 0, 0, vtkDataObject::POINT, "velocity");
    mapper->GetLICInterface()->SetEnhancedLIC(0);  // Disable enhanced LIC (for now)

    mapper->SelectColorArray("velocity");
    mapper->SetScalarVisibility(true);
    mapper->SetScalarModeToUsePointFieldData();
    mapper->SetLookupTable(colorTransferFunction);
}

void LIConvolution::SetInputConnection(std::shared_ptr<Pipe::AllInput> pipelines)
{
#ifndef NDEBUG
    std::cout << "LIC: Setting assignAttribute connection" << std::endl;
#endif

    mapper->SetInputConnection(pipelines->geometry->GetOutputPort());
}

void LIConvolution::ConnectToActor(vtkSmartPointer<vtkActor> actor)
{
#ifndef NDEBUG
    std::cout << "LIC: Connecting to new actor" << std::endl;
#endif
    actor->SetMapper(mapper);
}
