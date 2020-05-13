Author: Connor Raymond Stewart
ID: 101041125
Cmail: connorrstewart@cmail.carleton.ca
Course: COMP 3007
Year/Term: W18
Assignment Number: One
Question: One

Java Version: No java version was added to the assignment requirements
	Compiled using java version 11.0.1 LTS
	Java SE Runtime Environment 18.9

Compiling:
	Open the CMD/terminal to the root directory that the Java file is in
	type:
		javac comp3007_w19_101041125_a1_1.java
	if Java is not installed, you must download Java SE edition from Javas website
	and setup the path variable for windows.
	Otherwise, if using BASH (Linux), to install Java you must type:
		sudo apt-get update
		sudo apt install default-jdk
	now javac should world
To Run:
	type Java comp3007_w19_101041125_a1_1
Alternatively:
	You may use a Java IDE like Eclipse
	For Eclipse you must create a new java project (call it a1), then create a new class called comp3007_w19_101041125_a1_1
	add: package a1 to the start of the created project file, then copy the contents of the uploaded file below that
Other then the command prompt or Eclipse method, any method of compiling and running the code should be fine

To input into the application:
	you must write in snake case, that means you must write all space as a underscore ('_')
	if you start a word or end a word with a space, the input will be considered invalid.
		For example: _connor_stewart OR connor_stewart_ OR _connor_stewart_
			are all considered invalid inputs, as they don't contain logical space locations
	valid snake case input will have no capitals and must have a _ for every space.
		For example: connor_stewart_wrote_this 
			is considered valid snake case input for this application
	The application will PROMPT you for input inside a command line interface
	All symbols are considered invalid input, [, ], ?, and ' all for example will be considered
		non valid letters, and will cause the program to return invalid.
		For example: connor's_program is considered invalid input, as it contains a '
	Empty input is not supported by the application
		The application will crash if you add empty input
		input correctness is not specified in the assignment
		this is because .equals() is o(n)
		length is restricted by the professor
		javas isEmpty uses length
Output from application:
	The applications output will be inside a command line interface
	The output will be in list format:
		For example, the input: connor_stewart will be be converted to pig Latin
		and be outputed as: [onnorcay, ewartstay]

