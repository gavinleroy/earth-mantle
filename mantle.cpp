#include "mantle.h"

// TODO: we need to be able to load files in a lazy stream,
//       hard coding them is obviously the wrong thing to do,
//       we'll have to look at how to do time-series properly.
void Mantle::LoadInitialFile(/* TODO take a file */)
{
    vtkSmartPointer<vtkNetCDFReader> reader = vtkSmartPointer<vtkNetCDFReader>::New();
    reader->SetFileName("../data/FullMantle/spherical001.nc");
    reader->UpdateMetaData();
    reader->SetVariableArrayStatus("lon", 1);
    reader->SetVariableArrayStatus("lat", 1);
    reader->SetVariableArrayStatus("r", 1);
    reader->SetVariableArrayStatus("temperature", 1);

    reader->SetVariableArrayStatus("vx", 1);
    reader->SetVariableArrayStatus("vy", 1);
    reader->SetVariableArrayStatus("vz", 1);

    reader->Update();

    reader->Print(std::cout);
    reader->GetOutput()->Print(std::cout);

    vtkNew<vtkAssignAttribute> assignAttribute;
    assignAttribute->SetInputConnection(reader->GetOutputPort());
    assignAttribute->Assign("temperature", "SCALARS", "POINT_DATA");

    vtkNew<vtkSmartVolumeMapper> volumeMapper;
    volumeMapper->SetBlendModeToComposite();  // composite first
    // NOTE: using input data was just how they do it in the following example:
    // https://kitware.github.io/vtk-examples/site/Cxx/VolumeRendering/SmartVolumeMapper/
    // volumeMapper->SetInputData(/* TODO */);

    volumeMapper->SetInputConnection(assignAttribute->GetOutputPort());
    volumeMapper->SelectScalarArray("temperature");
    volumeMapper->SetScalarModeToUsePointFieldData();

    // this->mActor->SetMapper((vtkMapper *)&*volumeMapper);
    this->mVolume->SetMapper(volumeMapper);
}

// TODO: ultimately the constructor should just set up the
//       mantle volumes / actors / etc, but they don't necessarily
//       need to be computed, we can load the time series lazily.
Mantle::Mantle()
{
    // NOTE: Initialize all private members before calling
    //       any methods which assume a well-established object.
    this->mVolume = vtkSmartPointer<vtkVolume>::New();
    this->mActor  = vtkSmartPointer<vtkActor>::New();

    LoadInitialFile();

    // XXX: some old junk that Raphy was  trying.
    //        vtkSmartPointer<vtkLookupTable> lut =
    //        vtkSmartPointer<vtkLookupTable>::New(); lut->SetTableRange(0, 10000);
    //
    //        vtkSmartPointer<vtkDataSetMapper> mapper =
    //        vtkSmartPointer<vtkDataSetMapper>::New();
    //        mapper->SetLookupTable(lut);
    //        mapper->SetInputConnection(assignAttribute->GetOutputPort());
    //        mapper->SetScalarModeToUseCellData();
    //        mapper->SetColorModeToMapScalars();
    //        mapper->SetLookupTable(vtkColorTransferFunction::New());

    //        mapper->Print(std::cout);


    //        vtkSmartPointer<vtkResampleToImage> resample =
    //        vtkSmartPointer<vtkResampleToImage>::New();
    ////        resample->SetSamplingDimensions(50, 50, 50);
    //        resample->SetInputConnection(reader->GetOutputPort());
    //
    //        resample->Update();
    //        resample->GetOutput()->Print(std::cout);
    //

    // vtkSmartPointer<vtkPiecewiseFunction> opacityTransferFunction
    //     = vtkSmartPointer<vtkPiecewiseFunction>::New();
    // opacityTransferFunction->AddPoint(1, -1e5);
    // opacityTransferFunction->AddPoint(1, 1e5);

    // XXX
    // vtkSmartPointer<vtkColorTransferFunction> colorTransferFunction
    //     = vtkSmartPointer<vtkColorTransferFunction>::New();
    // colorTransferFunction->AddRGBPoint(-1e5, 1.0, 0.0, 0.0);
    // colorTransferFunction->AddRGBPoint(1e5, 0.0, 0.0, 1.0);

    // XXX
    // vtkSmartPointer<vtkVolumeProperty> volumeProperty
    //     = vtkSmartPointer<vtkVolumeProperty>::New();
    // volumeProperty->SetColor(colorTransferFunction);
    // volumeProperty->SetScalarOpacity(opacityTransferFunction);
    // volumeProperty->ShadeOn();

    // XXX: volume attemmpt
    // mVolume = vtkSmartPointer<vtkVolume>::New();
    // mVolume->SetMapper(volumeMapper);
    // mVolume->SetProperty(volumeProperty);

    //        mActor = vtkSmartPointer<vtkActor>::New();
    //        mActor->SetMapper(mVolume);
    //        mActor->SetPosition(0, 0, 0);
}

void Mantle::Update()
{ /* UNIMPLEMENTED */
}

// TODO:
std::vector<vtkSmartPointer<vtkActor>> Mantle::GetActors()
{
    auto v = std::vector<vtkSmartPointer<vtkActor>>({ this->mActor });
    return v;
}

// TODO:
std::vector<vtkSmartPointer<vtkVolume>> Mantle::GetVolumes()
{
    auto v = std::vector<vtkSmartPointer<vtkVolume>>({ this->mVolume });
    return v;
}
