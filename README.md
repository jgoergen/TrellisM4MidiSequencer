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
    would be cool if you could arpeggiate the channels it outputs onto
    some oscillators would be cool for midi cc values
    a microphone sensor for playing drums on surfaces would be cool
    a microphone note detection for midi playback of notes it's picking up
    tilt sensors to cc / mod / osc mod / pitch bend setting would be nice
    a simple 8 bit synth
    a match glitch synth

# Description of workflow
    Note Sequencer -> Keyboard -> Note modifier -> midi output
          /\            |
           |____________|