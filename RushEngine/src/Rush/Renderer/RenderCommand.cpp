#include "rspch.h"
#include "RenderCommand.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Rush
{
	RendererAPI* RenderCommand::s_RendererAPI = new OpenGLRendererAPI;
}