// System includes
#include <iostream>
#include <vector>
#include <stdio.h>
#include <memory>
#include <list>

// Ftxui includes
#include "ftxui/dom/elements.hpp"
#include "ftxui/screen/screen.hpp"

#include "ftxui/screen/color.hpp"
#include "ftxui/dom/node.hpp"

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"  
#include "ftxui/component/screen_interactive.hpp"

// Application local includes
#include "grid.h"

// Defines constant
#define size 20

using namespace ftxui;

int main(int argc, const char* argv[]) {
	//Make size variable with terminal size on startup
	Grid grid =  Grid(size, size);

	int cell_width = Dimension::Full().dimx/30;
	int cell_height = Dimension::Full().dimy/20;

	int value = 0;
	auto on_click = [&] {value ++;};

	auto cell = [&](const char* t, int i, int j) {
		return 
			Button(" ", on_click, ButtonOption::Animated(Color::Red));
	};

	auto container_V = Container::Vertical({});

	for(int i=0; i<10; i++){
		auto container_H = Container::Horizontal({});
		for(int j=0; j<30; j++){
			container_H->Add(cell("A", i, j));
		}
		container_V->Add(container_H);
	}
	
	auto renderer = container_V;

	auto screen = ScreenInteractive::Fullscreen();

	screen.Loop(renderer);
	getchar();

	return EXIT_SUCCESS;
}
