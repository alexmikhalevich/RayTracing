#include <iostream>
#include <string>
#include "cscene.h"
#include "cparser.h"

int main(int argc, char** argv) {
	bool gpu_process = false;
	bool testing = false;
	int width = 800;
	int height = 800;
	std::string filename = "";

	if(argc > 1) {
		for(int i = 0; i < argc; ++i) {
			if(strcmp(argv[i], "-g") == 0 || strcmp(argv[i], "--gpu") == 0)
				gpu_process = true;
			else if(strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--height") == 0) {
				height = atoi(argv[i + 1]);
				++i;
			}
			else if(strcmp(argv[i], "-w") == 0 || strcmp(argv[i], "--width") == 0) {
				height = atoi(argv[i + 1]);
				++i;
			}
			else if(strcmp(argv[i], "--fullscreen") == 0) {
				//TODO: fullscreen
			}				
			else if(strcmp(argv[i], "-t") == 0 || strcmp(argv[i], "--enable-testing") == 0)
				testing = true;
			else if(strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
				filename = std::string(argv[i + 1]);
				++i;
			}

		}
	}

	if(filename == "") {
		std::cerr << "Filename not specified! Use -f or --file to open scene file." << std::endl;
		exit(0);
	}

	std::time_t cur_time = std::time(NULL);

	CScene scene(width, height);
	CCustomParser parser;
	scene.load_file(&parser, filename);
	scene.render(gpu_process, testing);

	if(testing) {
		cur_time = std::time(NULL) - cur_time;
		double res = cur_time / CLOCKS_PER_SEC;
		std::cout << "All working time: " << res << "s" << std::endl;
	}
	return 0;
}
