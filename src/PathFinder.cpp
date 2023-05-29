#include "screen.h"
// Global variables
// CPU time benchmark 
extern float cpu_time;

// Real time benchmark
extern float real_time;

// Distance of the found path
extern int distance_path;

// Thread status
extern bool thread_active;

// Terminal state save var
struct termios Original_Termios;

// UI Thread refresh state
std::atomic<bool> refresh_ui_continue;

// Variable for storing if diagonals will be used
extern bool diag_checked;

// main function of the application
int main(int argc, const char* argv[]) {
	//Save terminal state for latter restoring 
	tcgetattr(STDIN_FILENO, &Original_Termios);

	using namespace ftxui;

	//Create the screen and calculate grid size
	auto screen = ScreenInteractive::Fullscreen();

	// Define grid default size
	Grid grid;

	// Return the grid converted to ftxui canvas 
	auto grid_renderer = Renderer( [&] { 
		return canvas(matrix_to_canvas(grid, screen));
	});

	// Convert ftxui terminal y mouse pointer location to matrix grid unit
	auto screen_to_cell_y = ([&] (int cols) {
		int dim_y = (int)((screen.dimy()*4)/(grid.height()+1));

		if(cols>0){
			return (cols-1)/(screen.dimy()/dim_y);
		}
		return cols;
	});

	// Convert ftxui terminal x mouse pointer location to matrix grid unit
	auto screen_to_cell_x = ([&] (int rows) {
		int dim_x = (int)(screen.dimx()*1.6)/grid.width();

		int rows_temp = rows - 1;
		int rows_div = (rows_temp / dim_x) * 2;
		int rows_mod = rows_temp % dim_x;

		if(rows_mod > 7) rows_div++;
		return rows_div;	
	});

	auto coord_temp = ftxui::text("");

	auto grid_with_mouse = CatchEvent(grid_renderer, [&](Event e) {
		if (e.is_mouse()) {
			auto &mouse = e.mouse();
			// Compute mouse position reffering to grid
			auto col_pixel = screen_to_cell_x(mouse.x);
			auto row_pixel = screen_to_cell_y(mouse.y);

			// Don't trigger mouse_event if mouse not on canvas
			if(col_pixel>19 || row_pixel>13) return false;

			// Create a string reflecting mouse position on canvas
			char str[6];
			sprintf(str, "%d %d", row_pixel, col_pixel);
			coord_temp = ftxui::text(str);	

			grid.on_mouse_event(row_pixel, 
								col_pixel, 
								mouse.button == Mouse::Left,
				   				mouse.motion/*mouse pressed/unpresed state*/
								);

		}
		return false;
	});

	// Radio buttons for choosing pathfinding algorithms
	int selected = 0;

	// The list for algorithms
	std::vector<std::string> algorithms_name = {
		"Breadth First Search",
		"Depth First Search",
		"Djikstra",
		"A Star"
	};

	// Define checkbox for chosing diagonal finding
	std::string diag_str = "Allow diagonal";

	auto menu = Container::Vertical({ Radiobox(&algorithms_name, &selected),
									  Checkbox(&diag_str, &diag_checked)});

	// Define the buttons and their functions
	auto button_style = ButtonOption::Animated(Color::Default, Color::GrayDark,
                                             Color::Default, Color::White);

	auto start_button = Button("Start", 
							   [&] { if(!thread_active){ grid.solve(selected); } }, 
							   &button_style
							  )|bold;

	auto reset_button = Button("Reset", 
							   [&] { if(!thread_active){ grid.reset(); } }, 
							   &button_style
							  )|bold;

	auto clear_button = Button("Clear", 
							   [&] { if(!thread_active){ 
								   grid.clear(Type(path)); 
								   grid.clear(Type(wall));
							   } }, 
							   &button_style
							  )|bold;

	auto map_button = Button("Map",
							   [&] { if(!thread_active){ grid.draw_map(); } },
							   &button_style
							  )|bold;

	// Generate shorter string from float(fractional digits less)
	auto toShorterFloat = [](float time, int len) {
			std::string real_time_string = std::to_string(time);
			std::string real_time_string_trunc (
					real_time_string.begin(),
					real_time_string.begin()+len
					);
			return text(real_time_string_trunc);
	};
	
	// Instructions for using the game
	Element instructions_en = vbox({
		paragraph("This Project is visual implementation of some of the most known path finding algorithms."),
		paragraph("Starting(red) and ending(green) point can be seen when opening the app."),
		paragraph("Those can be drag and droped to change their position."),
		paragraph("    Clicking and drawing on the empty cell will draw a wall(blue), that is working as a barrier for the path to be found."),
		paragraph("    The algorithm used for path finding can be chosen from bellow by selecting the apropriate radiobutton."),
		paragraph("    After pressing START, the path will be shown in YELLOW. And 'checked' cell in DARK-GRAY.")
	});

	auto buttons = Container::Horizontal({
			start_button,
		   	reset_button,
		   	clear_button,
			map_button
			});

	auto components = ftxui::Container::Horizontal({grid_with_mouse,  buttons, menu});

	auto console_renderer = Renderer(components, [&] {
		return hbox({ 
			grid_with_mouse->Render() | border,

			flex_grow(vbox({
				hbox({ text("Welcome to Path Finder") })|center|bold,
				hbox({
						text("Coord: [Y] [X]: "), 
						coord_temp
					 })|center,
				separator(),
				vbox({
						text("Quick Introduction")|center,
						instructions_en|borderEmpty|center
						}),
				separator(),
				text("PathFinding Algorithms")|center|bold,
				separatorEmpty(),

				hbox({
					separatorEmpty(),
					separatorEmpty(),
					menu->Render()
					}),
				filler(),
				separator(),

				vbox({
						text("Performance") | center,
						hbox({
							text("CPU time: "),
							toShorterFloat(cpu_time, 7),
							text(" ms")
						})|center,
						hbox({
							text("Real time: "),
							toShorterFloat(real_time, 5),
							text(" s")
						})|center,
						hbox({
							text("Distance: "),
							text(std::to_string(distance_path))
						})|center,
					}),
				separator(),
				hbox({ buttons->Render()}) | center | focus

			})|border)
		});
	});

	// Maps clear key event
	console_renderer |= CatchEvent([&](Event key) {
		if(key.is_character()){
			if(key == Event::Character('R')){
				grid.map_clear(); 
			}
			if(key == Event::Character('S')){
				grid.map_save(); 
			}
			if(key == Event::Character('Q')){
				// Stop UI refresh
				refresh_ui_continue = false;	
				std::cout << "\033[2J\033[1;1H";
				std::cout << "\033\143";
				tcsetattr(STDIN_FILENO, TCSANOW, &Original_Termios);
				std::exit(EXIT_SUCCESS);
			}
		}	
		return false;
	});

	// Threading the refresh ui program
	refresh_ui_continue = true;
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
