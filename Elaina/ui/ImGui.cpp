#include "pch.h"
#include "ImGui.h"

void Elaina::CImGui::init(GLFWwindow* vWindow)
{
	// Setup Dear ImGui context
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
	//io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // IF using Docking Branch

	// Setup Platform/Renderer backends
	ImGui_ImplGlfw_InitForOpenGL(vWindow, true);          // Second param install_callback=true will install GLFW callbacks and chain to existing ones.
	ImGui_ImplOpenGL3_Init();
}

void Elaina::CImGui::beginFrame()
{
	// (Your code calls glfwPollEvents())
	// ...
	// Start the Dear ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	//ImGui::ShowDemoWindow(); // Show demo window! :)
}

void Elaina::CImGui::endFrame()
{
	// Rendering
	// (Your code clears your framebuffer, renders your other stuff etc.)
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	// (Your code calls glfwSwapBuffers() etc.)
}

void Elaina::CImGui::shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

bool Elaina::CImGui::wantCaptureMouse()
{
	const ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureMouse;
}

bool Elaina::CImGui::wantCaptureKeyboard()
{
	const ImGuiIO& io = ImGui::GetIO();
	return io.WantCaptureKeyboard;
}
