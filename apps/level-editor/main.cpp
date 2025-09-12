
#include "editor.h"

int main(int argc, char* argv[])
{
	// Initialization
	//--------------------------------------------------------------------------------------
	const int screen_width = 1920;
	const int screen_height = 1080;

    Editor editor = Editor("LaeVo - Editor", screen_width, screen_height);
    editor.init();
    editor.run();

	return 0;
}