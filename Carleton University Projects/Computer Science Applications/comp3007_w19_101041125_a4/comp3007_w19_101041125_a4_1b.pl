/*Name: Connor Raymond Stewart
Student Number: 101041125*/

/*countZerofree takes a list of size 2 or greater, takes variable X which may not
be initialized, and inputs the data into interateList*/
countZerofree([Y|Z], X) :-
    iterateList([Y|Z], 0, X).
/*If the list is empty, it will proceed to print the results of the search
through the list
[] - an empty array
Q - users estimation
X - actual zero free numbers
*/
iterateList([], X, Q) :-
    printResult(X, Q).
/*Iterates through the list recursivly, element by element and sends each element to checkForZeros
Q = estimation
X = counted number of zero free numbers
Y = first element in the list
Z = remaining elements in the list
W = The absolute value of Y (the first element in the list
*/
iterateList([Y|Z], X, Q) :-
	(Y < 0 -> W is (-1*Y); W is Y),
    (checkForZeros(W) -> iterateList(Z, (X+1), Q); iterateList(Z, X, Q)).
/*checks if a zero exists within the current number, it does this by dividing
by ten, then multiplying by ten (this truncates the leftmost digit). The resulting
value is then subtracted from the original number, leaving the first decimal number
from the original number as the remainder. For example:
	1234X will result in X
The value of X is then checked, if that value equals a zero, then it is determined
that it is false to say this is a zero free number
X = estimation*/
checkForZeros(X) :-
    X < 1;
    P is (X - (div(X, 10))*10),
    P > 0,
    checkForZeros(div(X, 10)).
/*outputs the result of the tests. If the original value of X equals the number
of zero free digits within the array, then true is returned, along with the
original number entered. Otherwise false is returned with the original number.
If X was not initialized, then true is returned only if there are no zeros in any number,
false is returned if 1 or more numbers contains a zero, and X equals the number
of numbers containing a zero.
Q = estimation
X = counted number of zero free numbers
O = the sum of X (the counted number of free numbers)
*/
printResult(X, Q) :-
    O is X,
    (testInstentiation(Q) -> (write("X = "), write(O), write(" ;")); write("")),
    (testInstentiation(Q) -> (O > 0 -> false; true); write("")),
    (testInstentiation(Q) -> true; Q = O).
/*This function tests if X has been initialized. It does this by double negating
the value for x. A false should only be returned if X is not initialized,
otherwise a true should be retruned.
X = estimation
*/
testInstentiation(X) :-
    \+(\+(X=(a))).