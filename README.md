# Path_Finder
A CPP Console application that uses ftxui, it search the path between two points using diferent algoritms and show it in real time.
## Note
I know it's full of issues and not following any coding guidelines, but ideas for improvement are appreciated.

Project using the [FTXUI library](https://github.com/ArthurSonzogni/ftxui)

![image](https://github.com/Ruebled/Path_Finder/assets/4759106/77a136b8-38f9-4802-9b4f-8858bb7d1e5a)

![image](https://github.com/Ruebled/Path_Finder/assets/4759106/9cb126c9-d2a8-4529-b1ba-621d1aba7666)


# Dependencies
git
cmake
make
g++
xterm <!-- for resize terminal command to work -->

# Build instructions:
~~~bash
git clone https://github.com/Ruebled/Path_Finder.git
cd Path_Finder
mkdir build
cd build
<!-- Next comand requires internet connection and a lot of patience  -->
cmake ..
make -j
resize -s 45 194 <!-- optional, to resize the terminal size for app to open the right way -->
./Path_finder
~~~

## Note
Like specified in documentation, the project need to be made responsive
for different size of screens.

As of now the size of the terminal window needed for the project to work is **45x194**.

On running *./Path_finder* from /build directory, if the terminal window size is 
not matching, a message of unfitness will appear until terminal window size is right.

Application tested on Archlinux using alacritty and Ubuntu using gnome-terminal.
