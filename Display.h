#pragma once

class DisplayMode {
public:
	int width;
	int height;

	DisplayMode() : width(0), height(0) {}
	DisplayMode(int w, int h) : width(w), height(h){}

};

class ContextAttri {
public:
	int major;
	int minor;
	bool useProfileCore;

	ContextAttri() : major(0), minor(0), useProfileCore(false){}
	ContextAttri(int _major, int _minor) : major(_major), minor(_minor), useProfileCore(false){}
	ContextAttri(int _major, int _minor, bool _useProfileCore) : major(_major), minor(_minor), useProfileCore(_useProfileCore) {}
};

class Display {
public:
	void Create(ContextAttri attr);
	void Update();
	void Close();
};