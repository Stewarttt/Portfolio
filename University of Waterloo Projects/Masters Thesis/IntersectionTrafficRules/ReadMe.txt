Introduction:
    The DatabaseExtract.cpp file contains a set of methods for evaluated signalized intersection databases from Waterloos wise-lab:
        https://uwaterloo.ca/waterloo-intelligent-systems-engineering-lab/datasets/waterloo-multi-agent-traffic-dataset-intersection
Running the program:
    To compile the program, ensure that the system has SQLite3 installed:
        sudo apt install sqlite3
    The code can be compiled using the SQLite3 package with the following command:
        g++ -std=c++11 DatabaseExtract.cpp -l sqlite3
    The code can then be run using the following command:
        ./a.out
References:
    SQLite3 databases table information can be found at the following website:
        https://uwaterloo.ca/waterloo-intelligent-systems-engineering-lab/datasets/waterloo-multi-agent-traffic-dataset-intersection
Notes:
    The code relys on properly formatted databases for reading interaction dataset information as well.
Command Line Input:
    -o: Used to turn off output for specific test cases.
    -s: Used to turn off statistical output for all test cases.
    -a: Allows for setting vehicle breaking acceleration.
    -f: Currently unused, allows for setting road friction.
    -G: Currently unused, allows for setting road grade.
    -t: Currnetly unused, allows for setting driver reaction time.
