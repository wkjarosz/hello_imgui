#include "hello_imgui/renderer_backend_options.h"

#if defined(HELLOIMGUI_USE_GLFW3) && !defined(HELLOIMGUI_HAS_METAL)
#include <GLFW/glfw3.h>
#endif

namespace HelloImGui
{

// currently, only the metal backend has support for this
#ifndef HELLOIMGUI_HAS_METAL

#if defined(HELLOIMGUI_USE_GLFW3) && defined(GLFW_FLOATBUFFER)
// GLFW_FLOATBUFFER is only defined by HDR-enabling GLFW forks (e.g. https://github.com/Tom94/glfw, the
// fork tev/nanogui-1 use for the same purpose); on vanilla GLFW this whole branch compiles out and the
// fallback below (always false) is used instead, so linking against an HDR-enabled GLFW fork is the only
// way to opt into this.
//
// Mirrors that fork's own per-platform logic (see Screen::Screen in nanogui-1's screen.cpp): there is no
// pre-window capability query available on Windows in this GLFW fork -- the HDR query functions it adds
// (glfwGetWindowMaxLuminance, glfwGetWindowTransfer, etc.) are all window-scoped, not monitor-scoped, so
// they can't answer this before a window exists, and hasEdrSupport() is called well before that. Windows
// therefore just optimistically reports support and lets window creation itself fail gracefully if the
// GPU/driver doesn't actually support a float pixel format (see runner_glfw3.cpp's retry-without-hint
// fallback). Non-Wayland Linux has no HDR support in this fork at all.
//
// Note: on Linux this also requires glfwInit() to have already run (glfwGetPlatform() safely returns 0,
// i.e. "not Wayland", before that -- not a crash, just always false pre-init). hasEdrSupport() is currently
// called before glfwInit() in HDRView's startup sequence, so this branch is not yet meaningfully exercised
// there; flagging for whoever wires up the Linux/Wayland path.
bool hasEdrSupport()
{
#if defined(_WIN32)
    return true;
#elif defined(__linux__)
    return glfwGetPlatform() == GLFW_PLATFORM_WAYLAND;
#else
    return false;
#endif
}
#else
bool hasEdrSupport() { return false; }
#endif

#endif  // #ifndef HELLOIMGUI_HAS_METAL

}  // namespace HelloImGui
