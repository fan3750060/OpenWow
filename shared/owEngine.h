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
#include "../owEngine/UI/Fonts/FontsManager.h"

//
// Wrappers
//
#include "../owEngine/MaterialWrapper.h"
#include "../owEngine/MeshWrapper.h"


//
// UI
//

// base nodes
#include "../owEngine/UI/SceneUI.h"
#include "../owEngine/UI/SceneNodeUI.h"

// additional nodes
#include "../owEngine/UI/UIText.h"
#include "../owEngine/UI/UITexture.h"

// materials
#include "../owEngine/UI/Materials/UI_Color_Material.h"
#include "../owEngine/UI/Materials/UI_Texture_Material.h"

// base passes
#include "../owEngine/UI/Passes/BaseUIPass.h"
#include "../owEngine/UI/Passes/RenderUITechnique.h"



//
// 3D
//

// base nodes
#include "../owEngine/3D/Scene3D.h"
#include "../owEngine/3D/SceneNode3D.h"

// additional nodes

// materials
#include "../owEngine/3D/Materials/MaterialDebug.h"
#include "../owEngine/3D/Materials/MaterialTextured.h"

// base passes
#include "../owEngine/3D/Passes/BasePass.h"
#include "../owEngine/3D/Passes/RenderTechnique.h"

// other passes
#include "../owEngine/3D/Passes/ClearRenderTargetPass.h"
#include "../owEngine/3D/Passes/CopyBufferPass.h"
#include "../owEngine/3D/Passes/CopyTexturePass.h"
#include "../owEngine/3D/Passes/GenerateMipMapsPass.h"
#include "../owEngine/3D/Passes/PostprocessPass.h"
#include "../owEngine/3D/Passes/BeginQueryPass.h"
#include "../owEngine/3D/Passes/EndQueryPass.h"
#include "../owEngine/3D/Passes/DispatchPass.h"
#include "../owEngine/3D/Passes/InvokeFunctionPass.h"

// Postprocess
#include "../owEngine/GBuffer.h"
#include "../owEngine/3D/Light.h"
#include "../owEngine/3D/Light3D.h"
#include "../owEngine/3D/Passes/DeferredLightingPass.h"