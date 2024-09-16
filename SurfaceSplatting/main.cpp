#include <iostream>
#include "helper.h"
#include "imgui.h"
#include "PointRendering.h"
#include "SurfaceSplatting.h"
#include "base/Framebuffer.h"
#include "controller/ArcballController.h"
#include "core/GlfwWindow.h"
#include "ui/ImGui.h"

std::vector<std::shared_ptr<CRenderAlgorithm>> g_RenderAlgorithms{};
size_t g_ActiveAlgorithmIdx = 0;

class CMyHandler final : public Elaina::CInputHandler
{
public:
	void onWindowSizeChangeV(int vWidth, int vHeight) override
	{
		for (const auto& pRenderAlgorithm : g_RenderAlgorithms)
		{
			pRenderAlgorithm->onWindowSizeChangeV(vWidth, vHeight);
		}
	}
};

int main()
{
	Elaina::CGlfwWindow App;
	_ASSERTE(App.init(800, 600, "Surface Splatting"));
	Elaina::CFrameBuffer::initDefaultFrameBuffer(App.getWidth(), App.getHeight(), 0);
	const auto& pCamera = std::make_shared<Elaina::CCamera>(Elaina::CCamera::ECameraType::PERSP, static_cast<float>(App.getWidth()) / static_cast<float>(App.getHeight()));
	const auto& pCameraController = std::make_shared<Elaina::CArcballController>();
	pCameraController->controlV(pCamera);
	App.addInputHandler(pCameraController);
	App.addInputHandler(std::make_shared<CMyHandler>());

	std::vector<SSurfel> Surfels;
	std::cout << loadRsfFile("painted_santa_kd.rsf", Surfels);

	g_RenderAlgorithms.push_back(std::make_shared<CSurfaceSplatting>());
	g_RenderAlgorithms.push_back(std::make_shared<CPointRendering>());
	for (const auto& pRenderAlgorithm : g_RenderAlgorithms)
	{
		pRenderAlgorithm->initV(App.getWidth(), App.getHeight(), Surfels);
	}

	while (!App.shouldClose())
	{
		App.pollEvents();
		Elaina::CImGui::beginFrame();

		// ui
		for (size_t i = 0; i < g_RenderAlgorithms.size(); ++i)
		{
			if (ImGui::RadioButton(g_RenderAlgorithms[i]->getName().c_str(), i == g_ActiveAlgorithmIdx))
			{
				g_ActiveAlgorithmIdx = i;
			}
		}

		g_RenderAlgorithms[g_ActiveAlgorithmIdx]->renderV(pCamera);

		Elaina::CImGui::endFrame();
		App.swapBuffers();
	}

	for (const auto& pRenderAlgorithm : g_RenderAlgorithms)
	{
		pRenderAlgorithm->cleanup();
	}
	g_RenderAlgorithms.clear();

	Elaina::CImGui::shutdown();
	return 0;
}
