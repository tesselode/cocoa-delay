knobRadius = .43333333
scale = 64
buffer = .25

color =
	body: {218/255, 229/255, 242/255}
	gray: {58/255, 84/255, 110/255}
	accent: {52/255, 131/255, 209/255}

drawKnob = (angle, originAngle) -> with love.graphics
	angle = math.rad angle
	originAngle = math.rad originAngle
	.push 'all'
	.setColor color.body
	.circle 'fill', .5, .5, knobRadius, 64
	.setColor color.gray
	.setLineWidth 1/8
	.circle 'line', .5, .5, knobRadius, 64
	.setColor color.accent
	.circle 'fill', .5 + .2 * math.cos(angle), .5 + .2 * math.sin(angle), 1/16, 64
	.arc 'line', 'open', .5, .5, knobRadius, angle, originAngle, 64
	.pop!

drawKnobStrip = (originAngle) -> with love.graphics
	.push 'all'
	.translate buffer, buffer
	for angle = -215, 45, 5
		drawKnob angle, originAngle
		.translate 0, 1 + 2 * buffer
	.pop!

exportKnobStrip = (originAngle, filename) -> with love.graphics
	frames = (45 + 215) / 5 + 1
	width = scale * (1 + 2 * buffer)
	height = scale * (frames * (1 + 2 * buffer))
	canvas = .newCanvas width, height,
		msaa: 16
	canvas\renderTo ->
		.push 'all'
		.scale scale
		drawKnobStrip originAngle
		.pop!
	canvas\newImageData!\encode 'png', filename .. '.png'

exportKnobStrip -215, 'knob left'
exportKnobStrip -90, 'knob middle'
exportKnobStrip 45, 'knob right'

love.draw = -> with love.graphics
	.push 'all'
	.scale 64
	drawKnobStrip -215
	.pop!