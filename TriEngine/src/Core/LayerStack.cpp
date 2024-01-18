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
	auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), layer);
	if (it != m_Layers.end()) {
		layer->OnDetach();
		m_Layers.erase(it);
		m_LayerInsertIndex--;
	}
}

void TriEngine::LayerStack::PopOverlay(Layer* overlay)
{
	auto it = std::find(m_Layers.begin() + m_LayerInsertIndex, m_Layers.end(), overlay);
	if (it != m_Layers.end()) {
		overlay->OnDetach();
		m_Layers.erase(it);
	}
}
