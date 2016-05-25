#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include "cscene.h"
#include "cparser.h"

int main(int argc, char** argv) {
	bool gpu_process = false;
	bool testing = false;
	bool fullscreen = false;
	int width = 800;
	int height = 800;
	double backlight = 0.0;
	std::string filename = "";

	if(argc > 1) {
		for(int i = 0; i < argc; ++i) {
			if(strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--gpu") == 0)
				gpu_process = true;
			else if((strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--height") == 0) && argc > i + 1) {
				height = atoi(argv[i + 1]);
				++i;
			}
			else if((strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0) && argc > i + 1) {
				height = atoi(argv[i + 1]);
				++i;
			}
			else if(strcmp(argv[i], "--fullscreen") == 0) {
				fullscreen = true;
			}				
			else if(strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--enable-testing") == 0)
				testing = true;
			else if((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) && argc > i + 1) {
				filename = std::string(argv[i + 1]);
				++i;
			}
			else if(strcmp(argv[i], "--backlight") == 0 && argc > i + 1) {
				backlight = atof(argv[i + 1]);
				if(backlight < 0) backlight = 0;
				if(backlight > 100) backlight = 100;
				++i;
			}

		}
	}

	if(filename == "") {
		std::cerr << "Filename not specified! Use -f or --file to open scene file." << std::endl;
		exit(0);
	}

	SDL_Init(SDL_INIT_VIDEO);

	if(fullscreen) {
		SDL_DisplayMode mode;;
		if(SDL_GetCurrentDisplayMode(0, &mode) != 0) {
			std::cerr << "[EE]: Unable to get display mode." << std::endl;
			return -1;
		}
		width = mode.w;
		height = mode.h;
	}

	std::chrono::steady_clock::time_point t1;
	if(testing) t1 = std::chrono::steady_clock::now();

	CScene scene(width, height);
	CRTParser parser;
	scene.load_file(&parser, filename);
	scene.render(backlight, gpu_process, testing, fullscreen);

	if(testing) {
		std::chrono::steady_clock::time_point t2 = std::chrono::steady_clock::now();
		std::chrono::duration<double> time_span = std::chrono::duration_cast<std::chrono::duration<double>>(t2 - t1);
		std::cout << "All working time: " << time_span.count() << "s" << std::endl;
	}

	return 0;
}
