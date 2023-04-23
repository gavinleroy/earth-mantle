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

    // Extract the boundary surface
    auto surface = vtkSmartPointer<vtkDataSetSurfaceFilter>::New();
    surface->SetInputConnection(calculator->GetOutputPort());
    surface->Update();

    // Extract the data object from the boundary surface
    vtkSmartPointer<vtkDataObject> dataObject = surface->GetOutputDataObject(0);

    // Line integral convolution (LIC) mapper
    auto licMapper = vtkSmartPointer<vtkSurfaceLICMapper>::New();
    licMapper->SetInputConnection(surface->GetOutputPort());
    licMapper->SetInputDataObject(dataObject);
    licMapper->SetInputArrayToProcess(0, 0, 0, vtkDataObject::POINT, "velocity");

    // Actor
    mActor = vtkSmartPointer<vtkActor>::New();
    mActor->SetMapper(licMapper);
}

vtkSmartPointer<vtkActor> LineIntegralConvolution::GetActor() {
    return mActor;
}