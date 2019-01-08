#pragma once

void AddSkyPasses(IRenderDevice* device, RenderWindow* window, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene> scene);
void AddWDLPasses(IRenderDevice* device, RenderWindow* window, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene> scene);
void AddMCNKPasses(IRenderDevice* device, RenderWindow* window, RenderTechnique * technique, Viewport * viewport, std::shared_ptr<Scene> scene);
