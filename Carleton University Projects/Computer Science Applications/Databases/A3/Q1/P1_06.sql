/*
Question 6) Produce a table that lists the most popular service (or services). That is, give the name of the service
that has the most subscribers.
*/

DROP TABLE IF EXISTS Popularity;
CREATE TABLE Popularity AS 
	SELECT * from services 
	JOIN service_subscribers WHERE scode=service_subscribers.service 
	GROUP BY services.service 
	HAVING COUNT(service_subscribers.portid);
SELECT service, max(portid) subscribers FROM Popularity;
/*
Test Output:
service              subscribers
-------------------  -----------
Call Number Display  3
*/