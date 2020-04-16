#pragma once

class ImguiLayer: Layer
{
public:
	explicit ImguiLayer(const char* LayerName)
		: Layer{"ImGuiLayer"}
	{
	}

	void OnAttach() override;
	void OnDettach() override;
	void OnEvent(Event& event) override;
	void OnUpdate() override;
};

