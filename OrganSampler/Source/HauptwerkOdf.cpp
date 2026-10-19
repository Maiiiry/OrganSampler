#include "HauptwerkOdf.h"
#include "CrashHandler.h"

#include <algorithm>
#include <cstring>
#include <set>

namespace
{
    //==========================================================================
    // A row of an ODF table: the fields in file order. Looked up by the long
    // name or the compact letter (a compacted file writes only the letters).
    struct Row
    {
        std::vector<std::pair<juce::String, juce::String>> fields;
        const juce::String* find(const char* longName, const char* letter) const
        {
            for (const auto& f : fields) if (f.first == longName || (letter != nullptr && f.first == letter)) return &f.second;
            return nullptr;
        }
        juce::String text(const char* longName, const char* letter, const juce::String& fallback = {}) const
        {
            const auto* v = find(longName, letter);
            return v != nullptr ? *v : fallback;
        }
        int integer(const char* longName, const char* letter, int fallback = 0) const
        {
            const auto* v = find(longName, letter);
            if (v == nullptr || v->isEmpty()) return fallback;
            return (int)std::lround(v->getDoubleValue());   // compacted files write "1.27e+2"
        }
        float number(const char* longName, const char* letter, float fallback = 0.0f) const
        {
            const auto* v = find(longName, letter);
            return v == nullptr || v->isEmpty() ? fallback : (float)v->getDoubleValue();
        }
    };

    juce::String decodeEntities(const juce::String& text)
    {
        if (!text.containsChar('&')) return text;
        juce::String out;
        out.preallocateBytes((size_t)text.length());
        for (int i = 0; i < text.length(); ++i)
        {
            if (text[i] != '&') { out << text[i]; continue; }
            const int semicolon = text.indexOfChar(i, ';');
            if (semicolon < 0 || semicolon - i > 10) { out << text[i]; continue; }
            const auto entity = text.substring(i + 1, semicolon);
            if (entity == "amp") out << '&';
            else if (entity == "lt") out << '<';
            else if (entity == "gt") out << '>';
            else if (entity == "quot") out << '"';
            else if (entity == "apos") out << '\'';
            else if (entity.startsWithChar('#'))
            {
                const int code = entity[1] == 'x' || entity[1] == 'X' ? entity.substring(2).getHexValue32() : entity.substring(1).getIntValue();
                if (code > 0) out << juce::String::charToString((juce::juce_wchar)code);
            }
            else { out << text[i]; continue; }
            i = semicolon;
        }
        return out;
    }

    // The tables read, with the compact row tag "o" or the table's own name.
    // Every direct child element of an ObjectList is a row; every child of a
    // row is a field. The XML is scanned by hand: a 150 MB ODF is nothing to
    // put through a DOM.
    class Scanner
    {
    public:
        Scanner(const char* data, size_t size) : begin(data), end(data + size), p(data) {}

        // Advances to the next ObjectList and returns its ObjectType, or an empty string at the end.
        juce::String nextTable()
        {
            while (true)
            {
                const char* tag = findTag("<ObjectList");
                if (tag == nullptr) { p = end; return {}; }
                p = tag + 11;
                const char* close = (const char*)memchr(p, '>', (size_t)(end - p));
                if (close == nullptr) { p = end; return {}; }
                const juce::String head(p, (size_t)(close - p));
                p = close + 1;
                const int at = head.indexOf("ObjectType=\"");
                if (at < 0) continue;
                const auto rest = head.substring(at + 12);
                return rest.upToFirstOccurrenceOf("\"", false, false);
            }
        }
        void skipTable() { const char* e = findTag("</ObjectList>"); p = e != nullptr ? e + 13 : end; }
        double progress() const { return end > begin ? (double)(p - begin) / (double)(end - begin) : 1.0; }

        // The next row of the current table into 'row'; false at the table's end.
        bool nextRow(Row& row)
        {
            row.fields.clear();
            while (p < end)
            {
                const char* lt = (const char*)memchr(p, '<', (size_t)(end - p));
                if (lt == nullptr) { p = end; return false; }
                p = lt + 1;
                if (p >= end) return false;
                if (*p == '/')   // </ObjectList> or stray end tags
                {
                    if (startsWith(p + 1, "ObjectList")) { skipPastGreater(); return false; }
                    skipPastGreater();
                    continue;
                }
                if (*p == '!' || *p == '?') { skipPastGreater(); continue; }   // comments, declarations
                // A row element: read its name, then its fields up to the matching end tag.
                const char* nameStart = p;
                while (p < end && *p != '>' && *p != ' ' && *p != '/' && *p != '\t' && *p != '\r' && *p != '\n') ++p;
                const juce::String rowName(nameStart, (size_t)(p - nameStart));
                bool selfClosing = false;
                while (p < end && *p != '>') { if (*p == '/') selfClosing = true; ++p; }
                if (p < end) ++p;
                if (selfClosing) return true;   // an empty row
                readFields(row, rowName);
                return true;
            }
            return false;
        }

    private:
        const char* begin; const char* end; const char* p;

        bool startsWith(const char* at, const char* what) const
        {
            const size_t n = strlen(what);
            return (size_t)(end - at) >= n && memcmp(at, what, n) == 0;
        }
        void skipPastGreater()
        {
            const char* gt = (const char*)memchr(p, '>', (size_t)(end - p));
            p = gt != nullptr ? gt + 1 : end;
        }
        const char* findTag(const char* what) const
        {
            const size_t n = strlen(what);
            const char* s = p;
            while (s < end)
            {
                const char* lt = (const char*)memchr(s, '<', (size_t)(end - s));
                if (lt == nullptr) return nullptr;
                if ((size_t)(end - lt) >= n && memcmp(lt, what, n) == 0) return lt;
                s = lt + 1;
            }
            return nullptr;
        }
        void readFields(Row& row, const juce::String& rowName)
        {
            while (p < end)
            {
                const char* lt = (const char*)memchr(p, '<', (size_t)(end - p));
                if (lt == nullptr) { p = end; return; }
                p = lt + 1;
                if (p >= end) return;
                if (*p == '/')
                {
                    // The row's end tag (or any end tag at this level): the row is complete.
                    ++p;
                    const char* nameStart = p;
                    while (p < end && *p != '>') ++p;
                    const juce::String endName(nameStart, (size_t)(p - nameStart));
                    if (p < end) ++p;
                    if (endName.trim() == rowName || endName.startsWith("ObjectList")) return;
                    continue;
                }
                if (*p == '!' || *p == '?') { skipPastGreater(); continue; }
                const char* nameStart = p;
                while (p < end && *p != '>' && *p != ' ' && *p != '/' && *p != '\t') ++p;
                const juce::String fieldName(nameStart, (size_t)(p - nameStart));
                bool selfClosing = false;
                while (p < end && *p != '>') { if (*p == '/') selfClosing = true; ++p; }
                if (p < end) ++p;
                if (selfClosing) { row.fields.emplace_back(fieldName, juce::String()); continue; }
                // The text up to the field's end tag; CDATA taken as it is.
                const char* textStart = p;
                juce::String value;
                if (startsWith(p, "<![CDATA["))
                {
                    const char* cdataStart = p + 9;
                    const char* cdataEnd = cdataStart;
                    while (cdataEnd + 2 < end && !(cdataEnd[0] == ']' && cdataEnd[1] == ']' && cdataEnd[2] == '>')) ++cdataEnd;
                    value = juce::String::fromUTF8(cdataStart, (int)(cdataEnd - cdataStart));
                    p = juce::jmin(end, cdataEnd + 3);
                    const char* gt = (const char*)memchr(p, '>', (size_t)(end - p));
                    p = gt != nullptr ? gt + 1 : end;
                }
                else
                {
                    const char* textEnd = (const char*)memchr(p, '<', (size_t)(end - p));
                    if (textEnd == nullptr) { p = end; return; }
                    value = decodeEntities(juce::String::fromUTF8(textStart, (int)(textEnd - textStart)).trim());
                    p = textEnd;
                    skipPastGreater();   // the field's end tag
                }
                row.fields.emplace_back(fieldName, value);
            }
        }
    };

    bool containsFolderNamed(const juce::String& relativePath, const char* name)
    {
        for (const auto& part : juce::StringArray::fromTokens(relativePath.replaceCharacter('\\', '/'), "/", ""))
            if (part.equalsIgnoreCase(name)) return true;
        return false;
    }

    juce::File commonAncestor(juce::File a, const juce::File& b)
    {
        while (a != juce::File() && b != a && !b.isAChildOf(a))
        {
            const auto parent = a.getParentDirectory();
            if (parent == a) return {};
            a = parent;
        }
        return a;
    }

}

//==============================================================================
bool HauptwerkOdf::looksLikeOdf(const juce::File& file)
{
    if (!file.existsAsFile()) return false;
    if (file.getFileName().endsWithIgnoreCase(".Organ_Hauptwerk_xml")) return true;
    if (!file.hasFileExtension("xml")) return false;
    juce::FileInputStream in(file);
    if (!in.openedOk()) return false;
    char head[4096]{};
    const int n = in.read(head, (int)sizeof(head) - 1);
    return n > 0 && juce::String::fromUTF8(head, n).contains("<Hauptwerk");
}

juce::File HauptwerkOdf::hauptwerkRootFor(const juce::File& odf)
{
    auto folder = odf.getParentDirectory();
    if (folder.getFileName().equalsIgnoreCase("OrganDefinitions") && folder.getParentDirectory().getChildFile("OrganInstallationPackages").isDirectory())
        return folder.getParentDirectory();
    for (int up = 0; up < 5 && folder != juce::File(); ++up)
    {
        if (folder.getChildFile("OrganInstallationPackages").isDirectory()) return folder;
        const auto parent = folder.getParentDirectory();
        if (parent == folder) break;
        folder = parent;
    }
    return {};
}

juce::File HauptwerkOdf::packageFolder(const juce::File& hauptwerkRoot, int packageId)
{
    const auto packages = hauptwerkRoot.getChildFile("OrganInstallationPackages");
    const auto padded = packages.getChildFile(juce::String(packageId).paddedLeft('0', 6));
    if (padded.isDirectory()) return padded;
    const auto bare = packages.getChildFile(juce::String(packageId));
    return bare.isDirectory() ? bare : padded;
}

juce::String HauptwerkOdf::perspectiveOfRankName(const juce::String& name)
{
    // "Front A (direct): HW. Octave 2" - the part before the colon names the
    // perspective; its parenthesis is the most specific word, then the words.
    juce::String head = name.contains(":") ? name.upToFirstOccurrenceOf(":", false, false) : name;
    juce::String candidate;
    if (head.contains("(") && head.contains(")"))
        candidate = head.fromFirstOccurrenceOf("(", false, false).upToFirstOccurrenceOf(")", false, false);
    auto labelOf = [](const juce::String& text) -> juce::String
    {
        const auto lower = text.toLowerCase();
        struct Word { const char* word; const char* label; };
        static const Word words[]{
            { "direct", "Direct" }, { "dry", "Direct" }, { "diffuse", "Diffuse" }, { "ambient", "Diffuse" }, { "ambience", "Diffuse" },
            { "surround", "Diffuse" }, { "wet", "Diffuse" }, { "room", "Diffuse" }, { "hall", "Diffuse" }, { "rear", "Rear" }, { "back", "Rear" },
            { "distant", "Rear" }, { "far", "Rear" }, { "hinten", "Rear" }, { "lointain", "Rear" }, { "front", "Front" }, { "vorne", "Front" },
            { "close", "Close" }, { "near", "Close" }, { "nah", "Close" }, { "proche", "Close" }, { "middle", "Middle" }, { "mid", "Middle" },
            { "center", "Middle" }, { "centre", "Middle" }, { "mitte", "Middle" }, { "gallery", "Gallery" }, { "nave", "Nave" } };
        for (const auto& w : words)
        {
            const int at = lower.indexOf(w.word);
            if (at < 0) continue;
            // A whole word only ("mid" must not hit "middle"'s neighbours or "humid").
            const bool startOk = at == 0 || !juce::CharacterFunctions::isLetter(lower[at - 1]);
            const int after = at + (int)strlen(w.word);
            const bool endOk = after >= lower.length() || !juce::CharacterFunctions::isLetter(lower[after]);
            if (startOk && endOk) return w.label;
        }
        return {};
    };
    auto label = labelOf(candidate);
    if (label.isEmpty()) label = labelOf(head);
    return label;
}

HauptwerkOdf HauptwerkOdf::parse(const juce::File& file, const std::function<bool(double)>& progress)
{
    HauptwerkOdf odf;
    juce::MemoryBlock data;
    if (!file.loadFileAsData(data) || data.getSize() < 16) { odf.error = "The file could not be read."; return odf; }
    const char* text = (const char*)data.getData();
    if (juce::String::fromUTF8(text, (int)juce::jmin((size_t)4096, data.getSize())).indexOf("<Hauptwerk") < 0)
    {
        odf.error = "The file is not a Hauptwerk organ definition (no <Hauptwerk> element at its start).";
        return odf;
    }
    Scanner scanner(text, data.getSize());
    Row row;
    // Tables that refer to each other, in the order the file has them (any order).
    struct SampleRow { int packageId = 0; juce::String file; };
    std::map<int, SampleRow> samples;                 // SampleID -> file
    std::map<int, int> attackSampleOfLayer;           // LayerID -> SampleID (the first attack sample)
    struct LayerRow { int pipeId = 0; float levelDb = 0.0f; };
    std::map<int, LayerRow> layers;                   // LayerID -> pipe, level (file order: the first layer of a pipe comes first)
    std::vector<int> layerOrder;
    struct PipeRow { int rankId = 0, note = 60; };
    std::map<int, PipeRow> pipes;                     // PipeID
    std::vector<int> pipeOrder;
    struct StopRankRow { int stopId = 0, rankId = 0, alternateRankId = 0, firstNote = 0, count = 0, increment = 0; };
    std::vector<StopRankRow> stopRanks;
    std::map<int, std::vector<int>> keyboardNotes;    // KeyboardID -> notes of its KeyboardKey rows
    // The way from a key to a division: KeyboardKey.SwitchID -> SwitchLinkage
    // rows (source -> destination, possibly in several hops and under a
    // condition switch: a ventil, "Cp: Anches Pedal") -> DivisionInput.SwitchID.
    std::map<int, std::vector<int>> keyboardKeySwitches;             // KeyboardID -> the switches of its keys
    std::map<int, int> divisionOfInputSwitch;                        // DivisionInput switch -> DivisionID
    std::map<int, std::vector<std::pair<int, int>>> switchLinks;     // source switch -> (destination switch, condition switch or 0)
    // The tremulants: Tremulant (speed range) > TremulantWaveform (waveform ->
    // tremulant) > TremulantWaveformPipe (pipe -> waveform) - which pipes, so
    // which ranks, stops and divisions a tremulant works on.
    std::map<int, int> tremulantOfWaveform;                          // TremulantWaveformID -> TremulantID
    std::vector<std::pair<int, int>> tremulantPipes;                 // (PipeID, TremulantWaveformID)
    // The combinations: the crescendo is one per stage ("Cresc.Comb.12"); an
    // element = a switch the combination sets, on (Y) or off.
    std::map<int, juce::String> combinationNames;                    // CombinationID -> name
    struct ElementRow { int combinationId = 0, switchId = 0; bool on = false; };
    std::vector<ElementRow> combinationElements;
    int rows = 0;
    while (true)
    {
        const auto table = scanner.nextTable();
        if (table.isEmpty()) break;
        if (progress != nullptr && !progress(scanner.progress())) { odf.error = "Cancelled."; return odf; }
        const auto lower = table.toLowerCase();
        if (lower == "_general")
        {
            while (scanner.nextRow(row))
            {
                odf.organName = row.text("Identification_Name", nullptr);
                odf.builder = row.text("OrganInfo_Builder", nullptr);
                odf.location = row.text("OrganInfo_Location", nullptr);
                odf.supplier = row.text("Control_OrganDefinitionSupplierName", nullptr);
                odf.mainPackageId = row.integer("OrganInfo_InstallationPackageID", nullptr, 0);
                odf.compacted = row.text("Control_FileIsCompacted_AlwaysSetThisToNIfEditingManually", nullptr).equalsIgnoreCase("Y");
            }
        }
        else if (lower == "requiredinstallationpackage")
        {
            while (scanner.nextRow(row))
            {
                const int id = row.integer("InstallationPackageID", "a");
                if (id > 1 && std::find(odf.packageIds.begin(), odf.packageIds.end(), id) == odf.packageIds.end()) odf.packageIds.push_back(id);
            }
        }
        else if (lower == "division")
        {
            while (scanner.nextRow(row))
            {
                Division d; d.id = row.integer("DivisionID", "a"); d.name = row.text("Name", "b");
                if (d.id > 0) odf.divisions[d.id] = d;
            }
        }
        else if (lower == "keyboard")
        {
            while (scanner.nextRow(row))
            {
                Keyboard k;
                k.id = row.integer("KeyboardID", "a");
                k.name = row.text("Name", "b");
                k.divisionId = row.integer("Hint_PrimaryAssociatedDivisionID", "x");
                const int keys = row.integer("KeyGen_NumberOfKeys", "h", 0), first = row.integer("KeyGen_MIDINoteNumberOfFirstKey", "i", 0);
                if (keys > 0 && first > 0) { k.lowNote = first; k.highNote = first + keys - 1; }
                if (k.id > 0) odf.keyboards.push_back(k);
            }
        }
        else if (lower == "keyboardkey")
        {
            while (scanner.nextRow(row))
            {
                const int keyboard = row.integer("KeyboardID", "a"), note = row.integer("NormalMIDINoteNumber", "c", -1);
                if (keyboard > 0 && note >= 0) keyboardNotes[keyboard].push_back(note);
                const int keySwitch = row.integer("SwitchID", "b");
                if (keyboard > 0 && keySwitch > 0) keyboardKeySwitches[keyboard].push_back(keySwitch);
            }
        }
        else if (lower == "divisioninput")
        {
            while (scanner.nextRow(row))
            {
                const int division = row.integer("DivisionID", "a"), inputSwitch = row.integer("SwitchID", "b");
                if (division > 0 && inputSwitch > 0) divisionOfInputSwitch[inputSwitch] = division;
            }
        }
        else if (lower == "switchlinkage")
        {
            while (scanner.nextRow(row))
            {
                const int source = row.integer("SourceSwitchID", "a"), dest = row.integer("DestSwitchID", "b"), condition = row.integer("ConditionSwitchID", "c");
                if (source > 0 && dest > 0) switchLinks[source].push_back({ dest, condition });
            }
        }
        else if (lower == "tremulant")
        {
            while (scanner.nextRow(row))
            {
                Tremulant t;
                t.id = row.integer("TremulantID", "a");
                t.name = row.text("Name", "b");
                // The speed wanders between a minimum and a maximum (Hz): the middle is the rate.
                const float lowHz = row.number("ModRateHz_Min", "d", 0.0f), highHz = row.number("ModRateHz_Max", "e", 0.0f);
                t.rateHz = lowHz > 0.0f && highHz > 0.0f ? 0.5f * (lowHz + highHz) : juce::jmax(lowHz, highHz);
                if (t.id > 0) odf.tremulants.push_back(t);
            }
        }
        else if (lower == "tremulantwaveform")
        {
            while (scanner.nextRow(row))
            {
                const int waveform = row.integer("TremulantWaveformID", "a"), tremulant = row.integer("TremulantID", "c");
                if (waveform > 0 && tremulant > 0) tremulantOfWaveform[waveform] = tremulant;
            }
        }
        else if (lower == "tremulantwaveformpipe")
        {
            while (scanner.nextRow(row))
            {
                const int pipe = row.integer("PipeID", "a"), waveform = row.integer("TremulantWaveformID", "b");
                if (pipe > 0 && waveform > 0) tremulantPipes.push_back({ pipe, waveform });
            }
        }
        else if (lower == "combination")
        {
            while (scanner.nextRow(row))
            {
                const int id = row.integer("CombinationID", "a");
                if (id > 0) combinationNames[id] = row.text("Name", "b");
            }
        }
        else if (lower == "combinationelement")
        {
            while (scanner.nextRow(row))
            {
                ElementRow e;
                e.combinationId = row.integer("CombinationID", "b");
                e.switchId = row.integer("ControlledSwitchID", "c");
                e.on = row.text("InitialStoredStateIsOn", "e").equalsIgnoreCase("Y");
                if (e.combinationId > 0 && e.switchId > 0) combinationElements.push_back(e);
            }
        }
        else if (lower == "stop")
        {
            while (scanner.nextRow(row))
            {
                Stop s; s.id = row.integer("StopID", "a"); s.name = row.text("Name", "b"); s.divisionId = row.integer("DivisionID", "c");
                s.switchId = row.integer("ControllingSwitchID", "d");
                if (s.id > 0) odf.stops[s.id] = s;
            }
        }
        else if (lower == "stoprank")
        {
            while (scanner.nextRow(row))
            {
                StopRankRow r; r.stopId = row.integer("StopID", "a"); r.rankId = row.integer("RankID", "d"); r.alternateRankId = row.integer("AlternateRankID", "p");
                // The key mapping of the row: the first key, how many keys, the
                // semitones from the key to the pipe (an extension borrows another
                // stop's rank with 12 or -12 here).
                r.firstNote = row.integer("MIDINoteNumOfFirstMappedDivisionInputNode", "h");
                r.count = row.integer("NumberOfMappedDivisionInputNodes", "i");
                r.increment = row.integer("MIDINoteNumIncrementFromDivisionToRank", "j");
                if (r.stopId > 0 && r.rankId > 0) stopRanks.push_back(r);
            }
        }
        else if (lower == "rank")
        {
            while (scanner.nextRow(row))
            {
                Rank r; r.id = row.integer("RankID", "a"); r.name = row.text("Name", "b");
                if (r.id > 0) odf.ranks[r.id] = r;
            }
        }
        else if (lower == "pipe_soundengine01")
        {
            while (scanner.nextRow(row))
            {
                const int id = row.integer("PipeID", "a");
                PipeRow pr; pr.rankId = row.integer("RankID", "b"); pr.note = row.integer("NormalMIDINoteNumber", "d", 60);
                if (id > 0 && pipes.find(id) == pipes.end()) { pipes[id] = pr; pipeOrder.push_back(id); }
            }
        }
        else if (lower == "pipe_soundengine01_layer")
        {
            while (scanner.nextRow(row))
            {
                const int id = row.integer("LayerID", "a");
                LayerRow lr; lr.pipeId = row.integer("PipeID", "b"); lr.levelDb = row.number("AmpLvl_LevelAdjustDecibels", "h", 0.0f);
                if (id > 0 && layers.find(id) == layers.end()) { layers[id] = lr; layerOrder.push_back(id); }
            }
        }
        else if (lower == "pipe_soundengine01_attacksample")
        {
            while (scanner.nextRow(row))
            {
                const int layer = row.integer("LayerID", "b"), sample = row.integer("SampleID", "c");
                if (layer > 0 && sample > 0 && attackSampleOfLayer.find(layer) == attackSampleOfLayer.end()) attackSampleOfLayer[layer] = sample;
            }
        }
        else if (lower == "sample")
        {
            while (scanner.nextRow(row))
            {
                const int id = row.integer("SampleID", "a");
                SampleRow sr; sr.packageId = row.integer("InstallationPackageID", "b"); sr.file = row.text("SampleFilename", "c").replaceCharacter('\\', '/');
                if (id > 0) samples[id] = sr;
                if ((++rows & 4095) == 0 && progress != nullptr && !progress(scanner.progress())) { odf.error = "Cancelled."; return odf; }
            }
        }
        else scanner.skipTable();
    }
    if (odf.mainPackageId > 1 && std::find(odf.packageIds.begin(), odf.packageIds.end(), odf.mainPackageId) == odf.packageIds.end())
        odf.packageIds.insert(odf.packageIds.begin(), odf.mainPackageId);
    if (odf.stops.empty() || pipes.empty() || samples.empty())
    {
        odf.error = "The organ definition holds no stops with sample pipes.";
        return odf;
    }

    // The keyboards' compasses from their keys (the compact form leaves the
    // KeyGen fields out); the pedal 36-67, a manual 36-96 as the last resort.
    for (auto& k : odf.keyboards)
    {
        const auto notes = keyboardNotes.find(k.id);
        if (notes != keyboardNotes.end() && !notes->second.empty())
        {
            k.lowNote = *std::min_element(notes->second.begin(), notes->second.end());
            k.highNote = *std::max_element(notes->second.begin(), notes->second.end());
        }
        if (k.lowNote <= 0 || k.highNote < k.lowNote) { k.lowNote = 36; k.highNote = k.name.containsIgnoreCase("ped") ? 67 : 96; }
    }

    // The divisions every keyboard plays: from each key switch along the switch
    // linkages (any number of hops; a condition switch - a ventil, "Cp: Anches
    // Recit" - makes the link optional but it is the keyboard's division all the
    // same) to the division inputs. Caen: four keyboards, eight divisions
    // ("Pedal" + "PedalAnches" and so on): a reed division reached only with its
    // ventil drawn is still that keyboard's. A division reached from several
    // keyboards (a coupler built as a linkage) goes to the keyboard reaching it
    // with the most keys without a condition, then with the most keys at all;
    // the ODF's own hint stays the primary one when it is among them. A keyboard
    // without any linkage (an old or hand-written definition) keeps the hint.
    {
        struct Reach { int unconditional = 0, total = 0; };
        std::map<int, std::map<int, Reach>> reachOfKeyboard;   // keyboard -> division -> keys reaching it
        for (const auto& k : odf.keyboards)
        {
            const auto keys = keyboardKeySwitches.find(k.id);
            if (keys == keyboardKeySwitches.end()) continue;
            for (const int keySwitch : keys->second)
            {
                // Depth first from the key: every division input reached, and whether
                // some link on the way was conditional.
                std::set<int> seen;
                std::map<int, bool> conditionalArrival;   // division -> arrived only through a condition
                std::vector<std::pair<int, bool>> stack{ { keySwitch, false } };
                while (!stack.empty())
                {
                    const auto [sw, conditional] = stack.back(); stack.pop_back();
                    if (!seen.insert(sw).second) continue;
                    if (sw != keySwitch)
                    {
                        const auto input = divisionOfInputSwitch.find(sw);
                        if (input != divisionOfInputSwitch.end())
                        {
                            auto arrived = conditionalArrival.find(input->second);
                            if (arrived == conditionalArrival.end()) conditionalArrival[input->second] = conditional;
                            else arrived->second = arrived->second && conditional;
                        }
                    }
                    const auto links = switchLinks.find(sw);
                    if (links == switchLinks.end() || seen.size() > 4096) continue;
                    for (const auto& [dest, condition] : links->second) stack.push_back({ dest, conditional || condition != 0 });
                }
                for (const auto& [division, conditional] : conditionalArrival)
                {
                    auto& reach = reachOfKeyboard[k.id][division];
                    ++reach.total;
                    if (!conditional) ++reach.unconditional;
                }
            }
        }
        std::map<int, int> ownerOfDivision;   // division -> keyboard
        for (const auto& [division, d] : odf.divisions)
        {
            int best = 0; Reach bestReach;
            for (const auto& k : odf.keyboards)
            {
                const auto reach = reachOfKeyboard.count(k.id) > 0 && reachOfKeyboard.at(k.id).count(division) > 0 ? reachOfKeyboard.at(k.id).at(division) : Reach();
                if (reach.total == 0) continue;
                const bool better = best == 0 || reach.unconditional > bestReach.unconditional
                    || (reach.unconditional == bestReach.unconditional && (reach.total > bestReach.total || (reach.total == bestReach.total && k.divisionId == division)));
                if (better) { best = k.id; bestReach = reach; }
            }
            if (best > 0) ownerOfDivision[division] = best;
        }
        for (auto& k : odf.keyboards)
        {
            std::vector<std::pair<Reach, int>> mine;   // (reach, division) of the divisions this keyboard owns
            for (const auto& [division, owner] : ownerOfDivision)
                if (owner == k.id) mine.push_back({ reachOfKeyboard.at(k.id).at(division), division });
            if (mine.empty()) { if (k.divisionId > 0) k.divisionIds = { k.divisionId }; continue; }
            // The primary: the hint when it is among them, else the division most
            // keys play without a condition; the rest by division id.
            std::stable_sort(mine.begin(), mine.end(), [](const auto& a, const auto& b)
            {
                return a.first.unconditional != b.first.unconditional ? a.first.unconditional > b.first.unconditional
                     : a.first.total != b.first.total ? a.first.total > b.first.total : a.second < b.second;
            });
            int primary = mine.front().second;
            for (const auto& m : mine) if (m.second == k.divisionId) primary = k.divisionId;
            k.divisionIds.clear();
            k.divisionIds.push_back(primary);
            std::vector<int> rest;
            for (const auto& m : mine) if (m.second != primary) rest.push_back(m.second);
            std::sort(rest.begin(), rest.end());
            for (const int division : rest) k.divisionIds.push_back(division);
            k.divisionId = primary;
        }
    }

    // The stops' ranks (alternate = tremmed ranks marked, never taken).
    std::set<int> alternateRanks;
    for (const auto& sr : stopRanks) if (sr.alternateRankId > 0) alternateRanks.insert(sr.alternateRankId);
    for (const auto& sr : stopRanks)
    {
        auto stop = odf.stops.find(sr.stopId);
        if (stop == odf.stops.end()) continue;
        if (std::find(stop->second.rankIds.begin(), stop->second.rankIds.end(), sr.rankId) == stop->second.rankIds.end())
            stop->second.rankIds.push_back(sr.rankId);
        stop->second.rankUses.push_back({ sr.rankId, sr.firstNote, sr.count, sr.increment });
    }
    for (auto& [id, rank] : odf.ranks)
    {
        const auto lower = rank.name.toLowerCase();
        rank.tremmed = alternateRanks.count(id) > 0 || lower.contains("tremmed") || lower.contains("trem.") || lower.endsWith(" trem") || lower.contains("(trem");
        // Noises by whole words of the name ("Key action noise", "Blower",
        // "Stop action", "Tracker"): a substring test took "I. Zimbell" for a bell.
        juce::StringArray words;
        {
            juce::String word;
            for (int i = 0; i < lower.length(); ++i)
            {
                const auto c = lower[i];
                if (juce::CharacterFunctions::isLetter(c)) word << juce::String::charToString(c);
                else if (word.isNotEmpty()) { words.add(word); word.clear(); }
            }
            if (word.isNotEmpty()) words.add(word);
        }
        static const char* const noiseWords[] = { "noise", "noises", "calcant", "blower", "action", "tracker", "trackers",
            "zimbelstern", "cymbelstern", "cimbelstern", "nightingale", "cuckoo", "ruch" };
        rank.noise = false;
        for (const auto* noiseWord : noiseWords) if (words.contains(noiseWord)) rank.noise = true;
        if (lower.contains("noise")) rank.noise = true;   // "keynoise", "stopnoise"
        rank.perspective = perspectiveOfRankName(rank.name);
    }

    // Every pipe's first layer and its first attack sample: the file, the level.
    const auto root = hauptwerkRootFor(file);
    std::set<int> pipesDone;
    for (const int layerId : layerOrder)
    {
        const auto& layer = layers[layerId];
        if (layer.pipeId <= 0 || pipesDone.count(layer.pipeId) > 0) continue;   // the pipe's further layers (tremmed and the like) are not taken
        const auto pipeIt = pipes.find(layer.pipeId);
        if (pipeIt == pipes.end()) continue;
        pipesDone.insert(layer.pipeId);
        const auto attack = attackSampleOfLayer.find(layerId);
        if (attack == attackSampleOfLayer.end()) continue;
        const auto sample = samples.find(attack->second);
        if (sample == samples.end() || sample->second.file.isEmpty()) continue;
        auto rankIt = odf.ranks.find(pipeIt->second.rankId);
        if (rankIt == odf.ranks.end()) continue;
        ++odf.sampleCount;
        const auto& path = sample->second.file;
        if (path.endsWithIgnoreCase(".hbw") || path.endsWithIgnoreCase(".hbx")) ++odf.encryptedSamples;
        if (containsFolderNamed(path, "trem")) continue;   // a tremmed sample: left out
        Pipe pipe;
        pipe.id = layer.pipeId; pipe.rankId = pipeIt->second.rankId; pipe.note = pipeIt->second.note;
        pipe.levelDb = layer.levelDb; pipe.packageId = sample->second.packageId; pipe.file = path;
        rankIt->second.pipes.push_back(pipe);
    }
    // Every rank's folder: the parent folder of most of its pipes, made absolute
    // through its installation package.
    for (auto& [id, rank] : odf.ranks)
    {
        std::map<juce::String, int> count;
        for (const auto& pipe : rank.pipes)
        {
            const auto dir = pipe.file.upToLastOccurrenceOf("/", false, false);
            ++count[juce::String(pipe.packageId) + "|" + dir];
        }
        juce::String best; int bestCount = 0;
        for (const auto& [key, n] : count) if (n > bestCount) { best = key; bestCount = n; }
        if (best.isEmpty() || root == juce::File()) continue;
        const int packageId = best.upToFirstOccurrenceOf("|", false, false).getIntValue();
        const auto dir = best.fromFirstOccurrenceOf("|", false, false);
        rank.packageId = packageId;
        rank.folderRelative = dir;
        rank.folder = packageFolder(root, packageId).getChildFile(dir.isEmpty() ? juce::String(".") : dir);
    }
    // The crescendo: every combination whose name says "cresc" is one stage
    // (the number at the end of the name: "Cresc.Comb.12"); its elements set
    // switches, which reach the stops' own switches through the switch
    // linkages (Doesburg, Schwerin: "CrescendoCombSwitch ..." switches linked
    // to the stops), either way, within a few hops. An element reaching no stop
    // (a coupler, a tremulant) is left out.
    {
        std::map<int, int> stopOfSwitch;
        for (const auto& [id, stop] : odf.stops) if (stop.switchId > 0) stopOfSwitch[stop.switchId] = id;
        std::map<int, std::vector<int>> neighbours;   // both directions of the linkages
        for (const auto& [source, links] : switchLinks)
            for (const auto& [dest, condition] : links) { neighbours[source].push_back(dest); neighbours[dest].push_back(source); }
        auto stopOf = [&](int switchId) -> int
        {
            std::set<int> seen{ switchId };
            std::vector<int> frontier{ switchId };
            for (int hop = 0; hop < 8 && !frontier.empty(); ++hop)   // Schwerin: 4 hops (CrescendoCombSwitch > CrescendoIfOnSwitch > ... > C.<stop> > the stop)
            {
                for (const int s : frontier) { const auto it = stopOfSwitch.find(s); if (it != stopOfSwitch.end()) return it->second; }
                std::vector<int> next;
                for (const int s : frontier)
                    for (const int t : neighbours[s]) if (seen.insert(t).second) next.push_back(t);
                frontier.swap(next);
            }
            return 0;
        };
        std::map<int, std::set<int>> stopsOnByStage;
        std::map<int, int> stageOfCombination;
        for (const auto& [id, name] : combinationNames)
        {
            if (!name.containsIgnoreCase("cresc")) continue;
            const auto digits = name.trim().getTrailingIntValue();
            if (digits <= 0) continue;
            stageOfCombination[id] = digits;
            stopsOnByStage[digits];   // a stage with nothing drawn is a stage all the same
        }
        if (!stageOfCombination.empty())
        {
            std::map<int, int> stopCache;
            for (const auto& e : combinationElements)
            {
                const auto stage = stageOfCombination.find(e.combinationId);
                if (stage == stageOfCombination.end() || !e.on) continue;
                auto cached = stopCache.find(e.switchId);
                const int stopId = cached != stopCache.end() ? cached->second : (stopCache[e.switchId] = stopOf(e.switchId));
                if (stopId > 0) stopsOnByStage[stage->second].insert(stopId);
            }
            for (const auto& [stage, ids] : stopsOnByStage)
                odf.crescendo.push_back({ stage, std::vector<int>(ids.begin(), ids.end()) });
        }
    }
    // Which divisions every tremulant works on: its waveforms' pipes -> their
    // ranks -> the stop playing the rank straight -> that stop's division.
    {
        std::map<int, int> ownerOfRank;
        for (const auto& sr : stopRanks) if (sr.increment == 0 && ownerOfRank.count(sr.rankId) == 0) ownerOfRank[sr.rankId] = sr.stopId;
        for (const auto& sr : stopRanks) if (ownerOfRank.count(sr.rankId) == 0) ownerOfRank[sr.rankId] = sr.stopId;
        for (const auto& [pipeId, waveformId] : tremulantPipes)
        {
            const auto tremulant = tremulantOfWaveform.find(waveformId);
            const auto pipe = pipes.find(pipeId);
            if (tremulant == tremulantOfWaveform.end() || pipe == pipes.end()) continue;
            const auto owner = ownerOfRank.find(pipe->second.rankId);
            if (owner == ownerOfRank.end()) continue;
            const auto stop = odf.stops.find(owner->second);
            if (stop == odf.stops.end()) continue;
            for (auto& t : odf.tremulants) if (t.id == tremulant->second) t.divisionIds.insert(stop->second.divisionId);
        }
    }
    if (progress != nullptr)
        CrashHandler::note("Hauptwerk ODF parsed: divisions " + juce::String((int)odf.divisions.size()) + ", keyboards " + juce::String((int)odf.keyboards.size())
            + ", keyboard keys " + juce::String((int)keyboardNotes.size()) + ", stops " + juce::String((int)odf.stops.size()) + ", stop ranks " + juce::String((int)stopRanks.size())
            + ", ranks " + juce::String((int)odf.ranks.size()) + ", pipes " + juce::String((int)pipes.size()) + ", layers " + juce::String((int)layers.size())
            + ", attack samples " + juce::String((int)attackSampleOfLayer.size()) + ", samples " + juce::String((int)samples.size())
            + ", packages " + juce::String((int)odf.packageIds.size()));
    return odf;
}

//==============================================================================
HauptwerkOdf::Plan HauptwerkOdf::plan(const juce::File& hauptwerkRoot) const
{
    Plan result;
    result.organName = organName.isNotEmpty() ? organName : juce::String("Hauptwerk organ");
    if (error.isNotEmpty()) { result.error = error; return result; }
    if (hauptwerkRoot == juce::File()) { result.error = "The Hauptwerk folders were not found: the organ definition must lie in OrganDefinitions beside OrganInstallationPackages."; return result; }
    result.libraryRoot = hauptwerkRoot.getChildFile("OrganInstallationPackages");
    result.crescendo = crescendo;

    // The divisions in keyboard order, the pedal first: a keyboard whose name
    // (or division's name) says "ped", else the first keyboard with at most 32
    // keys; the rest keep the file's order.
    std::vector<Keyboard> ordered;
    std::set<int> divisionsUsed;
    auto isPedal = [this](const Keyboard& k)
    {
        const auto divisionName = divisions.count(k.divisionId) > 0 ? divisions.at(k.divisionId).name : juce::String();
        return k.name.containsIgnoreCase("ped") || divisionName.containsIgnoreCase("ped");
    };
    // Every division a keyboard plays (the ODF links them through switches,
    // Keyboard::divisionIds) is part of the keyboard's division of ours.
    auto divisionsOf = [](const Keyboard& k)
    {
        std::vector<int> ids = k.divisionIds;
        if (ids.empty() && k.divisionId > 0) ids.push_back(k.divisionId);
        return ids;
    };
    for (const auto& k : keyboards)
    {
        std::vector<int> ids;
        for (const int id : divisionsOf(k)) if (divisionsUsed.count(id) == 0) ids.push_back(id);
        if (ids.empty()) continue;   // a second keyboard of a division (a floating one) adds nothing
        const auto divisionName = divisions.count(k.divisionId) > 0 ? divisions.at(k.divisionId).name : juce::String();
        bool hasStops = false;
        for (const auto& [id, stop] : stops) if (std::find(ids.begin(), ids.end(), stop.divisionId) != ids.end()) { hasStops = true; break; }
        // A pedal without stops (a demo) stays - one of them: division P. is the
        // pedal or empty, never a manual; a second pedal keyboard without stops
        // (a split pedal) is left out.
        bool pedalKept = false;
        for (const auto& kept : ordered) if (isPedal(kept)) pedalKept = true;
        if (!hasStops && (!isPedal(k) || pedalKept)) { result.notes.add("division \"" + divisionName + "\" (keyboard \"" + k.name + "\") has no stops: left out"); continue; }
        for (const int id : ids) divisionsUsed.insert(id);
        Keyboard kept = k;
        kept.divisionIds = ids;
        if (std::find(ids.begin(), ids.end(), kept.divisionId) == ids.end()) kept.divisionId = ids.front();
        ordered.push_back(kept);
    }
    // The manuals from the lowest up: a number at the start of the keyboard's
    // name ("1. manual [lowest]", "2. manual") wins, else the keyboard ID (the
    // file lists the keyboards in any order).
    std::stable_sort(ordered.begin(), ordered.end(), [](const Keyboard& a, const Keyboard& b)
    {
        auto rankOf = [](const Keyboard& k)
        {
            const auto digits = k.name.trim().initialSectionContainingOnly("0123456789");
            return digits.isNotEmpty() ? digits.getIntValue() : 1000 + k.id;
        };
        return rankOf(a) < rankOf(b);
    });
    int pedalIndex = -1;
    for (size_t i = 0; i < ordered.size() && pedalIndex < 0; ++i) if (isPedal(ordered[i])) pedalIndex = (int)i;
    for (size_t i = 0; i < ordered.size() && pedalIndex < 0; ++i)
        if (ordered[i].highNote - ordered[i].lowNote + 1 <= 32) pedalIndex = (int)i;
    if (pedalIndex > 0) std::rotate(ordered.begin(), ordered.begin() + pedalIndex, ordered.begin() + pedalIndex + 1);
    if (pedalIndex < 0)
    {
        // No pedal (a harpsichord, a demo): division P. stays empty, the manuals begin at I.
        Keyboard empty; empty.name = "Pedal"; empty.lowNote = 36; empty.highNote = 67;
        ordered.insert(ordered.begin(), empty);
        result.notes.add("no pedal in the organ definition: division P. stays empty");
    }
    if (ordered.size() > 8) { result.notes.add(juce::String((int)ordered.size() - 8) + " division(s) beyond eight left out"); ordered.resize(8); }
    bool anyStops = false;
    for (const auto& k : ordered)
        for (const int divisionId : divisionsOf(k))
            for (const auto& [id, stop] : stops) if (divisionId > 0 && stop.divisionId == divisionId) anyStops = true;
    if (!anyStops) { result.error = "No keyboard with stops was found in the organ definition."; return result; }
    // The rows that carry folders: the stops without children, and the children
    // of the stops that have them (a stop folder's own row has no folders).
    auto leavesOf = [](PlanDivision& division)
    {
        std::vector<PlanStop*> leaves;
        for (auto& stop : division.stops)
        {
            if (stop.children.empty()) { leaves.push_back(&stop); continue; }
            for (auto& child : stop.children) leaves.push_back(&child);
        }
        return leaves;
    };

    // Who owns a rank: the stop that plays it without a transposition (the
    // first in the file when several do); a rank nobody plays straight belongs
    // to its first user. A stop whose rows borrow one other stop's ranks with
    // one transposition is an extension of that stop (a 4' playing the 8' rank
    // an octave up, with or without a few top pipes of its own); its folders
    // would otherwise come out as the whole division (the common part of two
    // stops' paths), and the division's tree as the whole package.
    std::map<int, int> ownerOfRank;
    for (const auto& [id, stop] : stops)
        for (const auto& use : stop.rankUses)
            if (use.increment == 0 && ownerOfRank.count(use.rankId) == 0) ownerOfRank[use.rankId] = id;
    for (const auto& [id, stop] : stops)
        for (const auto& use : stop.rankUses)
            if (ownerOfRank.count(use.rankId) == 0) ownerOfRank[use.rankId] = id;
    auto usableRank = [this](int rankId) -> const Rank*
    {
        const auto rankIt = ranks.find(rankId);
        if (rankIt == ranks.end()) return nullptr;
        const auto& rank = rankIt->second;
        if (rank.tremmed || rank.noise) return nullptr;   // counted below, over the whole definition
        if (rank.pipes.empty() || rank.folder == juce::File()) return nullptr;
        return &rank;
    };
    // The stop folder(s) of a set of ranks: the perspectives of a stop lie side
    // by side under it, possibly spread over several installation packages (the
    // rear perspective in its own package) with the same paths below each - so
    // the paths relative to the packages are compared: their common part is the
    // stop folder in every package concerned. A single rank is its own stop
    // folder unless its name ends with its parent's ("03-rearMk4-PEd_Princip16"
    // under "PEd_Princip16": the parent is the stop). A common part shorter than
    // two folders means the perspectives lie ABOVE the stops: every rank folder
    // is then a stop folder of its own.
    auto foldersFor = [&](const std::vector<int>& rankIdsIn) -> std::vector<juce::File>
    {
        std::vector<juce::String> relatives;
        std::set<int> packages;
        std::vector<juce::File> rankFolders;
        for (const int rankId : rankIdsIn)
        {
            const auto& rank = ranks.at(rankId);
            relatives.push_back(rank.folderRelative); packages.insert(rank.packageId); rankFolders.push_back(rank.folder);
        }
        if (relatives.empty()) return {};
        auto components = [](const juce::String& path) { return juce::StringArray::fromTokens(path, "/", ""); };
        juce::StringArray common = components(relatives.front());
        for (const auto& rel : relatives)
        {
            const auto parts = components(rel);
            int n = 0;
            while (n < common.size() && n < parts.size() && common[n].equalsIgnoreCase(parts[n])) ++n;
            common.removeRange(n, common.size() - n);
        }
        if (relatives.size() == 1)
        {
            const auto parts = components(relatives.front());
            if (parts.size() >= 2 && parts[parts.size() - 1].endsWithIgnoreCase(parts[parts.size() - 2]) && parts[parts.size() - 1].length() > parts[parts.size() - 2].length())
                common.removeRange(common.size() - 1, 1);
        }
        std::vector<juce::File> folders;
        if (common.size() >= 2)
        {
            const auto stopRelative = common.joinIntoString("/");
            for (const int packageId : packages)
            {
                const auto folder = packageFolder(hauptwerkRoot, packageId).getChildFile(stopRelative);
                if (folder.isDirectory()) folders.push_back(folder);
            }
        }
        if (folders.empty()) folders = rankFolders;   // the perspectives above the stops, or nothing in common
        return folders;
    };
    std::set<int> plannedStopIds;
    for (const auto& k : ordered)
    {
        PlanDivision division;
        division.divisionId = k.divisionId;
        division.divisionIds = divisionsOf(k);
        division.name = divisions.count(k.divisionId) > 0 ? divisions.at(k.divisionId).name : k.name;
        // The definition's tremulant on this division (one modulating any of its
        // ODF divisions): its speed becomes the division's tremulant rate.
        for (const auto& t : tremulants)
            for (const int divisionId : division.divisionIds)
                if (division.tremulantRateHz <= 0.0f && t.rateHz > 0.0f && t.divisionIds.count(divisionId) > 0) division.tremulantRateHz = t.rateHz;
        division.lowNote = k.lowNote; division.highNote = k.highNote;
        // The stops of every ODF division the keyboard plays, the primary
        // division's first (Caen: "GrandOrgue" then "GrandOrgueAnches").
        for (const int divisionId : division.divisionIds)
        for (const auto& [id, stop] : stops)
        {
            if (stop.divisionId != divisionId) continue;
            std::vector<juce::File> rankFolders;
            std::vector<int> rankIds;
            std::map<int, std::set<int>> borrowed;   // owner stop -> the transpositions used
            for (const auto& use : stop.rankUses)
            {
                const auto* rank = usableRank(use.rankId);
                if (rank == nullptr) continue;
                const int owner = ownerOfRank.count(use.rankId) > 0 ? ownerOfRank.at(use.rankId) : id;
                if (owner != id) { borrowed[owner].insert(use.increment); continue; }
                if (std::find(rankIds.begin(), rankIds.end(), use.rankId) != rankIds.end()) continue;
                rankFolders.push_back(rank->folder);
                rankIds.push_back(use.rankId);
            }
            if (borrowed.size() == 1 && borrowed.begin()->second.size() == 1 && stops.count(borrowed.begin()->first) > 0)
            {
                PlanStop extension;
                extension.stopId = id; extension.name = stop.name;
                extension.extensionOfStopId = borrowed.begin()->first;
                extension.shift = juce::jlimit(-24, 24, *borrowed.begin()->second.begin());
                division.stops.push_back(extension);
                plannedStopIds.insert(id);
                ++result.extensions;
                juce::String note = "\"" + stop.name + "\" plays the ranks of \"" + stops.at(extension.extensionOfStopId).name + "\""
                    + (extension.shift != 0 ? " shifted by " + juce::String(extension.shift) + " semitones" : juce::String()) + ": made a transmission of it";
                if (!rankFolders.empty()) note << " (its " << (int)rankFolders.size() << " rank folder(s) of its own, " << rankFolders.front().getFileName() << ", are not loaded)";
                result.notes.add(note);
                continue;
            }
            if (!borrowed.empty())
                result.notes.add("\"" + stop.name + "\" borrows ranks of " + juce::String((int)borrowed.size()) + " other stop(s) in several ways: only its own ranks are taken");
            if (rankFolders.empty()) { ++result.stopsWithoutSamples; result.notes.add("\"" + stop.name + "\" has no usable pipes (tremmed, noise or missing samples): left out"); continue; }
            PlanStop planStop;
            planStop.stopId = id; planStop.name = stop.name; planStop.rankIds = rankIds;
            // Several ranks in ONE perspective (Caen: a stereo stop recorded as
            // an "(L component)" and an "(R component)" rank, two folders for the
            // same perspective): OrganSampler's stop has one folder per
            // perspective, so the stop becomes a stop folder whose children are
            // the rank groups - the ranks matched across the perspectives by
            // their name without the perspective head ("Rec: Quintaton 16 (L
            // component)" in Front and Rear is one child), else by their folder
            // name. One rank per perspective stays one stop, as ever.
            {
                std::map<juce::String, int> ranksPerLabel;
                bool severalInOne = false;
                for (const int rankId : rankIds)
                {
                    const auto& rank = ranks.at(rankId);
                    if (++ranksPerLabel[rank.perspective.isNotEmpty() ? rank.perspective : juce::String("default")] > 1) severalInOne = true;
                }
                if (severalInOne)
                {
                    using Groups = std::vector<std::pair<juce::String, std::vector<int>>>;
                    auto groupBy = [&](bool byName)
                    {
                        Groups groups;
                        for (const int rankId : rankIds)
                        {
                            const auto& rank = ranks.at(rankId);
                            juce::String key;
                            if (byName)
                            {
                                key = rank.name;
                                if (rank.perspective.isNotEmpty() && key.contains(":")) key = key.fromFirstOccurrenceOf(":", false, false).trim();
                            }
                            if (key.isEmpty()) key = rank.folder.getFileName();
                            bool found = false;
                            for (auto& group : groups) if (group.first.equalsIgnoreCase(key)) { group.second.push_back(rankId); found = true; break; }
                            if (!found) groups.push_back({ key, { rankId } });
                        }
                        return groups;
                    };
                    Groups groups = groupBy(true);
                    if (groups.size() < 2) groups = groupBy(false);
                    if (groups.size() >= 2)
                    {
                        // The children by name ("(L component)" before "(R component)"), not in the file's order.
                        std::stable_sort(groups.begin(), groups.end(), [](const auto& a, const auto& b) { return a.first.compareNatural(b.first) < 0; });
                        for (const auto& [key, ids] : groups)
                        {
                            PlanStop child;
                            child.stopId = id; child.name = key; child.rankIds = ids;
                            child.folders = foldersFor(ids);
                            planStop.children.push_back(child);
                        }
                        result.notes.add("\"" + stop.name + "\" has " + juce::String((int)rankIds.size()) + " ranks in " + juce::String((int)ranksPerLabel.size())
                            + " perspective(s): made a stop folder with " + juce::String((int)groups.size()) + " stops (" + groups.front().first + " ...)");
                    }
                }
            }
            if (planStop.children.empty()) planStop.folders = foldersFor(rankIds);
            division.stops.push_back(planStop);
            plannedStopIds.insert(id);
        }
        result.divisions.push_back(division);
    }
    // An extension of a stop that is not in the plan (left out, or in a division
    // beyond the eight) cannot be made.
    for (auto& division : result.divisions)
        for (size_t i = 0; i < division.stops.size();)
        {
            auto& stop = division.stops[i];
            if (stop.extensionOfStopId > 0 && plannedStopIds.count(stop.extensionOfStopId) == 0)
            {
                result.notes.add("\"" + stop.name + "\": the stop it plays the ranks of is not in the plan - left out");
                --result.extensions; ++result.stopsWithoutSamples;
                division.stops.erase(division.stops.begin() + (long)i);
                continue;
            }
            ++i;
        }
    // No stop folder may hold another stop's folder: the loader would read the
    // inner stop twice (once as a perspective folder of the outer one). Such a
    // stop takes its rank folders as stop folders of their own instead.
    {
        std::vector<juce::File> allFolders;
        for (auto& division : result.divisions) for (auto* stop : leavesOf(division)) for (const auto& f : stop->folders) allFolders.push_back(f);
        for (auto& division : result.divisions)
            for (auto* stop : leavesOf(division))
            {
                bool holdsAnother = false;
                for (const auto& f : stop->folders)
                    for (const auto& g : allFolders)
                        if (g != f && g.isAChildOf(f)) holdsAnother = true;
                if (!holdsAnother) continue;
                std::vector<juce::File> rankFolders;
                for (const int rankId : stop->rankIds)
                    if (const auto* rank = usableRank(rankId)) rankFolders.push_back(rank->folder);
                if (rankFolders.empty()) continue;
                result.notes.add("\"" + stop->name + "\": its folder " + stop->folders.front().getFileName() + " holds other stops - its rank folders are taken one by one");
                stop->folders = rankFolders;
            }
    }
    // The trees: the parents of the stop folders, the one most stops lie in first.
    for (size_t d = 0; d < result.divisions.size(); ++d)
    {
        auto& division = result.divisions[d];
        const auto& k = ordered[d];
        std::map<juce::String, int> treeCount;   // folder path (lower case) -> stops
        std::map<juce::String, juce::File> treeFile;
        for (const auto* stop : leavesOf(division))
            for (const auto& stopFolder : stop->folders)
            {
                const auto tree = stopFolder.getParentDirectory();
                const auto key = tree.getFullPathName().toLowerCase();
                ++treeCount[key];
                treeFile[key] = tree;
            }
        std::vector<std::pair<int, juce::File>> trees;
        for (const auto& [key, n] : treeCount) trees.push_back({ n, treeFile[key] });
        std::sort(trees.begin(), trees.end(), [](const auto& a, const auto& b) { return a.first != b.first ? a.first > b.first : a.second.getFullPathName() < b.second.getFullPathName(); });
        for (const auto& t : trees) division.trees.push_back(t.second);
        bool anyFolders = false;
        for (const auto* stop : leavesOf(division)) if (!stop->folders.empty()) anyFolders = true;
        if (division.trees.empty() && anyFolders) result.notes.add("division \"" + division.name + "\": no sample folders (its stops have no usable pipes)");
        else if (division.stops.empty() && k.divisionId > 0) result.notes.add("division \"" + division.name + "\" (keyboard \"" + k.name + "\") has no stops: it stays as the empty pedal");
    }
    for (const auto& division : result.divisions)
        for (const auto& tree : division.trees)
            if (std::find(result.allTrees.begin(), result.allTrees.end(), tree) == result.allTrees.end()) result.allTrees.push_back(tree);
    // What the whole definition holds besides (for the message): the tremmed
    // ranks (alternate ranks of the stops, never listed under them) and the noises.
    for (const auto& [id, rank] : ranks)
    {
        if (rank.tremmed) ++result.skippedTremmedRanks;
        else if (rank.noise) ++result.skippedNoiseRanks;
    }

    // The perspective patterns: for every perspective the rank names tell, the
    // common prefix of its rank folders' names under the stop folder
    // ("03-rearMk4-" of "03-rearMk4-UW_Quintadena8"), as OrganSampler's own
    // "Name;prefix" lines. Only stops with two or more perspectives count.
    {
        // One perspective may carry several prefixes ("01-dirc-" in three
        // divisions, "03-dif2S-" in the fourth, both "Front A (direct)"): the
        // prefix of a rank folder is its name without the stop folder's name at
        // the end; every prefix seen twice or more joins the label's line
        // ("Direct;01-dirc-;03-dif2S-"), the commonest first.
        std::map<juce::String, std::map<juce::String, int>> prefixesByLabel;   // label -> prefix (as spelt first) -> rank folders
        std::map<juce::String, juce::String> spelling;                          // lower-case prefix -> first spelling
        for (auto& division : result.divisions)
            for (const auto* stopPtr : leavesOf(division))
            {
                const auto& stop = *stopPtr;
                if (stop.rankIds.size() < 2) continue;
                for (const int rankId : stop.rankIds)
                {
                    const auto& rank = ranks.at(rankId);
                    if (rank.perspective.isEmpty() || rank.folder == juce::File()) continue;
                    for (const auto& stopFolder : stop.folders)
                    {
                        if (rank.folder == stopFolder || !rank.folder.isAChildOf(stopFolder)) continue;
                        auto below = rank.folder;
                        while (below.getParentDirectory() != stopFolder && below.getParentDirectory() != below) below = below.getParentDirectory();
                        const auto name = below.getFileName(), stopName = stopFolder.getFileName();
                        juce::String prefix;
                        if (name.length() > stopName.length() && name.endsWithIgnoreCase(stopName)) prefix = name.dropLastCharacters(stopName.length());
                        else
                        {
                            // The rank folder is not "<prefix><stop folder>": the prefix is
                            // the name up to and including its last separator run.
                            int cut = -1;
                            for (int i = 0; i < name.length(); ++i) if (name[i] == '-' || name[i] == '_' || name[i] == ' ') cut = i;
                            prefix = cut >= 2 ? name.substring(0, cut + 1) : juce::String();
                        }
                        if (prefix.length() < 3) break;   // a digit or two ("0-") names nothing: the loader learns those itself
                        const auto key = prefix.toLowerCase();
                        if (spelling.count(key) == 0) spelling[key] = prefix;
                        ++prefixesByLabel[rank.perspective][key];
                        break;
                    }
                }
            }
        for (const auto& [label, prefixes] : prefixesByLabel)
        {
            std::vector<std::pair<int, juce::String>> ordered;
            for (const auto& [key, n] : prefixes) if (n >= 2) ordered.push_back({ n, spelling[key] });
            if (ordered.empty()) continue;
            std::sort(ordered.begin(), ordered.end(), [](const auto& a, const auto& b) { return a.first != b.first ? a.first > b.first : a.second < b.second; });
            juce::String line = label;
            for (const auto& [n, prefix] : ordered) line << ";" << prefix;
            result.perspectivePatterns.push_back(line);
        }
    }
    // Where the configuration goes: OrganConfigurations beside OrganDefinitions
    // and OrganInstallationPackages (0.8.125; before, the organ's main package
    // folder - right, but nobody finds "001318"). The library root stays
    // OrganInstallationPackages, reached from there as "../OrganInstallationPackages"
    // (sampleLibraryRootRelative), so the file works from that folder and from
    // anywhere else the root is known.
    juce::String fileName = result.organName;
    for (const auto c : juce::String("\\/:*?\"<>|")) fileName = fileName.replaceCharacter(c, '-');
    result.configurationFile = hauptwerkRoot.getChildFile("OrganConfigurations").getChildFile(fileName.trim() + ".json");
    if (encryptedSamples > 0)
        result.error = juce::String(encryptedSamples) + " of " + juce::String(sampleCount) + " samples are encrypted (.hbw): Hauptwerk's encrypted sample sets cannot be played by OrganSampler.";
    return result;
}

std::map<int, float> HauptwerkOdf::levelsForFolders(const std::vector<juce::File>& rankFolders) const
{
    std::map<int, float> sum;
    std::map<int, int> count;
    for (const auto& folder : rankFolders)
        for (const auto& [id, rank] : ranks)
        {
            if (rank.folder == juce::File() || rank.folder != folder) continue;
            for (const auto& pipe : rank.pipes) { sum[pipe.note] += pipe.levelDb; ++count[pipe.note]; }
        }
    std::map<int, float> result;
    for (const auto& [note, total] : sum) result[note] = total / (float)juce::jmax(1, count[note]);
    return result;
}

bool HauptwerkOdf::compassForFolders(const std::vector<juce::File>& rankFolders, int divisionLow, int divisionHigh, int& low, int& high) const
{
    // Every StopRank row that plays one of these rank folders without a
    // transposition: its keys are [first, first + count - 1]; a row without a
    // first key starts at the division's first key, one without a count runs
    // to the division's last. The union over the rows (a stop of several
    // pieces) is the compass. Nothing found: the whole compass.
    low = 128; high = -1;
    bool any = false;
    for (const auto& [id, stop] : stops)
        for (const auto& use : stop.rankUses)
        {
            if (use.increment != 0) continue;
            const auto rankIt = ranks.find(use.rankId);
            if (rankIt == ranks.end() || rankIt->second.folder == juce::File()) continue;
            bool matches = false;
            for (const auto& folder : rankFolders) if (folder == rankIt->second.folder) matches = true;
            if (!matches) continue;
            const int first = use.firstNote > 0 ? use.firstNote : divisionLow;
            const int last = use.count > 0 ? first + use.count - 1 : divisionHigh;
            low = juce::jmin(low, first); high = juce::jmax(high, last);
            any = true;
        }
    if (!any || high < low) return false;
    low = juce::jlimit(0, 127, low); high = juce::jlimit(0, 127, high);
    return true;
}
