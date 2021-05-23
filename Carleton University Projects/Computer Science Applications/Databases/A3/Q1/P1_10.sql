/*
Question 10) Produce a table that lists the name of all the service-subscribers that subscribe to at least all the
same services as Jason Allison subscribes to but possibly others as well. Jason Allison rents the line with portID=2.
(So this is the classic "subset" query.)
*/
DROP TABLE IF EXISTS People;
Create TABLE People AS
	SELECT subscribers.name FROM subscribers
	INNER JOIN service_subscribers ON service_subscribers.portid = subscribers.portid
	AND service_subscribers.service = 'CWT'
	INNER JOIN service_subscribers serv ON serv.portid = subscribers.portid
	AND serv.service = '3WC'
	group by name;
SELECT * FROM People;
/*
Test Output: 
name
--------------
Homer Simpson
Jason Allison
Joe Carter
Matt Stajan
Michael Jordan
*/