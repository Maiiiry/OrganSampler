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
free instruments to begin with) - and remain the property of their authors.
Impulse responses belong to the sample folders as well. A folder here holds
exactly two files, the configuration zipped (one `.json` inside - a zip is
downloaded from GitHub with one click, a bare `.json` would open as text) and
its `README.md`; a check in the repository (`Configurations check` workflow)
refuses anything else.

## Using a published configuration

1. Get the sample folders the configuration was made for (its README says
   which and where from).
2. Find the organ in [INDEX.md](INDEX.md), open its README and click its
   **Download** link: a zip with the `.json` inside. Unpack it and put the
   `.json` **beside the samples** - in the folder that contains their top
   folders, as the README shows. A configuration stores its
   sample paths relative to that folder (the "library root"), so the samples
   are found by themselves.
3. In OrganSampler: *Load Configuration* in the top bar, choose the file.

If the samples lie elsewhere, the configuration loads all the same with the
trees that are found; every division that misses a tree shows an orange
notice "Sample folder not found: <folder>" under its header. Open the Stop
List, *Load folder...* and choose the folder of that name wherever it is on
your computer: it takes the missing tree's place and the stops keep their
samples. Then *Save Configuration* to keep the paths.

## Publishing a configuration

One folder per organ, named after the organ, holding the two files:

```
configurations/
└── stralsund-jakobi-wegscheider/
    ├── README.md
    └── Stralsund-Jakobi-Wegscheider.zip      <- the configuration .json, zipped
```

Save the configuration with **Portable configuration (to share with
others)** ticked in the Save Configuration box: it then carries the organ
only - no MIDI devices, no audio devices or channels, nothing of your
computer (the manual chapter of the same name explains it). OrganSampler
writes two more files beside it (since 0.8.96): `<name>.zip`, the
configuration zipped for the upload, and `<name>.README.md`, the README -
the perspectives, the folders the configuration expects and where to put the
file, the routes, the version and the Stop List in the order of your grid.
Fill in the three "(fill in: ...)" places - the organ, the producer and
product of the samples, your name - and nothing else. Then either open a
pull request adding the folder with the zip and the README, or open an issue
with the "Organ configuration" template and attach the two files; after the
check the maintainer adds them, and the organ appears as a new line of
[INDEX.md](INDEX.md). Published configurations are under the repository's
licence (GPLv3).

## The published configurations

The list is [INDEX.md](INDEX.md): one line per organ with its divisions,
Stop List rows and perspectives, written automatically from the configuration
files. The README of each organ says how to download its `.json`.

## The demo organ: Rural Organ

The one exception to "no samples here": **Rural Organ**, a small village
organ - three divisions, 24 stops, dry recordings with IR reverb -
packaged with its sample folders and its portable configuration so that
OrganSampler can be tried at once. See [`rural-organ/README.md`](rural-organ/README.md);
the package `RuralOrgan.zip` (about 1 GB) is attached to the release
https://github.com/zurek-jiri/OrganSampler/releases/tag/rural-organ-0.8.
The samples are provided by Sonus Paradisi free of charge to try the program.
