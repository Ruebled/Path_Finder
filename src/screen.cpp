#include "screen.h"

// Cell color enum
ftxui::Color Color_cell[] = { 
	  ftxui::Color::Black,
	  ftxui::Color::Cyan,
	  ftxui::Color::Red,
	  ftxui::Color::GrayDark,
	  ftxui::Color::SandyBrown,
	  ftxui::Color::DarkGreen,
	  ftxui::Color::DarkBlue,
	  ftxui::Color::DarkOrange3,
	  ftxui::Color::GrayLight,
	  ftxui::Color::Orange1,
};

ftxui::Color Visited_cell[] = {
	  ftxui::Color::Salmon1,
	  ftxui::Color::Green3Bis,
	  ftxui::Color::Blue3Bis,
	  ftxui::Color::DarkOrange3Bis,
};

//ftxui::Color visit_color = ftxui::Color::GrayLight;
ftxui::Color path_color = ftxui::Color::Orange1;
							  							  
ftxui::Canvas matrix_to_canvas(Grid grid, ftxui::Screen& screen){
    int rows = grid.height();
	int cols = grid.width();

	int cell_dim_x = grid.cell_width();
	int cell_dim_y = grid.cell_height();

	auto canvas = ftxui::Canvas(cols*cell_dim_x+2, rows*(cell_dim_y+1));
	if(screen.dimx()<194){
		std::string v_text = "Too little horizontal space";
		canvas.DrawText(0,0, v_text);
		return canvas;
	}else if(screen.dimy()<45){
		std::string h_text = "Too little vertical space";
		canvas.DrawText(0,0, h_text);
		return canvas;	
	}

	// Draw the Main axis from point 0, 0
	canvas.DrawPointLine(0, 0, cols*cell_dim_x, 0, ftxui::Color::White);
   	canvas.DrawPointLine(0, 0, 0, rows*cell_dim_y, ftxui::Color::White);

	// Drawing the grid of lines
	for ( int r = 0; r < rows; r++){
		for ( int c = 0; c <= cols; c++){
			canvas.DrawPointLine(c*cell_dim_x+cell_dim_x, 
								 0, 
								 c*cell_dim_x+cell_dim_x, 
								 rows*cell_dim_y, 
								 ftxui::Color::White
								 );
		}
		canvas.DrawPointLine(0, 
							 r*cell_dim_y+cell_dim_y, 
							 cols*cell_dim_x, 
							 r*cell_dim_y+cell_dim_y, 
							 ftxui::Color::White
							 );
	}

	// Fill the cells with colors
	for ( int r = 0; r < rows; r++){
		for ( int c = 0; c < cols; c++){
			int color_ind = grid.get_index(r, c);

			if(color_ind == 0){
				continue;
			}

			ftxui::Color cell_color;

			if(color_ind < Type(visited)){
					cell_color = Color_cell[color_ind];
			}

			if(color_ind >= Type(visited)){
				if(color_ind % 10 == 0){
					cell_color = ftxui::Color::GrayLight;
				}else{
					cell_color = Visited_cell[(color_ind % 10)-4];
				}
			}

			if(color_ind >= Type(path)){
				cell_color = path_color;
			}

			for ( int l = 0; l <= cell_dim_y-2; l++){
				canvas.DrawPointLine(c*cell_dim_x+2, 
									 r*cell_dim_y+1+l, 
									 c*cell_dim_x+cell_dim_x-2, 
									 r*cell_dim_y+1+l, 
									 cell_color);
			}
		}
	}

	return canvas;
}
