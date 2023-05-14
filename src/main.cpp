// System includes
#include <iostream>
#include <vector>
#include <stdio.h>
#include <memory>
#include <list>
#include <thread> //Implement multithreading version
#include <chrono>


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
#include "Grid.h"
#include "Tracer.h"

//// Defines constant
///Block(cell) sizes in "braille dots"
#define dim_x 15
#define dim_y 12

// Cell color enum
ftxui::Color Color_cell[] = { ftxui::Color::Black,
							  ftxui::Color::Red,
							  ftxui::Color::Green,
							  ftxui::Color::Blue,
							  ftxui::Color::GrayDark,
							  ftxui::Color::YellowLight
							};

ftxui::Canvas matrix_to_canvas(Grid grid){
	int rows = grid.height();
	int cols = grid.width();

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

			ftxui::Color cell_color = Color_cell[color_ind];

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

	// Define grid with default sizes
	Grid grid;

	auto grid_renderer = Renderer( [&] { 
		return canvas(matrix_to_canvas(grid));
	});

	auto y_calc = ( [&] (int cols) {
		if(cols>0){
			return (cols-1)/3;
		}
		return cols;
	});

	auto x_calc = ( [&] (int rows) {
		int rows_temp = rows - 1;
		int rows_div = (rows_temp / 15) * 2;
		int rows_mod = rows_temp % 15;

		if(rows_mod > 7) rows_div++;
		return rows_div;	
	});

	auto text_temp = ftxui::text("");

	auto grid_with_mouse = CatchEvent(grid_renderer, [&](Event e) {
		if (e.is_mouse()) {
			auto &mouse = e.mouse();
			auto col_pixel = x_calc(mouse.x);//watch for any subtracting
			auto row_pixel = y_calc(mouse.y);//watch for any subtracting

			// Exit if mouse in the left window
			if(col_pixel>19) return false;
			if(row_pixel>13) return false;

			// change the position of the mouse and display
			char str[20];
			sprintf(str, "%d %d %d", row_pixel, col_pixel, mouse.motion);
			text_temp = ftxui::text(str);	

			grid.on_mouse_event(row_pixel, 
								col_pixel, 
								mouse.button == Mouse::Left,
				   				mouse.motion
								);
		}
		return false;
	});

	int value = 0;

	int selected = 0;

	// The list for algorithms
	std::vector<std::string> algorithms_name = {
		"A*",
		"Djikstra",
		"Placeholder"
	};

	auto menu = Container::Vertical({ Radiobox(&algorithms_name, &selected) });

	auto start_button = Button("Start", [&] { grid.solve(selected); });
	auto reset_button = Button("Reset", [&] { grid.reset(); });
	auto clear_button = Button("Clear", [&] { grid.clear(); });

	auto buttons = Container::Horizontal({start_button, reset_button, clear_button});

	//
	auto components = Container::Horizontal({grid_with_mouse, buttons, menu});

	auto console_renderer = Renderer(components, [&] {
		return hbox({ 
			grid_with_mouse->Render() | border,
			flex_grow(vbox({
				hbox({ text("Welcome to Path Finder") })|center,
				hbox({
						text("Coord: [Y] [X]: "), 
						text_temp
					 })|center,
				separator(),
				menu->Render()|border,
				filler(),
				hbox({ buttons->Render() }) | center
			})|border)
		});
	});

	// Threading the refresh ui program
	std::atomic<bool> refresh_ui_continue = true;
	std::thread refresh_ui([&] {
		while (refresh_ui_continue) {
			std::this_thread::sleep_for(std::chrono::milliseconds(100));
			screen.PostEvent(Event::Custom);
		}
	});

	screen.Loop(console_renderer);
	refresh_ui_continue = false;
	refresh_ui.join();
 
	return 0;
}
