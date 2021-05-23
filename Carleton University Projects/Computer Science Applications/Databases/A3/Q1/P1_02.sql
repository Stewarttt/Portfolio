/*
Question 2) Produce a table that lists all the area code, office code combinations and the number of subscribers
with that area code, office code combination.
*/

SELECT areacode, officecode, COUNT(portid) numberOfSubscribers FROM lines
	GROUP BY areacode+officecode;

/*
Test Output:
areacode  officecode  numberOfSubscribers
--------  ----------  -------------------
416       219         2
613       134         3
613       136         3
416       334         2
613       156         1
613       220         1
613       221         1
613       222         1
613       223         1
613       226         1
613       227         1
613       229         1
613       310         1
705       221         3
613       322         1
613       333         1
613       334         1
613       389         1
819       223         2
819       227         3
613       457         1
613       489         1
613       523         1
416       756         2
613       568         1
613       578         1
613       623         1
905       347         2
613       645         1
613       657         1
613       712         1
613       728         1
613       798         1
905       657         2
905       819         2
*/