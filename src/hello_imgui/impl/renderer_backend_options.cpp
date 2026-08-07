#include "hello_imgui/renderer_backend_options.h"

#if defined(HELLOIMGUI_USE_GLFW3) && !defined(HELLOIMGUI_HAS_METAL)
#include <GLFW/glfw3.h>
#endif
#if defined(__linux__)
#include <cstdlib>
#endif

namespace HelloImGui
{

// currently, only the metal backend has support for this
#ifndef HELLOIMGUI_HAS_METAL

#if defined(HELLOIMGUI_USE_GLFW3) && defined(GLFW_FLOATBUFFER)
// GLFW_FLOATBUFFER is only defined by HDR-enabling GLFW forks (e.g. https://github.com/Tom94/glfw); on
// vanilla GLFW this whole branch compiles out and the fallback below (always false) is used instead.
//
// The HDR query functions this fork adds are window-scoped, not monitor-scoped, so they can't answer this
// before a window exists, and hasEdrSupport() runs before that. Windows optimistically reports support and
// lets window creation fail gracefully if the GPU/driver can't deliver a float pixel format (see
// runner_glfw3.cpp's retry-without-hint fallback). Linux checks WAYLAND_DISPLAY directly for the same
// reason -- glfwGetPlatform() isn't usable pre-init (mirrors GLFW's own detection in src/platform.c). X11
// has no HDR support in this fork.
bool hasEdrSupport()
{
#if defined(_WIN32)
    return true;
#elif defined(__linux__)
    return getenv("WAYLAND_DISPLAY") != nullptr;
#else
    return false;
#endif
}
#else
bool hasEdrSupport() { return false; }
#endif

#endif  // #ifndef HELLOIMGUI_HAS_METAL

}  // namespace HelloImGui
