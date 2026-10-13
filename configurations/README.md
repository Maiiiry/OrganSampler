# Organ configurations

A configuration is a complete organ built in OrganSampler - the folder trees
of the samples, the Stop List, the voicing, the audio routes, couplers,
combinations and crescendo - saved with *Save Configuration* as one `.json`
file. With a published configuration a player who owns the same sample set
loads the whole organ in one step instead of building it.

No sample data is ever published here. Every configuration names the sample
set it needs and where that set comes from.

## Using a published configuration

1. Get the sample set the configuration was made for (see the configuration's
   README; the website lists where sample sets come from).
2. Put the `.json` file **beside the sample set** - in the folder that
   contains the set's top folder. A configuration stores its sample paths
   relative to the common folder of its sample trees (the "library root"),
   so when the file lies beside that root it finds the samples by itself:

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
   reverb) into the same folder as the `.json`: their paths are relative to
   the configuration file.
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
    └── Rural Church Front.wav         <- the IRs the configuration uses, if any (and if their licence allows)
```

The README of a configuration:

- **Organ**: the instrument, builder, place, year.
- **Sample set**: name, producer, a link to the page where it is sold or
  downloaded, which edition (wave format, which perspectives).
- **Expects**: the library root folder name and the tree folders the
  configuration refers to, exactly as saved (the Stop List shows them, the
  file carries them as `sampleLibraryRootName` and `folderRelativePath`).
- **Audio layout**: divisions, routes per division (perspectives, auxiliary),
  IR reverbs and their sources, the output channels the routes expect, the
  measured polyphony and the computer it was measured on.
- **Saved with**: the OrganSampler version.
- **Author** and the date.

Save the configuration **without MIDI** when asked (the MIDI devices of your
console are of no use to anybody else) and keep the IR files beside the
`.json` when you save, so their paths are stored relative to it. Then either
open a pull request adding the folder, or open an issue with the
"Organ configuration" template and attach the files; the maintainer adds
them. Published configurations are under the repository's licence (GPLv3);
the sample sets remain the property of their producers.

## Starter sample set

A small sample set that can be downloaded freely, with its configuration, is
planned for this folder (a trimmed version of the Kobrle organ samples), so
that OrganSampler can be tried without buying a set first. Until it is here,
the free sets listed on the website are the way to start.
