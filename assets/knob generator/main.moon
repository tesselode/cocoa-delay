scale = 64

color =
	body: {196/255, 212/255, 230/255}
	gray: {103/255, 125/255, 147/255}
	accent: {52/255, 131/255, 209/255}

drawKnob = (angle, originAngle) -> with love.graphics
	angle = math.rad angle
	originAngle = math.rad originAngle
	.push 'all'
	.setColor color.body
	.circle 'fill', .5, .5, .5, 64
	.setColor color.gray
	.setLineWidth 1/8
	.circle 'line', .5, .5, .5, 64
	.setColor color.accent
	.circle 'fill', .5 + .2 * math.cos(angle), .5 + .2 * math.sin(angle), 1/16, 64
	.arc 'line', 'open', .5, .5, .5, angle, originAngle, 64
	.pop!

drawKnobStrip = (originAngle) -> with love.graphics
	.push 'all'
	.translate .25, .25
	for angle = -215, 45, 5
		drawKnob angle, originAngle
		.translate 0, 1.25
	.pop!

exportKnobStrip = (originAngle, filename) -> with love.graphics
	frames = (45 + 215) / 5 + 1
	width = scale * 1.5
	height = scale * (frames * 1.25 + .25)
	canvas = .newCanvas width, height,
		msaa: 16
	canvas\renderTo ->
		.push 'all'
		.scale scale
		drawKnobStrip originAngle
		.pop!
	canvas\newImageData!\encode 'png', filename .. '.png'

exportKnobStrip -215, 'left'
exportKnobStrip -90, 'middle'
exportKnobStrip 45, 'right'

love.draw = -> with love.graphics
	.push 'all'
	.scale 64
	drawKnobStrip -215
	.pop!