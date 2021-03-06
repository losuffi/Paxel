#pragma once
#include "PXPCH.h"
class PAXEL_API LayerStack
{
public:
	LayerStack();
	~LayerStack();
	void PushLayer(Layer* layer);
	void PopLayer(Layer* layer);

	std::vector<Layer*>::iterator begin() { return m_Layers.begin(); }
	std::vector<Layer*>::iterator end() { return m_Layers.end(); }
private:
	std::vector<Layer*> m_Layers;
	unsigned int m_LayerIndex = 0;
};

