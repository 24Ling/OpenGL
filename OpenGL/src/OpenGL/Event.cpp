#include "Event.h"
#include <iostream>



bool Event::EventCallBack(KeyType keycode)
{
	GLFWwindow* currentWindow = glfwGetCurrentContext();

	auto status = glfwGetKey(currentWindow, keycode);

	return status;
}

