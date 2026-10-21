#include "LinuxWindowPlacement.h"
#include "CrashHandler.h"

#if JUCE_LINUX
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>

namespace
{
    juce::String windowName(Display* display, Window window)
    {
        const Atom utf8 = XInternAtom(display, "UTF8_STRING", False), netName = XInternAtom(display, "_NET_WM_NAME", False);
        Atom type = 0; int format = 0; unsigned long count = 0, after = 0; unsigned char* data = nullptr;
        if (XGetWindowProperty(display, window, netName, 0, 1024, False, utf8, &type, &format, &count, &after, &data) == Success && data != nullptr)
        {
            const juce::String name = juce::String::fromUTF8((const char*)data, (int)count);
            XFree(data);
            if (name.isNotEmpty()) return name;
        }
        XTextProperty text{};
        if (XGetWMName(display, window, &text) && text.value != nullptr)
        {
            const juce::String name = juce::String::fromUTF8((const char*)text.value);
            XFree(text.value);
            return name;
        }
        return {};
    }

    juce::String windowClass(Display* display, Window window)
    {
        XClassHint hint{};
        if (!XGetClassHint(display, window, &hint)) return {};
        juce::String result;
        if (hint.res_class != nullptr) { result << hint.res_class; XFree(hint.res_class); }
        if (hint.res_name != nullptr) { result << " " << hint.res_name; XFree(hint.res_name); }
        return result.toLowerCase();
    }

    void sendToRoot(Display* display, XEvent& event)
    {
        XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
    }

    void placeWindow(Display* display, Window window, juce::Rectangle<int> r)
    {
        // Out of the maximised state first (a maximised window ignores a move).
        XEvent state{};
        state.xclient.type = ClientMessage;
        state.xclient.window = window;
        state.xclient.message_type = XInternAtom(display, "_NET_WM_STATE", False);
        state.xclient.format = 32;
        state.xclient.data.l[0] = 0;   // _NET_WM_STATE_REMOVE
        state.xclient.data.l[1] = (long)XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_VERT", False);
        state.xclient.data.l[2] = (long)XInternAtom(display, "_NET_WM_STATE_MAXIMIZED_HORZ", False);
        state.xclient.data.l[3] = 2;   // the source: a pager/user action
        sendToRoot(display, state);
        XEvent move{};
        move.xclient.type = ClientMessage;
        move.xclient.window = window;
        move.xclient.message_type = XInternAtom(display, "_NET_MOVERESIZE_WINDOW", False);
        move.xclient.format = 32;
        move.xclient.data.l[0] = (1 << 8) | (1 << 9) | (1 << 10) | (1 << 11) | (2 << 12);   // x, y, width, height given; source: user
        move.xclient.data.l[1] = r.getX();
        move.xclient.data.l[2] = r.getY();
        move.xclient.data.l[3] = r.getWidth();
        move.xclient.data.l[4] = r.getHeight();
        sendToRoot(display, move);
        XMoveResizeWindow(display, window, r.getX(), r.getY(), (unsigned)juce::jmax(1, r.getWidth()), (unsigned)juce::jmax(1, r.getHeight()));   // for a manager without EWMH
        XRaiseWindow(display, window);
    }
}

bool LinuxWindowPlacement::placeSideBySide(const juce::String& folderName, juce::Rectangle<int> left, juce::Rectangle<int> right)
{
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) return false;
    const Window root = DefaultRootWindow(display);
    std::vector<Window> windows;
    {
        const Atom clientList = XInternAtom(display, "_NET_CLIENT_LIST", False);
        Atom type = 0; int format = 0; unsigned long count = 0, after = 0; unsigned char* data = nullptr;
        if (XGetWindowProperty(display, root, clientList, 0, 4096, False, XA_WINDOW, &type, &format, &count, &after, &data) == Success && data != nullptr)
        {
            for (unsigned long i = 0; i < count; ++i) windows.push_back(((Window*)data)[i]);
            XFree(data);
        }
    }
    Window active = 0;
    {
        const Atom activeAtom = XInternAtom(display, "_NET_ACTIVE_WINDOW", False);
        Atom type = 0; int format = 0; unsigned long count = 0, after = 0; unsigned char* data = nullptr;
        if (XGetWindowProperty(display, root, activeAtom, 0, 1, False, XA_WINDOW, &type, &format, &count, &after, &data) == Success && data != nullptr)
        {
            if (count > 0) active = ((Window*)data)[0];
            XFree(data);
        }
    }
    static const char* browsers[]{ "firefox", "chrom", "brave", "vivaldi", "opera", "epiphany", "falkon", "konqueror", "midori", "librewolf", "edge" };
    Window fileManager = 0, browser = 0;
    for (const Window w : windows)
    {
        const juce::String name = windowName(display, w), cls = windowClass(display, w);
        if (cls.contains("organsampler")) continue;
        if (fileManager == 0 && name.containsIgnoreCase(folderName)) { fileManager = w; continue; }
        if (browser == 0)
            for (const char* b : browsers) if (cls.contains(b)) { browser = w; break; }
    }
    if (browser == 0 && active != 0 && active != fileManager && !windowClass(display, active).contains("organsampler")) browser = active;
    if (fileManager != 0) placeWindow(display, fileManager, right);
    if (browser != 0) placeWindow(display, browser, left);
    XFlush(display);
    XCloseDisplay(display);
    CrashHandler::note("Report windows (X11): file manager " + juce::String(fileManager != 0 ? "placed" : "not found") + ", browser " + juce::String(browser != 0 ? "placed" : "not found"));
    return fileManager != 0 || browser != 0;
}
#else
bool LinuxWindowPlacement::placeSideBySide(const juce::String&, juce::Rectangle<int>, juce::Rectangle<int>) { return false; }
#endif