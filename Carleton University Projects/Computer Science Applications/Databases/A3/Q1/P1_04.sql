/*
Question 4) Find the names and address of all subscribers who subscribe to all of the available services. (Note the
result for the current data might be empty but your query should work if the TA's add more data to the databse.)
*/

SELECT name, address FROM subscribers
	INNER JOIN service_subscribers ON subscribers.portid = service_subscribers.portid
	GROUP BY subscribers.portid HAVING (SELECT COUNT(scode) FROM services) = COUNT(service_subscribers.service);
	
/*
Test Output: 
*/