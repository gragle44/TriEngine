#include "tripch.h"
#include "LayerStack.h"

TriEngine::LayerStack::LayerStack()
{

}

TriEngine::LayerStack::~LayerStack()
{
	for (Layer* layer : m_Layers)
		delete layer;
}

void TriEngine::LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerInsertIndex, layer);
	layer->OnAttach();
	m_LayerInsertIndex++;
}

void TriEngine::LayerStack::PushOverlay(Layer* overlay)
{
	m_Layers.emplace_back(overlay);
	overlay->OnAttach();
}

void TriEngine::LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end()) {
		m_Layers.erase(it);
		layer->OnDetach();
		m_LayerInsertIndex--;
	}
}

void TriEngine::LayerStack::PopOverlay(Layer* overlay)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), overlay);
	if (it != m_Layers.end()) {
		m_Layers.erase(it);
		overlay->OnDetach();
	}
}
