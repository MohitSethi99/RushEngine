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
		RS_INFO("ExampleLayer::Update");
	}

	void OnEvent(Rush::Event& event) override
	{
		RS_TRACE("{0}", event);
	}
};

class Sandbox : public Rush::Application
{
public:
	Sandbox() { PushLayer(new ExampleLayer()); }
	~Sandbox() {}
};

Rush::Application* Rush::CreateApplication() 
{
	return new Sandbox();
}