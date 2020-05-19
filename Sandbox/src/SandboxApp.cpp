#include <Rush.h>

class ExampleLayer : public Rush::Layer
{
public:
	ExampleLayer()
		: Layer("Example")
	{
	}

	void OnUpdate() override
	{
		if (Rush::Input::IsKeyPressed(RS_KEY_TAB))
			RS_TRACE("Tab key is pressed (poll)!");
	}

	void OnEvent(Rush::Event& event) override
	{
		if (event.GetEventType() == Rush::EventType::KeyPressed)
		{
			Rush::KeyPressedEvent& e = (Rush::KeyPressedEvent&)event;
			if (e.GetKeyCode() == RS_KEY_TAB)
				RS_TRACE("Tab key is pressed (event)!");
			RS_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox : public Rush::Application
{
public:
	Sandbox() 
	{
		PushLayer(new ExampleLayer());
	}
	~Sandbox() {}
};

Rush::Application* Rush::CreateApplication() 
{
	return new Sandbox();
}