#pragma once

#ifdef RS_PLATFORM_WINDOWS

extern Rush::Application* Rush::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Rush::CreateApplication();
	app->Run();
	delete app;
}

#endif // RS_PLATFORM_WINDOWS