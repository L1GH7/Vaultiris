extends Node

func _ready():
	pass


func _on_DeSerializeButton_pressed():
	# Crashes the engine
	#bytes2var(PoolByteArray([20, 0, 0, 0, 0, 0, 0, 255]))
	
	# Put data into memory
	#bytes2var(PoolByteArray([23, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 72, 101, 108, 108, 111, 32, 119, 111, 114, 108, 100, 33]))
	#bytes2var(PoolByteArray([23, 0, 0, 0, 1, 0, 0, 0, 3, 0, 0, 0, 116, 101, 97]))
	# Try to read data that still exists in memory until it is null terminated
	#bytes2var(PoolByteArray([23, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 255]))
	
	# Serialize data that does not have a size which is a multiple of 4
	var2bytes(PoolByteArray([1, 2, 3, 4, 1, 2, 3, 4, 1, 2]))
