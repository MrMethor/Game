#include "Controls.h"
#include <fstream>

Controls::Controls() {
	std::ifstream file("saved/user.con");
	if (file.fail())
		file.open("bin/default.con");
	for (int i = 0; i < sizeof(keymap) / sizeof(keymap[0]); i++)
		keymap[i] = file.get();
}