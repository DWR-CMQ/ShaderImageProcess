#include <iostream>
#include <vector>
#include <iostream>
#include "core/ps_window.h"


int main() 
{
	const std::string& windowTitle = "PS";
	constexpr auto majorVersion = 3;
	constexpr auto minorVersion = 3;

	PSWindow win;
	if (!win.CreateOpenglWindow(windowTitle, majorVersion, minorVersion, false))
	{
		std::cout << "Failed to create window with OpenGL context " << majorVersion << "." << minorVersion << "! Shutting down..." << std::endl;
		return 1;
	}
	win.Run();

	return 0;
}
