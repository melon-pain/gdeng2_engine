
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"

#include <d3dcompiler.h>

GraphicsEngine::GraphicsEngine()
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	m_render_system = new RenderSystem();
	m_render_system->init();
	return true;
}

bool GraphicsEngine::release()
{
	m_render_system->release();
	delete m_render_system;
	return true;
}

RenderSystem* GraphicsEngine::getRenderSystem()
{
	return this->m_render_system;
}

GraphicsEngine * GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}
