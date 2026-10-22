// X11 only - no JUCE header in this file (see LinuxWindowPlacement.h).
#include "LinuxWindowPlacement.h"

#if defined(__linux__) && !defined(__ANDROID__)
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xutil.h>
#include <algorithm>
#include <cctype>
#include <cstring>
#include <vector>

namespace
{
    std::string lowered(std::string s)
    {
        for (auto& c : s) c = (char)std::tolower((unsigned char)c);
        return s;
    }

    bool containsIgnoreCase(const std::string& haystack, const std::string& needle)
    {
        return !needle.empty() && lowered(haystack).find(lowered(needle)) != std::string::npos;
    }

    std::string windowName(Display* display, Window window)
    {
        const Atom utf8 = XInternAtom(display, "UTF8_STRING", False), netName = XInternAtom(display, "_NET_WM_NAME", False);
        Atom type = 0; int format = 0; unsigned long count = 0, after = 0; unsigned char* data = nullptr;
        if (XGetWindowProperty(display, window, netName, 0, 1024, False, utf8, &type, &format, &count, &after, &data) == Success && data != nullptr)
        {
            const std::string name((const char*)data, (size_t)count);
            XFree(data);
            if (!name.empty()) return name;
        }
        XTextProperty text{};
        if (XGetWMName(display, window, &text) && text.value != nullptr)
        {
            const std::string name((const char*)text.value);
            XFree(text.value);
            return name;
        }
        return {};
    }

    std::string windowClass(Display* display, Window window)
    {
        XClassHint hint{};
        if (!XGetClassHint(display, window, &hint)) return {};
        std::string result;
        if (hint.res_class != nullptr) { result += hint.res_class; XFree(hint.res_class); }
        if (hint.res_name != nullptr) { result += " "; result += hint.res_name; XFree(hint.res_name); }
        return lowered(result);
    }

    void sendToRoot(Display* display, XEvent& event)
    {
        XSendEvent(display, DefaultRootWindow(display), False, SubstructureRedirectMask | SubstructureNotifyMask, &event);
    }

    // Whether the window manager announces an EWMH message in _NET_SUPPORTED.
    bool wmSupports(Display* display, const char* atomName)
    {
        const Atom supported = XInternAtom(display, "_NET_SUPPORTED", False), wanted = XInternAtom(display, atomName, False);
        Atom type = 0; int format = 0; unsigned long count = 0, after = 0; unsigned char* data = nullptr;
        bool found = false;
        if (XGetWindowProperty(display, DefaultRootWindow(display), supported, 0, 4096, False, XA_ATOM, &type, &format, &count, &after, &data) == Success && data != nullptr)
        {
            for (unsigned long i = 0; i < count && !found; ++i) found = ((Atom*)data)[i] == wanted;
            XFree(data);
        }
        return found;
    }

    void placeWindow(Display* display, Window window, LinuxWindowPlacement::Box r, bool ewmh)
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
        move.xclient.data.l[1] = r.x;
        move.xclient.data.l[2] = r.y;
        move.xclient.data.l[3] = r.width;
        move.xclient.data.l[4] = r.height;
        // A manager with _NET_MOVERESIZE_WINDOW gets the message alone (0.8.149):
        // moving the client window as well under KWin left the frame and the
        // client at odds, the windows overlapping (#58). Without EWMH the plain move.
        if (ewmh) sendToRoot(display, move);
        else XMoveResizeWindow(display, window, r.x, r.y, (unsigned)std::max(1, r.width), (unsigned)std::max(1, r.height));
        XRaiseWindow(display, window);
    }
}

bool LinuxWindowPlacement::placeSideBySide(const std::string& folderName, Box left, Box right, std::string& note)
{
    Display* display = XOpenDisplay(nullptr);
    if (display == nullptr) { note = "Report windows (X11): no display"; return false; }
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
    static const char* fileManagers[]{ "dolphin", "nautilus", "thunar", "pcmanfm", "nemo", "caja", "spacefm", "krusader", "doublecmd", "org.gnome.files" };
    Window fileManager = 0, browser = 0;
    std::string seen;
    for (const Window w : windows)
    {
        const std::string name = windowName(display, w), cls = windowClass(display, w);
        if (cls.find("organsampler") != std::string::npos) continue;
        if (seen.size() < 600) seen += " [" + cls.substr(0, 24) + ": " + name.substr(0, 40) + "]";
        if (fileManager == 0 && containsIgnoreCase(name, folderName)) { fileManager = w; continue; }
        if (browser == 0)
            for (const char* b : browsers) if (cls.find(b) != std::string::npos) { browser = w; break; }
    }
    // The folder's window not found by its title (a file manager that shows the
    // folder name differently): the first file manager by its class (0.8.149).
    if (fileManager == 0)
        for (const Window w : windows)
        {
            const std::string cls = windowClass(display, w);
            if (cls.find("organsampler") != std::string::npos || w == browser) continue;
            bool fm = false;
            for (const char* f : fileManagers) if (cls.find(f) != std::string::npos) fm = true;
            if (fm) { fileManager = w; break; }
        }
    if (browser == 0 && active != 0 && active != fileManager && windowClass(display, active).find("organsampler") == std::string::npos) browser = active;
    const bool ewmh = wmSupports(display, "_NET_MOVERESIZE_WINDOW");
    if (fileManager != 0) placeWindow(display, fileManager, right, ewmh);
    if (browser != 0) placeWindow(display, browser, left, ewmh);
    XFlush(display);
    XCloseDisplay(display);
    note = std::string("Report windows (X11, ") + (ewmh ? "EWMH" : "no EWMH") + "): file manager " + (fileManager != 0 ? "placed" : "not found") + ", browser " + (browser != 0 ? "placed" : "not found")
        + "; left " + std::to_string(left.x) + "," + std::to_string(left.y) + " " + std::to_string(left.width) + "x" + std::to_string(left.height)
        + ", right " + std::to_string(right.x) + "," + std::to_string(right.y) + " " + std::to_string(right.width) + "x" + std::to_string(right.height)
        + "; windows seen:" + seen;
    return fileManager != 0 || browser != 0;
}
#else
bool LinuxWindowPlacement::placeSideBySide(const std::string&, Box, Box, std::string&) { return false; }
#endif
