extends Node2D

var InterpolationRect = preload("res://InterpolationRect.gd")

var interpolations = []

func _ready():
	var view_width = 1024
	var view_height = 576
	var rect_width = 512
	var rect_height = 36
	var num = 15
	for i in range(num):		
		var r = InterpolationRect.new()
		var l = Label.new()
		l.text = "%s"%r.interpolation.function_name(i)
		l.rect_position = Vector2(view_width / 2 - rect_width / 2 - 64, 24 + view_height / 2 - (rect_height * num) / 2 + i * rect_height)
		add_child(l)
		r.interpolation.function = i
		r.interpolation.operation = 3
		r.interpolation.loop = true
		r.interpolation.duration = 2
		r.rect_position = Vector2(view_width / 2 - rect_width / 2, 16 + view_height / 2 - (rect_height * num) / 2 + i * rect_height)
		r.rect_size = Vector2(rect_width, rect_height)
		r.color = Color(0, 0, 0, 0)
		add_child(r)
		interpolations.append(r)

func _input(event):
	if event is InputEventKey and event.is_pressed():
		if event.scancode >= KEY_1 and event.scancode <= KEY_3:
			pass

func _process(delta):
	# Do this in the sprite's _process if you don't need em synched.
	for r in interpolations:
		r.interpolation.timestep += delta
		r.update()
