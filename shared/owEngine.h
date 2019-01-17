#pragma once

//---------------------------------------------------------//
//--                    Common                           --//
//---------------------------------------------------------//
#include "../owEngine/common.h"

#include "../owEngine/GameStateManager.h"
#include "../owEngine/Loader.h"

// Base
#include "../owEngine/Application.h"

// Scenes
#include "../owEngine/Scene.h"
#include "../owEngine/SceneBase.h"
#include "../owEngine/SceneNode.h"

#include "../owEngine/UI/UIScene.h"
#include "../owEngine/UI/UISceneBase.h"
#include "../owEngine/UI/UINode.h"

// UI
#include "../owEngine/UI/Fonts/FontsManager.h"

// Passes
#include "../owEngine/Passes/BasePass.h"
#include "../owEngine/Passes/RenderTechnique.h"

#include "../owEngine/UI/Passes/BaseUIPass.h"
#include "../owEngine/UI/Passes/RenderUITechnique.h"

#include "../owEngine/Passes/ClearRenderTargetPass.h"
#include "../owEngine/Passes/CopyBufferPass.h"
#include "../owEngine/Passes/CopyTexturePass.h"
#include "../owEngine/Passes/GenerateMipMapsPass.h"
#include "../owEngine/Passes/PostprocessPass.h"
#include "../owEngine/Passes/BeginQueryPass.h"
#include "../owEngine/Passes/EndQueryPass.h"
#include "../owEngine/Passes/DispatchPass.h"
#include "../owEngine/Passes/InvokeFunctionPass.h"