# Organ configurations

A configuration is a complete organ built in OrganSampler - the folder trees
of the samples, the Stop List, the voicing, the audio routes, couplers,
combinations and crescendo - saved with *Save Configuration* as one `.json`
file. With a published configuration a player who owns the same sample
folders loads the whole organ in one step instead of building it.

**No samples are ever published here.** A configuration only names the
sample folders it needs; the samples themselves are bought or downloaded
elsewhere - from [Sonus Paradisi](https://www.sonusparadisi.cz/) above all
(their [Free stuff](https://www.sonusparadisi.cz/en/free-stuff.html) page has
free instruments and impulse responses to begin with) - and remain the
property of their authors. A check in the repository (`Configurations check`
workflow) refuses any audio file outside an `IR` subfolder, any file named
like a pipe sample (`036-C.wav`), any file above 40 MB and any file type
other than `.json`, `.md`, `.txt` and pictures.

## Using a published configuration

1. Get the sample folders the configuration was made for (its README says
   which and where from).
2. Put the `.json` file **beside the samples** - in the folder that contains
   their top folder. A configuration stores its sample paths relative to the
   common folder of its sample trees (the "library root"), so when the file
   lies beside that root it finds the samples by itself:

   ```
   D:\Samples\                          <- put Stralsund.json here ...
   ├── Stralsund.json
   └── 003136\                          <- ... beside the library root the configuration names
       └── pipe\
           ├── dir-Principal 8\
           └── ...
   ```

   The README of each configuration says the name of the library root and the
   tree folders it expects (the configuration also carries them as
   `sampleLibraryRootName` and `folderRelativePath`).
3. Copy the impulse responses published with it (the `.wav` files of the IR
   reverb, in its `IR` folder) next to the `.json`: their paths are relative
   to the configuration file.
4. In OrganSampler: *Load Configuration* in the top bar, choose the file.

If the samples lie elsewhere, the configuration loads all the same (since
0.8.49) with the trees that are found; every division that misses a tree
shows an orange notice "Sample folder not found: <folder>" under its header.
Open the Stop List, *Load folder...* and choose the folder of that name
wherever it is on your computer: it takes the missing tree's place and the
stops keep their samples. Then *Save Configuration* to keep the paths.

## Publishing a configuration

One folder per organ, named after the organ, holding:

```
configurations/
└── stralsund-jakobi-wegscheider/
    ├── README.md                      <- required, see below
    ├── Stralsund Jakobi Wegscheider.json
    └── IR/
        └── Rural Church Front.wav     <- the IRs the configuration uses, if any (and if their licence allows)
```

The README of a configuration:

- **Organ**: the instrument, builder, place, year.
- **Samples**: the name of the sample folders, their producer, a link to the
  page where they are sold or downloaded, which edition (wave format, which
  perspectives).
- **Expects**: the library root folder name and the tree folders the
  configuration refers to, exactly as saved (the Stop List shows them, the
  file carries them as `sampleLibraryRootName` and `folderRelativePath`).
- **Audio layout**: divisions, routes per division (perspectives, auxiliary),
  IR reverbs and their sources, the output channels the routes expect, the
  measured polyphony and the computer it was measured on.
- **Saved with**: the OrganSampler version.
- **Author** and the date.

Save the configuration **without MIDI** when asked (the MIDI devices of your
console are of no use to anybody else) and keep the IR files in an `IR`
folder beside the `.json` when you save, so their paths are stored relative
to it. Then either open a pull request adding the folder, or open an issue
with the "Organ configuration" template and attach the files; the maintainer
adds them. Published configurations are under the repository's licence
(GPLv3).

## Starter folders

A small set of sample folders that can be downloaded freely, with its
configuration, is planned for this folder (a trimmed version of the Kobrle
organ samples), so that OrganSampler can be tried at once. Until it is here,
Sonus Paradisi's Free stuff page is the way to start.
