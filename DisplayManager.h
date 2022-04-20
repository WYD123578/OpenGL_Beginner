#pragma once
#include <Display.h>

class DisplayManager {
private:
	Display* mDisplay;

public:
	
	DisplayManager() : mDisplay(nullptr) {};

	void CreateDisplay(Display* _display);
	void UpdateDisplay();
	void CloseDisplay();
};