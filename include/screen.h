#ifndef SCREEN_H
#define SCREEN_H

// System includes
#include <iostream>
#include <vector>
#include <stdio.h>
#include <memory>
#include <list>
#include <thread> //Implement multithreading version
#include <chrono>

// Terminal state restor headers
#include <signal.h>
#include <termios.h>

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

#include <sys/ioctl.h>

// Application local includes
#include "Grid.h"

// Defined fixed screen sizes for the project to work
const int screen_x = 194;
const int screen_y = 45;

ftxui::Canvas matrix_to_canvas(Grid grid, ftxui::Screen&);

#endif
