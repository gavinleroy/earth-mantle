#include "LineIntegralConvolution.h"

LineIntegralConvolution::LineIntegralConvolution()
    : Mantle()
{
    // Specify that we only care about the velocity variables
    auto variables = std::vector<std::string>({
        "vx",
        "vy",
        "vz",
    });
    // Load the data from the file
    auto loaded_from_file = LoadFromFile("spherical001.nc", variables);

    // Convert the cell data to point data
    vtkNew<vtkCellDataToPointData> cellDataToPointData;
    cellDataToPointData->SetInputData(loaded_from_file);
    cellDataToPointData->Update();
    auto data = cellDataToPointData->GetStructuredGridOutput();

    // Aggregate the velocity into one array
    auto calculator = vtkSmartPointer<vtkArrayCalculator>::New();
    calculator->SetInputDataObject(data);
    calculator->SetResultArrayName("velocity");
    calculator->AddScalarVariable("vx", "vx");
    calculator->AddScalarVariable("vy", "vy");
    calculator->AddScalarVariable("vz", "vz");
    calculator->SetFunction("(iHat * vx + jHat * vy + kHat * vz) * 1e9");
    calculator->Update();

    // TODO do we need this? or can we do this without the intermediate geometry filter?
    // Geometry filter
    auto geometry = vtkSmartPointer<vtkGeometryFilter>::New();
    geometry->SetInputConnection(calculator->GetOutputPort());
    geometry->Update();
    vtkSmartPointer<vtkDataObject> dataObject = geometry->GetOutputDataObject(0);

    // Line integral convolution (LIC) mapper
    auto licMapper = vtkSmartPointer<vtkSurfaceLICMapper>::New();
    licMapper->SetInputConnection(calculator->GetOutputPort());
    licMapper->SetInputDataObject(dataObject);
    licMapper->SetInputArrayToProcess(0, 0, 0, vtkDataObject::POINT, "velocity");

    // LIC parameters
    auto licInterface = licMapper->GetLICInterface();
    // Disable enhanced LIC (for now)
    licInterface->SetEnhancedLIC(0);

    // Slice the geometry
    auto clippingPlane = vtkSmartPointer<vtkPlane>::New();
    clippingPlane->SetOrigin(0., 0., 0);
    clippingPlane->SetNormal(0., 1.0, -1.0);
    licMapper->AddClippingPlane(clippingPlane);

    // Actor
    mActor = vtkSmartPointer<vtkActor>::New();
    // TODO adjust this offset so the scene looks nice
    mActor->SetPosition(14000, 0, 0);
    mActor->SetMapper(licMapper);
}

vtkSmartPointer<vtkActor> LineIntegralConvolution::GetActor() {
    return mActor;
}