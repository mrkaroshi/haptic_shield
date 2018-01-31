Haptic Shield cheatsheet:

'd' - debug mode, will print out all changes

'eX' - enable a driver, X is the number and they go in order left to right of '8, 4, 2, 1, 128, 64, 32, 16' (don't ask why)

'p' - puts selected driver into playback mode, and it will accept values of voltage level

'rX' - sets the voltage level of a driver output, 128 being the midpoint (highest)

'iX' - puts the selected driver into trigger mode, X is the number code for the waveform you want to set (see the datasheet). If you don't set X it will default to either 16 or the last used.

't' - triggers the enabled driver provided it has been selected with 'eX'

Troubleshooting:

The serial configuration as it currently stands is 230400 baud, even parity, 8 data bits with carriage return (CR) and new line (LN) needed.

It's been noted that communications sent with less than 2 milliseconds between them can cause unusual behaviour in the Haptic shield script. Because of this, with this current build, it's advised to group and initialise your haptic drivers in advance then trigger them as needed.
