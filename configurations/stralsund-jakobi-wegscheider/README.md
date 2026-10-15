# Stralsund, St. Jakobi - the Wegscheider organ

A complete OrganSampler configuration of the Wegscheider organ in the church
of St. Jakobi, Stralsund, as recorded by Sonus Paradisi: four divisions, fifty-three
stops, three recording perspectives on separate audio routes, the stop list
arranged, the voicing settled, couplers, combinations and a crescendo in place.
Whoever owns the recordings loads the whole instrument in a single step.

**No samples are published here.** The configuration merely names the folders
it expects; the recordings themselves are the property of Sonus Paradisi and are
obtained from them.

## What you need

- **The recordings.** The Sonus Paradisi *Stralsund, St. Jakobi, Wegscheider organ*,
  which is delivered as two Hauptwerk installation packages. Once unpacked, each
  package is a folder named after its number, `003135` and `003136`, and each holds
  its pipes under `pipe`, one folder per stop and perspective. The configuration
  draws on both packages in every division, so both are required.
- **Plain WAV files.** OrganSampler reads ordinary WAV files; the pipe folders must
  contain the recordings in that form.
- **OrganSampler 0.8.92 or later.** Earlier versions let a recalled combination
  replace a stop's transmissions, which would strip the two composite stops of
  their rows.

## Where to put the file

The two package folders must stand side by side in one folder; that folder is the
*library root* of this configuration. Place `Stralsund Jakobi Wegscheider.json`
in it, beside the two packages:

```
<any folder>\                              <- the library root: put the .json here
├── Stralsund Jakobi Wegscheider.json
├── 003135\
│   └── pipe\
│       ├── HW\  OW\  UW\  PE\             <- the divisions' folder trees
│       └── ...
└── 003136\
    └── pipe\
        ├── HW\  OW\  UW\  PE\
        └── ...
```

The paths inside the configuration are relative to that root (`003136/pipe/HW`
and so on), so the folders are found without a question. The root may be called
anything; if you prefer to keep the file elsewhere, a folder named `haupwerk`
holding the two packages and standing beside the `.json` is found as well.

Should a tree not be where the configuration expects it, the organ still loads
with the trees it finds, and the division concerned shows the notice
*Sample folder not found* with the folder's name. Open the Stop List, choose
*Load folder...* and point to that folder wherever it lies; it takes the missing
tree's place and the stops keep their samples. *Save Configuration* then keeps
the new paths.

## Using it

1. Start OrganSampler, choose *Load Configuration* in the top bar and open
   `Stralsund Jakobi Wegscheider.json`.
2. The organ plays at once through the computer's default sound device: every
   perspective route is saved on the default device at 48 kHz with a buffer of
   256 samples. A message then lists what remains to be set for your computer:
   the MIDI keyboard of each division (Settings in the division header; right-click
   the keyboard icon for MIDI Learn), and for every route its sound card, buffer and
   output channels in the audio tabs. The auxiliary route of each division is saved
   without a device and stays silent until you assign one.
3. *Measure the maximum polyphony*, then *Save Configuration* to keep your
   settings. Saving a copy with *Portable configuration* unticked is advisable, so
   that your devices are remembered.

## The configuration

- **Organ:** the organ of St. Jakobi, Stralsund, built by Kristian Wegscheider.
- **Samples:** Sonus Paradisi, *Stralsund, St. Jakobi, Wegscheider organ*
  (https://www.sonusparadisi.cz/), two Hauptwerk installation packages, `003135`
  and `003136`; three perspectives, recorded as *direct* (`01-dirMk2H`), *diffuse*
  (`02-difMk2H`) and *rear* (`03-rearMk4`), which the configuration keeps apart as
  the perspectives Direct, Diffuse and Rear.
- **Expects:** library root of any name, holding the trees `003135/pipe/PE`,
  `003135/pipe/UW`, `003135/pipe/HW`, `003135/pipe/OW` and the same four under
  `003136/pipe`. The file carries them as `sampleTrees` and `folderRelativePath`,
  with `sampleLibraryRootName` set to `haupwerk`.
- **Divisions:** Pedal (thirteen stops and the composite *Cornet*, a stop folder
  of three transmissions of the Octave 4'), I. Unterwerk (twelve stops),
  II. Hauptwerk (fourteen stops, Cornet included) and III. Oberwerk (twelve stops and
  the composite *Vox Humana Cornet*, a stop folder of five transmissions of the
  Vox Humana 8'). The stop names follow the folder names of the packages.
- **Audio layout:** every division has the routes *direct*, *diffuse* and *rear*,
  one per perspective, and an *auxiliary* route; no impulse responses, as the
  recordings carry the room of the church themselves. All routes are saved on the
  default device, output channels 1 and 2, until you spread them over your outputs.
  The polyphony has not been measured on a reference computer; measure it on yours.
- **Saved with:** OrganSampler 0.8.90, as a portable configuration - no MIDI
  devices, no audio devices, nothing of the computer it was made on.
- **Author:** zurek-jiri, 15 October 2026.
