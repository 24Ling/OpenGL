#include "OpenGL/OpenGLInfo.h"


#include "Stb/include/stb_image.h"


OpenGLInfo::OpenGLInfo()
{
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    m_Width = 1920;
    m_Height = 1080;

    WindowName = "Ling";
}
OpenGLInfo::~OpenGLInfo()
{
    
}

void OpenGLInfo::InitOpenGL()
{
    /* Initialize the library */
    if (!glfwInit())
        exit(1);

    /* Create a windowed mode window and its OpenGL context */
    m_Window = glfwCreateWindow(m_Width, m_Height, WindowName.c_str(), NULL, NULL);
    if (!m_Window)
    {
        glfwTerminate();
        exit(1);
    }

    /* Make the window's context current */
    glfwMakeContextCurrent(m_Window);

    if (glewInit() != GLEW_OK)
    {
        std::cout << "glewInit failed!" << std::endl;
        exit(1);
    }

    glfwSwapInterval(1); // Enable vsync

	// ¼ÓÔØÍ¼±ê
	int width, height, channels;
	unsigned char* data = stbi_load("Syndra.png", &width, &height, &channels, 4);
	if (data) {
		GLFWimage images[1];
		images[0].width = width;
		images[0].height = height;
		images[0].pixels = data;
		glfwSetWindowIcon(m_Window, 1, images);
		stbi_image_free(data);
	}
	else {
		std::cerr << "Failed to load icon" << std::endl;
	}

}
void OpenGLInfo::DestroyOpenGL()
{
    glfwDestroyWindow(m_Window);
    glfwTerminate();
}
bool OpenGLInfo::ShowWindow()
{
    return glfwWindowShouldClose(m_Window);
}

void OpenGLInfo::EnableBlend()
{
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void OpenGLInfo::PollEvent()
{
    /* Swap front and back buffers */
    glfwSwapBuffers(m_Window);
    /* Poll for and process events */
    glfwPollEvents();
}

void OpenGLInfo::ClearColor()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}