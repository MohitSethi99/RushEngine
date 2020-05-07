#include <Rush.h>

class Sandbox : public Rush::Application
{
public:
	Sandbox() {}
	~Sandbox() {}
};

Rush::Application* Rush::CreateApplication() 
{
	return new Sandbox();
}