# Stralsund, St. Jakobi - the Wegscheider organ

A complete OrganSampler configuration of the Wegscheider organ in the church
of St. Jakobi, Stralsund, as recorded by Sonus Paradisi: four divisions, 53 rows
in the Stop List (nested transmissions not counted), three recording
perspectives on separate audio routes, the voicing settled, couplers,
combinations and a crescendo in place. Whoever owns the recordings loads the
whole instrument in a single step.

**No samples are published here.** The configuration merely names the folders
it expects; the recordings themselves are the property of Sonus Paradisi and are
obtained from them.

## Where to put the file

The two package folders must stand side by side in one folder. Place
`Stralsund Jakobi Wegscheider.json` in it, beside the two packages:

```
<any folder>\
├── Stralsund Jakobi Wegscheider.json
├── 003135\
│   └── pipe\
│       ├── HW\  OW\  UW\  PE\
│       └── ...
└── 003136\
    └── pipe\
        ├── HW\  OW\  UW\  PE\
        └── ...
```

## Stop List

| Pedal | I | II | III |
|---|---|---|---|
| Ped BassFloete 8 | UW Fugara 4 | HW Cornet | OW Flote 8 |
| Ped ContraPos 32 | UW Gedackt 8 | HW Gamba 8 | OW Gedeckt 4 |
| Ped Fagot 16 | UW Gemshorn 2 | HW Gemshorn 8 | OW Lamento 8 |
| Ped Mixtur | UW Hautbois 8 | HW Mixtur | OW Mixtur |
| Ped octave 4 | UW Mixtur | HW Octava 8 | OW Octava 2 |
| Ped Octave 8 | UW Nasard 3 | HW Octave 2 | OW Octava 4 |
| Ped Offenbass 16 | UW Octave 4 | HW Octave 4 | OW Principal 8 |
| Ped Posaune 16 | UW Principal 8 | HW Principal 16 | OW Quintadena 16 |
| PEd Princip 16 | UW Quintadena 8 | HW Quinte 3 | OW Salicional 8 |
| Ped Principalbass 32 | UW Quinte 113 | HW Quinte 6 | OW Sesquialtera |
| Ped Schalmey 4 | UW Tertia 135 | HW Rohrflote 8 | OW VoxHumana 8 |
| PEd Subbass 16 | UW Traversflote 4 | HW SpitzFl 4 | OW Trompete 8 |
| Ped Trompete 8 | | HW Tromp 16 | Vox Humana Cornet |
| Cornet | | HW Zimbel | |

## The configuration

- **Organ:** the organ of St. Jakobi, Stralsund, built by Kristian Wegscheider.
- **Samples:** Sonus Paradisi, *Stralsund, St. Jakobi, Wegscheider organ*
  (https://www.sonusparadisi.cz/), two Hauptwerk installation packages, `003135`
  and `003136`, unpacked as plain WAV files; three perspectives, recorded as
  direct (`01-dirMk2H`), diffuse (`02-difMk2H`) and rear (`03-rearMk4`), which the
  configuration keeps apart as the perspectives Direct, Diffuse and Rear.
  OrganSampler 0.8.92 or later.
- **Expects:** library root of any name, holding the trees `003135/pipe/PE`,
  `003135/pipe/UW`, `003135/pipe/HW`, `003135/pipe/OW` and the same four under
  `003136/pipe`.
- **Audio layout:** every division has the routes direct, diffuse and rear, one
  per perspective, and an auxiliary route (unused).
- **Saved with:** OrganSampler 0.8.90.
- **Author:** zurek-jiri, 15 October 2026.
