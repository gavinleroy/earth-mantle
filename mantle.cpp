#include "mantle.h"

// TODO: we need to be able to load files in a lazy stream,
//       hard coding them is obviously the wrong thing to do,
//       we'll have to look at how to do time-series properly.
void Mantle::LoadInitialFile(/* TODO take a file */)
{
    vtkSmartPointer<vtkNetCDFCFReader> reader = vtkSmartPointer<vtkNetCDFCFReader>::New();
    reader->SetFileName("../data/FullMantle/spherical001.nc");
    reader->UpdateMetaData();
    reader->SetVariableArrayStatus("lon", 0);
    reader->SetVariableArrayStatus("lat", 0);
    reader->SetVariableArrayStatus("r", 0);
    reader->SetVariableArrayStatus("temperature", 1);

    reader->SetVariableArrayStatus("vx", 1);
    reader->SetVariableArrayStatus("vy", 1);
    reader->SetVariableArrayStatus("vz", 1);

    reader->Update();

    reader->GetOutput()->Print(std::cout);
    vtkSmartPointer<vtkStructuredGrid> structuredGrid = vtkStructuredGrid::SafeDownCast(reader->GetOutput());

    vtkSmartPointer<vtkStructuredGridGeometryFilter> geometryFilter = vtkSmartPointer<vtkStructuredGridGeometryFilter>::New();
    geometryFilter->SetInputData(structuredGrid);
    geometryFilter->Update();
    vtkSmartPointer<vtkPolyData> polyData = geometryFilter->GetOutput();

    vtkSmartPointer<vtkCellDataToPointData> c2p = vtkSmartPointer<vtkCellDataToPointData>::New();
    c2p->SetInputData(polyData);
    c2p->PassCellDataOn();
    c2p->Update();

    vtkSmartPointer<vtkImageData> imageData = vtkSmartPointer<vtkImageData>::New();
    imageData->SetSpacing(0.1, 0.1, 0.1);



    vtkSmartPointer<vtkResampleToImage> resample = vtkSmartPointer<vtkResampleToImage>::New();
//    resample->GetUpdateExtent(imageData->GetExtent());
    resample->SetInputConnection(c2p->GetOutputPort());
//    resample->SetSamplingDimensions(100, 100, 100);
//    resample->SetOutputSpacing(imageData->GetSpacing());
//    resample->SetOutputExtent(imageData->GetExtent());
    resample->Update();
    imageData = resample->GetOutput();

    vtkSmartPointer<vtkFloatArray> temperature = vtkFloatArray::SafeDownCast(polyData->GetCellData()->GetArray("temperature"));
    imageData->GetPointData()->SetScalars(temperature);

    vtkSmartPointer<vtkFixedPointVolumeRayCastMapper> mapper = vtkSmartPointer<vtkFixedPointVolumeRayCastMapper>::New();
//    vtkSmartPointer<vtkSmartVolumeMapper> mapper = vtkSmartPointer<vtkSmartVolumeMapper>::New();
    mapper->SetInputData(imageData);

//    imageData->Print(std::cout);

    vtkSmartPointer<vtkColorTransferFunction> colorFunc = vtkSmartPointer<vtkColorTransferFunction>::New();
    colorFunc->AddRGBPoint(-6378.21, 0.0, 0.0, 1.0);
    colorFunc->AddRGBPoint(0, 0.0, 1.0, 0.0);
    colorFunc->AddRGBPoint(6378.21, 1.0, 0.0, 0.0);
    vtkSmartPointer<vtkPiecewiseFunction> opacityFunc = vtkSmartPointer<vtkPiecewiseFunction>::New();
    opacityFunc->AddPoint(-1000.0, 1.0);
    opacityFunc->AddPoint(0.0, 1.0);
    opacityFunc->AddPoint(1000.0, 1.0);

    vtkSmartPointer<vtkVolumeProperty> property = vtkSmartPointer<vtkVolumeProperty>::New();
    property->SetColor(colorFunc);
    property->SetScalarOpacity(opacityFunc);
    property->ShadeOn();

    this->mVolume->SetMapper(mapper);
    this->mVolume->SetProperty(property);

//    vtkNew<vtkSmartVolumeMapper> volumeMapper;
//    volumeMapper->SetBlendModeToComposite();  // composite first
//    // NOTE: using input data was just how they do it in the following example:
//    // https://kitware.github.io/vtk-examples/site/Cxx/VolumeRendering/SmartVolumeMapper/
//    // volumeMapper->SetInputData(/* TODO */);
//
////    volumeMapper->SetInputConnection(assignAttribute->GetOutputPort());
//    volumeMapper->SetInputData(imageData);
//    volumeMapper->SelectScalarArray("temperature");
//    volumeMapper->SetScalarModeToUseCellData();
//    volumeMapper->SetRequestedRenderModeToRayCast();

    // this->mActor->SetMapper((vtkMapper *)&*volumeMapper);
//    this->mVolume->SetMapper(volumeMapper);
//    this->mActor->SetMapper(mapper);
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

    //    TODO: assignAttribute contains no Data, fix?
    //    assignAttribute->GetOutput()->Print(std::cout);

    //    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    //    plane->SetOrigin(0.0, 0.0, 0.0);
    //    plane->SetNormal(0.0, 1.0, 0.0);
    //
    //    vtkSmartPointer<vtkClipPolyData> clipper = vtkSmartPointer<vtkClipPolyData>::New();
    //    clipper->SetInputData(polyData);
    //    clipper->SetClipFunction(plane);
    //    clipper->InsideOutOn();
    //    clipper->Update();

    //    vtkSmartPointer<vtkCutter> cutter =
    //            vtkSmartPointer<vtkCutter>::New();
    //    cutter->SetInputData(structuredData);
    //    cutter->SetCutFunction(plane);
    //    cutter->Update();

    //    vtkSmartPointer<vtkDataSetMapper> mapper = vtkSmartPointer<vtkDataSetMapper>::New();
    //    mapper->SetInputData(clipper->GetOutput());
    //    mapper->SetScalarModeToUseCellData();
    //    mapper->SelectColorArray("temperature");
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
