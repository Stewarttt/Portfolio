/*
Question 7: Write an SQL query that will produce in one table a list of all the acceptable trunks that can be used to
route a call to the 416 area code, office code 334. This query should list the trunks in the order of preference. (The
answer should list trunks with routes 416,334 then those with 416,000 and then those with 000,000 for example)
*/

SELECT * FROM trunks INNER JOIN trunk_routes 
	ON (trunk_routes.area = '416' and trunk_routes.office = '334')
	OR (trunk_routes.area = '416' and trunk_routes.office = '000')
	OR (trunk_routes.area = '000' and trunk_routes.office = '000')
	GROUP BY trunks.portid
	ORDER BY area DESC, office DESC;

/*
Test Output:
portid  foreign_switch  portid  area  office
------  --------------  ------  ----  ------
100     KANATA          102     416   000
101     ORLEANS         102     416   000
102     RIDEAU          102     416   000
103     BASELINE        102     416   000
104     SOMERSET        102     416   000
105     NEPEAN          102     416   000
106     BAY             102     416   000
107     Innes           102     416   000
*/