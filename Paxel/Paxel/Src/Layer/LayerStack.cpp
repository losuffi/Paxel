#include "PXPCH.h"
#include "LayerStack.h"


LayerStack::LayerStack()
{
}


LayerStack::~LayerStack()
{
}

void LayerStack::PushLayer(Layer* layer)
{
	m_Layers.emplace(m_Layers.begin() + m_LayerIndex, layer);
	m_LayerIndex++;
}

void LayerStack::PopLayer(Layer* layer)
{
	auto it = std::find(m_Layers.begin(), m_Layers.end(), layer);
	if (it != m_Layers.end())
	{
		m_Layers.erase(it);
		m_LayerIndex--;
	}
}
