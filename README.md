# Path_Finder
A CPP Console application that uses ftxui, it search the path between two points using diferent algoritms and show it in real time.

Project using the [FTXUI library](https://github.com/ArthurSonzogni/ftxui)

# Dependencies
git
cmake
make
g++

# Build instructions:
~~~bash
git clone https://github.com/Ruebled/Path_Finder.git
cd Path_Finder
mkdir build
cd build
<!-- Next comand requires internet connection and a lot of patience  -->
cmake ..
make -j
./Path_finder
~~~

## Note
Like specified in documentation, the project need to be made responsive
for different size of screens.

As of now the size of the terminal window needed for the project to work is **45x194**.

On running *./Path_finder* from /build directory, if the terminal window size is 
not matching, a message of unfitness will appear until terminal window size is right.

Application tested on Archlinux using alacritty and Ubuntu using gnome-terminal.
