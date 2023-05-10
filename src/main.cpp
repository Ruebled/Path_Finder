// System includes
#include <functional>
#include <iostream>
#include <vector>
#include <stdio.h>
#include <memory>
#include <list>

// Ftxui includes
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/screen.hpp"

#include "ftxui/dom/node.hpp"
#include "ftxui/dom/canvas.hpp"
#include "ftxui/dom/elements.hpp"

#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component_options.hpp"  
#include "ftxui/component/screen_interactive.hpp"

// Application local includes
#include "grid.h"

//// Defines constant

ftxui::Canvas matrix_to_canvas(Grid grid, int size_x, int size_y){

	int rows = grid.get_rows();
	int cols = grid.get_cols();

	//block sizes
	int dim_x = 15;
	int dim_y = 12;

	auto canvas = ftxui::Canvas(cols*dim_x+2, rows*(dim_y+1));

	// Draw the Main axis from point 0, 0
	canvas.DrawPointLine(0, 0, cols*dim_x, 0, ftxui::Color::White);
   	canvas.DrawPointLine(0, 0, 0, rows*dim_y, ftxui::Color::White);
	//



	// Drawing the grid of lines
	for ( int r = 0; r <= rows; r++){
		for ( int c = 0; c <= cols; c++){
			canvas.DrawPointLine(c*dim_x+dim_x, 
								 0, 
								 c*dim_x+dim_x, 
								 rows*dim_y, 
								 ftxui::Color::White
								 );
		}
		canvas.DrawPointLine(0, 
							 r*dim_y+dim_y, 
							 cols*dim_x, 
							 r*dim_y+dim_y, 
							 ftxui::Color::White
							 );
	}

	// Fill the cells with colors
	for ( int r = 0; r < rows; r++){
		for ( int c = 0; c < cols; c++){
			int color_ind = grid.get_index(r, c);

			ftxui::Color cell_color;

			switch(color_ind){
				case 1:
					cell_color = ftxui::Color::Red;
					break;
				case 2:
					cell_color = ftxui::Color::Green;
					break;
				case 3:
					cell_color = ftxui::Color::Blue;
					break;
				default:
					cell_color = ftxui::Color::GrayDark;
					break;
			}

			//if(color_ind != 1 && color_ind != 2 && color_ind != 3){
			//	continue;
			//}

			if(color_ind == 0){
				continue;
			}

			for ( int l = 0; l <= dim_y-2; l++){
				// r = rows
				// c = cols

				canvas.DrawPointLine(c*dim_x+2, 
									 r*dim_y+1+l, 
									 c*dim_x+dim_x-2, 
									 r*dim_y+1+l, 
									 cell_color);
			}
		}
	}

	return canvas;
}
 
int main(int argc, const char* argv[]) {

	using namespace ftxui;

	//Create the screen and calculate grid size
	auto screen = ScreenInteractive::Fullscreen();

	int cell_x = 20;
	int cell_y = 14;

	Grid grid = Grid(cell_y, cell_x);

	auto grid_renderer = Renderer( [&] { 
		return canvas(matrix_to_canvas(grid, cell_y, cell_x));
	});

	auto grid_with_mouse = CatchEvent(grid_renderer, [&](Event e) {
		if (e.is_mouse()) {
			auto &mouse = e.mouse();
			auto row = mouse.y;//watch for any subtracting
			auto col = mouse.x;//watch for any subtracting
			grid.on_mouse_event(/**/);
		}
		return false;
	});

	int value = 0;

	auto start_button = Button("Start", [&] { value++; });
	auto reset_button = Button("Reset", [&] { value++; });
	auto clear_button = Button("Clear", [&] { value++; });

	auto buttons = Container::Horizontal({start_button, reset_button, clear_button});

	//
	auto components = CatchEvent(Container::Horizontal({grid_with_mouse, buttons}), [&](const Event &e){
		grid.on_refresh_event();
		return false;
	});

	auto console_renderer = Renderer(components, [&] {
		return hbox({ 
			grid_with_mouse->Render() | border,
			flex_grow(vbox({
				hbox({ text("Welcome to Path Finder") })|center,
				separator(),
				filler(),
				hbox({ buttons->Render() }) | center
			})|border)
		});
	});
	

	screen.Loop(console_renderer);
 
	return 0;
}
