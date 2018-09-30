extends Node2D

var Interpolation = preload("res://interpolation.gdns")
var InterpolatedSprite = preload("res://InterpolatedSprite.gd")

var sprites = []
var operation = 3

func _ready():
	var tex = load("res://icon.png")
	var x = 480
	var y = 32
	for f in range(16):
		var sprite = InterpolatedSprite.new()
		sprite.position = Vector2(x, y)
		sprite.texture = tex
		add_child(sprite)
		sprite.interpolation.function = f
		sprite.interpolation.operation = operation
		sprite.interpolation.loop = true
		sprite.interpolation.duration = 2
		sprite.interpolation.start = sprite.position
		sprite.interpolation.end = sprite.position + Vector2(1024, 0)
		sprites.append(sprite)
		var label = Label.new();
		label.rect_position = Vector2(16, y)
		label.text = "%s / %s"%[sprite.interpolation.function_name(f), sprite.interpolation.operation_name(operation)]
		label.rect_scale = Vector2(4, 4)
		add_child(label)
		y += 64

func _process(delta):
	# Do this in the sprite's _process if you don't need em synched.
	for sprite in sprites:
		sprite.interpolation.timestep += delta
		sprite.position = sprite.interpolation.get_value()

