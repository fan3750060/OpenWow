#pragma once

//---------------------------------------------------------//
//--                    Common                           --//
//---------------------------------------------------------//
#include "../owEngine/common.h"

#include "../owEngine/GameStateManager.h"
#include "../owEngine/Loader.h"

// Base
#include "../owEngine/Application.h"

//
// Wrappers
//
#include "../owEngine/Materials/MaterialWrapper.h"
#include "../owEngine/Materials/MaterialDebug.h"
#include "../owEngine/MeshWrapper.h"

//
// UI
//

#include "../owEngine/UI/UIScene.h"
#include "../owEngine/UI/UISceneBase.h"
#include "../owEngine/UI/UINode.h"

#include "../owEngine/UI/Fonts/FontsManager.h"

#include "../owEngine/UI/Passes/BaseUIPass.h"
#include "../owEngine/UI/Passes/RenderUITechnique.h"

//
// 3D
//

#include "../owEngine/3D/Scene.h"
#include "../owEngine/3D/SceneBase.h"
#include "../owEngine/3D/SceneNode.h"

#include "../owEngine/3D/Passes/BasePass.h"
#include "../owEngine/3D/Passes/RenderTechnique.h"

#include "../owEngine/3D/Passes/ClearRenderTargetPass.h"
#include "../owEngine/3D/Passes/CopyBufferPass.h"
#include "../owEngine/3D/Passes/CopyTexturePass.h"
#include "../owEngine/3D/Passes/GenerateMipMapsPass.h"
#include "../owEngine/3D/Passes/PostprocessPass.h"
#include "../owEngine/3D/Passes/BeginQueryPass.h"
#include "../owEngine/3D/Passes/EndQueryPass.h"
#include "../owEngine/3D/Passes/DispatchPass.h"
#include "../owEngine/3D/Passes/InvokeFunctionPass.h"