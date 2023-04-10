Depending on your operating system:

If Mac:
	ln -s macos.vscode .vscode
	ln -s Makefile.macos Makefile

If Linux (or running Linux under WSL in Windows):
	ln -s linux.vscode .vscode
	ln -s Makefile.linux Makefile

Run on command line, either run the app or run the tests with:
    make; build/main
	make test

Pre-requisites to run on command line:
	Macos: XCode Command Line tools, Homebrew, brew install cpputest
	Linux or WSL: sudo apt install build-essential cpputest

Run in vscode, either:
	open main.c and f5 (debug) using predefined clang or g predefined tasks
	open test_all.cpp and f5 (debug) using predefined clang++ or g++ predefined tasks

Pre-requisites to run in vscode:
	Microsoft's C/C++ extension
	Microsoft's Makefile extension
	
