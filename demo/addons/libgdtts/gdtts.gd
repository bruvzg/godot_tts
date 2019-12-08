tool
extends EditorPlugin

func _enter_tree():
    add_custom_type("TTSDriver", "Resource", preload("res://addons/libgdtts/classes/tts_driver.gdns"), preload("res://addons/libgdtts/icons/icon_t_t_s_driver.svg"))

func _exit_tree():
    remove_custom_type("TTSDriver")
