--destroy the previous records in the database from the last run, if any exist
DROP TABLE IF EXISTS employee;
DROP TABLE IF EXISTS department;
DROP TABLE IF EXISTS dept_locations;
DROP TABLE IF EXISTS project;
DROP TABLE IF EXISTS works_on;
DROP TABLE IF EXISTS dependent;

--create the tables as described in the PDF document

CREATE TABLE employee( 
      fname text NOT NULL, --the employee must have a first name, therefore NOT NULL
      minit text, --This is set to be NULL-capable because some people don't have middle names (this is in spite of the fact the data set shows everyone having a middle name, which is unrealistic in the real world)
      lname text NOT NULL, --the employee must have a last name, therefore NOT NULL
	  ssn integer primary key NOT NULL, --NOT NULL since its primary key
	  bdate text NOT NULL, --the empoyee must have been born some day, therefore NOT NULL
	  address text NOT NULL, --the employee must have an address, therefore NOT NULL
	  sex text NOT NULL, --NOT NULL since someone must have a sex which can be flagged as a text string
	  salary integer NOT NULL, --the employee must have a salary to be working, therefore NOT NULL
	  superssn integer, --not all employees have a super-SSN, thus this is set to be NULL-capable
      dno text NOT NULL --the department the employee works at must have a number, therefore NOT NULL
	  );

CREATE TABLE department( 
      dname text NOT NULL, -- the department must have a name, therefore NOT NULL
      dnumber integer primary key NOT NULL, --NOT NULL since its primary key
      mgrssn text NOT NULL,  --the manager must have a social security number, therefore NOT NULL
	  mgrstartdate text NOT NULL --the manager needed to start at some data, therefore NOT NULL
      );

CREATE TABLE dept_locations(
      dnumber integer NOT NULL, --NOT NULL since its primary key
      dlocation text NOT NULL, --NOT NULL since its primary key
	  primary key (dnumber, dlocation) --set these keys to be the primary columns
      );

CREATE TABLE project( 
      pname text NOT NULL, --the project should have a name, therefore NOT NULL
      pnumber integer primary key NOT NULL, --NOT NULL since its primary key
	  plocation text NOT NULL, --the project must be taking place somewhere, therefore NOT NULL
	  dnum integer NOT NULL --the project must be under the control of a department, therefore NOT NULL
      );

CREATE TABLE works_on(
	  essn integer NOT NULL, --NOT NULL since its primary key
	  pno integer NOT NULL, --NOT NULL since its primary key
	  hours REAL, --the person does NOT 'need' to be working hours, thus this can be NULL-capable
	  PRIMARY KEY (essn, pno) --set these keys to be the primary columns
	  );

CREATE TABLE dependent(
	  essn integer NOT NULL, --NOT NULL since its primary key
	  dpendent_name text NOT NULL, --NOT NULL since its primary key
	  sex text NOT NULL, --NOT NULL since someone must have a sex which can be flagged as a text string
	  bdate text NOT NULL, --NOT NULL since someone must have a birthrate
	  relationship text NOT NULL, --NOT NULL since someone must have had a family
	  primary key (essn, dpendent_name) --set these keys to be the primary columns
	  );

--begin writing to the database
BEGIN TRANSACTION;

--insert employee values
INSERT INTO employee(fname, minit, lname, ssn, bdate, address, sex, salary, superssn, dno) VALUES ('John', 'B', 'Smith', 123456789, '9-Jan-55', '731 Fondern', 'M', 30000, 333445555, 5);
INSERT INTO employee(fname, minit, lname, ssn, bdate, address, sex, salary, superssn, dno) VALUES ('Franklin', 'T', 'Wong', 333445555, '8-Dec-45', '638 Voss', 'M', 40000, 888665555, 5);
INSERT INTO employee(fname, minit, lname, ssn, bdate, address, sex, salary, superssn, dno) VALUES ('Alicia', 'J', 'Zelaya', 999887777, '19-Jul-58', '3321 Castle', 'F', 25000, 987987987, 4);
INSERT INTO employee(fname, minit, lname, ssn, bdate, address, sex, salary, superssn, dno) VALUES ('Jennifer', 'S', 'Wallace', 987654321, '20-Jun-31', '291 Berry', 'F', 43000, 888665555, 4);
INSERT INTO employee(fname, minit, lname, ssn, bdate, address, sex, salary, superssn, dno) VALUES ('Ramesh', 'K', 'Narayan', 666884444, '15-Sep-52', '975 Fire Oak', 'M', 38000, 333445555, 5);
INSERT INTO employee(fname, minit, lname, ssn, bdate, address, sex, salary, superssn, dno) VALUES ('Joyce', 'A', 'English', 453453453, '31-Jul-62', '5631 Rice', 'F', 25000, 333445555, 5);
INSERT INTO employee(fname, minit, lname, ssn, bdate, address, sex, salary, superssn, dno) VALUES ('Ahmed', 'V', 'Jabber', 987987987, '29-Mar-59', '980 Dallas', 'M', 25000, 987654321, 4);
INSERT INTO employee(fname, minit, lname, ssn, bdate, address, sex, salary, superssn, dno) VALUES ('James', 'E', 'Borg', 888665555, '10-Nov-27', '450 Stone', 'M', 55000, NULL, 1);

--insert department values
INSERT INTO department(dname, dnumber, mgrssn, mgrstartdate) VALUES ('Research', 5, 333445555, '22-May-78');
INSERT INTO department(dname, dnumber, mgrssn, mgrstartdate) VALUES ('Administration', 4, 987654321, '1-Jan-85');
INSERT INTO department(dname, dnumber, mgrssn, mgrstartdate) VALUES ('Headquarters', 1, 888665555, '19-Jun-71');

--insert department locations
INSERT INTO dept_locations(dnumber, dlocation) VALUES (1, 'Houston');
INSERT INTO dept_locations(dnumber, dlocation) VALUES (4, 'Stafford');
INSERT INTO dept_locations(dnumber, dlocation) VALUES (5, 'Bellaire');
INSERT INTO dept_locations(dnumber, dlocation) VALUES (5, 'Sugarland');
INSERT INTO dept_locations(dnumber, dlocation) VALUES (5, 'Houston');

--insert works locations
INSERT INTO works_on(essn, pno, hours) VALUES (123456789, 1, 32.50);
INSERT INTO works_on(essn, pno, hours) VALUES (123456789, 2, 7.50);
INSERT INTO works_on(essn, pno, hours) VALUES (666884444, 3, 40.00);
INSERT INTO works_on(essn, pno, hours) VALUES (453453453, 1, 20.00);
INSERT INTO works_on(essn, pno, hours) VALUES (453453453, 2, 20.00);
INSERT INTO works_on(essn, pno, hours) VALUES (333445555, 2, 10.00);
INSERT INTO works_on(essn, pno, hours) VALUES (333445555, 3, 10.00);
INSERT INTO works_on(essn, pno, hours) VALUES (333445555, 10, 10.00);
INSERT INTO works_on(essn, pno, hours) VALUES (333445555, 20, 10.00);
INSERT INTO works_on(essn, pno, hours) VALUES (999887777, 30, 30.00);
INSERT INTO works_on(essn, pno, hours) VALUES (999887777, 10, 10.00);
INSERT INTO works_on(essn, pno, hours) VALUES (987987987, 10, 35.00);
INSERT INTO works_on(essn, pno, hours) VALUES (987987987, 30, 5.00);
INSERT INTO works_on(essn, pno, hours) VALUES (987654321, 30, 20.00);
INSERT INTO works_on(essn, pno, hours) VALUES (987654321, 20, 15.00);
INSERT INTO works_on(essn, pno, hours) VALUES (888665555, 20, NULL);

--insert project locations
INSERT INTO project(pname, pnumber, plocation, dnum) VALUES ('ProductX', 1, 'Bellaire', 5);
INSERT INTO project(pname, pnumber, plocation, dnum) VALUES ('ProductY', 2, 'Sugarland', 5);
INSERT INTO project(pname, pnumber, plocation, dnum) VALUES ('ProductZ', 3, 'Houston', 5);
INSERT INTO project(pname, pnumber, plocation, dnum) VALUES ('Computerization', 10, 'Stafford', 4); --according to "A1 'Staffort' typo intentional or mistake", Straffort is a typo. Thus, I have placed Stafford here
INSERT INTO project(pname, pnumber, plocation, dnum) VALUES ('Reorganization', 20, 'Houston', 1);
INSERT INTO project(pname, pnumber, plocation, dnum) VALUES ('NewBenefits', 30, 'Stafford', 4);

--insert dependent locations
INSERT INTO dependent(essn, dpendent_name, sex, bdate, relationship) VALUES (333445555, 'Alice', 'F', '5-Apr-76', 'DAUGHTER');
INSERT INTO dependent(essn, dpendent_name, sex, bdate, relationship) VALUES (333445555, 'Theodore', 'M', '25-Oct-73', 'SON');
INSERT INTO dependent(essn, dpendent_name, sex, bdate, relationship) VALUES (333445555, 'Joy', 'F', '3-May-48', 'SPOUSE');
INSERT INTO dependent(essn, dpendent_name, sex, bdate, relationship) VALUES (987654321, 'Abner', 'M', '29-Feb-32', 'SPOUSE');
INSERT INTO dependent(essn, dpendent_name, sex, bdate, relationship) VALUES (123456789, 'Michael', 'M', '1-Jan-78', 'SON');
INSERT INTO dependent(essn, dpendent_name, sex, bdate, relationship) VALUES (123456789, 'Alice', 'F', '31-Jan-78', 'DAUGHTER');
INSERT INTO dependent(essn, dpendent_name, sex, bdate, relationship) VALUES (123456789, 'Elizabeth', 'F', '5-May-57', 'SPOUSE');

--stop writing to the database
END TRANSACTION;