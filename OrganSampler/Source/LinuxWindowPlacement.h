#pragma once

#include <string>

// Linux: the report's two windows side by side through X11 itself (0.8.144) -
// the file manager showing the report folder (found by its title) on the right
// half of the screen, the browser (by its window class, else the active window)
// on the left; wmctrl was asked for before and is rarely installed. Both are
// un-maximised, moved and resized through EWMH client messages and raised.
// No JUCE in this interface: Xlib's typedefs (Font, Time, Drawable) clash with
// JUCE's classes under the "using namespace juce" of JuceHeader.h, so the
// implementation file includes X11 alone (0.8.145: the CI Linux build of
// 0.8.144 failed on exactly that).
namespace LinuxWindowPlacement
{
    struct Box { int x = 0, y = 0, width = 0, height = 0; };
    // folderNameUtf8: the report folder's name (the file manager's title holds it).
    // note: filled with one line for the log. Returns true when a window was placed.
    // box: where the program's own "Report a problem" box belongs (width 0 = leave it);
    // KDE on Wayland places a new window where it likes, the KWin script puts it back.
    bool placeSideBySide(const std::string& folderNameUtf8, Box left, Box right, Box box, std::string& note);
}
