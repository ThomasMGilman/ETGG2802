#include <stdafx.h>
#include <Camera.h>

#pragma once
class InputManager
{
private:

	std::set<int> keySet;
	bool mouseLook = true;

protected:

	void check_keys_down(float elapsed);

	/// Handles Key Down events
	void key_down(int k, float elapsed);

	/// Handles Key Up events
	void key_up(int k, float elapsed);

	/// Handles Mouse Button Down events
	void mouse_button_down(float elapsed);

	/// Handles Mouse Button Up events
	void mouse_button_up(float elapsed);

	/// Handles Mouse Movement events
	void mouse_motion(SDL_MouseMotionEvent motion, float elapsed);

public:

	InputManager();

	~InputManager();

	/// Polls SDL event handleing and handles the work
	void update(float elapsed);
};