extends Node2D

var Interpolation = preload("res://interpolation.gdns")
var InterpolatedSprite = preload("res://InterpolatedSprite.gd")

var names = [
	"None",
	"Linear",
	"Square",
	"Cubic",
	"Bezier",
	"Sine",
	"Boolean",
	"Ease back",
	"Ease bounce",
	"Ease circ",
	"Ease cubic",
	"Ease elastic",
	"Ease expo",
	"Ease quad",
	"Ease quart",
	"Ease quint",
	"Ease sine"
]

var sprites = []

func _ready():
	var tex = load("res://icon.png")

	for i in range(17):
		var label = Label.new();
		label.rect_position = Vector2(16, 32 + i * 64)
		label.text = names[i]
		label.rect_scale = Vector2(4, 4)
		add_child(label)
		var sprite = InterpolatedSprite.new()
		sprite.position = Vector2(256, 32 + i * 64)
		sprite.texture = tex
		add_child(sprite)
		sprite.interpolation.function = i
		sprite.interpolation.loop = true
		sprite.interpolation.pingpong = true
		sprite.interpolation.duration = 2
		sprite.interpolation.start = sprite.position
		sprite.interpolation.end = sprite.position + Vector2(512, 0)
		sprites.append(sprite)

func _process(delta):
	# Do this in the sprite's _process if you don't need em synched.
	for sprite in sprites:
		sprite.interpolation.process(delta)
		sprite.position = sprite.interpolation.get_value()

