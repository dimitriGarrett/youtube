#include <iostream>

#define GLEW_STATIC
#include <GLEW/glew.h>
#include <GLFW/glfw3.h>

const char* vertexSource = 
"#version 330 core\n"
"layout (location = 0) in vec2 aPos;\n"
"void main() {\n"
"gl_Position = vec4(aPos, 0.0, 1.0);\n"
"}\n";
const char* fragmentSource = "#version 330 core\n"
"out vec4 color;\n"
"void main() {\n"
"color = vec4(0.0, 0.0, 1.0, 1.0);\n"
"}\n";

int main()
{
	if (!glfwInit())
	{
		std::cout << "GLFW INIT ERROR!" << std::endl;

		return -1;
	}

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	GLFWwindow* window = glfwCreateWindow(1280, 720, "Youtube", nullptr, nullptr);

	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;

	if (glewInit() != GLEW_OK)
	{
		std::cout << "GLEW INIT ERROR!" << std::endl;
		return -1;
	}

	unsigned int program = glCreateProgram();

	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShader, 1, &vertexSource, NULL);
	glCompileShader(vertexShader);

	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShader, 1, &fragmentSource, NULL);
	glCompileShader(fragmentShader);

	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);

	glLinkProgram(program);
	glValidateProgram(program);

	glUseProgram(program);

	glDetachShader(program, vertexShader);
	glDetachShader(program, fragmentShader);

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	float vertices[] = {
		-0.5f, 0.5f,
		0.5f, 0.5f,
		0.5f, -0.5f,
		-0.5f, -0.5f
	};

	unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	unsigned int vao;
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	unsigned int vbo;
	glGenBuffers(1, &vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	unsigned int ebo;
	glGenBuffers(1, &ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);
	glEnableVertexAttribArray(0);

	int success;
	char infoLog[1024];
	glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, NULL, infoLog);
		std::string message = "Program Linking Failed: ";
		message += infoLog;
	}

	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR)
	{
		std::cout << "OPENGL ERROR: " << err << std::endl;
	}

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(1.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		glUseProgram(program);
		glBindVertexArray(vao);

		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);

		glDrawElements(GL_TRIANGLES, sizeof(indices), GL_UNSIGNED_INT, 0);
		
		glfwSwapBuffers(window);

		glfwPollEvents();

		GLenum err;
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			std::cout << "OPENGL ERROR: " << err << std::endl;
		}
	}

	glfwTerminate();

	return 0;
}