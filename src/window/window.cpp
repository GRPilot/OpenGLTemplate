#include "incs.hpp"
#include "plog/Log.h"

#include "window.hpp"

Window::Window(const glm::vec2 &size, const std::string &title)
    : wnd(nullptr) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    wnd = glfwCreateWindow(size.x, size.y, title.c_str(), nullptr, nullptr);
    if(nullptr == wnd) {
        LOGE << "[Window] Cannot initialize window";
        return;
    }
    setupWindowSettings();
}

Window::Window(GLFWmonitor *monitor, const std::string &title) : wnd(nullptr) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    const GLFWvidmode *mode = glfwGetVideoMode(monitor);
    glfwWindowHint(GLFW_RED_BITS, mode->redBits);
    glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
    glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
    glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

    wnd = glfwCreateWindow(mode->width, mode->height, title.c_str(), monitor, nullptr);
    if(nullptr == wnd) {
        LOGE << "[Window] Cannot initialize window";
        return;
    }
    setupWindowSettings();
}

Window::~Window() {
    if(nullptr != wnd) {
        glfwDestroyWindow(wnd);
    }
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

}

Window &Window::add_event(Window::Event e) {
    if(nullptr != e) {
        events.push_back(e);
    }
    return *this;
}
Window &Window::remove_event(size_t id) {
    if(events.size() < id) {
        events.erase(events.begin() + id);
    }
    return *this;
}

void Window::poll_events() {
    for(const auto &e : events) {
        e();
    }
    
    glfwPollEvents();
}

void Window::clear(const glm::vec4 &clr) {
    glClearColor(clr.r, clr.g, clr.b, clr.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void Window::update() {
    glfwSwapBuffers(wnd);
}

bool Window::isAvtive() const {
    return nullptr != wnd && !glfwWindowShouldClose(wnd);
}

bool Window::isPressed(unsigned key) {
    static unsigned last_key = 0;

    int status = glfwGetKey(wnd, key);
    switch(status) {
        case GLFW_PRESS:
            last_key = key;
            return false;
        case GLFW_RELEASE: {
            unsigned lk = last_key;
            last_key = 0;
            return lk == key;
        }
    }
    return false;
}

bool Window::isHold(unsigned key) {
    return GLFW_PRESS == glfwGetKey(wnd, key);
}

size_t Window::width() const {
    int width = 0;
    glfwGetWindowSize(wnd, &width, nullptr);
    if(width < 0) width = 0;
    return static_cast<size_t>(width);
}

size_t Window::height() const {
    int height = 0;
    glfwGetWindowSize(wnd, nullptr, &height);
    if(height < 0) height = 0;
    return static_cast<size_t>(height);
}

GLFWwindow* Window::handler() {
    return wnd;
}

void Window::setupWindowSettings() {
    IMGUI_CHECKVERSION();
    ImGuiContext *ctx = ImGui::CreateContext();
    if(nullptr == ctx) {
        LOGE << "[ImGui] Cannot create context";
    }

    glfwSetFramebufferSizeCallback(wnd, Window::onWindowResized);
    glfwSetKeyCallback(wnd, Window::onKeyPressed);
    glfwSetMouseButtonCallback(wnd, Window::onMouseButtonPressed);
    glfwMakeContextCurrent(wnd);
    glfwSwapInterval(1);
    glfwSetWindowOpacity(wnd, 0.95);

    ImGui::SetCurrentContext(ctx);
    ImGui_ImplGlfw_InitForOpenGL(wnd, true);
    ImGui_ImplOpenGL3_Init("#version 330 core");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_ALWAYS);
}

void Window::onWindowResized(GLFWwindow *, int width, int height) {
    glViewport(0, 0, width, height);
}

void Window::onKeyPressed(GLFWwindow *window, int key, int scancode, int action, int mods) {
    if(key == GLFW_KEY_ESCAPE) {
        glfwSetWindowShouldClose(window, true);
    }
}

void Window::onMouseButtonPressed(GLFWwindow *window, int button, int action, int mods) {
    UNUSED(window);
    UNUSED(button);
    UNUSED(action);
    UNUSED(mods);
}
