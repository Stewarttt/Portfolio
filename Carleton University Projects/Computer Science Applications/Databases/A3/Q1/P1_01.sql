/*
Question 1) Produce a table consisting of the names and addresses of the subscribers and their phone numbers.
*/

SELECT distinct name, address, area, office, stn FROM Subscribers
	INNER JOIN Calls 
	ON Subscribers.portid = Calls.call_id;

/*
Test Output:
name            address                 area  office  stn
--------------  ----------------------  ----  ------  ----
Bryan MacCabe   23 MacLeod St.          613   227     0009
Eric Lindros    48 Elgin St.            613   389     0015
Eric Staal      423 Riverside Drive.    613   457     0016
George Bell     46 Colon By Dr.         416   334     0030
Jack Morris     23 Prince of Wales Dr.  705   221     0032
Jason Allison   46 Elgin St.            613   728     0025
Joe Carter      18 Summerset East.      905   347     0033
Mario Lemieux   1129 Bank Dr.           905   819     0035
Mats Sundin     45 Elgin St.            613   221     0005
Michael Jordan  1223 Carling Ave.       416   333     0029
Patrick Roy     1223 Greenbank Ave.     819   223     0039
Roberto Alomar  55 Moodie Dr.           905   657     0034
Roger Clemens   14 Hopewell Ave.        416   219     0027
Roy Halladay    23 Ogilvie Rd.          705   221     0038
Steve Nash      1129 Otterson Dr.       613   712     0024
Wayne Grekzy    45 Merviale.            819   227     0037
*/