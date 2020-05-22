#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Rush/Events/ApplicationEvent.h"

#include "Window.h"
#include "Rush/LayerStack.h"
#include "Rush/Events/Event.h"
#include "Rush/Events/ApplicationEvent.h"

#include "Rush/ImGui/ImGuiLayer.h"

#include "Rush/Renderer/Shader.h"
#include "Rush/Renderer/Buffer.h"
#include "Rush/Renderer/VertexArray.h"

#include "Rush/Renderer/OrthographicCamera.h"

namespace Rush
{
	class RUSH_API Application
	{
	public:
		Application();
		virtual ~Application() = default;

		void Run();

		void OnEvent(Event& e);

		void PushLayer(Layer* layer);
		void PushOverlay(Layer* layer);

		inline Window& GetWindow() { return *m_Window; }

		inline static Application& Get() { return *s_Instance; }
	private:
		bool OnWindowClose(WindowCloseEvent& e);

		std::unique_ptr<Window> m_Window;
		ImGuiLayer* m_ImGuiLayer;
		bool m_Running = true;
		LayerStack m_LayerStack;

		std::shared_ptr<Shader> m_Shader;
		std::shared_ptr<VertexArray> m_VertexArray;

		std::shared_ptr<Shader> m_BlueShader;
		std::shared_ptr<VertexArray> m_SquareVA;

		OrthographicCamera m_Camera;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

