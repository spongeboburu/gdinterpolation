extends Sprite

var Interpolation = preload("res://interpolation.gdns")
var interpolation = null;


func _init():
	interpolation = Interpolation.new()
	set_process(false)

func _ready():
	pass

