//                    SPIKE ENGINE
//Copyright 2021 - SpikeTechnologies - All Rights Reserved
#include "spkpch.h"
#include "WindowsWindow.h"
#include "Spike/Core/Input.h"
#include "Spike/Events/ApplicationEvent.h"
#include "Spike/Events/MouseEvent.h"
#include "Spike/Events/KeyEvent.h"
#include "Platform/OpenGL/OpenGLContext.h"
#include "Platform/DX11/DX11Context.h"

namespace Spike
{
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        SPK_CORE_LOG_ERROR("GLFW Error (%d): %s", error, description);
    }

    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps& props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        if (!s_GLFWInitialized)
        {
            // TODO: glfwTerminate on system shutdown
            int success = glfwInit();
            SPK_CORE_ASSERT(success, "Could not intialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

#ifdef RENDERER_API_DX11
        m_Context = CreateScope<DX11Context>(m_Window);
#elif defined RENDERER_API_OPENGL
        m_Context = CreateScope<OpenGLContext>(m_Window);
#endif

        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        //Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                data.Width = width;
                data.Height = height;

                WindowResizeEvent e(width, height);
                data.EventCallback(e);
            });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
                WindowCloseEvent e;
                data.EventCallback(e);
            });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        KeyPressedEvent e(key, 0);
                        data.EventCallback(e);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        KeyReleasedEvent e(key);
                        data.EventCallback(e);
                        break;
                    }
                    case GLFW_REPEAT:
                    {
                        KeyPressedEvent e(key, 1);
                        data.EventCallback(e);
                        break;
                    }
                }
            });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, Uint keycode)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                KeyTypedEvent e(static_cast<KeyCode>(keycode));
                data.EventCallback(e);
            });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                switch (action)
                {
                    case GLFW_PRESS:
                    {
                        MouseButtonPressedEvent e(button);
                        data.EventCallback(e);
                        break;
                    }
                    case GLFW_RELEASE:
                    {
                        MouseButtonReleasedEvent e(button);
                        data.EventCallback(e);
                        break;
                    }
                }
            });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseScrolledEvent e((float)xOffset, (float)yOffset);
                data.EventCallback(e);
            });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
            {
                WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

                MouseMovedEvent e((float)xPos, (float)yPos);
                data.EventCallback(e);
            });
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetTitle(const String& title)
    {
        m_Data.Title = title;
        glfwSetWindowTitle(m_Window, m_Data.Title.c_str());
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
    #ifdef RENDERER_API_OPENGL
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
    #endif // RENDERER_API_OPENGL

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return m_Data.VSync;
    }

}
