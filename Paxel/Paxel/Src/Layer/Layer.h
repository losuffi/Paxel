#pragma once
#include "PXPCH.h"

class PAXEL_API Layer
{
public:
	virtual ~Layer() = default;
	const char* LayerName(){ return m_LayerName; }
	virtual void OnAttach() = 0;
	virtual void OnDettach() = 0;
	virtual void OnEvent(Event& event) = 0;
	virtual void OnUpdate() = 0;
	Layer(const char* LayerName):m_LayerName(LayerName){}
protected:
	const char* m_LayerName;
};