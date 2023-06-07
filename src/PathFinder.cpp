#include "screen.h"

// Global variables
// CPU time benchmark 
extern float cpu_time;

// Real time benchmark
extern float real_time;

// Distance of the found path
extern int distance_path;

// Cost of the found path
extern float cost_path;

// Thread status
extern bool thread_active;

// Terminal state save var
struct termios Original_Termios;

// UI Thread refresh state
std::atomic<bool> refresh_ui_continue;

// Variable for storing if diagonals will be used
extern bool diag_checked;

bool diagonal_checked;

// Draw window flag
int depth = 2;

// Colors for different tiles
extern ftxui::Color Color_cell[];

// Radiobutton selector
int selected = 0;

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

	// ftxui variable to store pointer current coordinate
	auto coord_temp = ftxui::text("");

	auto grid_with_mouse = CatchEvent(grid_renderer, [&](Event e) {
		if (e.is_mouse()) {
			auto &mouse = e.mouse();
			// Compute mouse position reffering to grid
			auto col_pixel = screen_to_cell_x(mouse.x);
			auto row_pixel = screen_to_cell_y(mouse.y);

			// Create a string reflecting mouse position on canvas
			char str[6];

			// Don't reupdate mouse position if mouse not on canvas
			if(!(col_pixel>=grid.width() || row_pixel>=grid.height())) {
				// Rewrite the mouse indicator
				sprintf(str, "%d %d", row_pixel+1, col_pixel+1);
				coord_temp = ftxui::text(str);	
			}

			grid.on_mouse_event(row_pixel, 
								col_pixel, 
								mouse.button == Mouse::Left,
				   				mouse.motion/*mouse pressed/unpresed state*/
								);

		}
		return false;
	});

	// Radio buttons for choosing pathfinding algorithms

	// The list for algorithms
	std::vector<std::string> algorithms_name = {
		"Breadth First Search",
		"Dijkstra",
		"A Star"
	};

	auto algo_select_menu = Container::Vertical({ Radiobox(&algorithms_name, &selected)});

	// Define checkbox for chosing diagonal finding
	std::string diag_str = "Allow diagonal";

	auto diagonal_check = Container::Vertical({ Checkbox(&diag_str, &diagonal_checked)});

	// Define the buttons and their functions
	auto button_style = ButtonOption::Animated(Color::Default, Color::GrayDark,
                                             Color::Default, Color::White);

	auto start_button = Button("Start", 
							   [&] { if(!thread_active){ 
								   diag_checked = diagonal_checked; 
								   grid.solve(selected); 
							   } }, 
							   &button_style
							  )|bold;

	auto reset_button = Button("Reset", 
							   [&] { if(!thread_active){ grid.reset(); } }, 
							   &button_style
							  )|bold;

	auto clear_button = Button("Clear", 
							   [&] { if(!thread_active){ 
							       depth = 3;
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
	
	auto buttons = Container::Horizontal({
			start_button,
		   	reset_button,
		   	clear_button,
			map_button
			});

	auto components = ftxui::Container::Horizontal({grid_with_mouse, buttons, algo_select_menu, diagonal_check});

	// Instructions for main window
	ftxui::Element instructions_en = ftxui::vbox({
			ftxui::paragraph("This Project is visual implementation of some of the most known path finding algorithms.") | ftxui::center,
			ftxui::separatorEmpty(),
			ftxui::paragraph("====Press H for more====")|ftxui::center,
	});
	
	auto main_screen_renderer = Renderer(components, [&] {
		if(screen.dimx()!=screen_x || screen.dimy()!=screen_y){
			return hbox({ grid_with_mouse->Render() | border });
		}

		return hbox({ 
			grid_with_mouse->Render() | border,

			flex_grow(vbox({
				separatorEmpty(),
				hbox({ text("Welcome to Path Finder") })|center|bold,
				hbox({
						text("Coord: [Y] [X]: "), 
						coord_temp
					 })|center,
				separatorEmpty(),
				separator(),
				filler(),
				vbox({
						text("Quick Introduction") | bold | center,
						separatorEmpty(),
						instructions_en|borderEmpty|center
						}),
				filler(),
				separator(),
				text("PathFinding Algorithms")|center|bold,
				separatorEmpty(),

				vbox({
					algo_select_menu->Render(),
					separatorEmpty(),
					diagonal_check->Render()
					}) | borderEmpty,
				separator(),

				vbox({
						text("Performance") | center,
						hbox({
							text("CPU time: "),
							toShorterFloat(cpu_time, 5),
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
						hbox({
							text("Cost: "),
							toShorterFloat(cost_path, 6),
						})|center,
					}),
				separator(),
				hbox({ buttons->Render()}) | center | focus

			})|border)
		});
	});

	// Tiles types selecting modal
	// depth 1
	std::vector<std::string> types_of_tiles = {
		"    Wall     ",
		"    Sand     ",
		"    Woods    ",
		"    Water    ",
		"  Mountains  ",
	};

	// Think of this
	auto button_tile_style_wall = ButtonOption::Animated(Color_cell[Type(wall)], Color::Black, Color::Default, Color_cell[Type(wall)]);
	auto button_tile_style_sand = ButtonOption::Animated(Color_cell[Type(sand)], Color::Black, Color::Default, Color_cell[Type(sand)]);
	auto button_tile_style_woods = ButtonOption::Animated(Color_cell[Type(woods)], Color::Black, Color::Default, Color_cell[Type(woods)]);
	auto button_tile_style_water = ButtonOption::Animated(Color_cell[Type(water)], Color::Black, Color::Default, Color_cell[Type(water)]);
	auto button_tile_style_mountains = ButtonOption::Animated(Color_cell[Type(mountains)], Color::Black, Color::Default, Color_cell[Type(mountains)]);

	auto tiles_selector_container = Container::Vertical({
		Button(&types_of_tiles[0], [&] { grid.choose_tile(Type(wall)); depth = 0; }, &button_tile_style_wall)|center,
		Button(&types_of_tiles[1], [&] { grid.choose_tile(Type(sand)); depth = 0; }, &button_tile_style_sand)|center,
		Button(&types_of_tiles[2], [&] { grid.choose_tile(Type(woods)); depth = 0; }, &button_tile_style_woods)|center,
		Button(&types_of_tiles[3], [&] { grid.choose_tile(Type(water)); depth = 0; }, &button_tile_style_water)|center,
		Button(&types_of_tiles[4], [&] { grid.choose_tile(Type(mountains)); depth = 0; }, &button_tile_style_mountains)|center,
	});

	auto tiles_selector_modal_renderer = Renderer(tiles_selector_container, [&] {
		return vbox({
				text("Select the tile"),
				separator(),
				vbox(tiles_selector_container->Render()),
			}) | border;
	});

	// Help(instruction) window
	std::vector<std::string> help_text = {
		"This app implement some popular pathfinding algorithms in an interactive way.",

		"Drawing the map",
		"Using *rigth click* an window with clickable elements(types of tiles for choosing) will appear at the center of the screen",
		"The drawing is also done using the mouse",
		"To move the",
		"or",
		"points tiles, click, move and drop can be used",

		"Algorithms menu",
		"From the radio buttons list an algorithm for testing can be choosen",

		"Buttons",
		"Start - is used to start the path finding algorithm choosen",
		"Reset - is for reseting initial state of the board",
		"Clear - when clicked an window with:",
		"     *Clear path* - clears the path remained after the pathfinding",
		"     *Clear all* - clears all the tiles but the *start* and *end* points",
	   	"Map - is used to retrieve the maps saved before",

		"Path finding process",
		"After presssing the *Start* button, with a defined delay, the tiles checked by the algorithm will be colored in _gray_.",
		"When the path is found the algorithms stop checking any other tiles, and start to draw backwards the path found, then reverse visited tiles the way checked",
		"Then *real time* and the *CPU time* that the algorithm spent is displayed above the buttons.",

		"====Use K for list of shortcuts====",
		"----Use ESC button to close the window----",
		"start",
		"end",
	};
	

	auto help_window_renderer = Renderer( [&] {
		return vbox({
				text("The HELP!") | center | bold,
				separator(),
				vbox({
					paragraph(help_text[0]),
					separatorEmpty(),
					paragraph(help_text[1]) | bold,
					paragraph(help_text[2]), 	
					paragraph(help_text[3]), 	
					hbox({
						paragraph(help_text[4]),
						paragraph(help_text[22]) | color(Color::Cyan) | bold,
						paragraph(help_text[5]),
						paragraph(help_text[23]) | color(Color::Red) | bold,
						paragraph(help_text[6]),
					}),
					separatorEmpty(),
					paragraph(help_text[7]) | bold,	
					paragraph(help_text[8]), 	
					separatorEmpty(),
					paragraph(help_text[9]) | bold, 	
					paragraph(help_text[10]), 	
					paragraph(help_text[11]), 	
					paragraph(help_text[12]),	
					paragraph(help_text[13]),	
					paragraph(help_text[14]),	
					paragraph(help_text[15]),	
					separatorEmpty(),
					paragraph(help_text[16]) | bold,	
					paragraph(help_text[17]),	
					paragraph(help_text[18]),	
					separatorEmpty(),
					paragraph(help_text[19]),	
					separatorEmpty(),
					paragraph(help_text[20]) | bold | center,
					separatorEmpty(),
					paragraph(help_text[21]) | bold | center,
				}) | borderEmpty,
			}) | border;
	});

	// Shortcuts window
	std::vector<std::string> shortcuts_text = {
		" 'S' - saves the current drawn map;",
		" 'R' - clears the maps saved;", 
		" 'Q' - exits the app;",
		" '<ESC>' - exits aditional windows appeared;",
		" 's' - start the solving algorithm",
		" 'r' - reset the grid to the initial state",
		" 'c' - open choosing clear type window",
		" 'a' - clear all tiles besides start and end point",
		" 'm' - draw the next map saved on the screen",
		" 'd' - check 'allow diagonal' option",
		" '1' - choose first algorithm in the list",
		" '2' - choose second algorithm in the list",
		" '3' - choose third algorithm in the list",
	};
	

	auto shortcut_window_renderer = Renderer( [&] {
		return vbox({
				text("Shortcuts") | center | bold,
				separator(),
				hbox({
					dbox({})| size(WIDTH, EQUAL, 4),
					vbox({
						paragraph(shortcuts_text[0]),
						paragraph(shortcuts_text[1]),
						paragraph(shortcuts_text[2]), 	
						paragraph(shortcuts_text[3]), 	
						paragraph(shortcuts_text[4]), 	
						paragraph(shortcuts_text[5]),
						paragraph(shortcuts_text[6]),
						paragraph(shortcuts_text[7]), 	
						paragraph(shortcuts_text[8]), 	
						paragraph(shortcuts_text[9]), 	
						paragraph(shortcuts_text[10]), 	
						paragraph(shortcuts_text[11]), 	
						paragraph(shortcuts_text[12]), 	
						//paragraph(shortcuts_text[13]), 	
					}),
				}),
			}) | border;
	});

	// Clear window
	std::vector<std::string> clear_button_text = {
		"  Path ",
		"  All ",
	};

	auto clear_button_style = ButtonOption::Animated(Color::Default, Color::GrayDark,
                                             Color::Default, Color::White);

	auto clear_window_container = Container::Vertical({
		Button(&clear_button_text[0], [&] { if(!thread_active){grid.clear_path(); depth = 0;} }, &clear_button_style) | center,
		Button(&clear_button_text[1], [&] { if(!thread_active){grid.clear_all(); depth = 0;} }, &clear_button_style) | center,
	});

	auto clear_window_renderer = Renderer(clear_window_container, [&] {
		return vbox({
				vbox(clear_window_container->Render()),
		})|border;
	});


	// Main renderer section
	auto main_container = Container::Tab({
		main_screen_renderer,
		tiles_selector_modal_renderer,
		help_window_renderer,
		clear_window_renderer,
		shortcut_window_renderer,
	}, &depth);

	auto main_renderer = Renderer(main_container, [&] {
		Element document = main_screen_renderer->Render();

		if(screen.dimx()!=screen_x || screen.dimy()!=screen_y){
			return document;
		}
		
		if (depth == 1){
			document = dbox({
				document,
				tiles_selector_modal_renderer->Render() | clear_under | center,
			});
		}

		if (depth == 2){

			document = dbox({
				document,
				help_window_renderer->Render() | clear_under | size(HEIGHT, EQUAL, 39) | size(WIDTH, EQUAL, 80) | center ,
			});
		}
		
		if (depth == 3){
			document = dbox({
				document,
				hbox({
					dbox({text("")}) | size(WIDTH, EQUAL, 172),
					vbox({
						dbox({text("")}) | size(HEIGHT, EQUAL, 33),
						vbox({
							clear_window_renderer->Render() | clear_under | center,
							}),
						}),
					}),
				});
		}

		if (depth == 4){
			document = dbox({
				document,
				shortcut_window_renderer->Render() | clear_under | size(HEIGHT, EQUAL, 17) | size(WIDTH, EQUAL, 60) | center,
			});
		}

		return document;
	});


	// Maps clear key event
	main_renderer |= CatchEvent([&](Event e) {

		// Trigger action to open select type of menu option
		if(e.is_mouse()){
			if(e.mouse().button == Mouse::Right) { depth = 1; }
		}

		// On escape close all aditional windows
		if(depth && e == Event::Special("\x1B")){
			depth = 0;
			return false;
		}


		if(e.is_character()){
			//Trigger for opening help modal window
			if(e == Event::Character('H')){
				depth = 2;
				return false;
			}

			if(e == Event::Character('K')){
				depth = 4;
				return false;
			}

			// Trigger for deleting all saved maps
			if(e == Event::Character('R')){
				grid.map_clear(); 
				return false;
			}

			// Trigger for saving current drawn maps in a file
			if(e == Event::Character('S')){
				grid.map_save(); 
				return false;
			}

			// Trigger for choosing 1'st algorithm
			if(e == Event::Character('1')){
				selected = 0;
				return false;
			}
			
			// Trigger for choosing 2'nd algorithm
			if(e == Event::Character('2')){
				selected = 1;
				return false;
			}

			// Trigger for choosing 3'rd algorithm
			if(e == Event::Character('3')){
				selected = 2;
				return false;
			}

			if(e == Event::Character('d')){
				diagonal_checked = diagonal_checked ^ 1;
				return false;
			}

			// Trigger for button START
			if(e == Event::Character('s')){
				if(!thread_active){ 
					diag_checked = diagonal_checked;
					grid.solve(selected);
					return false;
				}
			}
			
			// Trigger for button RESET
			if(e == Event::Character('r')){
				if(!thread_active){ 
					grid.reset(); 
					return false;
				}
			}

			// Trigger for button CLEAR
			if(e == Event::Character('c')){
				depth = 3;
				return false;
			}

			// Trigger for clear all aditional tiles(added)
			if(e == Event::Character('a')){
				if(!thread_active){ 
					grid.clear_all();
					depth = 0;
					return false;
				}
			}

			// Trigger for clear only path created on the map
			if(e == Event::Character('p')){
				if(!thread_active){ 
					grid.clear_path();
					depth = 0;
					return false;
				}
			}

			// Draw the map on the screen
			if(e == Event::Character('m')){
				if(!thread_active){ 
					grid.draw_map();
					return false;
				}
			}

			// Trigger for quiting application in a clean way
			if(e == Event::Character('Q')){
				// closing the application it's own way
				//screen.ExitLoopClosure()();
				// Stop UI refresh
				refresh_ui_continue = false;	
				std::cout << "\033[2J\033[1;1H";
				std::cout << "\033\143";
				tcsetattr(STDIN_FILENO, TCSANOW, &Original_Termios);
				std::exit(EXIT_SUCCESS);
			}
			return false;
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

	screen.Loop(main_renderer);
	refresh_ui_continue = false;
	refresh_ui.join();
 
	return 0;
}
