#include "Volumes.h"
#include <vtkAssignAttribute.h>
#include <vtkDataSetAttributes.h>


void AddSegmentToCTF(
        const vtkSmartPointer<vtkColorTransferFunction>& ctf,
        double from, double r1, double g1, double b1,
        double to, double r2, double g2, double b2,
        int nr_contours = 2, double thickness = 25
        ) {
//            -1100, 0, 0, 1,
//            -150,  0, 1, 1
    double r_step = (r2-r1)/nr_contours; // 0
    double g_step = (g2-g1)/nr_contours; // 1
    double b_step = (b2-b1)/nr_contours; // 0

    double eps;
    if (from > 0) {
        eps = 1e-5;
    } else {
        eps = 1e-5;
    }

    double step_size = (to - from)/nr_contours;
    ctf->AddRGBPoint(
            from,
            r1,
            g1,
            b1
    );
    for (int i = 1; i < nr_contours; i++) {
        ctf->AddRGBPoint(
                from + i * step_size - thickness/2 - eps,
                r1 + r_step * i,
                g1 + g_step * i,
                b1 + b_step * i
        );
        ctf->AddRGBPoint(
                from + i * step_size - thickness/2,
                0,
                0,
                0
        );
        ctf->AddRGBPoint(
                from + i * step_size + thickness/2,
                0,
                0,
                0
        );
        ctf->AddRGBPoint(
                from + i * step_size + thickness/2 + eps,
                r1 + r_step * i,
                g1 + g_step * i,
                b1 + b_step * i
                );
    }
//    eps = -0.1;
//    ctf->AddRGBPoint(-1100, 0., 0., 1.);
//    ctf->AddRGBPoint(-150, 0., 1., 1.);
//    ctf->AddRGBPoint(-75, 0.5, 1., 1.);
//    ctf->AddRGBPoint(-74.99, 0., 0., 0.);
//    ctf->AddRGBPoint(-50.0, 0., 0., 0.);
//    ctf->AddRGBPoint(-49.99, 0.65, 1., 1.);
//    ctf->AddRGBPoint(0, 1., 1., 1.);
//    ctf->AddRGBPoint(150, 1., 1., 0.);
//    ctf->AddRGBPoint(1100, 1.0, 0., 0.);
}

Volumes::Volumes()
    : Resample::Resample()
    , mVolume(vtkSmartPointer<vtkVolume>::New())
{
    MantleIO::MantleAttr property  = MantleIO::MantleAttr::TempAnom;
    auto                 resampler = Resample::GetResampled();
    auto data = MantleIO::Mantle::GetCurrentStream();

    vtkNew<vtkAssignAttribute> assignAttribute;
    assignAttribute->SetInputConnection(data->GetOutputPort());
    assignAttribute->Assign(property.c_str(), vtkDataSetAttributes::SCALARS,
                            vtkAssignAttribute::POINT_DATA);

#ifndef NDEBUG
    assignAttribute->GetOutput()->Print(std::cout);
#endif

    double range[] = { -1100., 1100. };

    // Create a color transfer function
    vtkNew<vtkColorTransferFunction> colorTransferFunction;
    colorTransferFunction->SetNanOpacity(1.0);
    colorTransferFunction->SetColorSpaceToRGB();
    colorTransferFunction->SetScaleToLinear();
    colorTransferFunction->AdjustRange(range);
    AddSegmentToCTF(
            colorTransferFunction,
            -1100,0, 0, 1,
            -150, 0, 1, 1,
            3, 20
            );
    AddSegmentToCTF(
            colorTransferFunction,
            -150,0, 1, 1,
            0, 1, 1, 1,
            4, 20
    );
    AddSegmentToCTF(
            colorTransferFunction,
            0,1, 1, 1,
            150, 1, 1, 0,
            4, 20
    );
    AddSegmentToCTF(
            colorTransferFunction,
            150,1, 1, 0,
            1100, 1, 0, 0,
            3, 20
    );
    // Create a piecewise function
    vtkNew<vtkPiecewiseFunction> opacityTransferFunction;
    opacityTransferFunction->AddPoint(-1100, 0.8);
    opacityTransferFunction->AddPoint(1100, 0.8);

    vtkNew<vtkVolumeProperty> volumeProperty;
    volumeProperty->ShadeOff();
    volumeProperty->SetInterpolationTypeToLinear();
    volumeProperty->SetColor(colorTransferFunction);
    volumeProperty->SetScalarOpacity(opacityTransferFunction);

    vtkSmartPointer<vtkPlane> plane = vtkSmartPointer<vtkPlane>::New();
    plane->SetOrigin(0., 0., 0.);
    plane->SetNormal(0., 1.0, 0);


    // Add a mapper to create graphic primitives from the data
    vtkNew<vtkSmartVolumeMapper> volumeMapper;
    volumeMapper->SetInputConnection(assignAttribute->GetOutputPort());
#ifndef NDEBUG
    std::cout << "Loading scalar array: " << property.c_str() << " for volume render"
              << std::endl;
#endif
    volumeMapper->SelectScalarArray(property.c_str());
    volumeMapper->AddClippingPlane(plane);
    // volumeMapper->SetBlendModeToIsoSurface();
    // volumeMapper->SetSampleDistance(4);

    this->mVolume->SetMapper(volumeMapper);
    this->mVolume->SetProperty(volumeProperty);
    // TODO adjust this offset so the scene looks nice
    // this->mVolume->SetPosition(14000, 0, 14000);
}

void Volumes::ConnectToScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->AddVolume(mVolume);
}

void Volumes::RemoveFromScene(vtkSmartPointer<vtkRenderer> renderer)
{
    renderer->RemoveVolume(mVolume);
}
