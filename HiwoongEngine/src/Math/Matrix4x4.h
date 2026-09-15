#pragma once

#include "Core/Core.h"
#include "Math/Vector4.h"
#include "Math/Vector3.h"

namespace Hiwoong
{
	//Represents a 4x4 transfromation matrix.
	//Converts positions and directions between coordinate spaces.
	//
	//Rendering transfromation order:
	//Local -> World(Model) -> Camera(View) -> Projection
	class Hiwoong_API Matrix4x4
	{


	public:
		Matrix4x4();
		
		// Creates an identity matrix that does not change the input vector.
		static Matrix4x4 Identity();

		Vector4 operator*(const Vector4& vector) const;

		Matrix4x4 operator*(const Matrix4x4& other) const;


		/*
			1 0 0  MoveX
			0 1 0  MoveY
			0 0 1  MoveZ
			0 0 0    1
		*/
		static Matrix4x4 Translation(const Vector3& translation);

	
		/*	
			1    0      0    0
			0   cos   -sin   0
			0   sin    cos   0
			0    0      0    1
		*/
		static Matrix4x4 RotationX(float radians);


		/*	
			cos   0   sin   0
			 0    1    0    0
		   -sin   0   cos   0
			 0    0    0    1
		*/
		static Matrix4x4 RotationY(float radians);


		/*
			cos  -sin   0   0
			sin   cos   0   0
			 0     0    1   0
			 0     0    0   1
		 */
		static Matrix4x4 RotationZ(float radians);


		/*
		 scale.x   0        0      0
		   0    scale.y     0      0
		   0       0     scale.z   0
		   0       0        0      1
	   */
		static Matrix4x4 Scale(const Vector3& scale);


		static Matrix4x4 LookAt(
			const Vector3& cameraPosition,
			const Vector3& target,
			const Vector3& up
		);

		//FieldOfView :: The camera's vertical field of view in radian.
		//aspectRatio :: Ratio of width / height
		//nearPlane :: the neariest distance when the camera see.
		//farPlane :: the farest distance when the camera see.

		static Matrix4x4 Perspective(
			float fieldOfView,
			float aspectRatio,
			float nearPlane,
			float farPlane
		);


	private:
		float values[4][4];
	};
}


