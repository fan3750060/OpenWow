#pragma once

//---------------------------------------------------------//
//--                    Common                           --//
//---------------------------------------------------------//
#include "../owEngineNew/common.h"

#include "../owEngineNew/GameStateManager.h"
#include "../owEngineNew/Loader.h"

// Base
#include "../owEngineNew/Application.h"

// Scenes
#include "../owEngineNew/Scene.h"
#include "../owEngineNew/SceneBase.h"
#include "../owEngineNew/SceneNode.h"

// Passes
#include "../owEngineNew/Passes/BasePass.h"
#include "../owEngineNew/Passes/RenderTechnique.h"
#include "../owEngineNew/Passes/ClearRenderTargetPass.h"
#include "../owEngineNew/Passes/CopyBufferPass.h"
#include "../owEngineNew/Passes/CopyTexturePass.h"
#include "../owEngineNew/Passes/GenerateMipMapsPass.h"
#include "../owEngineNew/Passes/OpaquePass.h"
#include "../owEngineNew/Passes/TransparentPass.h"
#include "../owEngineNew/Passes/PostprocessPass.h"
#include "../owEngineNew/Passes/BeginQueryPass.h"
#include "../owEngineNew/Passes/EndQueryPass.h"
#include "../owEngineNew/Passes/DispatchPass.h"
#include "../owEngineNew/Passes/InvokeFunctionPass.h"