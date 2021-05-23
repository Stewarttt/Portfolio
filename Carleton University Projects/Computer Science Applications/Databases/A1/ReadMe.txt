The database scripts were written using sqlite3 3.34.0 as described in the course notes

In order to run, open the command prompt and set the current PATH directory to be that of the working sqlite3 executable script (i.e. C:/sqlite)

Question One:
	Go to the directory of the script in the question one folder and type: 
		sqlite3 myTunes.db
		.read insert_script.sql
		.read insert_script.sql
	To output the data you can next input:
		.mode column
		.header on
		select * from cds;
		select * from songs;
Question Two:
	Go to the directory of the script in the question two folder and type: 
		sqlite3 testOne
		.read elmasri_navathe_data.sql
		.read elmasri_navathe_data.sql
	To output, you can next input:
		.mode column
		.header on
		select * from employee;
		select * from department;
		select * from dept_locations;
		select * from project;
		select * from works_on;
		select * from dependent;
Question Three:
	Go to the question three folder and open the PDF to read my response