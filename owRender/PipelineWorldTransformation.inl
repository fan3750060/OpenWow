#pragma once

// Set Translate

inline void PipelineWorldTransformation::Translate(float x, float y, float z)
{
	worldTransformation.translate(x, y, z);
}
inline void PipelineWorldTransformation::Translate(cvec3 _translate)
{
	worldTransformation.translate(_translate.x, _translate.y, _translate.z);
}

// Set Rotation

inline void PipelineWorldTransformation::Rotate(float _yaw, float _pitch, float _roll)
{
	worldTransformation.rotate(_yaw, _pitch, _roll);
}
inline void PipelineWorldTransformation::Rotate(cvec3 _rotation)
{
	worldTransformation.rotate(_rotation);
}

// Set Scale

inline void PipelineWorldTransformation::Scale(float _scale)
{
	worldTransformation.scale(_scale, _scale, _scale);
}
inline void PipelineWorldTransformation::Scale(float _scaleX, float _scaleY, float _scaleZ)
{
	worldTransformation.scale(_scaleX, _scaleY, _scaleZ);
}
inline void PipelineWorldTransformation::Scale(cvec3 _scale)
{
	worldTransformation.scale(_scale.x, _scale.y, _scale.z);
}


inline void PipelineWorldTransformation::SetWorld(const mat4 _world)
{
	worldTransformation = _world;
}

inline const mat4& PipelineWorldTransformation::GetWorld()
{
	return worldTransformation;
}