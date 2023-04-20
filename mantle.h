#pragma once

#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkTexture.h>
#include <vtkRenderer.h>
#include <vtkNetCDFCFReader.h>
#include <vtkNetCDFReader.h>
#include <vtkPiecewiseFunction.h>
#include <vtkColorTransferFunction.h>
#include <vtkVolumeProperty.h>
#include <vtkDataArraySelection.h>
#include <vtkAssignAttribute.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkVolume.h>
#include <vtkStructuredGrid.h>
#include <vtkResampleToImage.h>
#include <vtkDataArrayCollection.h>
#include <vtkImageData.h>
#include <vtkPointData.h>
#include <vtkDoubleArray.h>
#include <vtkDataSetMapper.h>
#include <vtkLookupTable.h>
#include <vtkStringArray.h>
#include <vtkAssignAttribute.h>

/// <summary>
/// Class that represents the Earth.
/// </summary>
class Mantle
{
public:
    /// <summary>
    /// Constructor.
    /// </summary>
    Mantle()
    {
        vtkSmartPointer<vtkNetCDFReader> reader = vtkSmartPointer<vtkNetCDFReader>::New();
        reader->SetFileName("../data/mantle01/spherical001.nc");
        reader->UpdateMetaData();
        reader->SetVariableArrayStatus("lon", 1);
        reader->SetVariableArrayStatus("lat", 1);
        reader->SetVariableArrayStatus("r", 1);
        reader->SetVariableArrayStatus("temperature", 1);
//        reader->SetVariableArrayStatus("vx", 1);
//        reader->SetVariableArrayStatus("vy", 1);
//        reader->SetVariableArrayStatus("vz", 1);
        reader->Update();

        reader->Print(std::cout);
        reader->GetOutput()->Print(std::cout);

        vtkSmartPointer<vtkAssignAttribute> assignAttribute = vtkSmartPointer<vtkAssignAttribute>::New();
        assignAttribute->SetInputConnection(reader->GetOutputPort());
        assignAttribute->Assign("temperature", "SCALARS", "POINT_DATA");

//        vtkSmartPointer<vtkLookupTable> lut = vtkSmartPointer<vtkLookupTable>::New();
//        lut->SetTableRange(0, 10000);
//
//        vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
//        mapper->SetInputConnection(assignAttribute->GetOutputPort());
//        mapper->SetLookupTable(lut);
//        mapper->SetScalarModeToUseCellData();
//        mapper->SetColorModeToMapScalars();
//        mapper->SetLookupTable(vtkColorTransferFunction::New());

//        mapper->Print(std::cout);



//        vtkSmartPointer<vtkResampleToImage> resample = vtkSmartPointer<vtkResampleToImage>::New();
////        resample->SetSamplingDimensions(50, 50, 50);
//        resample->SetInputConnection(reader->GetOutputPort());
//
//        resample->Update();
//        resample->GetOutput()->Print(std::cout);
//
        vtkSmartPointer<vtkSmartVolumeMapper> volumeMapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
        volumeMapper->SetInputConnection(assignAttribute->GetOutputPort());
//        volumeMapper->SetScalarModeToUsePointFieldData();
//        volumeMapper->SelectScalarArray("temperature");
//        volumeMapper->SetScalarModeToUsePointFieldData();
//        volumeMapper->SetRequestedRenderModeToGPU();

        vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction = vtkSmartPointer<vtkPiecewiseFunction>::New();
        opacityTransferFunction->AddPoint(1, -1e5);
        opacityTransferFunction->AddPoint(1, 1e5);

        vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction = vtkSmartPointer<vtkColorTransferFunction>::New();
        colorTransferFunction->AddRGBPoint(-1e5, 1.0, 0.0, 0.0);
        colorTransferFunction->AddRGBPoint(1e5, 0.0, 0.0, 1.0);

        vtkSmartPointer<vtkVolumeProperty> volumeProperty = vtkSmartPointer<vtkVolumeProperty>::New();
        volumeProperty->SetColor(colorTransferFunction);
        volumeProperty->SetScalarOpacity(opacityTransferFunction);
        volumeProperty->ShadeOn();

        mVolume = vtkSmartPointer<vtkVolume>::New();
        mVolume->SetMapper(volumeMapper);
        mVolume->SetProperty(volumeProperty);

//        mActor = vtkSmartPointer<vtkActor>::New();
//        mActor->SetMapper(mVolume);
//        mActor->SetPosition(0, 0, 0);
    }

    /// <summary>
    /// Updates the properties of the Earth.
    /// </summary>
    /// <param name="dt">Time passed since the last Update in milliseconds.</param>
    /// <param name="t">Total time passed since start of the application in milliseconds.</param>
    void Update()
    {
    }

    /// <summary>
    /// Adds the actors to the renderer.
    /// </summary>
    /// <param name="renderer">Renderer to add the actors to.</param>
    void InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
    {
        renderer->AddVolume(mVolume);
//        renderer->AddActor(mActor);
    }

private:
    Mantle(const Mantle&) = delete;			// Delete the copy-constructor.
    void operator=(const Mantle&) = delete;	// Delete the assignment operator.

    vtkSmartPointer<vtkVolume> mVolume;
    vtkSmartPointer<vtkActor> mActor;
};