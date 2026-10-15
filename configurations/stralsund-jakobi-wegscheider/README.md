# Stralsund, St. Jakobi - the Wegscheider organ

A complete OrganSampler configuration of the Wegscheider organ in the church
of St. Jakobi, Stralsund, as recorded by Sonus Paradisi in three perspectives,
Direct, Diffuse and Rear, each on its own audio route: four divisions and 71 rows
in the Stop List, transmissions and extensions included (only the rows nested in
the two composite stops are not counted). Whoever owns the recordings loads the
whole instrument in a single step.

**Download:** https://github.com/zurek-jiri/OrganSampler/releases/download/configurations/Stralsund-Jakobi-Wegscheider.json

**No samples are published here.** The configuration merely names the folders
it expects; the recordings themselves are the property of Sonus Paradisi and are
obtained from them.

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

## Where to put the file

The two packages, `003135` and `003136`, must stand side by side in one folder.
Place `Stralsund-Jakobi-Wegscheider.json` in that folder:

```
<any folder>\
├── Stralsund-Jakobi-Wegscheider.json
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

Transmissions and extensions are given as *source, shift in semitones*; an
alias precedes its source in parentheses. The two composite stops list their
nested rows in parentheses.

| Pedal (18) | I (18) | II (20) | III (15) |
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
| Ped Trompete 8 | UW Gedackt 8, −12 | HW Tromp 16 | Vox Humana Cornet (OW VoxHumana 8: −12, 0, +12, +16, +19) |
| Cornet (Ped octave 4: +7, +12, +16) | UW Gedackt 8, +12 | HW Zimbel | Bombarde (OW Trompete 8, −12) |
| Ped ContraPos 32, −12 | UW Gedackt 8, +24 | Flöte (UW Tertia 135, −4) | Clairon (OW Trompete 8, +12) |
| Ped Posaune 16, −24 | UW Octave 4, +12 | HW Gamba 8, −12 | |
| Ped Principalbass 32, −12 | Septime (UW Quinte 113, +3) | Montre (HW Octava 8, −24) | |
| BauernFlöte (UW Nasard 3, +5) | Regal (OW VoxHumana 8, −12) | HW Rohrflote 8, −12 | |
| | | HW Tromp 16, +12 | |
| | | HW Tromp 16, +24 | |
