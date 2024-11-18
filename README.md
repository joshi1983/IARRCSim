To get this working on your computer without recompiling, do the following:
1. Download this repository as a zip file.
2. Extract the file anywhere you want.
1. Open build/Windows/IARRC_Simulator/IARRC_Sim.exe

To build from source, open the source project with [Code::Blocks](https://www.codeblocks.org/downloads/).  source/IARRC Simulator.cbp is the project file.

The application runs with a graphical user interface but it also runs an HTTP server that hosts API's you can connect with using any general purpose programming language.  A web browser will open as the simulator runs to show documentation about the simulator's API's.

DEPENDENCIES:

- freeglut
	- How to install freeglut on Windows: https://www.youtube.com/watch?v=M4fm-cHGoYU
- pthreads
- winsock

