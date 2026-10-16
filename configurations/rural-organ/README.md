# Rural Organ - the demo organ

A small village organ to try OrganSampler at once: three divisions (Pedal,
I, II), 24 stops, **dry recordings with IR reverb** - the pipes were
recorded close and dry, and the room comes from the impulse responses
`Rural Church Front.wav` and `Rural Church Rear.wav` that the configuration
puts on its routes. That is the point of the demo: hear what OrganSampler's
IR reverb per route does with dry samples, and what the auxiliary route with
the rear impulse response adds on a second pair of speakers.

This is the **only** package of OrganSampler that carries samples. Every
other configuration published here names sample folders that are bought or
downloaded elsewhere (Sonus Paradisi).

## Download

The package `RuralOrgan.zip` (about 1 GB: 1 600 WAV files) is attached to
the release "Rural Organ" on GitHub:
https://github.com/zurek-jiri/OrganSampler/releases/tag/rural-organ-0.8

It holds:

```
Rural Organ/
├── Rural Organ.json          <- the portable configuration
├── README.txt
└── Samply/                   <- the sample folders (library root "Rural Organ")
    ├── TA/  NW/  CO/         <- the three divisions' stop folders
    └── IR/                   <- the two impulse responses the configuration uses
```

## Using it

1. Unpack the zip anywhere (a fast disk; the samples need about 1 GB of RAM
   when loaded).
2. Start OrganSampler, *Load Configuration* in the top bar, choose
   `Rural Organ\Rural Organ.json`. The folders are found beside the file.
3. The organ plays through the computer's default sound device at once. A
   message then says what to set for your computer: the MIDI keyboard of
   each division (Settings in the division header, right-click the keyboard
   icon for MIDI Learn), the sound card, buffer and output channels of every
   route in the audio tabs - the auxiliary routes with the rear impulse
   response play on the default device too until you move them to a second
   pair of speakers - then *Measure the maximum polyphony* and *Save
   Configuration*.

## The configuration

`Rural-Organ.zip` in this folder holds the same portable configuration as the
package (`Rural Organ.json`): saved with "Portable configuration (to share
with others)", so it carries the organ only - no MIDI devices, no audio
devices, nothing of the computer it was made on. Saved with OrganSampler 0.8.103.

- Organ: a Czech village organ, three divisions, recorded dry.
- Samples: Sonus Paradisi, provided free of charge with OrganSampler to try
  the program; they remain the property of their author.
- Expects: library root `Rural Organ`, tree `Samply`; the IRs in `Samply\IR`.
- Audio layout: Pedal, I, II, each with the route `default` = the front
  (IR "Rural Church Front", dry/wet 72 %) and the route `auxiliary` = the
  rear (IR "Rural Church Rear", dry/wet 92 %, for a second pair of
  speakers); every route on the default device until you spread them over
  your outputs.
