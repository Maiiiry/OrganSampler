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

    juce::String commonPrefixIgnoringCase(const std::vector<juce::String>& names)
    {
        if (names.empty()) return {};
        juce::String prefix = names.front();
        for (const auto& name : names)
        {
            int n = 0;
            while (n < prefix.length() && n < name.length() && juce::CharacterFunctions::toLowerCase(prefix[n]) == juce::CharacterFunctions::toLowerCase(name[n])) ++n;
            prefix = prefix.substring(0, n);
        }
        return prefix;
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
    struct StopRankRow { int stopId = 0, rankId = 0, alternateRankId = 0; };
    std::vector<StopRankRow> stopRanks;
    std::map<int, std::vector<int>> keyboardNotes;    // KeyboardID -> notes of its KeyboardKey rows
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
            }
        }
        else if (lower == "stop")
        {
            while (scanner.nextRow(row))
            {
                Stop s; s.id = row.integer("StopID", "a"); s.name = row.text("Name", "b"); s.divisionId = row.integer("DivisionID", "c");
                if (s.id > 0) odf.stops[s.id] = s;
            }
        }
        else if (lower == "stoprank")
        {
            while (scanner.nextRow(row))
            {
                StopRankRow r; r.stopId = row.integer("StopID", "a"); r.rankId = row.integer("RankID", "d"); r.alternateRankId = row.integer("AlternateRankID", "p");
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

    // The stops' ranks (alternate = tremmed ranks marked, never taken).
    std::set<int> alternateRanks;
    for (const auto& sr : stopRanks) if (sr.alternateRankId > 0) alternateRanks.insert(sr.alternateRankId);
    for (const auto& sr : stopRanks)
    {
        auto stop = odf.stops.find(sr.stopId);
        if (stop == odf.stops.end()) continue;
        if (std::find(stop->second.rankIds.begin(), stop->second.rankIds.end(), sr.rankId) == stop->second.rankIds.end())
            stop->second.rankIds.push_back(sr.rankId);
    }
    for (auto& [id, rank] : odf.ranks)
    {
        const auto lower = rank.name.toLowerCase();
        rank.tremmed = alternateRanks.count(id) > 0 || lower.contains("tremmed") || lower.contains("trem.") || lower.endsWith(" trem") || lower.contains("(trem");
        rank.noise = lower.contains("noise") || lower.contains("calcant") || lower.contains("blower") || lower.contains("action")
            || lower.contains("ruch") || lower.contains("bell") || lower.contains("zimbelstern") || lower.contains("nightingale");
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
    for (const auto& k : keyboards)
    {
        if (k.divisionId <= 0 || divisionsUsed.count(k.divisionId) > 0) continue;   // a second keyboard of a division (a floating one) adds nothing
        const auto divisionName = divisions.count(k.divisionId) > 0 ? divisions.at(k.divisionId).name : juce::String();
        bool hasStops = false;
        for (const auto& [id, stop] : stops) if (stop.divisionId == k.divisionId) { hasStops = true; break; }
        // A pedal without stops (a demo) stays - one of them: division P. is the
        // pedal or empty, never a manual; a second pedal keyboard without stops
        // (a split pedal) is left out.
        bool pedalKept = false;
        for (const auto& kept : ordered) if (isPedal(kept)) pedalKept = true;
        if (!hasStops && (!isPedal(k) || pedalKept)) { result.notes.add("division \"" + divisionName + "\" (keyboard \"" + k.name + "\") has no stops: left out"); continue; }
        divisionsUsed.insert(k.divisionId);
        ordered.push_back(k);
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
    for (const auto& k : ordered) for (const auto& [id, stop] : stops) if (k.divisionId > 0 && stop.divisionId == k.divisionId) anyStops = true;
    if (!anyStops) { result.error = "No keyboard with stops was found in the organ definition."; return result; }

    for (const auto& k : ordered)
    {
        PlanDivision division;
        division.divisionId = k.divisionId;
        division.name = divisions.count(k.divisionId) > 0 ? divisions.at(k.divisionId).name : k.name;
        division.lowNote = k.lowNote; division.highNote = k.highNote;
        std::map<juce::String, int> treeCount;   // folder path (lower case) -> stops
        std::map<juce::String, juce::File> treeFile;
        for (const auto& [id, stop] : stops)
        {
            if (stop.divisionId != k.divisionId) continue;
            std::vector<juce::File> rankFolders;
            std::vector<int> rankIds;
            for (const int rankId : stop.rankIds)
            {
                const auto rankIt = ranks.find(rankId);
                if (rankIt == ranks.end()) continue;
                const auto& rank = rankIt->second;
                if (rank.tremmed || rank.noise) continue;   // counted below, over the whole definition
                if (rank.pipes.empty() || rank.folder == juce::File()) continue;
                rankFolders.push_back(rank.folder);
                rankIds.push_back(rankId);
            }
            if (rankFolders.empty()) { ++result.stopsWithoutSamples; continue; }
            // The stop folder: the perspectives of a stop lie side by side under
            // it, possibly spread over several installation packages (the rear
            // perspective in its own package) with the same paths below each - so
            // the paths relative to the packages are compared: their common part
            // is the stop folder in every package concerned. A single rank is its
            // own stop folder unless its name ends with its parent's ("03-rearMk4-
            // PEd_Princip16" under "PEd_Princip16": the parent is the stop). A
            // common part shorter than two folders means the perspectives lie
            // ABOVE the stops: every rank folder is then a stop folder of its own.
            std::vector<juce::String> relatives;
            std::set<int> packages;
            for (const int rankId : rankIds) { relatives.push_back(ranks.at(rankId).folderRelative); packages.insert(ranks.at(rankId).packageId); }
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
            PlanStop planStop;
            planStop.name = stop.name; planStop.rankIds = rankIds;
            if (common.size() >= 2)
            {
                const auto stopRelative = common.joinIntoString("/");
                for (const int packageId : packages)
                {
                    const auto folder = packageFolder(hauptwerkRoot, packageId).getChildFile(stopRelative);
                    if (folder.isDirectory()) planStop.folders.push_back(folder);
                }
            }
            if (planStop.folders.empty()) planStop.folders = rankFolders;   // the perspectives above the stops, or nothing in common
            for (const auto& stopFolder : planStop.folders)
            {
                const auto tree = stopFolder.getParentDirectory();
                const auto key = tree.getFullPathName().toLowerCase();
                ++treeCount[key];
                treeFile[key] = tree;
            }
            division.stops.push_back(planStop);
        }
        std::vector<std::pair<int, juce::File>> trees;
        for (const auto& [key, n] : treeCount) trees.push_back({ n, treeFile[key] });
        std::sort(trees.begin(), trees.end(), [](const auto& a, const auto& b) { return a.first != b.first ? a.first > b.first : a.second.getFullPathName() < b.second.getFullPathName(); });
        for (const auto& t : trees) division.trees.push_back(t.second);
        if (division.trees.empty() && !division.stops.empty()) result.notes.add("division \"" + division.name + "\": no sample folders (its stops have no usable pipes)");
        else if (division.stops.empty() && k.divisionId > 0) result.notes.add("division \"" + division.name + "\" (keyboard \"" + k.name + "\") has no stops: it stays as the empty pedal");
        result.divisions.push_back(division);
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
        std::map<juce::String, std::vector<juce::String>> namesByLabel;
        for (const auto& division : result.divisions)
            for (const auto& stop : division.stops)
            {
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
                        namesByLabel[rank.perspective].push_back(below.getFileName());
                        break;
                    }
                }
            }
        for (const auto& [label, names] : namesByLabel)
        {
            if (names.size() < 2) continue;
            auto prefix = commonPrefixIgnoringCase(names);
            // A prefix worth a pattern names the perspective ("03-rearMk4-"), a
            // digit or two in common ("0") does not: the loader learns those itself.
            if (prefix.length() < 3) continue;
            result.perspectivePatterns.push_back(label + ";" + prefix);
        }
    }
    const auto mainPackage = packageFolder(hauptwerkRoot, mainPackageId > 1 ? mainPackageId : (packageIds.empty() ? 0 : packageIds.front()));
    juce::String fileName = result.organName;
    for (const auto c : juce::String("\\/:*?\"<>|")) fileName = fileName.replaceCharacter(c, '-');
    result.configurationFile = (mainPackage.isDirectory() ? mainPackage : result.libraryRoot).getChildFile(fileName.trim() + ".json");
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
