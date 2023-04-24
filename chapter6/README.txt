Depending on your operating system:

If Mac:
	ln -s macos.vscode .vscode

If Linux (or running Linux under WSL in Windows):
	ln -s linux.vscode .vscode

Run on command line, either run the app or run the tests with:
	make run
	make test

Pre-requisites to run on command line:
	Macos: XCode Command Line tools, Homebrew, brew install ncurses
	Linux or WSL: sudo apt install ncurses-base ncurses-dev ncurses-term

Run in vscode, either:
	open main.c and f5 (debug) using predefined clang or gcc predefined tasks
	open test_???.cpp and f5 (debug) using predefined clang++ or g++ predefined tasks

Pre-requisites to run in vscode:
	Microsoft's C/C++ extension
	Microsoft's Makefile extension
