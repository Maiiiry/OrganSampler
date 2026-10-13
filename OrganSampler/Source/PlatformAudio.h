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

#if JUCE_LINUX
// JACK through PipeWire: JUCE loads "libjack.so.0" by name, which on most
// distributions is the jackd2 client library - useless unless a jackd runs.
// PipeWire ships its own libjack in a directory of its own (what pw-jack puts
// on the library path). Loading that one first, globally, makes JUCE's later
// dlopen of the same soname find it, so "JACK" in the device list connects to
// PipeWire and shows every channel of every card. Returns what was loaded.
#include <dlfcn.h>
inline void*& jackLibraryHandle() { static void* handle = nullptr; return handle; }
inline juce::String preloadPipeWireJack()
{
    static const char* const candidates[]{
        "/usr/lib/x86_64-linux-gnu/pipewire-0.3/jack/libjack.so.0",
        "/usr/lib64/pipewire-0.3/jack/libjack.so.0",
        "/usr/lib/pipewire-0.3/jack/libjack.so.0",
        "/usr/lib/aarch64-linux-gnu/pipewire-0.3/jack/libjack.so.0",
        "/usr/local/lib/pipewire-0.3/jack/libjack.so.0" };
    for (const char* path : candidates)
        if (juce::File(path).existsAsFile())
            if (void* handle = dlopen(path, RTLD_NOW | RTLD_GLOBAL)) { jackLibraryHandle() = handle; return path; }
    jackLibraryHandle() = dlopen("libjack.so.0", RTLD_NOW | RTLD_GLOBAL);
    return {};
}

// JUCE's JACK type lists every client that owns a port, MIDI bridges and MIDI
// devices included ("Midi-Bridge", "BLE MIDI 1" - routes "opened" on them run
// with a buffer of 1 frame). This wrapper keeps the clients that own AUDIO
// ports in the wanted direction, asked of the JACK server through the library
// loaded above. The type name stays "JACK" for the saved configurations.
class FilteredJackDeviceType final : public juce::AudioIODeviceType
{
public:
    FilteredJackDeviceType() : juce::AudioIODeviceType("JACK"), inner(juce::AudioIODeviceType::createAudioIODeviceType_JACK()) {}

    void scanForDevices() override
    {
        if (inner == nullptr) return;
        inner->scanForDevices();
        const auto audioOutputs = audioClients(true);    // clients with audio INPUT ports: where we can play
        const auto audioInputs = audioClients(false);
        outputs.clear(); inputs.clear();
        for (const auto& name : inner->getDeviceNames(false)) if (audioOutputs.contains(name)) outputs.add(name);
        for (const auto& name : inner->getDeviceNames(true)) if (audioInputs.contains(name)) inputs.add(name);
    }
    juce::StringArray getDeviceNames(bool wantInputNames = false) const override { return wantInputNames ? inputs : outputs; }
    int getDefaultDeviceIndex(bool forInput) const override { return (forInput ? inputs : outputs).isEmpty() ? -1 : 0; }
    int getIndexOfDevice(juce::AudioIODevice* device, bool asInput) const override
    {
        return device == nullptr ? -1 : (asInput ? inputs : outputs).indexOf(device->getName());
    }
    bool hasSeparateInputsAndOutputs() const override { return true; }
    juce::AudioIODevice* createDevice(const juce::String& outputDeviceName, const juce::String& inputDeviceName) override
    {
        return inner != nullptr ? inner->createDevice(outputDeviceName, inputDeviceName) : nullptr;
    }
private:
    // The clients owning audio ports of one direction, by the JACK API itself
    // (only the handful of functions needed, taken from the loaded library).
    static juce::StringArray audioClients(bool playbackPorts)
    {
        juce::StringArray clients;
        void* lib = jackLibraryHandle();
        if (lib == nullptr) return clients;
        using ClientOpen = void* (*)(const char*, int, int*, ...);
        using GetPorts = const char** (*)(void*, const char*, const char*, unsigned long);
        using Free = void (*)(void*);
        using ClientClose = int (*)(void*);
        auto open = (ClientOpen)dlsym(lib, "jack_client_open");
        auto getPorts = (GetPorts)dlsym(lib, "jack_get_ports");
        auto freePorts = (Free)dlsym(lib, "jack_free");
        auto close = (ClientClose)dlsym(lib, "jack_client_close");
        if (open == nullptr || getPorts == nullptr || freePorts == nullptr || close == nullptr) return clients;
        int status = 0;
        void* client = open("OrganSamplerScan", 0x01 /* JackNoStartServer */, &status);
        if (client == nullptr) return clients;
        // Playback ports of the server are INPUT ports (we write into them): JackPortIsInput = 1, JackPortIsOutput = 2.
        if (const char** ports = getPorts(client, nullptr, "32 bit float mono audio", playbackPorts ? 1UL : 2UL))
        {
            for (const char** p = ports; *p != nullptr; ++p)
            {
                const juce::String name = juce::CharPointer_UTF8(*p);
                clients.addIfNotAlreadyThere(name.upToFirstOccurrenceOf(":", false, false));
            }
            freePorts(ports);
        }
        close(client);
        return clients;
    }
    std::unique_ptr<juce::AudioIODeviceType> inner;
    juce::StringArray outputs, inputs;
};

inline void installFilteredJackType(juce::AudioDeviceManager& manager)
{
    for (auto* type : manager.getAvailableDeviceTypes())
        if (type != nullptr && type->getTypeName() == "JACK" && dynamic_cast<FilteredJackDeviceType*>(type) == nullptr)
        {
            manager.removeAudioDeviceType(type);
            break;
        }
    auto filtered = std::make_unique<FilteredJackDeviceType>();
    filtered->scanForDevices();
    manager.addAudioDeviceType(std::move(filtered));
}
#endif

inline void installPlatformAudioTypes(juce::AudioDeviceManager& manager)
{
    addAggregatedWASAPIType(manager);
   #if JUCE_LINUX
    installFilteredAlsaType(manager);
    installFilteredJackType(manager);
   #endif
}
