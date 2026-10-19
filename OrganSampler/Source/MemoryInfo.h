#pragma once

#include <JuceHeader.h>

#if JUCE_WINDOWS
 #ifndef NOMINMAX
  #define NOMINMAX
 #endif
 #include <windows.h>
 #ifndef PSAPI_VERSION
  #define PSAPI_VERSION 2   // GetProcessMemoryInfo from kernel32 (K32...), no psapi.lib
 #endif
 #include <psapi.h>
#elif JUCE_MAC
 #include <mach/mach.h>
 #include <mach/mach_host.h>
 #include <mach/task_info.h>
 #include <sys/sysctl.h>
#endif

//==============================================================================
// The memory figures the loading, the merge, the polyphony display and the
// measurement report share. Every figure means the same on every system:
//  - free: what the system could give the program now (Windows: available
//    physical; Linux: MemAvailable; macOS: free + inactive pages);
//  - held: what the program holds, the pages the system has compressed or
//    swapped out included (Windows: the larger of working set and private
//    bytes; Linux: VmRSS + VmSwap; macOS: the physical footprint Activity
//    Monitor shows - the resident size alone said 995 MB for an organ of
//    several GB on an 8 GB Mac, the rest sat compressed);
//  - swapped: the program's pages the system has swapped out (Linux VmSwap;
//    macOS: the compressor's pages of the whole system; Windows: 0);
//  - installed: the physical memory of the computer.
namespace MemoryInfo
{
    inline juce::int64 installedBytes()
    {
        return (juce::int64)juce::SystemStats::getMemorySizeInMegabytes() * 1024 * 1024;
    }

    inline juce::int64 freeBytes()
    {
       #if JUCE_WINDOWS
        MEMORYSTATUSEX status{};
        status.dwLength = sizeof(status);
        if (GlobalMemoryStatusEx(&status)) return (juce::int64)status.ullAvailPhys;
       #elif JUCE_LINUX
        const auto text = juce::File("/proc/meminfo").loadFileAsString();
        for (const auto& line : juce::StringArray::fromLines(text))
            if (line.startsWith("MemAvailable:"))
                return line.fromFirstOccurrenceOf(":", false, false).trim().upToFirstOccurrenceOf(" ", false, false).getLargeIntValue() * 1024;
       #elif JUCE_MAC
        vm_statistics64_data_t stats{};
        mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
        if (host_statistics64(mach_host_self(), HOST_VM_INFO64, (host_info64_t)&stats, &count) == KERN_SUCCESS)
        {
            vm_size_t pageSize = 0;
            host_page_size(mach_host_self(), &pageSize);
            return (juce::int64)(stats.free_count + stats.inactive_count) * (juce::int64)pageSize;
        }
       #endif
        return installedBytes() / 2;   // unknown: half of the installed memory
    }

    inline juce::int64 heldBytes()
    {
       #if JUCE_WINDOWS
        PROCESS_MEMORY_COUNTERS_EX counters{};
        counters.cb = sizeof(counters);
        if (GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&counters, sizeof(counters)))
            return (juce::int64)juce::jmax(counters.WorkingSetSize, counters.PrivateUsage);
       #elif JUCE_LINUX
        const auto text = juce::File("/proc/self/status").loadFileAsString();
        juce::int64 resident = 0, swapped = 0;
        for (const auto& line : juce::StringArray::fromLines(text))
        {
            auto kb = [&line] { return line.fromFirstOccurrenceOf(":", false, false).trim().upToFirstOccurrenceOf(" ", false, false).getLargeIntValue() * 1024; };
            if (line.startsWith("VmRSS:")) resident = kb();
            else if (line.startsWith("VmSwap:")) swapped = kb();
        }
        if (resident > 0) return resident + swapped;
       #elif JUCE_MAC
        {
            task_vm_info_data_t vm{};
            mach_msg_type_number_t vmCount = TASK_VM_INFO_COUNT;
            if (task_info(mach_task_self(), TASK_VM_INFO, (task_info_t)&vm, &vmCount) == KERN_SUCCESS && vm.phys_footprint > 0)
                return (juce::int64)vm.phys_footprint;
        }
        mach_task_basic_info_data_t info{};
        mach_msg_type_number_t count = MACH_TASK_BASIC_INFO_COUNT;
        if (task_info(mach_task_self(), MACH_TASK_BASIC_INFO, (task_info_t)&info, &count) == KERN_SUCCESS)
            return (juce::int64)info.resident_size;
       #endif
        return 0;
    }

    inline juce::int64 swappedBytes()
    {
       #if JUCE_LINUX
        const auto text = juce::File("/proc/self/status").loadFileAsString();
        for (const auto& line : juce::StringArray::fromLines(text))
            if (line.startsWith("VmSwap:"))
                return line.fromFirstOccurrenceOf(":", false, false).trim().upToFirstOccurrenceOf(" ", false, false).getLargeIntValue() * 1024;
       #elif JUCE_MAC
        vm_statistics64_data_t stats{};
        mach_msg_type_number_t count = HOST_VM_INFO64_COUNT;
        if (host_statistics64(mach_host_self(), HOST_VM_INFO64, (host_info64_t)&stats, &count) == KERN_SUCCESS)
        {
            vm_size_t pageSize = 0;
            host_page_size(mach_host_self(), &pageSize);
            return (juce::int64)stats.compressor_page_count * (juce::int64)pageSize;
        }
       #endif
        return 0;
    }

    // The memory the system keeps for itself and the window: 1.5 GB or a fifth
    // of the installed memory. The program must stop loading before its own
    // footprint reaches installed - share: beyond it the system pages samples
    // out, and every stroke that touches them stalls the sound (two-core
    // laptops with 8 GB and a 7 GB organ, issues #16, #17, #19).
    inline juce::int64 systemShareBytes()
    {
        return juce::jmax((juce::int64)1536 * 1024 * 1024, installedBytes() / 5);
    }
    // The free memory the loading leaves untouched: 768 MB or a sixteenth.
    inline juce::int64 reserveBytes()
    {
        return juce::jmax((juce::int64)768 * 1024 * 1024, installedBytes() / 16);
    }

    // macOS: the kernel's own memory pressure level (1 normal, 2 warning, 4
    // critical) - it already counts what freeing the inactive pages would give,
    // which a user process cannot force (purge needs root and only drops the
    // disk cache; madvise touches our own pages only). 0 where unknown.
    inline int pressureLevel()
    {
       #if JUCE_MAC
        int level = 0;
        size_t size = sizeof(level);
        if (sysctlbyname("kern.memorystatus_vm_pressure_level", &level, &size, nullptr, 0) == 0) return level;
       #endif
        return 0;
    }

    // Is the memory short now - must a loading stop? Any one sign: the free
    // memory under the reserve; the program's footprint within the system's
    // share; the program (Linux) or the system (macOS compressor) swapping
    // more than 256 MB; the kernel reporting pressure (macOS, warning or worse).
    inline bool shortNow(juce::String* reason = nullptr)
    {
        auto gb = [](juce::int64 b) { return juce::String((double)b / 1073741824.0, 2) + " GB"; };
        const juce::int64 installed = installedBytes(), free = freeBytes(), held = heldBytes(), swapped = swappedBytes();
        if (const int level = pressureLevel(); level >= 2)
        {
            if (reason != nullptr) *reason = juce::String("the system reports memory pressure (level ") + juce::String(level) + ", " + (level >= 4 ? "critical" : "warning") + "), " + gb(free) + " free, the program holds " + gb(held);
            return true;
        }
        if (free < reserveBytes())
        {
            if (reason != nullptr) *reason = gb(free) + " free, the reserve is " + gb(reserveBytes());
            return true;
        }
        if (held > 0 && held > installed - systemShareBytes())
        {
            if (reason != nullptr) *reason = "the program holds " + gb(held) + " of " + gb(installed) + " installed and the system needs " + gb(systemShareBytes());
            return true;
        }
        if (swapped > (juce::int64)256 * 1024 * 1024)
        {
            if (reason != nullptr) *reason = "the system is paging already (" + gb(swapped) + " swapped or compressed), " + gb(free) + " free";
            return true;
        }
        return false;
    }
}
