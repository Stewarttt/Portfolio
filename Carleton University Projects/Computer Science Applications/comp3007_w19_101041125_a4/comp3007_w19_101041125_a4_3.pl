/*Name: Connor Raymond Stewart
Student Number: 101041125

Define the following possible finite positions for the neanderthal, humans, and the sabertooth tigers in the boat*/
passengers(1, 0, 0).
passengers(1, 1, 0).
passengers(1, 0, 1).
passengers(0, 1, 0).
passengers(0, 1, 1).
passengers(0, 2, 0).

/*Describe the transition from the east side of the river to the west side
there are two states, the current state, and the possible future state
the cross is the potential cross to be taken
check if the new state is a valid one*/
transition( state(A, B, C, D, E, F, east), cross(I, J, K), state(W, X, Y, Z, U, V, west) ) :- 
	passengers(I, J, K),
	W is A-I, 
	W >= 0, 
	X is B-J, 
	X >= 0, 
	Y is C-K,
	Y >= 0,
	Z is D+I, 
	U is E+J, 
	V is F+K,
	safe(W, X, Y, Z, U, V).
/*Describe the transition from the west side of the river to the east side
there are two states, the current state, and the possible future state
the cross is the potential cross to be taken
check if the new state is a valid one*/
transition( state(A, B, C, D, E, F, west), cross(I, J, K), state(W, X, Y, Z, U, V, east) ) :- 
	passengers(I, J, K), 
	W is A+I, 
	X is B+J, 
	Y is C+K,
	Z is D-I,
	Z >= 0,
	U is E-J,
	U >= 0,
	V is F-K,
	V >= 0,
	safe(W, X, Y, Z, U, V).

/*Checks if the Humans will be eaten by the sabertooth tigers
there can either be an equal or greater number of humans then the tigers
or there can be no humans on a side of the river*/
safe(_, B, C, _, E, F) :-
	(B >= C ; B == 0),
	(E >= F ; E == 0).

/*
Checks all the possible states for river crossing
Runs based on the size of the list
prevents repetitive patterns from occuring*/
crossSafely(S, S, []).
crossSafely(S, E, [X | Y]) :- 
	S \= E, 
	transition(S, X, I), 
	crossSafely(I, E, Y).

/*entry point for using this program
the first argument is the starting point
the last argument is the ending point
this defines the range of crosses to search
for example: entryPoint(1, 15) will check all possible crosses from 1 to 15 crosses in total
*/
entryPoint(S, E) :-
	S =< E,
	find_solution(S, S, E).

/*This method checks all the possible river crossings for a list of the specified size
this function will run to determine all possible crossings for a specific range
for example: all possible crossings of size 9
*/
find_solution(Z, Y, T) :- 
	(Z > T -> write("All possible solutions in the given range have been displayed (There will be no output only if no solution exists)"), true;
	((length(L, Z),
	crossSafely(state(1, 3, 2, 0, 0, 0, east), state(0, 0, 0, 1, 3, 2, _), L),
	writeln(L),
	Z < Y,
	J is Z + 1,
	find_solution(J, Y, T));
	(J is Z + 1,
	find_solution(J, Y, T)))).