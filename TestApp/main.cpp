#include "stdafx.h"

// Additional (OW)
#include "GameState_World.h"

// Additional (Windows)
#include <ctime>

int main(int argumentCount, char* arguments[])
{
#ifdef _DEBUG 
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	//_CrtSetReportMode(_CRT_ERROR, _CRTDBG_MODE_DEBUG);
#endif
	{
		_BaseManager = std::make_shared<CBaseManager>();

		std::shared_ptr<CSettings> settings = std::make_shared<CSettings>();
		AddManager<ISettings>(settings);
		settings->AddDefaults();
		

		std::shared_ptr<CLog> log = std::make_shared<CLog>();
		AddManager<ILog>(log);

		std::shared_ptr<CConsole> console = std::make_shared<CConsole>();
		AddManager<IConsole>(console);
		console->AddCommonCommands();
		
		std::shared_ptr<IMPQArchiveManager> mpqArchiveManager = std::make_shared<CMPQArchiveManager>();
		AddManager<IMPQArchiveManager>(mpqArchiveManager);

		std::shared_ptr<IFilesManager> filesManager = std::make_shared<CFilesManager>();
		AddManager<IFilesManager>(filesManager);

		OpenDBs();

		Application app;
		app.Load();

		std::shared_ptr<CGameStateManager> gsManager = std::make_shared<CGameStateManager>();
		AddManager<IGameStateManager>(gsManager);
		gsManager->AddGameState(GameStatesNames::GAME_STATE_WORLD, std::make_shared<CGameState_World>());
		gsManager->SetGameState(GameStatesNames::GAME_STATE_WORLD);
		
		app.Run();



		//std::shared_ptr<Texture> depthStencilBuffer = app.GetRenderWindow()->GetRenderTarget()->GetTexture(RenderTarget::AttachmentPoint::DepthStencil);
		//g_ForwardTechnique.AddPass(std::make_shared<CopyTexturePass>(depthStencilBuffer, depthStencilTexture));


		//g_ForwardTechnique.AddPass(std::make_shared<DeferredPass>(g_Config.Lights, g_Sphere, g_Cone, g_pDeferredLightingPipeline1, g_pDeferredLightingPipeline2, g_pDirectionalLightsPipeline, diffuseTexture, specularTexture, normalTexture, depthStencilTexture));


		const uint32 cnt = 10;
		/*std::shared_ptr<Character> m_CharExtra[cnt * cnt];

		std::vector<uint32> exists;
		for (int i = 0; i < cnt; i++)
		{
			for (int j = 0; j < cnt; j++)
			{
				int index = i + j * cnt;
				m_CharExtra[index] = std::make_shared<Character>();

				while (true)
				{
					int random = Random::GenerateMax(32000);

					DBC_CreatureDisplayInfoRecord* rec = DBC_CreatureDisplayInfo[random];
					if (rec == nullptr)	continue;

					const DBC_CreatureDisplayInfoExtraRecord* exRec = rec->Get_HumanoidData();
					if (exRec == nullptr) continue;

					if (exRec->Get_Race()->Get_ID() > 10) continue;


					if (std::find(exists.begin(), exists.end(), random) != exists.end()) continue;

					
					m_CharExtra[index]->InitFromDisplayInfo(random);
					m_CharExtra[index]->SetParent(g_pScene->GetRootNode());
					m_CharExtra[index]->CreateInstances();
					m_CharExtra[index]->SetScale(vec3(5.0f));

					exists.push_back(random);
					break;
				}

				m_CharExtra[index]->SetTranslate(vec3(i * 10.0f, 0.0f, j * 10.0f));
				m_CharExtra[index]->GetLocalTransform();
			}
		}*/

		/*std::shared_ptr<Creature> m_Char[cnt * cnt];
		std::vector<uint32> exists;
		for (int i = 0; i < cnt; i++)
		{
			for (int j = 0; j < cnt; j++)
			{
				int index = i + j * cnt;
				m_Char[index] = std::make_shared<Creature>();
				//m_Char->InitDefault();

				while (true)
				{
					int random = Random::GenerateMax(32000);
					DBC_CreatureDisplayInfoRecord* rec = DBC_CreatureDisplayInfo[random];
					if (rec == nullptr)	continue;

					if (rec->Get_HumanoidData() != nullptr) continue;
					if (std::find(exists.begin(), exists.end(), random) != exists.end()) continue;

					m_Char[index]->InitFromDisplayInfo(random);
					m_Char[index]->SetParent(g_pScene->GetRootNode());
					m_Char[index]->CreateInstances();
					m_Char[index]->SetTranslate(vec3(i * 15.0f, 0.0f, j * 15.0f));
					m_Char[index]->GetLocalTransform();
					exists.push_back(random);
					break;
				}
			}
		}*/

		//
		// Character
		//

		/*CharacterTemplate tempPala;
		tempPala.TemplateFillDefaultPaladin();

		CharacterTemplate tempShaman;
		tempShaman.TemplateFillDefaultShaman();

		std::shared_ptr<Character> character  = std::make_shared<Character>();
		character->InitFromTemplate(tempPala);
		character->CreateInstances();
		character->SetParent(g_pScene->GetRootNode());
		character->SetTranslate(vec3(0, 15, 0));
		character->SetTranslate(vec3(x * C_TileSize, 200, y * C_TileSize));
		character->SetScale(vec3(10.0f));
		character->GetLocalTransform();*/

		//
		// UI
		//

		/*std::vector<vec2> vecrtices;
		vecrtices.push_back(vec2(-1.0f, -1.0f));
		vecrtices.push_back(vec2(1.0f, -1.0f));
		vecrtices.push_back(vec2(-1.0f, 1.0f));
		vecrtices.push_back(vec2(1.0f, 1.0f));
		std::shared_ptr<IBuffer> __vb = _RenderDevice->CreateVertexBuffer(vecrtices);

		uint16 indexes[6] = { 0, 1, 2, 2, 1, 3 };
		std::shared_ptr<IBuffer> __ib = _RenderDevice->CreateIndexBuffer(indexes, 6);

		std::shared_ptr<UI_Color_Material> uiMaterial = std::make_shared<UI_Color_Material>();
		uiMaterial->SetColor(vec4(0, 1, 0, 0.3f));

		std::shared_ptr<IMesh> __geom = _RenderDevice->CreateMesh();
		__geom->AddVertexBuffer(BufferBinding("POSITION", 0), __vb);
		__geom->SetIndexBuffer(__ib);
		__geom->SetMaterial(uiMaterial);

		// FONT BEGIN
		std::shared_ptr<Font> defFont = GetManager<IFontsManager>()->GetMainFont();
		// FONT END


		std::shared_ptr<UINode> node = std::make_shared<UINode>();
		node->SetMesh(defFont);
		node->SetTranslate(vec2(80.0f, 80.0f));
		//node->SetScale(vec2(10.0f, 10.0f));
		node->GetLocalTransform();
		node->SetParent(g_pUIScene->GetRootNode());

		BlendState::BlendMode alphaBlending(true, false, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha, BlendState::BlendOperation::Add, BlendState::BlendFactor::SrcAlpha, BlendState::BlendFactor::OneMinusSrcAlpha);
		BlendState::BlendMode disableBlending;
		DepthStencilState::DepthMode enableDepthWrites(true, DepthStencilState::DepthWrite::Enable);
		DepthStencilState::DepthMode disableDepthWrites(false, DepthStencilState::DepthWrite::Disable);

		std::shared_ptr<PipelineState> UIPipeline = renderDevice->CreatePipelineState();
		UIPipeline->GetBlendState().SetBlendMode(alphaBlending);
		UIPipeline->GetDepthStencilState().SetDepthMode(enableDepthWrites);
		UIPipeline->GetRasterizerState().SetCullMode(RasterizerState::CullMode::None);
		UIPipeline->GetRasterizerState().SetFillMode(RasterizerState::FillMode::Solid);
		UIPipeline->SetRenderTarget(app.GetRenderWindow()->GetRenderTarget());
		UIPipeline->GetRasterizerState().SetViewport(g_Viewport);

		g_UITechnique.SetPass(std::make_shared<BaseUIPass>(g_pUIScene, UIPipeline));*/


	}

	return 0;
}
