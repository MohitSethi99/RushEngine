#include <Rush.h>

#include "Platform/OpenGL/OpenGLShader.h"

#include "imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class ExampleLayer : public Rush::Layer
{
public:
	ExampleLayer()
		: Layer("Example"), m_Camera(45.0f, 1280, 720, 0.1, 100), m_CameraPosition(0.0f)
	{
		m_VertexArray.reset(Rush::VertexArray::Create());

		float vertices[3 * 7] = {
			-0.5f, -0.5f, 0.0f, 0.8f, 0.2f, 0.8f, 1.0f,
			 0.5f, -0.5f, 0.0f, 0.2f, 0.3f, 0.8f, 1.0f,
			 0.0f,  0.5f, 0.0f, 0.8f, 0.8f, 0.2f, 1.0f
		};

		std::shared_ptr<Rush::VertexBuffer> vertexBuffer;
		vertexBuffer.reset(Rush::VertexBuffer::Create(vertices, sizeof(vertices)));
		Rush::BufferLayout layout = {
			{ Rush::ShaderDataType::Float3, "a_Position" },
			{ Rush::ShaderDataType::Float4, "a_Color" }
		};
		vertexBuffer->SetLayout(layout);
		m_VertexArray->AddVertexBuffer(vertexBuffer);

		uint32_t indices[3] = { 0, 1, 2 };
		std::shared_ptr<Rush::IndexBuffer> indexBuffer;
		indexBuffer.reset(Rush::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t)));
		m_VertexArray->SetIndexBuffer(indexBuffer);

		m_SquareVA.reset(Rush::VertexArray::Create());

		float squareVertices[3 * 4] = {
			-0.5f, -0.5f, 0.0f,
			 0.5f, -0.5f, 0.0f,
			 0.5f,  0.5f, 0.0f,
			-0.5f,  0.5f, 0.0f
		};

		std::shared_ptr<Rush::VertexBuffer> squareVB;
		squareVB.reset(Rush::VertexBuffer::Create(squareVertices, sizeof(squareVertices)));
		squareVB->SetLayout({
			{ Rush::ShaderDataType::Float3, "a_Position" }
			});
		m_SquareVA->AddVertexBuffer(squareVB);

		uint32_t squareIndices[6] = { 0, 1, 2, 2, 3, 0 };
		std::shared_ptr<Rush::IndexBuffer> squareIB;
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
	}

	void OnUpdate(Rush::Timestep ts) override
	{
		if (Rush::Input::IsMouseButtonPressed(RS_MOUSE_BUTTON_RIGHT))
		{
			if (firstMouse)
			{
				lastX = Rush::Input::GetMouseX();
				lastY = Rush::Input::GetMouseY();
				firstMouse = false;
			}

			float xoffset = Rush::Input::GetMouseX() - lastX;
			float yoffset = lastY - Rush::Input::GetMouseY();
			lastX = Rush::Input::GetMouseX();
			lastY = Rush::Input::GetMouseY();

			m_Camera.Rotate(xoffset, yoffset);

			if (Rush::Input::IsKeyPressed(RS_KEY_W))
				m_Camera.Move(Rush::CameraMoveDirection::FORWARD, ts);
			else if (Rush::Input::IsKeyPressed(RS_KEY_S))
				m_Camera.Move(Rush::CameraMoveDirection::BACK, ts);

			if (Rush::Input::IsKeyPressed(RS_KEY_A))
				m_Camera.Move(Rush::CameraMoveDirection::LEFT, ts);
			else if (Rush::Input::IsKeyPressed(RS_KEY_D))
				m_Camera.Move(Rush::CameraMoveDirection::RIGHT, ts);

			if (Rush::Input::IsKeyPressed(RS_KEY_Q))
				m_Camera.Move(Rush::CameraMoveDirection::UP, ts);
			else if (Rush::Input::IsKeyPressed(RS_KEY_E))
				m_Camera.Move(Rush::CameraMoveDirection::DOWN, ts);
		}

		if (Rush::Input::IsKeyPressed(RS_KEY_R))
			m_CameraPosition = { 0.0f, 0.0f, 5.0f };

		Rush::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1 });
		Rush::RenderCommand::Clear();

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

		glm::mat4 trans = glm::rotate(glm::mat4(1.0f), glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		Rush::Renderer::Submit(m_Shader, m_VertexArray, trans);

		Rush::Renderer::EndScene();
	}

	void OnEvent(Rush::Event& event) override
	{
		if (event.GetEventType() == Rush::EventType::MouseButtonReleased) {
			Rush::MouseButtonReleasedEvent& e = (Rush::MouseButtonReleasedEvent&)event;
			if (e.GetMouseButton() == RS_MOUSE_BUTTON_RIGHT)
				firstMouse = true;
		}

		if (event.GetEventType() == Rush::EventType::MouseScrolled) {
			Rush::MouseScrolledEvent& e = (Rush::MouseScrolledEvent&)event;
			m_Camera.Zoom(e.GetYOffset());
		}
	}

	void OnImGuiRender()
	{
		ImGui::Begin("Camera Settings");

		ImGui::Text("Rotation: %f, %f, %f", m_Camera.GetRotation().x, m_Camera.GetRotation().y, m_Camera.GetRotation().z);
		ImGui::Text("Position: %f, %f, %f", m_Camera.GetPosition().x, m_Camera.GetPosition().y, m_Camera.GetPosition().z);
		ImGui::Text("FOV: %f", m_Camera.GetFOV());
		ImGui::SliderFloat("Travel Speed", &m_Camera.m_TravelSpeed, 1.0f, 50.0f);
		ImGui::SliderFloat("Mouse Sensitivity", &m_Camera.m_MouseSensitivity, 0.1f, 2.0f);

		ImGui::ColorEdit3("Square Color", glm::value_ptr(m_SquareColor));
		
		ImGui::End();
	}

	private:
		std::shared_ptr<Rush::Shader> m_Shader;
		std::shared_ptr<Rush::VertexArray> m_VertexArray;

		std::shared_ptr<Rush::Shader> m_FlatColorShader;
		std::shared_ptr<Rush::VertexArray> m_SquareVA;

		glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };

		Rush::PerspectiveCamera m_Camera;
		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 5.0f };
		float m_CameraMoveSpeed = 5.0f;

		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f };
		float m_CameraRotationSpeed = 180.0f;

		bool firstMouse = true;
		float lastX = 1280 / 2;
		float lastY = 720 / 2;
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