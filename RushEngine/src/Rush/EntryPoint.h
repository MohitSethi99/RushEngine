#pragma once

#ifdef RS_PLATFORM_WINDOWS

extern Rush::Application* Rush::CreateApplication();

int main(int argc, char** argv)
{
	Rush::Log::Init();
	RS_CORE_WARN("Initialized Log");
	RS_ERROR("Hello! {0}", 5);

	auto app = Rush::CreateApplication();
	app->Run();
	delete app;
}

#endif // RS_PLATFORM_WINDOWS