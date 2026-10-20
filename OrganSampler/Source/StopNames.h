#pragma once

#include <JuceHeader.h>

//==============================================================================
// Stop names made by the program (the tree scan's row names, the aliases from
// folder names, the Hauptwerk import's aliases) - never a name the user typed.
namespace StopNames
{
    // The first letter of the stop name proper after a division abbreviation
    // is a capital (0.8.135, the user's rule): "P. trombone 16" -> "P. Trombone
    // 16", "3 geigenpr 8" -> "3 Geigenpr 8", "Rec: hautbois 8" -> "Rec: Hautbois
    // 8". An abbreviation is the first token - up to five letters or digits
    // ending in '.' or ':' ("Ped." "GO." "II." "Rec:"), or a bare number or
    // roman numeral ("3", "II") - followed by a space.
    inline juce::String capitaliseAfterDivisionPrefix(juce::String name)
    {
        const int length = name.length();
        int i = 0;
        while (i < length && name[i] == ' ') ++i;
        const int start = i;
        while (i < length && name[i] != ' ' && name[i] != '.' && name[i] != ':') ++i;
        const juce::String stem = name.substring(start, i);
        if (stem.isEmpty() || stem.length() > 5) return name;
        for (int c = 0; c < stem.length(); ++c)
            if (!juce::CharacterFunctions::isLetterOrDigit(stem[c])) return name;
        bool marked = false;
        if (i < length && (name[i] == '.' || name[i] == ':')) { marked = true; ++i; }
        if (i >= length || name[i] != ' ') return name;   // "P.trombone", a bare word: no
        if (!marked)
        {
            const auto lower = stem.toLowerCase();
            const bool number = stem.containsOnly("0123456789") && stem.length() <= 2;
            const bool roman = lower.containsOnly("ivx") && lower.length() <= 4;
            if (!number && !roman) return name;
        }
        while (i < length && name[i] == ' ') ++i;
        if (i >= length) return name;
        const juce::juce_wchar c = name[i];
        if (!juce::CharacterFunctions::isLowerCase(c)) return name;
        return name.substring(0, i) + juce::String::charToString(juce::CharacterFunctions::toUpperCase(c)) + name.substring(i + 1);
    }
}
