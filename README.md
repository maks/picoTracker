# picoTracker

picoTracker is a project that aims to provide a low cost open source and DIY hardware music tracker platform. It's firmware is essentially a modified version of [LittleGPTracker](https://littlegptracker.com/) (a.k.a piggy tracker) and keeps 90%+ of it's functionality, but I made some custom modifications, both functional and aesthetic and it might diverge more in the future. It implements a user interface similar to the refined track-by-joypad software [*littlesounddj*](http://www.littlesounddj.com/). picoTracker currently supports 8 monophonic 8Bit/16Bit/44.1Khz stereo channels as well as 16 channel midi output.

## Features

* 8 song channels
* 128 chains
* 128 phrases
* 32 tables
* 16 Sample instruments
* 16 MIDI instruments
* 1MB sample memory
* 8 or 16bit samples up to 44.1kHz, mono or stereo
* 16bit/44.1kHz/Stereo audio output

## Limitations
* Will probably struggle with 8 song channels playing at the same time in most cases. I modified the source to parametrically reduce the total songs, but didn't want to make the decision of supporting only 6 songs or so just yet. There is still room for improvement by either multithreading or increasing CPU frequency.
* Cannot load LGPT projects (thou I wrote an ugly script to convert projects).
* Samples are played copied to flash upon load and played from there. Since flash has to be shared with program code, only 1MB is available for it. (in reality the available space as of this version is closer to 1.6MB, but this may change in the future as program code grows)
* Instrument count is also pretty low due to memory constraints. 16 Sample and 16 MIDI instruments.
* Sample instrument feedback feature has been removed due to memory constraints.
* Sample fonts support has been removed to save some memory (thou it could be added back).

## Known issues
* Loading a project will leak memory. Current workaround is to do a full system reset upon project exit (done automatically).

## TODO/Improvements/Wishlist
- [ ] Fix memory leak issues and modernize code
- [ ] Add new synth instruments (C64 SID WIP)
- [ ] Create custom PCB
- [ ] Improve SDIO performance and explore further playing samples from SD
- [ ] Further memory savings and bring back some features/instruments
- [ ] Improve performance/make instrument rendering multithreading
- [ ] Add output effects via potentially another Pico, maybe using [pico-dsp](https://github.com/playduck/pico-dsp) or similar
- [ ] USB mass storage for writing samples into SD
- [ ] MIDI In
- [ ] USB MIDI

## BUILD (WIP)
Head over to the [Build Guide](docs/BUILD.md)

## MANUAL
Head over to the [Manual](docs/MANUAL.md)

## Development
Head over to the [Developer Guide](docs/DEV.md)

## Want to know more
I posted an article about the development of the picoTracker at [democloid.com](http://democloid.com/2023/04/20/picoTracker.html)
