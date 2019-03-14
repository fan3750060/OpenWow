#pragma once

//---------------------------------------------------------//
//--                    Common                           --//
//---------------------------------------------------------//
#include "../owEngine/common.h"


// Base
#include "../owEngine/Application.h"
#include "../owEngine/CameraControllerBase.h"
#include "../owEngine/ArcBallCameraController.h"
#include "../owEngine/FreeCameraController.h"
#include "../owEngine/RTSCameraController.h"
#include "../owEngine/CreatePasses.h"
#include "../owEngine/EngineTime.h"
#include "../owEngine/Loader.h"
#include "../owEngine/UI/Fonts/FontsManager.h"
#include "../owEngine/WindowCreator.h"

//
// Wrappers
//
#include "../owEngine/MaterialWrapper.h"
#include "../owEngine/MeshWrapper.h"


//
// UI
//

// base nodes
#include "../owEngine/UI/UIScene.h"
#include "../owEngine/UI/UIBaseNode.h"

// common nodes
#include "../owEngine/UI/UIButton.h"
#include "../owEngine/UI/UIColor.h"
#include "../owEngine/UI/UIBeizerLine.h"
#include "../owEngine/UI/UIText.h"
#include "../owEngine/UI/UITexture.h"

// additional nodes
#include "../owEngine/UI/UISlateEditor.h"
#include "../owEngine/UI/UIContainerGarmoshka.h"

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
#include "../owEngine/3D/SceneNodeModel3D.h"
#include "../owEngine/3D/SceneNodeFont3D.h"

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