#include <Rush.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Rush::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Rush::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		Rush::Ref<Rush::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Rush::VertexBuffer::Create(vertices, sizeof(vertices)));
		Rush::BufferLayout layout = {
			{ Rush::ShaderDataType::Float3, "a_Position" },
			{ Rush::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		Rush::Ref<Rush::IndexBuffer> indexBuffer;
		indexBuffer.reset(Rush::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Rush::VertexArray::Create());

		float squareVertices[5 * 4] = {
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f,
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f
		};

		Rush::Ref<Rush::VertexBuffer> squareVB;
		squareVB.reset(Rush::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Rush::ShaderDataType::Float3, "a_Position" },
			{ Rush::ShaderDataType::Float2, "a_TexCoord" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		Rush::Ref<Rush::IndexBuffer> squareIB;
		squareIB.reset(Rush::IndexBuffer::Create(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));
		m_SquareVA->SetIndexBuffer(squareIB);

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			out vec4 v_Color;
			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;
			in vec4 v_Color;
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
				color = v_Color;
			}
		)";

		m_Shader.reset(Rush::Shader::Create(vertexSrc, fragmentSrc));

		std::string flatColorShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;
			out vec3 v_Position;
			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string flatColorShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec3 v_Position;

			uniform vec3 u_Color;

			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(Rush::Shader::Create(flatColorShaderVertexSrc, flatColorShaderFragmentSrc));

		std::string textureShaderVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjection;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);	
			}
		)";

		std::string textureShaderFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;
			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;

			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(Rush::Shader::Create(textureShaderVertexSrc, textureShaderFragmentSrc));

		m_Texture = Rush::Texture2D::Create("assets/textures/Checkerboard.png");

		std::dynamic_pointer_cast<Rush::OpenGLShader>(m_TextureShader)->Bind();
		std::dynamic_pointer_cast<Rush::OpenGLShader>(m_TextureShader)->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Rush::Timestep ts) override
	{
		if (Rush::Input::IsKeyPressed(RS_KEY_LEFT))
			m_CameraPosition.x -= m_CameraMoveSpeed * ts;
		else if (Rush::Input::IsKeyPressed(RS_KEY_RIGHT))
			m_CameraPosition.x += m_CameraMoveSpeed * ts;

		if (Rush::Input::IsKeyPressed(RS_KEY_UP))
			m_CameraPosition.y += m_CameraMoveSpeed * ts;
		else if (Rush::Input::IsKeyPressed(RS_KEY_DOWN))
			m_CameraPosition.y -= m_CameraMoveSpeed * ts;

		if (Rush::Input::IsKeyPressed(RS_KEY_A))
			m_CameraRotation += m_CameraRotationSpeed * ts;
		if (Rush::Input::IsKeyPressed(RS_KEY_D))
			m_CameraRotation -= m_CameraRotationSpeed * ts;

		Rush::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Rush::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Rush::Renderer::BeginScene(m_Camera);

		glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		std::dynamic_pointer_cast<Rush::OpenGLShader>(m_FlatColorShader)->Bind();
		std::dynamic_pointer_cast<Rush::OpenGLShader>(m_FlatColorShader)->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 20; x++)
			{
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;
				Rush::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);
			}
		}

		m_Texture->Bind();
		Rush::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		// Triangle
		// Rush::Renderer::Submit(m_Shader, m_VertexArray);

		Rush::Renderer::EndScene();
	}

	void OnEvent(Rush::Event& event) override
	{
	}

	void OnImGuiRender()
	{
		ImGui::Begin("Camera Settings");

		ImGui::Text("Rotation: %f, %f, %f", m_Camera.GetRotation());
		ImGui::Text("Position: %f, %f, %f", m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		
		ImGui::End();
	}

	private:
		Rush::Ref<Rush::Shader> m_Shader;
		Rush::Ref<Rush::VertexArray> m_VertexArray;

		Rush::Ref<Rush::Shader> m_FlatColorShader, m_TextureShader;
		Rush::Ref<Rush::VertexArray> m_SquareVA;

		glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

		Rush::Ref<Rush::Texture2D> m_Texture;

		Rush::OrthographicCamera m_Camera;
		glm::vec3 m_CameraPosition;
		float m_CameraMoveSpeed = 5.0f;

		float m_CameraRotation = 0.0f;
		float m_CameraRotationSpeed = 180.0f;
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