/*
Question 5) Find the names of all the subscribers who subscribe to at least three services.
*/

SELECT name FROM subscribers
	JOIN service_subscribers ON service_subscribers.portid = subscribers.portid 
	GROUP BY service_subscribers.portid 
	HAVING COUNT(service_subscribers.portid) >= 3;

/*
Test Output:
name
--------------
Michael Jordan
Joe Carter
Homer Simpson
Vince Carter
Chris Pronger
Frank Thomas
Steve Sampras
Matt Stajan
*/