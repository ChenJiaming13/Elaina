#include "pch.h"
#include "Sandbox.h"
#include "base/Framebuffer.h"
#include "core/GlfwWindow.h"
#include "ui/ImGui.h"

int main()
{
	Elaina::CGlfwWindow App;
	_ASSERTE(App.init(800, 600));
	Elaina::CFrameBuffer::initDefaultFrameBuffer(App.getWidth(), App.getHeight(), 0);
	auto pSandbox = std::make_shared<CSandbox>();
	pSandbox->init(App.getWidth(), App.getHeight());
	App.addInputHandler(pSandbox->getCameraController());
	App.addInputHandler(pSandbox);

	float LastTime = 0.0f;
	while (!App.shouldClose())
	{
		App.pollEvents();
		Elaina::CImGui::beginFrame();
		const float CurrTime = static_cast<float>(glfwGetTime());
		const float DeltaTime = CurrTime - LastTime;
		LastTime = CurrTime;
		pSandbox->render(CurrTime, DeltaTime);
		Elaina::CImGui::endFrame();
		App.swapBuffers();
	}
	pSandbox.reset();
	return 0;
}
