#include "stdafx.h"

// Include
#include "MapController.h"

// General
#include "SkyManager.h"

const float C_SkyRadius = 400.0f;
const uint32 C_SkySegmentsCount = 32;

//.............................top.............................med.............................medh............................horiz...........................bottom
const float  C_SkyAngles[] = { 90.0f,                          30.0f,                          15.0f,                          5.0f,                           0.0f,                           -30.0f,                       -90.0f                       };
const uint32 C_Skycolors[] = { LightColors::LIGHT_COLOR_SKY_0, LightColors::LIGHT_COLOR_SKY_1, LightColors::LIGHT_COLOR_SKY_2, LightColors::LIGHT_COLOR_SKY_3, LightColors::LIGHT_COLOR_SKY_4, LightColors::LIGHT_COLOR_FOG, LightColors::LIGHT_COLOR_FOG };
const uint32 C_SkycolorsCount = 7;

SkyManager::SkyManager(MapController* _mapController, DBC_MapRecord _mapRecord) :
	m_MapController(_mapController)
{
	for (auto& it : DBC_Light)
	{
		if (_mapRecord.Get_ID() == it.Get_MapID()->Get_ID())
		{
			Sky* sky = new Sky(it);
			skies.push_back(sky);
		}
	}

	std::sort(skies.begin(), skies.end(), [](const Sky* lhs, const Sky* rhs)
	{
		if (lhs->m_IsGlobalSky)
			return false;
		else if (rhs->m_IsGlobalSky)
			return true;
		else
			return lhs->m_Range.max < rhs->m_Range.max;
	});

	if (skies.size() > 0 && !skies.back()->m_IsGlobalSky)
	{
		Log::Error("Sky for maps [%d] size [%d] don't have global sky!!!", _mapRecord.Get_ID(), skies.size());
		skies.back()->m_IsGlobalSky = true;
	}

	InitBuffer();

	AddManager<ISkyManager>(this);
}

SkyManager::~SkyManager()
{
	ERASE_VECTOR(skies);

	_Bindings->UnregisterRenderable3DObject(this);
}

void SkyManager::Calculate(uint32 _time)
{
	if (skies.empty())
	{
		return;
	}

	//CalculateSkiesWeights(_Render->getCamera()->Position);

	// interpolation
	m_Interpolated.Clear();
	for (auto& it : skies)
	{
		if (it->m_Wight > 0.0f)
		{
			SkyParams params = it->Interpolate(_time);
			params *= it->m_Wight;

			m_Interpolated += params;
		}
	}

	// Geometry
	vector<vec4> colors;
	for (uint32 h = 0; h < C_SkySegmentsCount; h++)
	{
		for (uint32 v = 0; v < C_SkycolorsCount - 1; v++)
		{
			colors.push_back(vec4(m_Interpolated.m_Colors[C_Skycolors[v]], 0.0f));
			colors.push_back(vec4(m_Interpolated.m_Colors[C_Skycolors[v + 1]], 0.0f));
			colors.push_back(vec4(m_Interpolated.m_Colors[C_Skycolors[v + 1]], 0.0f));
			colors.push_back(vec4(m_Interpolated.m_Colors[C_Skycolors[v + 1]], 0.0f));
			colors.push_back(vec4(m_Interpolated.m_Colors[C_Skycolors[v]], 0.0f));
			colors.push_back(vec4(m_Interpolated.m_Colors[C_Skycolors[v]], 0.0f));
		}
	}

	// Fill buffer with color
	//colorsBuffer->updateBufferData(0, colors.size() * sizeof(vec4), colors.data());
}



bool SkyManager::DEBUG_Render()
{
	/*_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);

	CDebug_GeometryPass* pass = _Render->getTechniquesMgr()->Debug_Pass.operator->();
	pass->Bind();
	{
		_Render->r.setGeometry(_Render->getRenderStorage()->_sphereGeo);

		for (auto& it : skies)
		{
			mat4 worldMatrix;
			worldMatrix = glm::translate(worldMatrix, it->m_Position);
			worldMatrix = glm::scale(worldMatrix, vec3(it->m_Range.max));
			pass->setWorld(worldMatrix);

			pass->SetColor4(vec4(1.0f, 1.0f, 0.0f, 0.3f));

			_Render->r.drawIndexed(0, 128 * 3, 0, 126, nullptr, false);
		}
	}
	pass->Unbind();


	_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);*/

	return false;
}

bool SkyManager::PreRender3D()
{
	if (skies.empty())
	{
		return false;
	}

	Calculate(m_MapController->getTime()->GetTime());

	return true;
}

void SkyManager::Render3D()
{
	/*_Render->r.setDepthTest(false);
	//_Render->r.setFillMode(R_FillMode::RS_FILL_WIREFRAME);
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);

	CSky_GeometryPass* pass = _Render->getTechniquesMgr()->Sky_Pass.operator->();
	pass->Bind();
	{
		mat4 worldMatrix;
		worldMatrix = glm::translate(worldMatrix, _Render->getCamera()->Position);
		pass->setWorld(worldMatrix);

		_Render->r.setGeometry(__geom);
		_Render->r.draw(0, __vertsSize);
	}
	pass->Unbind();

	//_Render->r.setFillMode(R_FillMode::RS_FILL_SOLID);
	_Render->r.setCullMode(R_CullMode::RS_CULL_BACK);
	_Render->r.setDepthTest(true);*/

	//DEBUG_Render();
}

void SkyManager::InitBuffer()
{
	// Draw sky
	vec3 basepos1[C_SkycolorsCount];
	vec3 basepos2[C_SkycolorsCount];

	vector<vec3> vertices;

	for (uint32 h = 0; h < C_SkySegmentsCount; h++)
	{
		for (uint32 i = 0; i < C_SkycolorsCount; i++)
		{
			basepos1[i] = basepos2[i] = vec3(cosf(glm::radians(C_SkyAngles[i])), sinf(glm::radians(C_SkyAngles[i])), 0.0f) * C_SkyRadius;
			rotate(0, 0, &basepos1[i].x, &basepos1[i].z, glm::two_pi<float>() / C_SkySegmentsCount * (h + 0));
			rotate(0, 0, &basepos2[i].x, &basepos2[i].z, glm::two_pi<float>() / C_SkySegmentsCount * (h + 1));
		}

		for (uint32 v = 0; v < C_SkycolorsCount - 1; v++)
		{
			vertices.push_back(basepos1[v]);
			vertices.push_back(basepos1[v + 1]);
			vertices.push_back(basepos2[v + 1]);

			vertices.push_back(basepos2[v + 1]);
			vertices.push_back(basepos2[v]);
			vertices.push_back(basepos1[v]);
		}
	}

	// Vertex buffer
	SharedBufferPtr vertexBuffer = Application::Get().GetRenderDevice()->CreateFloatVertexBuffer((const float*)vertices.data(), vertices.size(), sizeof(vec3));

	// Colors buffer
	colorsBuffer = Application::Get().GetRenderDevice()->CreateFloatVertexBuffer(nullptr, vertices.size(), sizeof(vec4));

	// Geometry
	__geom = Application::Get().GetRenderDevice()->CreateMesh();
	__geom->AddVertexBuffer(BufferBinding("POSITION", 0), vertexBuffer);
	__geom->AddVertexBuffer(BufferBinding("COLOR", 1), colorsBuffer);

	//__geom = _Render->r.beginCreatingGeometry(PRIM_TRILIST, _Render->getRenderStorage()->__layout_GxVBF_PC);
	//__geom->setGeomVertexParams(vertexBuffer, R_DataType::T_FLOAT, 0, 0);
	//__geom->setGeomVertexParams(colorsBuffer, R_DataType::T_FLOAT, 0, 0);
	//__geom->finishCreatingGeometry();

	_Bindings->RegisterRenderable3DObject(this, 15);
}

void SkyManager::CalculateSkiesWeights(cvec3 pos)
{
	skies.back()->m_Wight = 1.0f;
	_ASSERT(skies.back()->m_IsGlobalSky);

	for (int i = skies.size() - 2; i >= 0; i--)
	{
		Sky* s = skies[i];
		const float dist = glm::length(pos - s->m_Position);

		if (dist < s->m_Range.min)
		{
			// we're in a sky, zero out the rest
			s->m_Wight = 1.0f;
			for (uint32_t j = i + 1; j < skies.size(); j++)
			{
				skies[j]->m_Wight = 0.0f;
			}
		}
		else if (dist < s->m_Range.max)
		{
			// we're in an outer area, scale down the other weights
			float r = (dist - s->m_Range.min) / (s->m_Range.max - s->m_Range.min);
			s->m_Wight = 1.0f - r;
			for (uint32_t j = i + 1; j < skies.size(); j++)
			{
				skies[j]->m_Wight *= r;
			}
		}
		else
		{
			s->m_Wight = 0.0f;
		}
	}
}