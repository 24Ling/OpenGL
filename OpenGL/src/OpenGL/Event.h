#pragma once
#include <GL/glew.h>
#include <GLFW/glfw3.h>

enum KeyType;

class Event
{
public:
	Event() {};
	~Event() {};

	bool EventCallBack(KeyType keycode);
};

enum KeyType
{
	HAZEL_KEY_SPACE = 32,
	HAZEL_KEY_0 = 48,
	HAZEL_KEY_1 = 49,
	HAZEL_KEY_2 = 50,
	HAZEL_KEY_3 = 51,
	HAZEL_KEY_4 = 52,
	HAZEL_KEY_5 = 53,
	HAZEL_KEY_6 = 54,
	HAZEL_KEY_7 = 55,
	HAZEL_KEY_8 = 56,
	HAZEL_KEY_9 = 57,
	HAZEL_KEY_A = 65,
	HAZEL_KEY_B = 66,
	HAZEL_KEY_C = 67,
	HAZEL_KEY_D = 68,
	HAZEL_KEY_E = 69,
	HAZEL_KEY_F = 70,
	HAZEL_KEY_G = 71,
	HAZEL_KEY_H = 72,
	HAZEL_KEY_I = 73,
	HAZEL_KEY_J = 74,
	HAZEL_KEY_K = 75,
	HAZEL_KEY_L = 76,
	HAZEL_KEY_M = 77,
	HAZEL_KEY_N = 78,
	HAZEL_KEY_O = 79,
	HAZEL_KEY_P = 80,
	HAZEL_KEY_Q = 81,
	HAZEL_KEY_R = 82,
	HAZEL_KEY_S = 83,
	HAZEL_KEY_T = 84,
	HAZEL_KEY_U = 85,
	HAZEL_KEY_V = 86,
	HAZEL_KEY_W = 87,
	HAZEL_KEY_X = 88,
	HAZEL_KEY_Y = 89,
	HAZEL_KEY_Z = 90
};