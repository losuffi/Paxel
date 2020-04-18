#pragma once

class ImguiLayer:public Layer
{
public:
	ImguiLayer()
		: Layer{"ImGuiLayer"}
	{
	}

	void OnBegin() override;
	void OnEnd() override;
	void OnAttach() override;
	void OnDettach() override;
	void OnEvent(Event& event) override;
	void OnUpdate() override;
	void SetNativeWindow(void* NVWindow) { NativeWindow = NVWindow; };
	
	void SetVkRenderCoreInfoList(void* vkInfoList);
protected:
	void* NativeWindow;
	void* RenderCoreInfoList;
};

