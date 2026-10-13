#pragma once

#include <JuceHeader.h>
#include "AggregatedWASAPIAudio.h"

// The audio device types every AudioDeviceManager of the program gets beyond
// JUCE's own: the Aggregated WASAPI device on Windows, and on Linux an ALSA
// list without the template PCMs.

#if JUCE_LINUX
// ALSA advertises, for every card, the PCM templates of its configuration
// (surround40 ... surround71, front, rear, center_lfe, side, dmix, dsnoop)
// whether or not the card has those channels; most cannot be opened and the
// list drowns the real devices. This type wraps JUCE's ALSA type and lists
// only the real ones: the card's hardware device, the default and the
// digital/HDMI outputs. The saved device type name stays "ALSA".
class FilteredAlsaDeviceType final : public juce::AudioIODeviceType
{
public:
    FilteredAlsaDeviceType()
        : juce::AudioIODeviceType("ALSA"), inner(juce::AudioIODeviceType::createAudioIODeviceType_ALSA()) {}

    static bool isTemplate(const juce::String& name)
    {
        return name.containsIgnoreCase("Surround output") || name.containsIgnoreCase("Front output")
            || name.containsIgnoreCase("Rear output") || name.containsIgnoreCase("Center and Subwoofer")
            || name.containsIgnoreCase("Side output") || name.containsIgnoreCase("Direct sample mixing")
            || name.containsIgnoreCase("Direct sample snooping");
    }
    void scanForDevices() override
    {
        if (inner == nullptr) return;
        inner->scanForDevices();
        outputs.clear(); inputs.clear();
        for (const auto& name : inner->getDeviceNames(false)) if (!isTemplate(name)) outputs.add(name);
        for (const auto& name : inner->getDeviceNames(true)) if (!isTemplate(name)) inputs.add(name);
    }
    juce::StringArray getDeviceNames(bool wantInputNames = false) const override { return wantInputNames ? inputs : outputs; }
    int getDefaultDeviceIndex(bool forInput) const override
    {
        if (inner == nullptr) return -1;
        const auto& names = forInput ? inputs : outputs;
        const auto all = inner->getDeviceNames(forInput);
        const int inner_index = inner->getDefaultDeviceIndex(forInput);
        if (inner_index >= 0 && inner_index < all.size())
        {
            const int index = names.indexOf(all[inner_index]);
            if (index >= 0) return index;
        }
        return names.isEmpty() ? -1 : 0;
    }
    int getIndexOfDevice(juce::AudioIODevice* device, bool asInput) const override
    {
        return device == nullptr ? -1 : (asInput ? inputs : outputs).indexOf(device->getName());
    }
    bool hasSeparateInputsAndOutputs() const override { return inner != nullptr && inner->hasSeparateInputsAndOutputs(); }
    juce::AudioIODevice* createDevice(const juce::String& outputDeviceName, const juce::String& inputDeviceName) override
    {
        return inner != nullptr ? inner->createDevice(outputDeviceName, inputDeviceName) : nullptr;
    }
private:
    std::unique_ptr<juce::AudioIODeviceType> inner;
    juce::StringArray outputs, inputs;
};

inline void installFilteredAlsaType(juce::AudioDeviceManager& manager)
{
    // JUCE's own ALSA type goes, the filtered one takes its place (same name,
    // so saved configurations still say "ALSA").
    for (auto* type : manager.getAvailableDeviceTypes())
        if (type != nullptr && type->getTypeName() == "ALSA" && dynamic_cast<FilteredAlsaDeviceType*>(type) == nullptr)
        {
            manager.removeAudioDeviceType(type);
            break;
        }
    auto filtered = std::make_unique<FilteredAlsaDeviceType>();
    filtered->scanForDevices();
    manager.addAudioDeviceType(std::move(filtered));
}
#endif

inline void installPlatformAudioTypes(juce::AudioDeviceManager& manager)
{
    addAggregatedWASAPIType(manager);
   #if JUCE_LINUX
    installFilteredAlsaType(manager);
   #endif
}

#if JUCE_LINUX
// JACK through PipeWire: JUCE loads "libjack.so.0" by name, which on most
// distributions is the jackd2 client library - useless unless a jackd runs.
// PipeWire ships its own libjack in a directory of its own (what pw-jack puts
// on the library path). Loading that one first, globally, makes JUCE's later
// dlopen of the same soname find it, so "JACK" in the device list connects to
// PipeWire and shows every channel of every card. Returns what was loaded.
#include <dlfcn.h>
inline juce::String preloadPipeWireJack()
{
    static const char* const candidates[]{
        "/usr/lib/x86_64-linux-gnu/pipewire-0.3/jack/libjack.so.0",
        "/usr/lib64/pipewire-0.3/jack/libjack.so.0",
        "/usr/lib/pipewire-0.3/jack/libjack.so.0",
        "/usr/lib/aarch64-linux-gnu/pipewire-0.3/jack/libjack.so.0",
        "/usr/local/lib/pipewire-0.3/jack/libjack.so.0" };
    for (const char* path : candidates)
        if (juce::File(path).existsAsFile() && dlopen(path, RTLD_NOW | RTLD_GLOBAL) != nullptr) return path;
    return {};
}
#endif
