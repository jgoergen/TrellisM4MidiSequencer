# TrellisM4MidiSequencer
A Midi Instrument Firmware For the Adafruit Trellis M4

# Button layout
All buttonss are capable of 3 forms of user input: press, double press.

The bottom right red blinking button is used for switching between a mode / settings and the mode select.

# Mode / Setting buttons
1/1 - 2/8 are used to select between modes
    1/1: Note sequencer
    1/2: Keyboard
    1/3: Note modifier ( arp / chords )

3/1 - 3/8 are used for mode specific settings
    Found under mode details

4/1 - 4/8 are used for global settings
    4/1: Tempo in 5 bpm increments from 60 to 215
    4/2: Enable / Disable notes. Choose base octave. setup chords

# Todo:
    need a way to change what the tilt sensors do
    would be cool if you could arpeggiate the channels it outputs onto

# Description of workflow
    Note Sequencer -> Keyboard -> Note modifier -> midi output
          /\            |
           |____________|