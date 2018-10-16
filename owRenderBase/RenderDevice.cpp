#include "stdafx.h"

// General
#include "RenderDevice.h"

// GLM's own quaternion from two vector constructor does not handle cases 
// where the vectors may be pointing in opposite directions.
// This method handles the cases where the u and v vectors are opposites.
// source: http://lolengine.net/blog/2014/02/24/quaternion-from-two-vectors-final
// accessed: 26/05/2015
inline glm::quat RotationFromTwoVectors(cvec3 u, cvec3 v)
{
	float normUV = glm::sqrt(glm::dot(u, u) * glm::dot(v, v));
	float real = normUV + glm::dot(u, v);

	vec3 vec;

	if (real < 1.e-6f * normUV)
	{
		/* If u and v are exactly opposite, rotate 180 degrees
		* around an arbitrary orthogonal axis. Axis normalisation
		* can happen later, when we normalise the quaternion.
		*/
		real = 0.0f;
		vec = (glm::abs(u.x) > abs(u.z)) ? vec3(-u.y, u.x, 0.0f) : vec3(0.0f, -u.z, u.y);
	}
	else
	{
		/* Otherwise, build quaternion the standard way. */
		vec = glm::cross(u, v);
	}

	return glm::normalize(glm::quat(real, vec));
}

std::shared_ptr<Mesh> RenderDevice::CreatePlane(cvec3 N)
{
	vec3 p[4];
	p[0] = vec3(1.0f, 0, 1.0f);
	p[1] = vec3(-1.0f, 0, 1.0f);
	p[2] = vec3(-1.0f, 0, -1.0f);
	p[3] = vec3(1.0f, 0, -1.0f);

	vec2 t[4];
	t[0] = vec2(1, 1);
	t[1] = vec2(0, 1);
	t[2] = vec2(0, 0);
	t[3] = vec2(1, 0);

	uint16 i[6];
	i[0] = 0;
	i[1] = 1;
	i[2] = 2;
	i[3] = 2;
	i[4] = 3;
	i[5] = 0;

	std::shared_ptr<Mesh> mesh = CreateMesh();

	std::shared_ptr<Buffer> __vb = CreateFloatVertexBuffer((const float*)p, 4, sizeof(vec3));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<Buffer> __tb = CreateFloatVertexBuffer((const float*)t, 4, sizeof(vec2));
	mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __tb);

	std::shared_ptr<Buffer> __ib = CreateUInt16IndexBuffer(i, 6);
	mesh->SetIndexBuffer(__ib);

	//std::shared_ptr<Material> mat = CreateMaterial();
	//mat->SetDiffuseColor(vec4(1, 0, 0, 1));
	//mesh->SetMaterial(mat);

	return mesh;
}

std::shared_ptr<Mesh> RenderDevice::CreateScreenQuad(float left, float right, float bottom, float top, float z)
{
	vec3 p[4]; // Vertex position
	vec3 n[4]; // Vertex normal (required for texture patch polygons)
	vec2 t[4]; // Texture coordinates
			   // Winding order is assumed to be right-handed. Front-facing polygons have
			   // a counter-clockwise winding order.
			   // Assimp flips the winding order of vertices.. Don't ask me why. To account for this,
			   // the vertices are loaded in reverse order :)
	p[0] = vec3(right, bottom, z);   n[0] = vec3(0, 0, 1);    t[0] = vec2(1, 0);
	p[1] = vec3(left, bottom, z);    n[1] = vec3(0, 0, 1);    t[1] = vec2(0, 0);
	p[2] = vec3(left, top, z);       n[2] = vec3(0, 0, 1);    t[2] = vec2(0, 1);
	p[3] = vec3(right, top, z);      n[3] = vec3(0, 0, 1);    t[3] = vec2(1, 1);


	std::shared_ptr<Mesh> mesh = CreateMesh();

	std::shared_ptr<Buffer> __vb = CreateFloatVertexBuffer((const float*)p, 4, sizeof(vec3));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<Buffer> __tb = CreateFloatVertexBuffer((const float*)t, 4, sizeof(vec2));
	mesh->AddVertexBuffer(BufferBinding("TEXCOORD", 0), __tb);

	std::shared_ptr<Buffer> __nb = CreateFloatVertexBuffer((const float*)n, 4, sizeof(vec3));
	mesh->AddVertexBuffer(BufferBinding("NORMAL", 0), __nb);

	//std::shared_ptr<Material> mat = CreateMaterial();
	//mat->SetDiffuseColor(vec4(1, 1, 1, 1));
	//mesh->SetMaterial(mat);

	return mesh;
}

std::shared_ptr<Mesh> RenderDevice::CreateSphere()
{
	vec3 spVerts[126] =
	{  // x, y, z
		vec3(0.0f, 1.0f, 0.0f),        vec3(0.0f, -1.0f, 0.0f),
		vec3(-0.707f, 0.0f, 0.707f),   vec3(0.707f, 0.0f, 0.707f),
		vec3(0.707f, 0.0f, -0.707f),   vec3(-0.707f, 0.0f, -0.707f)
	};

	uint16 spInds[128 * 3] = {  // Number of faces: (4 ^ iterations) * 8
		2, 3, 0,   3, 4, 0,   4, 5, 0,   5, 2, 0,   2, 1, 3,   3, 1, 4,   4, 1, 5,   5, 1, 2
	};

	for (uint32 i = 0, nv = 6, ni = 24; i < 2; ++i)  // Two iterations
	{
		// Subdivide each face into 4 tris by bisecting each edge and push vertices onto unit sphere
		for (uint32 j = 0, prevNumInds = ni; j < prevNumInds; j += 3)
		{
			spVerts[nv++] = glm::normalize(((spVerts[spInds[j + 0]] + spVerts[spInds[j + 1]]) * 0.5f));
			spVerts[nv++] = glm::normalize(((spVerts[spInds[j + 1]] + spVerts[spInds[j + 2]]) * 0.5f));
			spVerts[nv++] = glm::normalize(((spVerts[spInds[j + 2]] + spVerts[spInds[j + 0]]) * 0.5f));

			spInds[ni++] = spInds[j + 0];
			spInds[ni++] = nv - 3;
			spInds[ni++] = nv - 1;

			spInds[ni++] = nv - 3;
			spInds[ni++] = spInds[j + 1];
			spInds[ni++] = nv - 2;

			spInds[ni++] = nv - 2;
			spInds[ni++] = spInds[j + 2];
			spInds[ni++] = nv - 1;

			spInds[j + 0] = nv - 3;
			spInds[j + 1] = nv - 2;
			spInds[j + 2] = nv - 1;
		}
	}

	std::shared_ptr<Mesh> mesh = CreateMesh();

	std::shared_ptr<Buffer> __vb = CreateFloatVertexBuffer((const float*)spVerts, 126, sizeof(vec3));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<Buffer> __ib = CreateUInt16IndexBuffer((const uint16*)spInds, 128 * 3);
	mesh->SetIndexBuffer(__ib);

	//std::shared_ptr<Material> mat = CreateMaterial();
	//mat->SetDiffuseColor(vec4(1, 0, 0, 1));
	//mesh->SetMaterial(mat);

	return mesh;
}

std::shared_ptr<Mesh> RenderDevice::CreateCube()
{
	float cubeVerts[8 * 3] = {  // x, y, z
		-0.5f, -0.5f,  0.5f,   0.5f, -0.5f,  0.5f,  0.5f, 0.5f,  0.5f,   -0.5f, 0.5f, 0.5f,
		-0.5f, -0.5f, -0.5f,   0.5f, -0.5f, -0.5f,  0.5f, 0.5f, -0.5f,   -0.5f, 0.5f, -0.5f
	};
	uint16 cubeInds[36] = {
		0, 1, 2, 2, 3, 0,   1, 5, 6, 6, 2, 1,   5, 4, 7, 7, 6, 5,
		4, 0, 3, 3, 7, 4,   3, 2, 6, 6, 7, 3,   4, 5, 1, 1, 0, 4
	};

	std::shared_ptr<Mesh> mesh = CreateMesh();

	std::shared_ptr<Buffer> __vb = CreateFloatVertexBuffer((const float*)cubeVerts, 8 * 3, sizeof(vec3));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<Buffer> __ib = CreateUInt16IndexBuffer((const uint16*)cubeInds, 36);
	mesh->SetIndexBuffer(__ib);

	//std::shared_ptr<Material> mat = CreateMaterial();
	//mat->SetDiffuseColor(vec4(1, 0, 0, 1));
	//mesh->SetMaterial(mat);

	return mesh;
}

std::shared_ptr<Mesh> RenderDevice::CreateCylinder(float baseRadius, float apexRadius, float height, cvec3 axis)
{
	/*std::shared_ptr<Scene> scene = CreateScene();
	std::stringstream ss;

	// Create a white diffuse material for the cylinder.
	// f red green blue Kd Ks Shine transmittance indexOfRefraction
	ss << "f 1 1 1 1 0 0 0 0" << std::endl;

	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << "0 0 0 " << baseRadius << std::endl;

	vec3 apex = axis * height;
	// apex.x apex.y apex.z apexRadius
	ss << apex.x << " " << apex.y << " " << apex.z << " " << apexRadius << std::endl;

	if (scene->LoadFromString(ss.str(), "nff"))
	{
	return scene;
	}

	// An error occurred while loading the scene.
	DestroyScene(scene);*/
	return nullptr;
}

std::shared_ptr<Mesh> RenderDevice::CreateCone()
{
	float coneVerts[13 * 3] = {  // x, y, z
		0.f, 0.f, 0.f,
		0.f, 1.f, -1.f,   -0.5f, 0.866f, -1.f,   -0.866f, 0.5f, -1.f,
		-1.f, 0.f, -1.f,   -0.866f, -0.5f, -1.f,   -0.5f, -0.866f, -1.f,
		0.f, -1.f, -1.f,   0.5f, -0.866f, -1.f,   0.866f, -0.5f, -1.f,
		1.f, 0.f, -1.f,   0.866f, 0.5f, -1.f,   0.5f, 0.866f, -1.f,
	};
	uint16 coneInds[22 * 3] = {
		0, 1, 2,   0, 2, 3,   0, 3, 4,   0, 4, 5,   0, 5, 6,   0, 6, 7,
		0, 7, 8,   0, 8, 9,   0, 9, 10,   0, 10, 11,   0, 11, 12,   0, 12, 1,
		10, 6, 2,   10, 8, 6,   10, 9, 8,   8, 7, 6,   6, 4, 2,   6, 5, 4,   4, 3, 2,
		2, 12, 10,   2, 1, 12,   12, 11, 10
	};

	std::shared_ptr<Mesh> mesh = CreateMesh();

	std::shared_ptr<Buffer> __vb = CreateFloatVertexBuffer((const float*)coneVerts, 13 * 3, sizeof(vec3));
	mesh->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);

	std::shared_ptr<Buffer> __ib = CreateUInt16IndexBuffer((const uint16*)coneInds, 22 * 3);
	mesh->SetIndexBuffer(__ib);

	//std::shared_ptr<Material> mat = CreateMaterial();
	//mat->SetDiffuseColor(vec4(1, 0, 0, 1));
	//mesh->SetMaterial(mat);

	return mesh;
}

std::shared_ptr<Mesh> RenderDevice::CreateArrow(cvec3 tail, cvec3 head, float radius)
{
	/*std::shared_ptr<Scene> scene = CreateScene();
	std::stringstream ss;

	vec3 dir = head - tail;
	vec3 apex = head + (dir * 0.5f);

	// Create a white diffuse material for the arrow.
	// f red green blue Kd Ks Shine transmittance indexOfRefraction
	ss << "f 1 1 1 1 0 0 0 0" << std::endl;

	// Create a cylinder for the arrow body.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << tail.x << " " << tail.y << " " << tail.z << " " << radius << std::endl;
	// apex.x apex.y apex.z apexRadius
	ss << head.x << " " << head.y << " " << head.z << " " << radius << std::endl;

	// Create a cone for the arrow head.
	ss << "c" << std::endl;
	// base.x base.y base.z baseRadius
	ss << head.x << " " << head.y << " " << head.z << " " << radius * 2.0f << std::endl;

	// apex.x apex.y apex.z apexRadius
	ss << apex.x << " " << apex.y << " " << apex.z << " 0" << std::endl;

	if (scene->LoadFromString(ss.str(), "nff"))
	{
	return scene;
	}*/

	// An error occurred while loading the scene.
	return nullptr;

}

std::shared_ptr<Mesh> RenderDevice::CreateAxis(float radius, float length)
{

	return nullptr;
}

// Template specializations for vertex buffers.
template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<float> >(const std::vector<float>& data)
{
	return CreateFloatVertexBuffer(&(data[0]), (uint32)data.size(), sizeof(float));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec2> >(const std::vector<vec2>& data)
{
	return CreateFloatVertexBuffer(glm::value_ptr(data[0]), (uint32)data.size(), sizeof(vec2));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec3> >(const std::vector<vec3>& data)
{
	return CreateFloatVertexBuffer(glm::value_ptr(data[0]), (uint32)data.size(), sizeof(vec3));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateVertexBuffer< std::vector<vec4> >(const std::vector<vec4>& data)
{
	return CreateFloatVertexBuffer(glm::value_ptr(data[0]), (uint32)data.size(), sizeof(vec4));
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateIndexBuffer< std::vector<uint16> >(const std::vector<uint16>& data)
{
	return CreateUInt16IndexBuffer(&(data[0]), (uint16)data.size());
}

template<>
std::shared_ptr<Buffer> RenderDevice::CreateIndexBuffer< std::vector<uint32> >(const std::vector<uint32>& data)
{
	return CreateUInt32IndexBuffer(&(data[0]), (uint32)data.size());
}