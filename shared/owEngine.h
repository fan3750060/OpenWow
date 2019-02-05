#pragma once

//---------------------------------------------------------//
//--                    Common                           --//
//---------------------------------------------------------//
#include "../owEngine/common.h"


// Base
#include "../owEngine/Application.h"
#include "../owEngine/CameraController.h"
#include "../owEngine/CreatePasses.h"
#include "../owEngine/EngineTime.h"
#include "../owEngine/Loader.h"

//
// Wrappers
//
#include "../owEngine/Materials/MaterialWrapper.h"
#include "../owEngine/Materials/MaterialDebug.h"
#include "../owEngine/MeshWrapper.h"

//
// UI
//

#include "../owEngine/UI/SceneUI.h"
#include "../owEngine/UI/SceneNodeUI.h"

#include "../owEngine/UI/Fonts/FontsManager.h"
#include "../owEngine/UI/UIText.h"
#include "../owEngine/UI/Passes/UI_Color_Material.h"

#include "../owEngine/UI/Passes/BaseUIPass.h"
#include "../owEngine/UI/Passes/RenderUITechnique.h"

//
// 3D
//

#include "../owEngine/3D/Scene3D.h"
#include "../owEngine/3D/SceneNode3D.h"

#include "../owEngine/3D/Passes/BasePass.h"
#include "../owEngine/3D/Passes/RenderTechnique.h"

#include "../owEngine/3D/Passes/ClearRenderTargetPass.h"
#include "../owEngine/3D/Passes/CopyBufferPass.h"
#include "../owEngine/3D/Passes/CopyTexturePass.h"
#include "../owEngine/3D/Passes/DeferredPass.h"
#include "../owEngine/3D/Passes/GenerateMipMapsPass.h"
#include "../owEngine/3D/Passes/PostprocessPass.h"
#include "../owEngine/3D/Passes/BeginQueryPass.h"
#include "../owEngine/3D/Passes/EndQueryPass.h"
#include "../owEngine/3D/Passes/DispatchPass.h"
#include "../owEngine/3D/Passes/InvokeFunctionPass.h"