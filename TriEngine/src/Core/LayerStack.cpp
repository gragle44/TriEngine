#include "tripch.h"
#include "LayerStack.h"

TriEngine::LayerStack::LayerStack()
{
	m_LayerInsert = m_Layers.begin();
}

TriEngine::LayerStack::~LayerStack()
{
	for (Layer* layer : m_Layers)
		delete layer;
}

void TriEngine::LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_LayerInsert, layer);
}

void TriEngine::LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
}

void TriEngine::LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end()) {
		m_Layers.erase(it);
		m_LayerInsert--;
	}
}

void TriEngine::LayerStack::PopOverlay(Layer* overlay)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it != m_Layers.end())
		m_Layers.erase(it);
}
