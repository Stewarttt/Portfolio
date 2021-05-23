/*
Question 8) Write an SQL query that would find if the line with phone number (613) 712-0024 is currently available
to take a call because its channel is IDLE. Rev 1: That is, select the portid, directory number, and channel state of
line (613) 712-0024.
*/

SELECT lines.portid, areacode, officecode, stationcode, channels.state FROM lines 
	INNER JOIN channels ON channels.state = 'IDLE'
	WHERE areacode = '613'
	AND officecode = '712'
	AND stationCode = '0024'
	GROUP BY lines.portid;

/*
Test Output:
portid  areacode  officecode  stationcode  state
------  --------  ----------  -----------  -----
24      613       712         0024         IDLE
*/