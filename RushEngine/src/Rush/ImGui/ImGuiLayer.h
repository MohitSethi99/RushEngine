#pragma once

#include "Rush/Layer.h"

#include "Rush/Events/ApplicationEvent.h"
#include "Rush/Events/KeyEvent.h"
#include "Rush/Events/MouseEvent.h"

namespace Rush
{
	class RUSH_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer();
		~ImGuiLayer();

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;

		void Begin();
		void End();
	private:
		float m_Time = 0.0f;
	};
}