#pragma once
#include "Core.h"
#include "Events/Event.h"
#include "Rush/Events/ApplicationEvent.h"

#include "Window.h"
#include "Rush/LayerStack.h"
#include "Rush/Events/Event.h"
#include "Rush/Events/ApplicationEvent.h"

#include "Rush/Core/Timestep.h"
#include "Rush/ImGui/ImGuiLayer.h"

namespace Rush
{
	class Application
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
		float m_LastFrameTime = 0.0f;

		static Application* s_Instance;
	};

	// To be defined in CLIENT
	Application* CreateApplication();
}

