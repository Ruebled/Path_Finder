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
///Block(cell) sizes
#define dim_x 15
#define dim_y 12

ftxui::Canvas matrix_to_canvas(Grid grid){
	int rows = grid.get_rows();
	int cols = grid.get_cols();

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

	int cell_x = 20; // cell on x axis of the grid
	int cell_y = 14; // cell on y axis of the grid

	Grid grid = Grid(cell_y, cell_x);

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
		int rows_div = (rows_temp/15)*2;
		int rows_mod = rows_temp%15;

		if(rows_mod > 7) rows_div++;
		if(rows_div > 19) return 19;
		return rows_div;	
	});

	auto text_temp = ftxui::text("fas");


	auto grid_with_mouse = CatchEvent(grid_renderer, [&](Event e) {
		if (e.is_mouse()) {
			auto &mouse = e.mouse();
			auto col_pixel = x_calc(mouse.x);//watch for any subtracting
			auto row_pixel = y_calc(mouse.y);//watch for any subtracting

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


	auto start_button = Button("Start", [&] { value++; });
	auto reset_button = Button("Reset", [&] { grid.reset(); });
	auto clear_button = Button("Clear", [&] { grid.clear(); });

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
				text_temp|border,
				separator(),
				filler(),
				hbox({ buttons->Render() }) | center
			})|border)
		});
	});
	

	screen.Loop(console_renderer);
 
	return 0;
}
