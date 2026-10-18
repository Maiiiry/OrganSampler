#pragma once

#include <JuceHeader.h>
#include <map>
#include <vector>
#include <functional>

//==============================================================================
// Hauptwerk organ definition files (ODF, "<name>.Organ_Hauptwerk_xml"): read
// for what OrganSampler needs and nothing more - the divisions with their
// keyboards, the stops with their ranks, the ranks' sample files (which give
// the folder trees, the stop folders and the perspectives) and the per-pipe
// level adjustments (the voicing). Couplers, switches, displays, tremulants,
// wind, combinations and everything else of the ODF is ignored: in
// OrganSampler the folder structure decides the organ.
//
// An ODF is an XML of tables: <ObjectList ObjectType="Stop"> holding one row
// per record, each a flat list of <Field>value</Field> pairs. A compacted ODF
// (the usual form shipped with sample sets) names the rows <o> and the fields
// with letters (<a>, <b>, ... <a1>); the letters below are the ones Hauptwerk
// 4-8 write (the same table the OdfEdit tool and the Masterpiece project use).
// Both forms are read; a field at its default is simply absent.
//
// The sample files: Sample records name a file relative to an installation
// package ("pipe/UW/UW_Quintadena8/03-rearMk4-UW_Quintadena8/036-C.wav" in
// package 3136); the packages live in OrganInstallationPackages/<id as six
// digits> beside the OrganDefinitions folder - the Hauptwerk standard layout.
// Encrypted samples (.hbw/.hbx) cannot be played and make the import refuse.
// Tremmed samples (alternate "tremmed" ranks, the "trem" folders, further
// pipe layers) are left out: OrganSampler has its own tremulant.
struct HauptwerkOdf
{
    struct Pipe
    {
        int id = 0, rankId = 0, note = 60;
        float levelDb = 0.0f;           // Pipe_SoundEngine01_Layer AmpLvl_LevelAdjustDecibels of the pipe's first layer
        int packageId = 0;
        juce::String file;              // the attack sample, relative to the package ("pipe/UW/.../036-C.wav")
    };
    struct Rank
    {
        int id = 0;
        juce::String name;
        bool tremmed = false;           // an alternate (tremulant) rank, or named "(tremmed)"
        bool noise = false;             // key/stop action noises, blower, tremulant noise: no stop of ours
        juce::String perspective;       // "Direct", "Diffuse", "Rear", "Front" ... from the name; empty when the name says nothing
        std::vector<Pipe> pipes;        // the first layer's attack sample of every pipe (tremmed layers and "trem" folders excluded)
        juce::File folder;              // the folder most of the pipes lie in (absolute, once the packages are resolved)
        int packageId = 0;              // the installation package that folder lies in
        juce::String folderRelative;    // the folder relative to its package ("pipe/PE/PEd_Princip16/01-dirMk2H-PEd_Princip16")
    };
    struct Stop
    {
        int id = 0, divisionId = 0;
        juce::String name;
        std::vector<int> rankIds;       // the ranks of its StopRank rows (alternate ranks excluded), in file order
        // How each StopRank row maps the keys to the rank: the first key mapped,
        // how many keys, and the semitones from the key to the pipe (12 = a 4'
        // stop playing an 8' rank an octave up - an extension; 0 = the rank's own).
        struct RankUse { int rankId = 0, firstNote = 0, count = 0, increment = 0; };
        std::vector<RankUse> rankUses;
    };
    struct Division { int id = 0; juce::String name; };
    struct Keyboard { int id = 0, divisionId = 0, lowNote = 0, highNote = 0; juce::String name; };

    juce::String organName, builder, location, supplier;
    int mainPackageId = 0;
    std::vector<int> packageIds;                    // RequiredInstallationPackage, the main one included
    std::map<int, Division> divisions;
    std::vector<Keyboard> keyboards;                // in file order
    std::map<int, Stop> stops;
    std::map<int, Rank> ranks;
    int sampleCount = 0, encryptedSamples = 0;      // of the samples the pipes use
    bool compacted = false;
    juce::String error;                             // set when the file is not an ODF or holds no playable stop

    // The file looks like a Hauptwerk ODF: by its name, or by "<Hauptwerk" near the start.
    static bool looksLikeOdf(const juce::File& file);
    // Parses the file (seconds for a large one; progress 0..1, return false to cancel).
    static HauptwerkOdf parse(const juce::File& file, const std::function<bool(double)>& progress = nullptr);

    // The Hauptwerk root (the folder holding OrganDefinitions and
    // OrganInstallationPackages) for an ODF at this path, or an empty file.
    static juce::File hauptwerkRootFor(const juce::File& odf);
    static juce::File packageFolder(const juce::File& hauptwerkRoot, int packageId);
    // The perspective a rank name tells ("Front A (direct): HW. Octave 2" -> "Direct",
    // "Rear: UW. Nasard 3" -> "Rear"); empty when none.
    static juce::String perspectiveOfRankName(const juce::String& name);

    //==========================================================================
    // What OrganSampler makes of it: divisions in keyboard order (the pedal
    // first), each with its folder trees, the stops with their stop folders,
    // the perspective patterns, and the voicing per pipe.
    struct PlanStop
    {
        int stopId = 0;                              // the ODF stop
        juce::String name;
        std::vector<juce::File> folders;             // the stop folder(s): one per installation package the stop's ranks lie in
        std::vector<int> rankIds;
        // An extension or a transmission: the stop plays the ranks of another
        // stop (its owner, by ODF stop id) shifted by 'shift' semitones (an ODF
        // stop whose StopRank rows all borrow one other stop's ranks with the
        // same transposition). It has no folders of its own; OrganSampler makes
        // a transmission of the owner into this division with that shift.
        int extensionOfStopId = 0;
        int shift = 0;
    };
    struct PlanDivision
    {
        juce::String name;                           // the ODF's division name
        int divisionId = 0, lowNote = 36, highNote = 96;
        std::vector<juce::File> trees;               // the folders holding the stop folders, the main one first
        std::vector<PlanStop> stops;
    };
    struct Plan
    {
        juce::String organName;
        std::vector<PlanDivision> divisions;         // [0] = pedal (when the organ has one), then the manuals
        std::vector<juce::String> perspectivePatterns;   // "Direct;01-dirMk2H-" ... (empty: a single perspective)
        std::vector<juce::File> allTrees;
        juce::File libraryRoot;                      // OrganInstallationPackages
        juce::File configurationFile;                // where the configuration goes: the main package folder
        int stopsWithoutSamples = 0, skippedTremmedRanks = 0, skippedNoiseRanks = 0, extensions = 0;
        juce::StringArray notes;                     // what was left out and why (for the log and the message)
        juce::String error;
    };
    Plan plan(const juce::File& hauptwerkRoot) const;

    // The voicing of a loaded stop: for every note the mean of the level
    // adjustments of the pipes the given rank folders hold (one per perspective).
    // Returns the notes found and their dB; the caller takes the mean as the
    // stop's volume and the rest as per-key voicing.
    std::map<int, float> levelsForFolders(const std::vector<juce::File>& rankFolders) const;
};
