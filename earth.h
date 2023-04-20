#pragma once

#include <vtkTexturedSphereSource.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkProperty.h>
#include <vtkJPEGReader.h>
#include <vtkTexture.h>
#include <vtkRenderer.h>

/// <summary>
/// Class that represents the Earth.
/// </summary>
class Earth
{
public:
	/// <summary>
	/// Constructor.
	/// </summary>
	Earth()
	{
		// Create a sphere with texture coordinates
		const double sphere_radius = 0.05;
		vtkNew<vtkTexturedSphereSource> sphereSource;
		sphereSource->SetRadius(sphere_radius);
		sphereSource->SetPhiResolution(100);
		sphereSource->SetThetaResolution(100);
		vtkNew<vtkPolyDataMapper> mapper;
		mapper->SetInputConnection(sphereSource->GetOutputPort());

		// read texture
		vtkNew<vtkJPEGReader> reader;
		reader->SetFileName("../images/earth.jpg");
		vtkNew<vtkTexture> texture;
		texture->SetInputConnection(reader->GetOutputPort());
		texture->SetUseSRGBColorSpace(true);

		mActor = vtkSmartPointer<vtkActor>::New();
		mActor->SetMapper(mapper);
		mActor->GetProperty()->SetColor(1, 1, 1);
		mActor->GetProperty()->SetMetallic(0);
		mActor->GetProperty()->SetRoughness(1);
		mActor->GetProperty()->SetBaseColorTexture(texture);
		mActor->GetProperty()->SetInterpolationToPBR();
		mActor->SetPosition(0, 0, 0);
	}

	/// <summary>
	/// Updates the properties of the Earth.
	/// </summary>
	/// <param name="dt">Time passed since the last Update in milliseconds.</param>
	/// <param name="t">Total time passed since start of the application in milliseconds.</param>
	void Update(double dt, double t)
	{
		// rotate actor
		double angle_dt = dt * 0.1;							// not a realistic rotation speed!
		mActor->RotateWXYZ(angle_dt, 0, 0.398749, 0.91706);	// 23.5� titled rotation axis
	}

	/// <summary>
	/// Adds the actors to the renderer.
	/// </summary>
	/// <param name="renderer">Renderer to add the actors to.</param>
	void InitRenderer(vtkSmartPointer<vtkRenderer> renderer)
	{
		renderer->AddActor(mActor);
	}

private:
	Earth(const Earth&) = delete;			// Delete the copy-constructor.
	void operator=(const Earth&) = delete;	// Delete the assignment operator.

	vtkSmartPointer<vtkActor> mActor;		// Actor that represents the scene geometry.
};