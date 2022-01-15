#ifndef __WINDOW_H__
#define __WINDOW_H__

struct GLFWwindow;

class Window {
public:
    using Ref = std::shared_ptr<Window>;
    using Event = int (*)();
    Window(const glm::vec2 &size, const std::string &title);
    Window(GLFWmonitor *monitor, const std::string &title);
    virtual ~Window();

    Window &add_event(Event e);
    Window &remove_event(size_t id);

    void poll_events();
    void clear(const glm::vec4 &clr);
    void update();

    bool isActive() const;
    bool isPressed(unsigned key);
    bool isHold(unsigned key);

    size_t width() const;
    size_t height() const;

    GLFWwindow *handler();
private:
    GLFWwindow *wnd;
    std::vector<Event> events;

    void setupWindowSettings();

    static void onWindowResized(GLFWwindow *, int width, int height);
    static void onKeyPressed(GLFWwindow *window, int key, int scancode, int action, int mods);
    static void onMouseButtonPressed(GLFWwindow *window, int button, int action, int mods);
};

#endif // __WINDOW_H__
