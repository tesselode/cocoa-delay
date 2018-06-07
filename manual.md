# Cocoa Delay Manual

## Time section

![Time section screenshot](images\time.png)

- **Delay**
	- **Time** - adjusts the delay time from 10 milliseconds to 2 seconds.
	- **Tempo sync** - sets the delay time to a certain note length based on the tempo of the project.
		- Note: when set to anything other than "Off", this overrides the time knob. The time knob will be grayed out to reflect this.
- **LFO**
	- **Amount** - sets the amount of oscillation to apply to the delay time.
	- **Frequency** - sets the speed of the oscillation from 0.1Hz to 10Hz.
- **Drift**
	- **Amount** - sets the amount of random fluctuation of the delay time.

## Feedback section

![Feedback section screenshot](images\feedback.png)

- **Amount**: sets the amount of the delay signal to feed back into the input.
	- Note: When the knob is turned to the left, the signal will be inverted each time it is fed back into the input. This results in some slight destructive interference, which can make sounds seem less "heavy" or "muddy". Set to taste.
- **Stereo**: sets the difference in delay time between the left and right channels, creating a stereo widening effect.
- **Pan**: sets the amount and direction the audio should be panned when it is fed back into the input.
- **Pan mode**: sets the way audio should be panned.
	- **Static**: the input is panned only once when it is first fed back into the input.
	- **Ping pong**: same as static, but the left and right channels are flipped each time the signal is fed back into the input.
	- **Circular**: the signal is panned further and further each time it is fed back into the input. Sounds will eventually loop back around to the other side. Try it, it's weird sounding!