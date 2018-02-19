This hardware and software is provided under the MIT License.

Haptic Shield Firmware cheatsheet:

'd' - debug mode, will print out all changes

'eX' - enable a driver, X is the number and they go in order left to right of '8, 4, 2, 1, 128, 64, 32, 16' (don't ask why)

'p' - puts selected driver into playback mode, and it will accept values of voltage level

'rX' - sets the voltage level of a driver output, 128 being the midpoint (highest)

'iX' - puts the selected driver into trigger mode, X is the number code for the waveform you want to set (see the datasheet). If you don't set X it will default to either 16 or the last used.

't' - triggers the enabled driver provided it has been selected with 'eX'
