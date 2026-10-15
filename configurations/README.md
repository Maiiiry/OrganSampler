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

Save the configuration with **Portable configuration (to share with
others)** ticked in the Save Configuration box: it then carries the organ
only - no MIDI devices, no audio devices or channels, nothing of your
computer (the manual chapter of the same name explains it) - and keep the IR
files in an `IR` folder beside the `.json` when you save, so their paths are
stored relative to it. Then either open a pull request adding the folder, or open an issue
with the "Organ configuration" template and attach the files; the maintainer
adds them. Published configurations are under the repository's licence
(GPLv3).

## Published configurations

- [`stralsund-jakobi-wegscheider`](stralsund-jakobi-wegscheider/README.md) -
  the Wegscheider organ of St. Jakobi, Stralsund, from the Sonus Paradisi
  recordings (two Hauptwerk installation packages, `003135` and `003136`):
  four divisions, 53 stops, three perspectives on separate routes.
- [`rural-organ`](rural-organ/README.md) - the demo organ, see below.

## The demo organ: Rural Organ

The one exception to "no samples here": **Rural Organ**, a small village
organ - three divisions, 24 stops, dry recordings with IR reverb -
packaged with its sample folders and its portable configuration so that
OrganSampler can be tried at once. See [`rural-organ/README.md`](rural-organ/README.md);
the package `RuralOrgan.zip` (about 1 GB) is attached to the release
https://github.com/zurek-jiri/OrganSampler/releases/tag/rural-organ-0.8.
The samples are provided by Sonus Paradisi free of charge to try the program.
