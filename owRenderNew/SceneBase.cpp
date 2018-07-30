#include "stdafx.h"

#include "Timer.h"

#include "Material.h"
#include "Texture.h"
#include "Mesh.h"
#include "BufferBinding.h"
#include "SceneNode.h"
#include "Visitor.h"


#include "SceneBase.h"

#define EXPORT_FORMAT "assbin"
#define EXPORT_EXTENSION "assbin"

// A private class that is registered with Assimp's importer
// Provides feedback on the loading progress of the scene files.
// 

SceneBase::SceneBase()
	: base()
	, m_pRootNode(nullptr)
	, m_bFileChanged(false)
{
}

SceneBase::~SceneBase()
{
}

bool SceneBase::LoadFromString(cstring sceneStr, cstring format)
{
	/*Assimp::Importer importer;
	const aiScene* scene = nullptr;

	importer.SetProgressHandler(new ProgressHandler(*this, L"String"));

	importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
	importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

	unsigned int preprocessFlags = aiProcessPreset_TargetRealtime_MaxQuality;

	scene = importer.ReadFileFromMemory(sceneStr.data(), sceneStr.size(), preprocessFlags, format.c_str());

	if (!scene)
	{
		Log::Error(importer.GetErrorString());
		return false;
	}
	else
	{
		// If we have a previously loaded scene, delete it.
		if (m_pRootNode)
		{
			m_pRootNode.reset();
		}

		// Import scene materials.
		for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
		{
			ImportMaterial(*scene->mMaterials[i], fs::current_path());
		}
		// Import meshes
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			ImportMesh(*scene->mMeshes[i]);
		}

		m_pRootNode = ImportSceneNode(m_pRootNode, scene->mRootNode);
	}*/

	return true;
}

bool SceneBase::LoadFromFile(cstring fileName)
{
	/*fs::path filePath(fileName);
	fs::path parentPath;

	m_SceneFile = fileName;

	// Setup the dependency tracker.
	m_DependencyTracker = DependencyTracker(m_SceneFile);
	if (!m_DependencyTracker.Load())
	{
		// If there is no dependency tracker file, try to create one.
		m_DependencyTracker.Save();
	}

	if (filePath.has_parent_path())
	{
		parentPath = filePath.parent_path();
	}
	else
	{
		parentPath = fs::current_path();
	}

	const aiScene* scene;
	Assimp::Importer importer;

	importer.SetProgressHandler(new ProgressHandler(*this, fileName));

	fs::path exportPath = filePath;
	exportPath.replace_extension(EXPORT_EXTENSION);

	// Force the last load time to be that of the exported scene file. This will ensure that if the original scene file 
	// or any of its dependencies have been modified, the scene will be re-imported.
	m_DependencyTracker.SetLastLoadTime((fs::exists(exportPath) && fs::is_regular_file(exportPath)) ? fs::last_write_time(exportPath) : 0);

	if (fs::exists(exportPath) && fs::is_regular_file(exportPath) && !m_DependencyTracker.IsStale())
	{
		// If a previously exported file exists, load that instead (scene has already been preprocessed).
		scene = importer.ReadFile(exportPath.string(), 0);
	}
	else
	{
		// If no serialized version of the model file exists (or the original model is newer than the serialized version),
		// reimport the original scene and export it as binary.
		importer.SetPropertyFloat(AI_CONFIG_PP_GSN_MAX_SMOOTHING_ANGLE, 80.0f);
		importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE, aiPrimitiveType_POINT | aiPrimitiveType_LINE);

		unsigned int preprocessFlags = aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_OptimizeGraph;
		scene = importer.ReadFile(filePath.string(), preprocessFlags);

		// Update the last load time for the dependency tracker.
		m_DependencyTracker.SetLastLoadTime();

		if (scene)
		{
			// Now export the preprocessed scene so we can load it faster next time.
			Assimp::Exporter exporter;
			exporter.Export(scene, EXPORT_FORMAT, exportPath.string(), preprocessFlags);
		}
	}

	if (!scene)
	{
		Log::Error(importer.GetErrorString());
		return false;
	}
	else
	{
		// If we have a previously loaded scene, delete it.
		mat4 localTransform(1);
		if (m_pRootNode)
		{
			// Save the root nodes local transform
			// so it can be restored on reload.
			localTransform = m_pRootNode->GetLocalTransform();
			m_pRootNode.reset();
		}
		// Delete the previously loaded assets.
		m_MaterialMap.clear();
		m_Materials.clear();
		m_Meshes.clear();

		// Import scene materials.
		for (unsigned int i = 0; i < scene->mNumMaterials; ++i)
		{
			ImportMaterial(*scene->mMaterials[i], parentPath);
		}
		// Import meshes
		for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
		{
			ImportMesh(*scene->mMeshes[i]);
		}

		m_pRootNode = ImportSceneNode(m_pRootNode, scene->mRootNode);
		m_pRootNode->SetLocalTransform(localTransform);
	}*/

	return true;
}


std::shared_ptr<SceneNode> SceneBase::GetRootNode() const
{
	return m_pRootNode;
}

void SceneBase::Render(RenderEventArgs& args)
{
	if (m_pRootNode)
	{
		m_pRootNode->Render(args);
	}
}

void SceneBase::OnLoadingProgress(ProgressEventArgs& e)
{
	base::OnLoadingProgress(e);
}


void SceneBase::Accept(Visitor& visitor)
{
	visitor.Visit(*this);

	if (m_pRootNode)
	{
		m_pRootNode->Accept(visitor);
	}
}