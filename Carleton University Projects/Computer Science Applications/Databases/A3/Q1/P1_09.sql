/*
Question 9) Do the same as question 1.7 but this time only include trunks that have at least one 'IDLE' channel.
That is, write an SQL query that will produce in one table a list of all the acceptable trunks that can be used to route
a call to the 416 area code, office code 334 have at least one idle channel. This query should list the trunks in the
order of preference. (The answer should list trunks routes 416,334 then 416,000 then 000,000 for example)
*/

SELECT * FROM trunks INNER JOIN trunk_routes 
	ON (trunk_routes.area = '416' and trunk_routes.office = '334')
	OR (trunk_routes.area = '416' and trunk_routes.office = '000')
	OR (trunk_routes.area = '000' and trunk_routes.office = '000')
	INNER JOIN channels ON channels.state = 'IDLE' AND trunk_routes.portid = channels.portid
	GROUP BY trunks.portid
	ORDER BY area DESC, office DESC;
/*
Test Output:
portid  foreign_switch  portid  area  office  portid  channel  state
------  --------------  ------  ----  ------  ------  -------  -----
100     KANATA          102     416   000     102     0        IDLE
101     ORLEANS         102     416   000     102     0        IDLE
102     RIDEAU          102     416   000     102     0        IDLE
103     BASELINE        102     416   000     102     0        IDLE
104     SOMERSET        102     416   000     102     0        IDLE
105     NEPEAN          102     416   000     102     0        IDLE
106     BAY             102     416   000     102     0        IDLE
107     Innes           102     416   000     102     0        IDLE
*/