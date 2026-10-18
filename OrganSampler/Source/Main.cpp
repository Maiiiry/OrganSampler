/*
  ==============================================================================

    This file contains the basic startup code for a JUCE application.

  ==============================================================================
*/

#include <JuceHeader.h>
#include "MainComponent.h"
#include "CrashHandler.h"
#include "PlatformAudio.h"
#include "UIScale.h"
#include "EngineSelfTest.h"

#if JUCE_MSVC
 #pragma comment(lib, "dbghelp.lib")
#endif

//==============================================================================
class OrganSamplerApplication  : public juce::JUCEApplication
{
public:
    //==============================================================================
    OrganSamplerApplication() {}

    const juce::String getApplicationName() override       { return ProjectInfo::projectName; }
    const juce::String getApplicationVersion() override    { return ProjectInfo::versionString; }
    // One instance only: a second one would open the same audio devices. The
    // running instance is told (anotherInstanceStarted) and warns the user.
    // Offline modes (--selftest, --asiotest) do not open the GUI and may run alongside.
    bool moreThanOneInstanceAllowed() override
    {
        const auto commandLine = juce::JUCEApplicationBase::getCommandLineParameters();
        return commandLine.contains("--selftest") || commandLine.contains("--asiotest");
    }

    //==============================================================================
    void initialise([[maybe_unused]] const juce::String& commandLine) override
    {
        // Any fatal error writes <AppData>/OrganSampler/CrashLogs/crash-<time>.txt (+ .dmp).
        CrashHandler::install(getApplicationName(), getApplicationVersion());
       #if JUCE_LINUX
        {
            // JACK through PipeWire, before any audio device type is created (PlatformAudio.h).
            const auto jack = preloadPipeWireJack();
            CrashHandler::note(jack.isNotEmpty() ? "JACK: PipeWire's libjack loaded from " + jack
                                                 : juce::String("JACK: no PipeWire libjack found, the system's libjack.so.0 is used"));
        }
       #endif
        // JUCE's own audio-driver diagnostics (ASIO buffer creation, resets)
        // go to CrashLogs/juce-audio.log so device problems can be traced.
        juceLog = std::make_unique<juce::FileLogger>(juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
            .getChildFile("OrganSampler").getChildFile("CrashLogs").getChildFile("juce-audio.log"),
            "OrganSampler " + getApplicationVersion() + " (build " __DATE__ " " __TIME__ ")", 512 * 1024);
        juce::Logger::setCurrentLogger(juceLog.get());
        if (commandLine.contains("--asiotest"))
        {
            // Driver probe: OrganSampler.exe --asiotest "Fireface"
            const auto filter = commandLine.fromFirstOccurrenceOf("--asiotest", false, false).trim().unquoted().trim();
            const auto report = EngineSelfTest::runAsioProbe(filter);
            juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                .getChildFile("OrganSampler").getChildFile("asiotest.txt").replaceWithText(report);
            std::fputs(report.toRawUTF8(), stdout);
            quit();
            return;
        }
        if (commandLine.contains("--hauptwerk"))
        {
            // Development aid: OrganSampler.exe --hauptwerk "<file>.Organ_Hauptwerk_xml" parses the
            // organ definition and prints what the import would make of it (hauptwerk.txt as well).
            const juce::File odfFile(commandLine.fromFirstOccurrenceOf("--hauptwerk", false, false).trim().unquoted().trim());
            juce::String report;
            const auto odf = HauptwerkOdf::parse(odfFile);
            report << "Hauptwerk ODF: " << odfFile.getFullPathName() << "\n" << (odf.error.isNotEmpty() ? "ERROR " + odf.error + "\n" : juce::String())
                   << "organ \"" << odf.organName << "\" by " << odf.builder << ", " << odf.location << "; supplier " << odf.supplier
                   << "; " << (odf.compacted ? "compacted" : "plain") << "; main package " << odf.mainPackageId << ", packages:";
            for (int id : odf.packageIds) report << " " << id;
            report << "\ndivisions " << (int)odf.divisions.size() << ", keyboards " << (int)odf.keyboards.size() << ", stops " << (int)odf.stops.size()
                   << ", ranks " << (int)odf.ranks.size() << ", pipe samples " << odf.sampleCount << " (encrypted " << odf.encryptedSamples << ")\n";
            for (const auto& [id, d] : odf.divisions) report << "  division " << id << ": " << d.name << "\n";
            for (const auto& k : odf.keyboards) report << "  keyboard " << k.id << ": " << k.name << " -> division " << k.divisionId << ", keys " << k.lowNote << "-" << k.highNote << "\n";
            int shown = 0;
            for (const auto& [id, r] : odf.ranks)
                if (shown++ < 12) report << "  rank " << id << ": " << r.name << " [" << r.perspective << (r.tremmed ? ", tremmed" : "") << (r.noise ? ", noise" : "")
                    << "] " << (int)r.pipes.size() << " pipes, folder " << r.folder.getFullPathName() << "\n";
            const auto root = HauptwerkOdf::hauptwerkRootFor(odfFile);
            const auto plan = odf.plan(root);
            report << "root " << root.getFullPathName() << "\nplan: " << (plan.error.isNotEmpty() ? "ERROR " + plan.error : juce::String("ok")) << "\n";
            for (size_t i = 0; i < plan.divisions.size(); ++i)
            {
                const auto& d = plan.divisions[i];
                report << "  [" << (int)i << "] " << d.name << " keys " << d.lowNote << "-" << d.highNote << ", " << (int)d.stops.size() << " stops\n";
                for (const auto& t : d.trees) report << "      tree " << t.getFullPathName() << "\n";
                for (size_t s = 0; s < d.stops.size() && s < 4; ++s)
                {
                    report << "      stop " << d.stops[s].name << " ->";
                    for (const auto& f : d.stops[s].folders) report << " " << f.getFullPathName();
                    report << "\n";
                }
            }
            for (const auto& p : plan.perspectivePatterns) report << "  perspective pattern: " << p << "\n";
            report << "  tremmed ranks skipped " << plan.skippedTremmedRanks << ", noise ranks " << plan.skippedNoiseRanks << ", stops without samples " << plan.stopsWithoutSamples << "\n";
            for (const auto& n : plan.notes) report << "  note: " << n << "\n";
            report << "  configuration: " << plan.configurationFile.getFullPathName() << "\n";
            juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory).getChildFile("OrganSampler").getChildFile("hauptwerk.txt").replaceWithText(report);
            std::fputs(report.toRawUTF8(), stdout);
            quit();
            return;
        }
        if (commandLine.contains("--selftest"))
        {
            // Offline engine stress test: OrganSampler.exe --selftest "<sample folder>"
            // Several folders may be given separated by ';' (loaded as divisions 0, 1, ...).
            juce::String path = commandLine.fromFirstOccurrenceOf("--selftest", false, false).trim().unquoted().trim();
            std::vector<juce::File> folders;
            bool allFound = true;
            for (const auto& part : juce::StringArray::fromTokens(path, ";", "\""))
            {
                const juce::File folder(part.trim().unquoted());
                if (folder.isDirectory()) folders.push_back(folder);
                else if (part.trim().isNotEmpty()) allFound = false;
            }
            // The user's lists when saved (and not empty), otherwise the defaults.
            std::vector<juce::String> perspectives, ignored;
            const auto settings = juce::JSON::parse(juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                .getChildFile("OrganSampler").getChildFile("settings.json").loadFileAsString());
            if (auto* array = settings.getProperty("perspectivePatterns", juce::var()).getArray())
                for (const auto& item : *array) if (item.toString().trim().isNotEmpty()) perspectives.push_back(item.toString());
            if (auto* array = settings.getProperty("ignoredPatterns", juce::var()).getArray())
                for (const auto& item : *array) if (item.toString().trim().isNotEmpty()) ignored.push_back(item.toString());
            if (perspectives.empty()) perspectives = SamplerEngine::defaultPerspectivePatterns();
            if (ignored.empty()) ignored = SamplerEngine::defaultIgnoredPatterns();
            const auto report = allFound && !folders.empty() ? EngineSelfTest::run(folders, perspectives, ignored)
                : juce::String("Self test: folder not found: " + path + "\n");
            juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                .getChildFile("OrganSampler").getChildFile("selftest.txt").replaceWithText(report);
            std::fputs(report.toRawUTF8(), stdout);
            quit();
            return;
        }
        // The interface size before any window exists: the saved percentage, or
        // the screen's estimate at the first start (MainComponent reads the same
        // key and saves the estimate, see UIScale.h).
        {
            const auto settings = juce::JSON::parse(juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
                .getChildFile("OrganSampler").getChildFile("settings.json").loadFileAsString());
            int percent = UIScale::percentFromSettings(settings);
            if (percent <= 0) percent = UIScale::estimatePercent();
            UIScale::set(percent);
            CrashHandler::note("Interface size " + juce::String(UIScale::percent()) + " % (screen estimate "
                + juce::String(UIScale::estimatePercent()) + " %)");
        }
        lookAndFeel = std::make_unique<UIScale::LookAndFeel>();
        juce::LookAndFeel::setDefaultLookAndFeel(lookAndFeel.get());
        // A stall of the message thread (a frozen window) goes to the activity
        // log with the thread's stack, so it can be traced afterwards.
        stallWatchdog = std::make_unique<CrashHandler::StallWatchdog>();
        mainWindow.reset(new MainWindow(getApplicationName()));
    }

    void shutdown() override
    {
        // The windows disappear first: closing the audio devices, saving the state
        // and winding the engine down take a moment, which must not look like a
        // frozen program.
        if (mainWindow != nullptr) mainWindow->setVisible(false);
        for (int i = juce::Desktop::getInstance().getNumComponents(); --i >= 0;)
            if (auto* window = juce::Desktop::getInstance().getComponent(i)) window->setVisible(false);
        mainWindow = nullptr; // (deletes our window)
        stallWatchdog.reset();   // after the window: a slow shutdown is logged like any stall
        juce::LookAndFeel::setDefaultLookAndFeel(nullptr);
        lookAndFeel.reset();
        juce::Logger::setCurrentLogger(nullptr);
        juceLog.reset();
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        // This is called when the app is being asked to quit: you can ignore this
        // request and let the app carry on running, or call quit() to allow the app to close.
        quit();
    }

    void anotherInstanceStarted(const juce::String& commandLine) override
    {
        // A second OrganSampler was started (and refused, see moreThanOneInstanceAllowed):
        // bring this one to the front and say so.
        if (commandLine.contains("--selftest") || commandLine.contains("--asiotest")) return;
        if (mainWindow != nullptr)
        {
            mainWindow->setMinimised(false);
            mainWindow->toFront(true);
        }
        juce::AlertWindow::showMessageBoxAsync(juce::AlertWindow::WarningIcon, "OrganSampler is already running",
            "Another OrganSampler was started and closed again: only one instance can run at a time "
            "(a second one would open the same audio devices). This is the running instance.");
    }

    //==============================================================================
    /*
        This class implements the desktop window that contains an instance of
        our MainComponent class.
    */
    class MainWindow    : public juce::DocumentWindow
    {
    public:
        MainWindow (juce::String name)
            : DocumentWindow (name,
                              juce::Desktop::getInstance().getDefaultLookAndFeel()
                                                          .findColour (juce::ResizableWindow::backgroundColourId),
                              DocumentWindow::allButtons)
        {
            setUsingNativeTitleBar (true);
            // The program's icon on the window (title bar, task bar, dock): the
            // 1024 px icon embedded at build time, scaled by the window system.
            setIcon (juce::ImageCache::getFromMemory (BinaryData::AppIcon_png, BinaryData::AppIcon_pngSize));
            setContentOwned (new MainComponent(), true);

           #if JUCE_IOS || JUCE_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
            // Where the user left it last time (or centred), with the title bar on
            // the screen: placed after the window exists, so its frame size is known.
            if (auto* main = dynamic_cast<MainComponent*>(getContentComponent())) main->placeMainWindow(*this);
        }

        // The interface size (UIScale.h): the window's content is drawn and laid out at this scale.
        float getDesktopScaleFactor() const override { return UIScale::factor(); }

        void closeButtonPressed() override
        {
            // Let the main component ask about unsaved configuration changes first.
            if (auto* main = dynamic_cast<MainComponent*>(getContentComponent()))
            {
                main->requestQuit([] { JUCEApplication::getInstance()->systemRequestedQuit(); });
                return;
            }
            JUCEApplication::getInstance()->systemRequestedQuit();
        }

        /* Note: Be careful if you override any DocumentWindow methods - the base
           class uses a lot of them, so by overriding you might break its functionality.
           It's best to do all your work in your content component instead, but if
           you really have to override any DocumentWindow methods, make sure your
           subclass also calls the superclass's method.
        */

    private:
        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<UIScale::LookAndFeel> lookAndFeel;   // the default look and feel: alert boxes at the interface size
    std::unique_ptr<CrashHandler::StallWatchdog> stallWatchdog;
    std::unique_ptr<MainWindow> mainWindow;
    std::unique_ptr<juce::FileLogger> juceLog;
};

//==============================================================================
// This macro generates the main() routine that launches the app.
START_JUCE_APPLICATION (OrganSamplerApplication)
