#pragma once

#include <JuceHeader.h>

// Linux: the report's two windows side by side through X11 itself (0.8.144) -
// the file manager showing the report folder (found by its title) on the right
// half of the screen, the browser (by its window class, else the active window)
// on the left; wmctrl was asked for before and is rarely installed. Both are
// un-maximised, moved and resized through EWMH client messages and raised.
namespace LinuxWindowPlacement
{
    bool placeSideBySide(const juce::String& folderName, juce::Rectangle<int> left, juce::Rectangle<int> right);
}