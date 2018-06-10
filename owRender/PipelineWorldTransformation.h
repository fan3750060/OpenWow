#pragma once

class PipelineWorldTransformation
{
public:
	PipelineWorldTransformation() 
	{ 
		Clear();
	}
	inline void Clear()
	{
		worldTransformation.c[0][0] = 1; worldTransformation.c[1][0] = 0; worldTransformation.c[2][0] = 0; worldTransformation.c[3][0] = 0;
		worldTransformation.c[0][1] = 0; worldTransformation.c[1][1] = 1; worldTransformation.c[2][1] = 0; worldTransformation.c[3][1] = 0;
		worldTransformation.c[0][2] = 0; worldTransformation.c[1][2] = 0; worldTransformation.c[2][2] = 1; worldTransformation.c[3][2] = 0;
		worldTransformation.c[0][3] = 0; worldTransformation.c[1][3] = 0; worldTransformation.c[2][3] = 0; worldTransformation.c[3][3] = 1;
	}

	inline void Translate(float x, float y, float z);
	inline void Translate(cvec3 _translate);

	inline void RotateX(float _yaw);
	inline void RotateY(float _pitch);
	inline void RotateZ(float _roll);
	inline void Rotate(float _yaw, float _pitch, float _roll);
	inline void Rotate(cvec3 _rotation);

	inline void Scale(float _scale);
	inline void Scale(float _scaleX, float _scaleY, float _scaleZ);
	inline void Scale(cvec3 _scale);

	//

	inline void SetWorld(const mat4 _world);
	inline const mat4& GetWorld();

private:
	mat4 worldTransformation;
};

#include "PipelineWorldTransformation.inl"