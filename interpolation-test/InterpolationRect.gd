extends ColorRect

var Interpolation = preload("res://interpolation.gdns")

var interpolation = null;
var label = null;

func _init():
	interpolation = Interpolation.new()

func _ready():
	pass # Replace with function body.

func _draw():
	var size = Vector2(rect_size.x / 2 - 2, rect_size.y - 2)
	draw_rect(Rect2(1, 1, size.x, size.y), Color(0, 0, 0, 1), true)
	var resolution = rect_size.x / 2	
	var prev = Vector2(1, size.y * 0.9 - size.y * 0.8 * interpolation.get_factor_for_timestep(0))
	for i in range(1, resolution + 1):
		var f = (float(i) / resolution)
		var ipol = interpolation.get_factor_for_timestep(f * interpolation.duration)
		var current = Vector2(1 + f * size.x, size.y * 0.9 - (size.y * 0.8) * ipol)
		if interpolation.function == 3:
			var fpre = f - 0.001
			var fnext = f + 0.001
			if fpre < 0:
				fpre = 0
			if fnext > 1.0:
				fnext = 1.0
			var ipolpre = interpolation.get_factor_for_timestep(fpre * interpolation.duration)
			var ipolnext = interpolation.get_factor_for_timestep(fnext * interpolation.duration)
			var pre = Vector2(1 + fpre * size.x, size.y * 0.9 - (size.y * 0.8) * ipolpre)
			var next = Vector2(1 + fnext * size.x, size.y * 0.9 - (size.y * 0.8) * ipolnext)
			draw_line(prev, pre, Color(1,1,1,1), 1.0, true)
			draw_line(current, next, Color(1,1,1,1), 1.0, true)
			draw_line(pre, current, Color(1,1,1,1), 1.0, true)
			prev = next
		else:
			draw_line(prev, current, Color(1,1,1,1), 1.0, true)
			prev = current
	var ipol = interpolation.get_factor()
	var f = fmod(interpolation.timestep / interpolation.duration, 1.0)
	draw_circle(Vector2(1 + size.x * f, size.y * 0.9 - (size.y * 0.8) * ipol), 8, Color(1, 1, 0, 1))
	draw_rect(Rect2(rect_size.x / 2 + 2, 2, rect_size.x / 2 - 8, rect_size.y - 4), Color(0, 0, 0, 1), true)
	draw_rect(Rect2(rect_size.x / 2 + 2, 2, (rect_size.x / 2 - 8) * ipol, rect_size.y - 4), Color(1, 1, 1, 1), true)
