#include "Application.h"

#include "Rush/Events/ApplicationEvent.h"
#include "Rush/Log.h"

namespace Rush
{
	Application::Application() {}
	Application::~Application() {}

	void Application::Run() 
	{
		WindowResizeEvent e(1280, 720);

		if (e.IsInCategory(EventCategoryApplication))
		{
			RS_WARN(e);
		}
		if (e.IsInCategory(EventCategoryInput))
		{
			RS_WARN(e);
		}

		while (true);
	}
}