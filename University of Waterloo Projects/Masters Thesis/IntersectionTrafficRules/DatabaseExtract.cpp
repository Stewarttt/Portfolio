//import c and c++ libraries
#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <cmath>

//import sqlite3 header file
#include <sqlite3.h>

//define enumeration values for specific coefficients
enum {NO_OUTPUT = 0, NO_RESULT = -1};

//define default output string representations
struct defaultValues {
   const std::string NullInput = "NULL";
}; //END defaultValues

//container for various string identifiers
struct lightSignals {
   const std::string Red = "R";
   const std::string Yellow = "Y";
   const std::string Green = "G";
}; //END lightIdentifiers

//container for all intersection paths
struct intersectionDirections {
   //intersection directions
   const std::string LNE = "lne";
   const std::string LSW = "lsw";
   const std::string LNS = "lns";
   const std::string LSE = "lse";
   const std::string LSN = "lsn";
   const std::string LWN = "lwn";
   const std::string LES = "les";
   const std::string LWE = "lwe";
   const std::string LEW = "lew";
   const std::string LWS = "lws";

   //intersection turn types
   const std::string LEFT_TURN = "lt";
   const std::string STRAIGHT = "st";
   const std::string RIGHT_TURN = "rt";

   //intersection directions
   const char NORTH = 'n';
   const char SOUTH = 's';
   const char EAST = 'e';
   const char WEST = 'w';
}; //END intersectionDirections

//container for command line input
struct commandLineInput {
   //contains the vehicles breaking acceleration in terms of meters per second squared
   double vehicleBreakingAcc = 7.84532; //equal to 0.8g

   //reaction delay time for vehicles
   double reactionTime = 0.75;

   //flags for specific output types
   bool caseOutput = true;
   bool statisticsOutput = true;

   //command line representations for case output flags
   const std::string vehicleBreakingAccToggle = "-a";
   const std::string caseOutputToggle = "-o";
   const std::string statisticsOutputToggle = "-s";
}; //commandLineInput

//container for the vehicle tracks
struct tracks {
   int trackID;
   int fileID;
   std::string type;
   double width;
   double length;
   std::string entryGate;
   double entryTime;
   std::string exitGate;
   double exitTime;
   double traveledDist;
   double averageSpeed;
}; //END tracks

//container for the light at every recorded moment they changed
struct lights {
   //current moment of time since recording started
   double time;

   //ID of the given file
   int fileID;

   //traffic light states for given lights
   std::string LNE;
   std::string LSW;
   std::string LNS;
   std::string LSE;
   std::string LSN;
   std::string LWN;
   std::string LES;
   std::string LWE;
   std::string LEW;
   std::string LWS;
}; //END lights

//container for the stop line geometry
struct stopLineGeometry {
   //name of the stop line in terms of ID value
   int name;

   //shape and region for the stop line
   std::string shape;
   std::string regionProperty;

   //the x and y position of the stop lines boundaries
   std::vector<double> xPositions;
   std::vector<double> yPositions;
}; //END stopLineGeometry

//container for the trajectory of the vehicles
struct trajectory {
   //track ID for the trajectory
   int trackID;

   //x and y position for the vehicle at the given moment
   double xPosition;
   double yPosition;

   //speed for the vehicle at the given moment
   double speed;

   //tangental acceleration for the vehicle at the given moment
   double tangentialAcc;

   //lateral acceleration for the vehicle at the given moment
   double lateralAcc;

   //current moment in time since the recording started
   double time;

   //vehicle angle in terms of its yaw
   double angle;
   std::string region;
}; //END trajectory

//contains callback function IDs
enum Callback {String = -1, Tracks = 0, Lights = 1, Regions = 2, Trajectory = 3};

//contains codes for intersection gates
enum Gates {North = 20, East = 37, South = 68, West = 67};

/* Make a Struct Containing Vehicle Data */
struct commandLineInput ARGUMENTS;

//define variables to access the containers
struct lightSignals LightStates;
struct intersectionDirections IntersectionDir;
struct defaultValues Defaults;

//struct to contain the data for the lights
std::vector<struct lights> lightData;

//struct to contain the data for the tracks
std::vector<struct tracks> trackData;

//contains geometry information for the stop lines
std::vector<struct stopLineGeometry> stopLines;

//contains the trajectory information for the vehicles
std::vector<struct trajectory> trajectoryInformation;

//statistics containers
int redLights = 0;
int redLightPass = 0;
int redLightPassNoStop = 0;
int permissiveLeft = 0;
int leftEncounter = 0;
int yellowFail = 0;
int yellowPass = 0;
int green = 0;
int redLightStops = 0;
int yellowLightStops = 0;
int totalRedLights = 0;
int redLightPassesWithSlowdown = 0;
int totalRedLightPasses = 0;
int totalRedLightPassesNoStop = 0;
int totalRedLightPassesWithSlowdown = 0;
int totalPermissiveLeft = 0;
int totalLeftEncounter = 0;
int totalYellowFail = 0;
int totalYellowPass = 0;
int totalGreen = 0;
int totalRedLightStops = 0;
int totalYellowLightStops = 0;

//config for traffic light database formatting
int config = 0;

/*
Input:
   void * data - Contains any error output from SQLite3.
   int argc - Contains the number of c-strings extracted from the database.
   char ** argv - Contains the c-strings extracted from the database.
   char ** azColName - Contains the name of the column for the c-string with the corresponding index.
Output:
   static int callbackRegions - Signals if the function was successful or not.
Method:
   Extract the results from the database and place them into a struct.
Formatting:
   We can find formatting for the database TRAJECTORIES_0<file_id> table from the following:
      https://uwaterloo.ca/waterloo-intelligent-systems-engineering-lab/datasets/waterloo-multi-agent-traffic-dataset-intersection
   The table is formatted as follows:
     |   Column Name    |                                                   Description                                                    
     |TRACK_ID	         | Track ID of the agent in the file.
     |X	               | UTM lateral co-ordinate (zone 17T) of the position of the agent.
     |Y	               | UTM longitudinal co-ordinate (zone 17T) of the position of the agent.
     |SPEED	            | Speed of the agent in kilometers per hour at a specific moment.
     |TAN_ACC	         | Tangential acceleration of the agent in meters per second squared at a specific moment.
     |LAT_ACC	         | Lateral acceleration of the agent in meters per second squared at a specific moment.
     |TIME	            | Seconds from the beginning of the video, where the position, speed, and acceleration of the vehicle are measured.
     |ANGLE	            | Vehicle angle for each exported position of a Trajectory.
     |TRAFFIC_REGIONS   | [NO DESCRIPTION]
Description:
   Callback function for extracting data from the trajectory database.
*/
static int callbackTrajectory(void * data, int argc, char ** argv, char ** azColName){
   //contains the current traffic state at the given moment
   struct trajectory temp;

   //add data to the struct
   temp.trackID = argv[0] ? std::stoi(argv[0]) : NO_RESULT;
   temp.xPosition = argv[1] ? std::stod(argv[1]) : NO_RESULT;
   temp.yPosition = argv[2] ? std::stod(argv[2]) : NO_RESULT;
   temp.speed = argv[3] ? std::stod(argv[3]) : NO_RESULT;
   temp.tangentialAcc = argv[4] ? std::stod(argv[4]) : NO_RESULT;
   temp.lateralAcc = argv[5] ? std::stod(argv[5]) : NO_RESULT;
   temp.time = argv[6] ? std::stod(argv[6]) : NO_RESULT;
   temp.angle = argv[7] ? std::stod(argv[7]) : NO_RESULT;
   temp.region = argv[8] ? argv[8] : Defaults.NullInput;

   //add the traffic state at the given moment to the list
   trajectoryInformation.push_back(temp);

   //return success
   return 0;
} //END callbackTrajectory

/*
Input:
   void * data - Contains any error output from SQLite3.
   int argc - Contains the number of c-strings extracted from the database.
   char ** argv - Contains the c-strings extracted from the database.
   char ** azColName - Contains the name of the column for the c-string with the corresponding index.
Output:
   static int callbackRegions - Signals if the function was successful or not.
Method:
   Extract the results from the database and place them into a struct.
Formatting:
   We can find formatting for the database TRAJECTORIES_0<file_id> table from the following:
      https://uwaterloo.ca/waterloo-intelligent-systems-engineering-lab/datasets/waterloo-multi-agent-traffic-dataset-intersection
   The table is formatted as follows:
     |   Column Name    |                                                   Description
     |TRACK_ID	         | Track ID of the agent in the file.
     |X	               | UTM lateral co-ordinate (zone 17T) of the position of the agent.
     |Y	               | UTM longitudinal co-ordinate (zone 17T) of the position of the agent.
     |SPEED	            | Speed of the agent in kilometers per hour at a specific moment.
     |TAN_ACC	         | Tangential acceleration of the agent in meters per second squared at a specific moment.
     |LAT_ACC	         | Lateral acceleration of the agent in meters per second squared at a specific moment.
     |TIME	            | Seconds from the beginning of the video, where the position, speed, and acceleration of the vehicle are measured.
     |ANGLE	            | Vehicle angle for each exported position of a Trajectory.
     |TRAFFIC_REGIONS   | [NO DESCRIPTION]
Description:
   Callback function for extracting data from the lights database.
*/
static int callbackLights(void * data, int argc, char ** argv, char ** azColName){
   //contains the current traffic state at the given moment
   struct lights temp;

   if (config == 1) {
      //add data to the struct
      temp.fileID = argv[0] ? std::stoi(argv[0]) : NO_RESULT;
      temp.LNE = argv[1] ? argv[1] : Defaults.NullInput;
      temp.LSW = argv[2] ? argv[2] : Defaults.NullInput;
      temp.LNS = argv[3] ? argv[3] : Defaults.NullInput;
      temp.LSE = argv[4] ? argv[4] : Defaults.NullInput;
      temp.LSN = argv[5] ? argv[5] : Defaults.NullInput;
      temp.LWN = argv[6] ? argv[6] : Defaults.NullInput;
      temp.LES = argv[7] ? argv[7] : Defaults.NullInput;
      temp.LWE = argv[8] ? argv[8] : Defaults.NullInput;
      temp.LEW = argv[9] ? argv[9] : Defaults.NullInput;
      temp.LWS = argv[10] ? argv[10] : Defaults.NullInput;
      temp.time = argv[11] ? std::stod(argv[11]) : NO_RESULT;
   }
   else if (config == 2) {
      //add data to the struct
      temp.fileID = argv[0] ? std::stoi(argv[0]) : NO_RESULT;
      temp.LNE = argv[1] ? argv[1] : Defaults.NullInput;
      temp.LNS = argv[2] ? argv[2] : Defaults.NullInput;
      temp.LSN = argv[3] ? argv[3] : Defaults.NullInput;
      temp.LEW = argv[4] ? argv[4] : Defaults.NullInput;
      temp.LWE = argv[5] ? argv[5] : Defaults.NullInput;
      temp.LSW = argv[6] ? argv[6] : Defaults.NullInput;
      temp.LES = argv[7] ? argv[7] : Defaults.NullInput;
      temp.LWN = argv[8] ? argv[8] : Defaults.NullInput;
      temp.LSE = argv[9] ? argv[9] : Defaults.NullInput;
      temp.LWS = argv[10] ? argv[10] : Defaults.NullInput;
      temp.time = argv[11] ? std::stod(argv[11]) : NO_RESULT;
   } //END IF

   //add the traffic state at the given moment to the list
   lightData.push_back(temp);

   //return success
   return 0;
} //END callbackLights

/*
Input:
   void * data - Contains any error output from SQLite3.
   int argc - Contains the number of c-strings extracted from the database.
   char ** argv - Contains the c-strings extracted from the database.
   char ** azColName - Contains the name of the column for the c-string with the corresponding index.
Output:
   static int callbackRegions - Signals if the function was successful or not.
Method:
   Extract the results from the database and outputs it to the terminal.
   Results are automatically formatted and printed based on the number of arguments and columns.
Description:
   Callback function for extracting data from any database and outputting the formatted data to the terminal.
*/
static int callbackString(void * data, int argc, char ** argv, char ** azColName) {
   //print the data to the terminal
   fprintf(stderr, "%s: ", (const char *) data);

   //print the output to the terminal
   for (int i = 0; i < argc; i++) {
      ARGUMENTS.caseOutput ? printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL") : NO_OUTPUT;
   } //END FOR

   //add a newline character
   ARGUMENTS.caseOutput ? printf("\n") : NO_OUTPUT;

   //return success
   return 0;
} //END callbackString

/*
Input:
   void * data - Contains any error output from SQLite3.
   int argc - Contains the number of c-strings extracted from the database.
   char ** argv - Contains the c-strings extracted from the database.
   char ** azColName - Contains the name of the column for the c-string with the corresponding index.
Output:
   static int callbackRegions - Signals if the function was successful or not.
Method:
   Extract the results from the database and place them into a struct.
Formatting:
   We can find formatting for the database TRAFFIC_REGIONS_DEF table from the following:
      https://uwaterloo.ca/waterloo-intelligent-systems-engineering-lab/datasets/waterloo-multi-agent-traffic-dataset-intersection
   The table is formatted as follows:
     |   Column Name    |                                                   Description
     |NAME	            | Name of the spatial entity. See additional column info below.
     |REGION_ID	      | [NO DESCRIPTION]
     |SHAPE	            | Geometric shape of this spatial entity.
     |REGION_PROPERTY	| Additional information about the type of this spatial region. See additional column info below.
     |X_POSITIONS	      | UTM lateral points (zone 17T) of the this spatial entity.
     |                  | For centerlines, the points are listed in sequence along the direction of travel.
     |                  | For entry and exit boundaries, the points are listed from left to right when encountered.
     |                  | along the direction of travel.
     |Y_POSITIONS	      | UTM longitudinal points (zone 17T) of the this spatial entity.
     |                  | For centerlines, the points are listed in sequence along the direction of travel.
     |                  | For entry and exit boundaries, the points are listed from left to right when encountered
     |                  | along the direction of travel.
   Name value Information:
     |                         Discrete Value                         |                            Description
     | prep-turn_s, exec-turn_s, l_s_n, l_n_s_l, ln_e_3, ln_e_2,      | These are spatial traffic regions. Please refer to the diagram.
     | ln_e_1, ln_n_1, ln_n_2, ln_n_3, ln_s_-2, ln_s_-1, l_n_s_r,     | below for a visual depiction of the location of each region in.
     | int-entry_n, ln_w_-1, ln_w_-2, ln_s_1, ln_e_-1, ln_e_-2,       | the intersection.
     | l_w_e_l, l_w_e_r, ln_w_2, ln_w_3, ln_w_1, l_e_-1, l_e_-2,      |
     | ln_w_4, l_e_w_l, l_e_w_r, ln_n_-1, ln_n_-2, l_s_n_l,           |
     | l_s_n_r, prep-turn_w, exec-turn_w, ln_s_2, ln_s_3, ln_s_4,     |
     | rt_exec-turn_s, rt_exec-turn_w, rt_prep-turn_s,                |
     | rt_prep-turn_w, prep-turn_e, exec-turn_e, exec-turn_n,         |
     | prep-turn_n.                                                   |
     | S_W, W_N, E_S, N_E, S_E, W_S, S_N, N_S, E_W, W_E	             | Direction code.
     | 67, 21, 20, 36, 37, 69, 68, 66	                               | Gate IDs for the SEGMENT_GATE gates.
   Region Property Information:
     |  Discrete Value  |                                                   Description
     | exit_boundary    | Exit boundary of the traffic segment.
     | left_boundary    | Left boundary of the traffic segment.
     | center_line      | Entry boundary of the traffic segment.
     | entry_boundary   | Entry boundary of the traffic segment.
     | gate_line        | The location of the SEGMENT_GATE gate of the corresponding gate id.
     | lane_boundary	   | The spatial boundary that covers all the lanes corresponding to the travel direction.
Description:
   Callback function for extracting data from the regions database.
*/
static int callbackRegions(void * data, int argc, char ** argv, char ** azColName) {
   //contains the current traffic state at the given moment
   struct stopLineGeometry temp;

   //contains the delimiter being used
   std::string delimiter = ",";

   //container for argument
   std::string argument;

   //contains the substring returned
   std::string substring;

   //add data to the struct
   temp.name = argv[0] ? std::stoi(argv[0]) : NO_RESULT;
   temp.shape = argv[2] ? argv[2] : Defaults.NullInput;
   temp.regionProperty = argv[3] ? argv[3] : Defaults.NullInput;

   //obtain the boundary line data
   argument = argv[4] ? argv[4] : Defaults.NullInput;

   //check if the argument contains data
   if (argument != Defaults.NullInput) {
      //erase the specified character
      argument.erase(0, 1);

      //obtain the specified substring
      argument = argument.std::string::substr(0, argument.find("]"));

      //get the y position from the specified substring
      temp.xPositions.push_back(std::stod(argument.substr(0, argument.find(delimiter))));

      //get the x position from the specified substring
      temp.xPositions.push_back(std::stod(argument.substr(1, argument.find(delimiter))));
   } //END IF

   //obtain the boundary line data
   argument = argv[5] ? argv[5] : Defaults.NullInput;

   //check if the argument contains data
   if (argument != Defaults.NullInput) {
      //erase the specified character
      argument.erase(0, 1);

      //obtain the specified substring
      argument = argument.std::string::substr(0, argument.find("]"));

      //get the y position from the specified substring
      temp.yPositions.push_back(std::stod(argument.substr(0, argument.find(delimiter))));

      //get the x position from the specified substring
      temp.yPositions.push_back(std::stod(argument.substr(1, argument.find(delimiter))));
   } //END IF

   //add the traffic state at the given moment to the list
   stopLines.push_back(temp);

   //return success
   return 0;
} //END callbackRegions

/*
Input:
   void * data - Contains any error output from SQLite3.
   int argc - Contains the number of c-strings extracted from the database.
   char ** argv - Contains the c-strings extracted from the database.
   char ** azColName - Contains the name of the column for the c-string with the corresponding index.
Output:
   static int callbackTracks - Signals if the function was successful or not.
Method:
   Extract the results from the database and place them into a struct.
Description:
   Callback function for extracting data from the tracks database.
*/
static int callbackTracks(void * data, int argc, char ** argv, char ** azColName) {
   //contains the current traffic state at the given moment
   struct tracks temp;

   //add data to the struct
   temp.trackID = argv[1] ? std::stoi(argv[1]) : NO_OUTPUT;
   temp.fileID = argv[0] ? std::stoi(argv[0]) : NO_OUTPUT;
   temp.type = argv[2] ? argv[2] : Defaults.NullInput;
   temp.width = argv[3] ? std::stod(argv[3]) : NO_OUTPUT;
   temp.length = argv[4] ? std::stod(argv[4]) : NO_OUTPUT;
   temp.entryGate = argv[5] ? argv[5] : Defaults.NullInput;
   temp.entryTime = argv[6] ? std::stod(argv[6]) : NO_OUTPUT;
   temp.exitGate = argv[7] ? argv[7] : Defaults.NullInput;
   temp.exitTime = argv[8] ? std::stod(argv[8]) : NO_OUTPUT;
   temp.traveledDist = argv[9] ? std::stod(argv[9]) : NO_OUTPUT;
   temp.averageSpeed = argv[10] ? std::stod(argv[10]) : NO_OUTPUT;

   //add the traffic state at the given moment to the list
   trackData.push_back(temp);

   //return success
   return 0;
} //END callbackTracks

/*
Input:
   const char * command - Contains the given command for SQLite3 database call.
   const char * database - Contains the path to the given SQLite3 database.
   int index - Represents the type of struct the data should be stored in.
Output:
   int sqlCall - Returns if the operation was successful or not.
Methods:
   Uses SQLite3's sqlite3_open function with inputted commands and databases to extract data.
   Data is placed into the correct struct based on the given database format.
Formatting:
   We can find formatting for the database TRAJECTORIES_0<file_id> table from the following:
      https://uwaterloo.ca/waterloo-intelligent-systems-engineering-lab/datasets/waterloo-multi-agent-traffic-dataset-intersection
   The table is formatted as follows:
     |   Column Name    |                                                   Description
     | FILE_ID          | [NO DESCRIPTION]
     | TRACK_ID         | Track ID of the agent in the file.
     | TYPE             | Type of the agent.
     | TRACK_WIDTH      | [NO DESCRIPTION]
     | TRACK_LENGTH     | [NO DESCRIPTION]
     | ENTRY_GATE       | The entry gate ID of intersection from which the agent enters.
     | ENTRY_TIME       | The time in seconds since the start of the file when the agent crosses the entry gate.
     | EXIT_GATE        | The exit gate ID of intersection from which the agent exits the intersection.
     | EXIT_TIME        | The time in seconds since the start of the file when the agent crosses the exit gate.
     | TRAVELED_DIST    | Distance in meters travelled by the agent between the two gates.
     | AVG_SPEED        | Average speed in kilometers per hour of the agent.
   The types of agents are listed as follows:
     |  Discrete Value  |   Description
     | Car              | Car
     | Pedestrian       | Pedestrian
     | Heavy Vehicle    | Heavy Vehicle
     | Medium Vehicle   | Medium Vehicle
     | Bicycle          | Bicycle
     | Bus              | Bus
Description:
   Function to extract data from the SQLite3 database using the correct format and putting data into the correct struct.
*/
int sqlCall(const char * command, const char * database, int index) {
   //contains error messages
   char * zErrMsg = 0;

   //contains the sqlite3 pointer
   sqlite3 * db;

   //contains weather or not the database opened correctly
   int rc;

   //returns that the callback function was called
   const char * data = "Callback function called";

   //attempt to open the database file using sqlite3
   rc = sqlite3_open(database, &db);

   //checks if the database could not be opened
   if (rc) {
      //return error message to terminal
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));

      //return that the function did not execute successfully
      return 0;
   } //END IF

   //determine the format of the database to put data into the correct struct or output data to the terminal directly
   if (index == String) {
      rc = sqlite3_exec(db, command, callbackString, (void*) data, & zErrMsg);
   }
   else if (index == Tracks) {
      /* Execute SQL statement */
      rc = sqlite3_exec(db, command, callbackTracks, (void*) data, & zErrMsg);
   }
   else if (index == Lights) {
      /* Execute SQL statement */
      rc = sqlite3_exec(db, command, callbackLights, (void*) data, & zErrMsg);
   }
   else if (index == Regions) {
      /* Execute SQL statement */
      rc = sqlite3_exec(db, command, callbackRegions, (void*) data, & zErrMsg);
   }
   else if (index == Trajectory) {
      /* Execute SQL statement */
      rc = sqlite3_exec(db, command, callbackTrajectory, (void*) data, & zErrMsg);
   } //END IF

   //if the database could not open
   if (rc != SQLITE_OK) {
      //print an error message
      fprintf(stderr, "SQL error: %s\n", zErrMsg);

      //free memory
      sqlite3_free(zErrMsg);
   }
   else {
      ARGUMENTS.caseOutput ? fprintf(stdout, "Operation done successfully\n") : NO_OUTPUT;
   } //END IF

   //close the sqlite database
   sqlite3_close(db);

   //return that the function executed successfully
   return 1;
} //END sqlCall

/*
Input:
   double speed - The current speed of the vehicle.
   ARGUMENTS.vehicleBreakingAcc - Contains the current breaking acceleration for the vehicles.
Output:
   double getBreakingDistance - Contains the breaking distance of the given vehicle.
Methods:
   Calcualtes the breaking distance given a formula.
Formula:
   We determine breaking distance from the equation:
      Vf^2 = Vi^2 + 2a(Δx)
Description:
   Gets the breaking distance given the vehicles current speed and the vehicles breaking acceleration.
*/
double getBreakingDistance(double speed) {
   /*
   We know that: A_b = V^2 / (2 * SD) as v_f^2 = v_i^2 + 2a(Δx)
      A_b - the Braking Acceleration (m/s^2)
      V - the velocity before braking (m/s)
      SD - the total stopping distance (m)
   */

   //Returns the Results of the Breaking Distance Equation
   return (((speed*0.277778) * (speed*0.277778)) / (2 * ARGUMENTS.vehicleBreakingAcc));
} //END getBreakingDistance

/*
Input:
   int ID - Contains the current track ID.
   double xpos - Contains the current x position for the vehicle.
   double ypos - Contains the current y position for the vehicle.
   double entryTime - Contains the current time for the vehicle.
Output:
   bool findDistance - Determines if a vehicle is capable of stopping at a yellow light or not.
Method:
   Determines if a vehicle approaching a stop line is can brake before the line.
Description:
   Determines if a vehicle can stop at the line during a yellow light interval.
*/
//checks if the vehicle is capable of stopping at the line
bool findDistance(int ID, double xpos, double ypos, double entryTime) {
   //container for the x and y positions and related distances for vehicle
   double xPosNew, yPosNew, breakingDistance, lineDistance, reactionDistance;

   //for every vehicle trajectory recorded
   for (long unsigned int i = 0; i < trajectoryInformation.size(); i++) {
      //if the vehicle ID is not recorded
      if (trajectoryInformation[i].trackID == NO_RESULT || trajectoryInformation[i].time == NO_RESULT) {
         //skip to the next record
         continue;
      }
      //if the vehicle ID is the vehicle we are looking for and the time is past the entry moment
      else if (trajectoryInformation[i].trackID == ID && trajectoryInformation[i].time >= entryTime) {
         //contains the new x and y positions for the vehicle
         xPosNew = trajectoryInformation[i].xPosition;
         yPosNew = trajectoryInformation[i].yPosition;

         //breaking distance is converted from speed (km/h) to distance in meters
         breakingDistance = getBreakingDistance(trajectoryInformation[i].speed);

         //gets the reaction distance for the vehicle
         reactionDistance = trajectoryInformation[i].speed * ARGUMENTS.reactionTime;

         //UTM coordinates are measured in meters
         lineDistance = sqrt(pow(xPosNew - xpos, 2) + pow(yPosNew - ypos, 2) * 1.0);

         //if the breaking distance is smaller then the distance to the entry gate
         if (breakingDistance + reactionDistance <= lineDistance) {
            //return output if applicable
            ARGUMENTS.caseOutput ? printf("  -> Vehicle Speed: %fkm/h, Vehicle X: %fm, Vehicle Y: %fm, Breaking Distance: %fm\n",
                                          trajectoryInformation[i].speed, xPosNew, yPosNew,
                                          breakingDistance + reactionDistance) : NO_OUTPUT;
                                          //END printf
            ARGUMENTS.caseOutput ? printf("  -> Gate X: %fm, Gate Y: %fm, Gate Distance: %fm\n",
                                          xpos, ypos, lineDistance) : NO_OUTPUT;
                                          //END printf

            //return success
            return true;
         } //END IF
      } //END IF
   } //END FOR

   //return that the vehicle is not capable of stopping at the line
   return false;
} //END findDistance

/*
Input:
   int ID - Contains the current track ID.
   double xpos - Contains the current x position for the vehicle.
   double ypos - Contains the current y position for the vehicle.
   double startTime - Contains the time the vehicle crossed the entry gate into the intersection.
   double distanceThreshold - Contains the distance threshold for vehicle stopping distance to the stop line.
   double speedThreshold - Contains the distance threshold for minimum vehicle stopping speed when stopping at the stop line.
   bool countStops - Flag to check if a vehicle stops at red or yellow lights before crossing.
Output:
   bool stoppedWithinLine - Determines if a vehicle is capable of stopping at a yellow light or not.
Method:
   Determines if a vehicle approaching a stop line is can brake before the line.
Description:
   Determines if a vehicle can stop at the line during a yellow light interval.
*/
//checks if the vehicle is capable of stopping at the line
bool stoppedWithinLine(int ID, double xpos, double ypos, double startTime, double distanceThreshold, double speedThreshold, bool countStops, std::string light) {
   //container for the x and y positions and related distances for vehicle
   double xPosNew, yPosNew, lineDistance;

   //contains the current signal state for the traffic light
   std::string state;

   //for every vehicle trajectory recorded
   for (long unsigned int i = 0; i < trajectoryInformation.size(); i++) {
      //if the vehicle ID is not recorded
      if (trajectoryInformation[i].trackID == NO_RESULT || trajectoryInformation[i].time == NO_RESULT) {
         //skip to the next record
         continue;
      }
      //if the vehicle ID is the vehicle we are looking for and the time is past the entry moment
      else if (trajectoryInformation[i].trackID == ID) {
         //contains the new x and y positions for the vehicle
         xPosNew = trajectoryInformation[i].xPosition;
         yPosNew = trajectoryInformation[i].yPosition;

         //UTM coordinates are measured in meters
         lineDistance = std::min(std::abs(xPosNew - xpos), std::abs(yPosNew - ypos));

         //for every light data element
         for (long unsigned int k = 0; k < lightData.size(); k++) {
            //check the traffic light state at the given moment for the current trajectory interval
            if (lightData[k].time <= trajectoryInformation[i].time) {
               //check the light position matches the intersections light position and extract the lights current state
               if (light == IntersectionDir.LNE) {
                  state = lightData[k].LNE;
               }
               else if (light == IntersectionDir.LSW) {
                  state = lightData[k].LSW;
               }
               else if (light == IntersectionDir.LNS) {
                  state = lightData[k].LNS;
               }
               else if (light == IntersectionDir.LSE) {
                  state = lightData[k].LSE;
               }
               else if (light == IntersectionDir.LSN) {
                  state = lightData[k].LSN;
               }
               else if (light == IntersectionDir.LWN) {
                  state = lightData[k].LWN;
               }
               else if (light == IntersectionDir.LES) {
                  state = lightData[k].LES;
               }
               else if (light == IntersectionDir.LWE) {
                  state = lightData[k].LWE;
               }
               else if (light == IntersectionDir.LEW) {
                  state = lightData[k].LEW;
               }
               else if (light == IntersectionDir.LWS) {
                  state = lightData[k].LWS;
               } //END IF
            } //END IF
         } //END FOR

         //if the breaking distance is smaller then the distance to the entry gate
         if (lineDistance <= distanceThreshold && trajectoryInformation[i].speed <= speedThreshold && trajectoryInformation[i].time <= startTime) {
            //check if we need to count stops at lights
            if (countStops) {
               //check if there is a red or a yellow light state
               if (state == LightStates.Red) {
                  redLightStops += 1;
               }
               else if (state == LightStates.Yellow) {
                  yellowLightStops += 1;
               } //END IF
            } //END IF

            //return success
            return true;
         } //END IF
      } //END IF
   } //END FOR

   //return that the vehicle is not capable of stopping at the line
   return false;
} //END stoppedWithinLine

/*
Input:
   std::string gate - Contains the string representation for the current gate.
   int index - The location in the gate string where the character representing the gate direction is located.
Output:
   int findGateID - Finds the numerical ID for a given gate string.
Method:
   Finds the direction of the gate and returns the gateID based off that value.
Description:
   Determines the ID value for a given gate string.
*/
int findGateID(std::string gate, int index = 1) {
   //container for the gate ID
   int gateID;

   //if the gate is recorded the default value of NULL is not the value of the gate string
   if (gate != Defaults.NullInput) {
      //check all four gates and record based off thier ID value
      if (std::to_string(gate[index]) == std::to_string(IntersectionDir.NORTH)) {
         gateID = Gates::North;
      }
      else if (std::to_string(gate[index]) == std::to_string(IntersectionDir.EAST)) {
         gateID = Gates::East;
      }
      else if (std::to_string(gate[index]) == std::to_string(IntersectionDir.SOUTH)) {
         gateID = Gates::South;
      }
      else if (std::to_string(gate[index]) == std::to_string(IntersectionDir.WEST)) {
         gateID = Gates::West;
      }
      else {
         //output error message to terminal
         printf("ERROR: CANNOT FIND DIRECTION OF GATE");
      } //END IF
   } //END IF

   //return the gate ID as they are referenced for the vehicles
   return gateID;
} //END findGateID

/*
Input:
   double startX - This is the starting x position.
   double startY - This is the starting y position.
   double endX - This is the ending x position.
   double endY - This is the ending y position.
   double speed - This is the current speed of the vehicle.
Output:
   double findCrossingTimeMax - Returns the result of the crossing time calculation.
Method:
   Finds crossing time by taking the distance between the two points asedges along a
   rectangle and determining how long it would take to cross the given path.
Description:
   Determines the amount of time needed to enter/exit an intersection
   given a specific speed that is assumed to be constant.
*/
double findCrossingTimeMax(double startX, double startY, double endX, double endY, double speed) {
   //determine the horizontal and vertical difference between the two points as a right angle
   double horizontalDifference = (startX - endX) * (startX - endX);
   double verticalDifference = (startY - endY) * (startY - endY);

   //finds the crossing time based on vehicle speed at the current moment
   return (std::pow(horizontalDifference + verticalDifference, 0.5) / (speed * 0.277778));
} //END findCrossingTimeMax

/*
Input:
   int ID - The ID of the vehicle track.
   std::string entryGate - The string name of the entry gate at the intersection.
   std::string exitGate - The string name of the exit gate at the intersection.
   double lightChangeTime - The start time upon entering the intersection.
   double endTime - The end time upon leaving the intersection.
Output:
   bool checkPermissiveLeftTurn - Signals if a permissive left turn was possible at the given moment.
Method:
   Determines if a permissive left turn was possible at the moment the vehicle left the intersection.
   Checks by determining if the vehicle made any other vehicles slow down during its duration entering and exiting the intersection.
Description:
   Checks if a permissive left turn has occured without forcing an oncoming vehicle to slow down
*/
int checkPermissiveLeftTurn(int ID, std::string entryGate, std::string exitGate, double lightChangeTime, double endTime) {
   //containers for relevant geometric and vehicle constraints
   double startX, startY, endX, endY, speed, crossingTime, otherCrossingTime;

   //container for the entry gates name
   std::string entryGateID;

   //determine the time it took for the reference vehicle to cross the intersection
   crossingTime = abs(lightChangeTime - endTime);

   //for each trajectory recorded in the struct
   for (long unsigned int i = 0; i < trajectoryInformation.size(); i++) {
      //if the trajectory track ID OR the trajectory time are not recorded
      if (trajectoryInformation[i].trackID == NO_RESULT ||
          trajectoryInformation[i].time == NO_RESULT) {
          //END IF
         //continue to the next iteration of the loop
         continue;
      }
      /*
      We check the following:
         Checks if the trajectory ID is not the same as the reference ID.
         The trajectory time at the current moment is greater then or equal to the start moment for the reference vehicle.
         The trajectory time at the current moment is less then or equal to the end moment for the reference vehicle.
      */
      else if (trajectoryInformation[i].trackID != ID &&
               trajectoryInformation[i].time >= lightChangeTime &&
               trajectoryInformation[i].time <= endTime) {
               //END IF
         //we check the starting x and y coordinates and the speed of the vehicle
         startX = trajectoryInformation[i].xPosition;
         startY = trajectoryInformation[i].yPosition;
         speed = trajectoryInformation[i].speed;

         //for every track recorded
         for (long unsigned int k = 0; k < trackData.size(); k++) {
            /*
            We check the following:
               The vehicle is different then the reference vehicle.
               The track ID is the same between the track and trajectory.
               The track has its exit gate recorded.
            */
            if (trackData[k].trackID != ID &&
                trackData[k].trackID == trajectoryInformation[i].trackID &&
                trackData[k].entryGate != Defaults.NullInput) {
                //END IF
               //get the entry gate ID
               entryGateID = trackData[k].entryGate.c_str()[2];

               //The lane does not have a permissive left turn
               if (entryGate.c_str()[2] != 'n') {
                  return -1;
               } //END IF

               //for every stop line recorded
               for (long unsigned int j = 0; j < stopLines.size(); j++) {
                  //if the other vehicles stop line has the same ID as the entry gate ID
                  if (stopLines[j].name == findGateID(entryGateID, 0) && entryGateID.c_str()[0] == 's') {
                     //determines the x and y values of the start of the intersection
                     endX = stopLines[j].xPositions[0];
                     endY = stopLines[j].yPositions[0];

                     //determines the crossing time for the other vehicle
                     otherCrossingTime = findCrossingTimeMax(startX, startY, endX, endY, speed);
                     
                     //determine if the other vehicle will take less time to cross then the reference vehicle
                     if (otherCrossingTime - crossingTime < 0) {
                        //return details for the vehicles
                        ARGUMENTS.caseOutput ? printf("     -> Track ID: %i\n", trackData[k].trackID) : NO_OUTPUT;
                        ARGUMENTS.caseOutput ? printf("     -> Start Position: %fm (x) %fm (y)\n", startX, startY) : NO_OUTPUT;
                        ARGUMENTS.caseOutput ? printf("     -> End Position: %fm (x) %fm (y)\n", endX, endY) : NO_OUTPUT;
                        ARGUMENTS.caseOutput ? printf("     -> Speed: %fkm/h\n", speed) : NO_OUTPUT;
                        ARGUMENTS.caseOutput ? printf("     -> Time for Reference (%fs) to Cross Intersection vs Other Vehicle (%fs)\n",
                                                      crossingTime, otherCrossingTime) : NO_OUTPUT;
                                                      //END printf

                        //returns that the other vehicle must slow down for the permissive left to have occured
                        return 0;
                     } //END IF
                  } //END IF
               } //END FOR
            } //END IF
         } //END IF
      } //END IF
   } //END FOR

   //returns that the other vehicle does not need to slow down for the permissive left to have occured
   return 1;
} //END checkPermissiveLeftTurn

/*
Input:
   int ID - The ID of the vehicle track.
   std::string entryGate - The string name of the entry gate at the intersection.
   std::string exitGate - The string name of the exit gate at the intersection.
   double lightChangeTime - The start time upon entering the intersection.
   double endTime - The end time upon leaving the intersection.
Output:
   bool checkRedLightYieldViolation - Signals if a red light right turn slowed down vehicles going straight from the left.
Method:
   Determines if a permissive left turn was possible at the moment the vehicle left the intersection.
   Checks by determining if the vehicle made any other vehicles slow down during its duration entering and exiting the intersection.
Description:
   Checks if a permissive left turn has occured without forcing an oncoming vehicle to slow down
*/
int checkRedLightYieldViolation(int ID, std::string entryGate, std::string exitGate, double lightChangeTime, double endTime) {
   //containers for relevant geometric and vehicle constraints
   double startX, startY, endX, endY, speed, crossingTime, otherCrossingTime;

   //container for the entry gates name
   std::string entryGateID, turnType;

   //determine the time it took for the reference vehicle to cross the intersection
   crossingTime = abs(lightChangeTime - endTime);

   //for each trajectory recorded in the struct
   for (long unsigned int i = 0; i < trajectoryInformation.size(); i++) {
      //if the trajectory track ID OR the trajectory time are not recorded
      if (trajectoryInformation[i].trackID == NO_RESULT ||
          trajectoryInformation[i].time == NO_RESULT) {
          //END IF
         //continue to the next iteration of the loop
         continue;
      }
      /*
      We check the following:
         Checks if the trajectory ID is not the same as the reference ID.
         The trajectory time at the current moment is greater then or equal to the start moment for the reference vehicle.
         The trajectory time at the current moment is less then or equal to the end moment for the reference vehicle.
      */
      else if (trajectoryInformation[i].trackID != ID &&
               trajectoryInformation[i].time >= lightChangeTime &&
               trajectoryInformation[i].time <= endTime) {
               //END IF
         //we check the starting x and y coordinates and the speed of the vehicle
         startX = trajectoryInformation[i].xPosition;
         startY = trajectoryInformation[i].yPosition;
         speed = trajectoryInformation[i].speed;

         //for every track recorded
         for (long unsigned int k = 0; k < trackData.size(); k++) {
            /*
            We check the following:
               The vehicle is different then the reference vehicle.
               The track ID is the same between the track and trajectory.
               The track has its exit gate recorded.
            */
            if (trackData[k].trackID != ID &&
                trackData[k].trackID == trajectoryInformation[i].trackID &&
                trackData[k].entryGate != Defaults.NullInput) {
                //END IF
               //get the entry gate ID
               entryGateID = trackData[k].entryGate.c_str()[2];

               //for every stop line recorded
               for (long unsigned int j = 0; j < stopLines.size(); j++) {
                  //if the other vehicles stop line has the same ID as the entry gate ID
                  if (stopLines[j].name == findGateID(entryGateID, 0)) {
                     //determines the x and y values of the start of the intersection
                     endX = stopLines[j].xPositions[0];
                     endY = stopLines[j].yPositions[0];

                     //clear the turn type string
                     turnType.clear();

                     //push back new characters to the string from predefined locations in the entry gate c-string
                     turnType.push_back(trackData[k].entryGate.c_str()[7]);
                     turnType.push_back(trackData[k].entryGate.c_str()[8]);

                     //determines the crossing time for the other vehicle
                     otherCrossingTime = findCrossingTimeMax(startX, startY, endX, endY, speed);
                     
                     //determine if the other vehicle will take less time to cross then the reference vehicle
                     if (otherCrossingTime - crossingTime < 0 && turnType == IntersectionDir.STRAIGHT) {
                        //return details for the vehicles
                        ARGUMENTS.caseOutput ? printf("     -> Track ID: %i\n", trackData[k].trackID) : NO_OUTPUT;
                        ARGUMENTS.caseOutput ? printf("     -> Start Position: %fm (x) %fm (y)\n", startX, startY) : NO_OUTPUT;
                        ARGUMENTS.caseOutput ? printf("     -> End Position: %fm (x) %fm (y)\n", endX, endY) : NO_OUTPUT;
                        ARGUMENTS.caseOutput ? printf("     -> Speed: %fkm/h\n", speed) : NO_OUTPUT;
                        ARGUMENTS.caseOutput ? printf("     -> Time for Reference (%fs) to Cross Intersection vs Other Vehicle (%fs)\n",
                                                      crossingTime, otherCrossingTime) : NO_OUTPUT;
                                                      //END printf

                        //returns that the other vehicle must slow down for the permissive left to have occured
                        return 0;
                     } //END IF
                  } //END IF
               } //END FOR
            } //END IF
         } //END IF
      } //END IF
   } //END FOR

   //returns that the other vehicle does not need to slow down for the permissive left to have occured
   return 1;
} //END checkRedLightYieldViolation

/*
Input:
   int ID - The ID of the track being checked.
   std::string gate - The name of the gate being used.
   double startTime - Contains the time the vehicle crossed the entry gate into the intersection.
   double distanceThreshold - Contains the distance threshold for vehicle stopping distance to the stop line.
   double speedThreshold - Contains the distance threshold for minimum vehicle stopping speed when stopping at the stop line.
   bool countStops - Flag to check if a vehicle stops at red or yellow lights before crossing.
Output:
   Outputs the result of findDistance given the input gate x and y position found in the given function.
Method:
   Calls the stop distance for the correct gate by searching through the
   .stopLines struct to find the gate corresponding to vehicles gate.
Description:
   Checks the distance between vehicles at specific moments and stop lines.
*/
bool withinStopline(int ID, std::string gate, double startTime, double distanceThreshold, double speedThreshold, bool countStops, std::string light) {
   //we find the gate ID of the entry gate
   int gateID = findGateID(gate, 2);

   //for each stop line in the dataset
   for (long unsigned int i = 0; i < stopLines.size(); i++) {
      //determine if the name of the stopline is equal to the getID for the entry gate
      if (stopLines[i].name == gateID) {
         //returns if the vehicle is capable of stopping at the line
         return stoppedWithinLine(ID, stopLines[i].xPositions[0], stopLines[i].yPositions[0], startTime, distanceThreshold, speedThreshold, countStops, light);
      } //END FOR
   } //END FOR

   //returns that the vehicle is not capable of stopping at the line by default
   return false;
} //END withinStopline


/*
Input:
   int ID - The ID of the track being checked.
   std::string gate - The name of the gate being used.
   double lightChangeTime - The current timestamp since the start of the recording.
Output:
   Outputs the result of findDistance given the input gate x and y position found in the given function.
Method:
   Calls the stop distance for the correct gate by searching through the
   stopLines struct to find the gate corresponding to vehicles gate.
Description:
   Checks the distance between vehicles at specific moments and stop lines.
*/
bool checkStopDistance(int ID, std::string gate, double lightChangeTime) {
   //we find the gate ID of the entry gate
   int gateID = findGateID(gate, 2);

   //for each stop line in the dataset
   for (long unsigned int i = 0; i < stopLines.size(); i++) {
      //determine if the name of the stopline is equal to the getID for the entry gate
      if (stopLines[i].name == gateID) {
         //returns if the vehicle is capable of stopping at the line
         return findDistance(ID, stopLines[i].xPositions[0], stopLines[i].yPositions[0], lightChangeTime);
      } //END FOR
   } //END FOR

   //returns that the vehicle is not capable of stopping at the line by default
   return false;
} //END checkStopDistance

/*
Input:
   trackData - A struct containing all tracks related to the database being checked.
   lightData - A struct containing all traffic light states during the start of the database recording.
Output:
   Prints the outcome of the intersection passes to the terminal.
Method:
   Sorts through formatted trackData and lightData structs to determine:
      If a vehicle has run through a red, yellow, or green light.
      If a vehicle needed to run a yellow light or not.
      If a vehicle running a red light on a left turn has the right to perform a permissive left turn or not.
      Keeps track of results to output final statistics for both the individual
      database being analyzed and all databases executed during the programs runtime.
Description:
   Function is used to extract traffic values from and format them.
*/
int getTrafficValues() {
   //contains string values for lanes
   std::string startGate, endGate, turnType, light, state;

   //contains time interval data
   double startTime, endTime, lightTime;

   //signals if a permissive left turn has occured
   int permissiveLeftNeeded;

   //signals if this is the first run of the given loop
   bool firstRun = true;

   //signals stop state for the current vehicle
   bool stopState = false;

   //for every track completed
   for (long unsigned int i = 0; i < trackData.size(); i++) {
      //signal that the loop is occuring
      firstRun = true;

      //reset for a new vehicle
      stopState = true;

      //clear the turn type string
      turnType.clear();

      //push back new characters to the string from predefined locations in the entry gate c-string
      turnType.push_back(trackData[i].entryGate.c_str()[7]);
      turnType.push_back(trackData[i].entryGate.c_str()[8]);

      //obtain the start and end gate characters
      startGate = trackData[i].entryGate.c_str()[2];
      endGate = trackData[i].exitGate.c_str()[2];

      //obtain the start and end time values
      startTime = trackData[i].entryTime;
      endTime = trackData[i].exitTime;

      //checks if both the entry time was recorded AND the entry gate were recorded
      if (startTime != NO_RESULT && endTime != NO_RESULT && trackData[i].entryGate != Defaults.NullInput && trackData[i].exitGate != Defaults.NullInput) {
         //continue if taking the right turn side lane
         if (startGate == "e" && endGate == "n") {
            continue;
         } 
         else if (startGate == "n" && endGate == "w") {
            continue;
         } //END IF

         //formats the string for the traffic light position
         light = "l" + startGate + endGate;

         //for every light data element
         for (long unsigned int k = 0; k < lightData.size(); k++) {
            /* Determine if:
            If the moments time for the traffic light state record is less then or equal to the vehicle intersection entry time
            This is our first run through this function at the iteration of the topmost for loop
            */
            if (lightData[k].time <= startTime && firstRun) {
               //check the light position matches the intersections light position and extract the lights current state
               if (light == IntersectionDir.LNE) {
                  state = lightData[k].LNE;
               }
               else if (light == IntersectionDir.LSW) {
                  state = lightData[k].LSW;
               }
               else if (light == IntersectionDir.LNS) {
                  state = lightData[k].LNS;
               }
               else if (light == IntersectionDir.LSE) {
                  state = lightData[k].LSE;
               }
               else if (light == IntersectionDir.LSN) {
                  state = lightData[k].LSN;
               }
               else if (light == IntersectionDir.LWN) {
                  state = lightData[k].LWN;
               }
               else if (light == IntersectionDir.LES) {
                  state = lightData[k].LES;
               }
               else if (light == IntersectionDir.LWE) {
                  state = lightData[k].LWE;
               }
               else if (light == IntersectionDir.LEW) {
                  state = lightData[k].LEW;
               }
               else if (light == IntersectionDir.LWS) {
                  state = lightData[k].LWS;
               } //END IF

               //records that we have already run through this step
               firstRun = false;

               //if the traffic light state is red
               if (state == LightStates.Red) {
                  //print data to terminal if applicable
                  ARGUMENTS.caseOutput ? printf("Vehicle: %i runs a red light\n", trackData[i].trackID) : NO_OUTPUT;

                  //check if the red light pass was the result of a right turn
                  if (turnType == IntersectionDir.RIGHT_TURN) {
                     if (checkRedLightYieldViolation(trackData[i].trackID, trackData[i].entryGate, trackData[i].exitGate, startTime, endTime)) {
                        //count that vehicles going straight from the left were forced to slow down
                        redLightPassesWithSlowdown += 1;
                     } //END IF

                     if (withinStopline(trackData[i].trackID, trackData[i].entryGate, startTime, 6, 7, false, light)) {
                        //note that the red light encounter was the reuslt of a right turn which is not a violation
                        ARGUMENTS.caseOutput ? printf("  -> Vehicle: %i correctly turned right on a red light\n", trackData[i].trackID) : NO_OUTPUT;

                        //count that another red light was encountered
                        redLightPass++;
                     }
                     else {
                        //note that the red light encounter was the result of a right turn without a stop occuring first, which is a violation
                        ARGUMENTS.caseOutput ? printf("  -> Vehicle: %i correctly turned right on a red light\n", trackData[i].trackID) : NO_OUTPUT;

                        //count that another red light was encountered
                        redLightPassNoStop++;
                     }
                  }
                  else {
                     //note that the red light encounter was the reuslt of a right turn which is not a violation
                     ARGUMENTS.caseOutput ? printf("  -> Vehicle: %i correctly turned right on a red light\n", trackData[i].trackID) : NO_OUTPUT;

                     //count that another red light was encountered
                     redLights++;
                  } //END IF
               }
               //check if the light is yellow at the given moment
               else if (state == LightStates.Yellow) {
                  //print data to terminal if applicable
                  ARGUMENTS.caseOutput ? printf("Vehicle: %i runs a yellow light\n", trackData[i].trackID) : NO_OUTPUT;

                  //checks if the vehicle was moving too fast to stop at the yellow light
                  if (checkStopDistance(trackData[i].trackID, trackData[i].entryGate, lightData[k].time)) {
                     //print data to terminal if applicable
                     ARGUMENTS.caseOutput ? printf("  -> Vehicle should have stopped given its speed and its distance from the gate.\n") : NO_OUTPUT;

                     //counts that the vehicle could not have passed a yellow light correctly
                     yellowFail++;
                  }
                  else {
                     //print data to terminal if applicable
                     ARGUMENTS.caseOutput ? printf("  -> Vehicle could not have stopped at the light.\n") : NO_OUTPUT;

                     //counts that the vehicle has passed a yellow light correctly
                     yellowPass++;
                  } //END IF
               }
               //checks if the light is green at the given moment
               else if (state == LightStates.Green) {
                  //check if a vehicle has stopped before a red or yellow light before continuing
                  withinStopline(trackData[i].trackID, trackData[i].entryGate, startTime, 1000, 2, true, light);

                  //determine if there was a left turn
                  if (turnType != IntersectionDir.LEFT_TURN) {
                     //print data to terminal if applicable
                     ARGUMENTS.caseOutput ? printf("Vehicle: %i runs a green light\n", trackData[i].trackID) : NO_OUTPUT;

                     //counts that the vehicle has passed a green light correctly
                     green++;
                  }
                  else {
                     //print data to terminal if applicable
                     ARGUMENTS.caseOutput ? printf("  %s -> Permissive left possible\n", turnType.c_str()) : NO_OUTPUT;

                     //check if a permissive left turn has occured
                     permissiveLeftNeeded = checkPermissiveLeftTurn(trackData[i].trackID, trackData[i].entryGate,
                                                                    trackData[i].exitGate, startTime, endTime);
                                                                    //END checkPermissiveLeftTurn

                     //checks if the permissive left turn is applicable
                     if (permissiveLeftNeeded != -1) {
                        //print data to terminal if applicable
                        ARGUMENTS.caseOutput ? printf("     -> Result: %s\n", (permissiveLeftNeeded == 1) ?
                                                      "Permissive Left Can Occur at Intersection During Given Time Window" :
                                                      "Permissive Left Cannot Occur Since it Must Slow Another Vehicle Down") :
                                                      NO_OUTPUT;
                                                      //END printf
                     } //END IF

                     //count that a left turn occured during a green light
                     (permissiveLeftNeeded == 0) ? leftEncounter++ : NO_OUTPUT;

                     //count that a permissive left turn occured
                     (permissiveLeftNeeded == 1) ? permissiveLeft++ : NO_OUTPUT;

                     //count that a regular green light turn occured
                     (permissiveLeftNeeded == -1) ? green++ : NO_OUTPUT;
                  } //END IF
               }
               else { //print an error message
                  //print data to terminal if applicable
                  ARGUMENTS.caseOutput ? printf("Vehicle: %i runs an unkown light\n", trackData[i].trackID) : NO_OUTPUT;
               } //END IF
            } //END IF
         } //END FOR
      } //END IF
   } //END FOR

   //return that the function has executed successfully
   return 1;
} //END getTrafficValues

/*
Input:
   const char * sqlGetTrajectories - Contains a selection command for the given database organized by TRACK_ID and TIME.
   const char * databasePath - Contains a path to the database file.
Output:
   Data is printed to the terminal using printf functions.
Method:
   Clears data structs and executes SQLite3 calls to collect data.
Description:
   Initializer for reading and outputting data from the databases.
*/
void getData(const char * sqlGetTrajectories, const char * databasePath) {
   /* Create SQL statement */
   const char * sqlGetTrackData = "SELECT * from TRACKS;";
   const char * sqlGetStopLines = "SELECT * from TRAFFIC_LIGHTS ORDER BY TIME DESC;";
   const char * getLaneGeometry = "SELECT * from TRAFFIC_REGIONS_DEF WHERE REGION_PROPERTY = 'gate_line';";

   /* Clear Vector Data */
   lightData.clear();
   trackData.clear();
   stopLines.clear();
   trajectoryInformation.clear();

   /* Reset Traffic Statistics */
   redLights = 0;
   redLightPass = 0;
   redLightPassNoStop = 0;
   redLightPassesWithSlowdown = 0;
   permissiveLeft = 0;
   leftEncounter = 0;
   yellowFail = 0;
   yellowPass = 0;
   green = 0;
   redLightStops = 0;
   yellowLightStops = 0;

   /* Open Database Data */
   sqlCall(sqlGetTrackData, databasePath, Callback::Tracks);
   sqlCall(sqlGetStopLines, databasePath, Callback::Lights);
   sqlCall(getLaneGeometry, databasePath, Callback::Regions);
   sqlCall(sqlGetTrajectories, databasePath, Callback::Trajectory);

   /* Output Results */
   getTrafficValues();

   /* Contains the Rule Violation Results Accross all Datasets */
   totalRedLights += redLights;
   totalRedLightPasses += redLightPass;
   totalRedLightPassesNoStop += redLightPassNoStop;
   totalRedLightPassesWithSlowdown += redLightPassesWithSlowdown;
   totalPermissiveLeft += permissiveLeft;
   totalLeftEncounter += leftEncounter;
   totalYellowFail += yellowFail;
   totalYellowPass += yellowPass;
   totalGreen += green;
   totalRedLightStops += redLightStops;
   totalYellowLightStops += yellowLightStops;

   /* Output Statistics */
   ARGUMENTS.statisticsOutput ? printf("STATISTICS FOR: %s\n",
                                       databasePath) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf(" -> Number of vehicles to pass green light: %d\n",
                                       green + leftEncounter + permissiveLeft) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  ->  Number of vehicles to pass green light via permissive left turn: %d\n",
                                       permissiveLeft) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  ->  Number of vehicles to pass green light illegally by left turn: %d\n",
                                       leftEncounter) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf(" -> Number of vehicles to stop at a red light: %d\n",
                                       redLightStops) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf(" -> Number of vehicles to encounter a red light while crossing the intersection: %d\n",
                                       redLights + redLightPass + redLightPassNoStop) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  -> Number of vehicles to violate red light: %d\n",
                                       redLights) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  -> Number of vehicles to pass red light via right turns: %d\n",
                                       redLightPass) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  -> Number of vehicles causing delays of vehicles going straight from the left: %d\n",
                                       redLightPassesWithSlowdown) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  -> Number of vehicles to pass red light via right turns without stopping: %d\n",
                                       redLightPassNoStop) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf(" ->  Number of vehicles to stop at a yellow light: %d\n",
                                       yellowLightStops) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf(" -> Number of vehicles to pass yellow light: %d\n",
                                       yellowFail + yellowPass) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  ->  Number of vehicles to pass yellow light illegally: %d\n",
                                       yellowFail) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  ->  Number of vehicles to pass yellow light since vehicle was moving too fast: %d\n",
                                       yellowPass) : NO_OUTPUT;
                                       //END printf
} //END getData

/*
Input:
   int argc - contains the number of command line arguments.
   char * argv[] - contains a list of all command line arguments.
Output:
   Loads command line arguments into ARGUMENTS structure.
   Runs program on all intersection databases listed below.
Method:
   The following deliverable relys on SQLite3 which can be installed using the following command:
      sudo apt install sqlite3
   Reads SQLite3 databases representing the signalized intersection database files made by Waterloos Wise Lab.
      Databases can be found at:
         https://uwaterloo.ca/waterloo-intelligent-systems-engineering-lab/datasets/waterloo-multi-agent-traffic-dataset-intersection
   Parses command line input into the ARGUMENTS array and loads up all database files.
Compilation:
   The code can be compiled using the SQLite3 package with the following command:
      g++ DatabaseExtract.cpp -l sqlite3
Description:
   Entry function for the signalized intersection interpreter.
*/
int main(int argc, char* argv[]) {
   /* Get Coefficients from User Input if Given */
   for (int i = 0; i < argc; i++) {
      if (std::string(argv[i]) == ARGUMENTS.vehicleBreakingAccToggle) {
         ARGUMENTS.vehicleBreakingAcc = std::stod(argv[i + 1]);
      }
      else if (std::string(argv[i]) == ARGUMENTS.caseOutputToggle) {
         ARGUMENTS.caseOutput = false;
      }
      else if (std::string(argv[i]) == ARGUMENTS.statisticsOutputToggle) {
         ARGUMENTS.statisticsOutput = false;
      } //END IF
   } //END FOR

   //uses the altnerative config system for traffic light database formatting
   config = 2;

   /*Open all Related Databases and Extract Data*/
   const char * sqlGetTrajectories = "SELECT * from TRAJECTORIES_0769 ORDER BY TRACK_ID, TIME;";
   const char * databasePath = "./db/intsc_data_769.db";
   getData(sqlGetTrajectories, databasePath);
   
   const char * sqlGetTrajectoriesTwo = "SELECT * from TRAJECTORIES_0770 ORDER BY TRACK_ID, TIME;";
   char databasePathTwo[] = "./db/intsc_data_770.db";
   getData(sqlGetTrajectoriesTwo, databasePathTwo);
   
   const char * sqlGetTrajectoriesThree = "SELECT * from TRAJECTORIES_0771 ORDER BY TRACK_ID, TIME;";
   char databasePathThree[] = "./db/intsc_data_771.db";
   getData(sqlGetTrajectoriesThree, databasePathThree);

   const char * sqlGetTrajectoriesFour = "SELECT * from TRAJECTORIES_0775 ORDER BY TRACK_ID, TIME;";
   char databasePathFour[] = "./db/intsc_data_775.db";
   getData(sqlGetTrajectoriesFour, databasePathFour);

   const char * sqlGetTrajectoriesFive = "SELECT * from TRAJECTORIES_0776 ORDER BY TRACK_ID, TIME;";
   char databasePathFive[] = "./db/intsc_data_776.db";
   getData(sqlGetTrajectoriesFive, databasePathFive);

   const char * sqlGetTrajectoriesSix = "SELECT * from TRAJECTORIES_0777 ORDER BY TRACK_ID, TIME;";
   char databasePathSix[] = "./db/intsc_data_777.db";
   getData(sqlGetTrajectoriesSix, databasePathSix);

   //uses the standard config system for traffic light database formatting
   config = 1;

   const char * sqlGetTrajectoriesSeven = "SELECT * from TRAJECTORIES_0778 ORDER BY TRACK_ID, TIME;";
   char databasePathSeven[] = "./db/intsc_data_778.db";
   getData(sqlGetTrajectoriesSeven, databasePathSeven);

   const char * sqlGetTrajectoriesEight = "SELECT * from TRAJECTORIES_0779 ORDER BY TRACK_ID, TIME;";
   char databasePathEight[] = "./db/intsc_data_779.db";
   getData(sqlGetTrajectoriesEight, databasePathEight);

   const char * sqlGetTrajectoriesNine = "SELECT * from TRAJECTORIES_0780 ORDER BY TRACK_ID, TIME;";
   char databasePathNine[] = "./db/intsc_data_780.db";
   getData(sqlGetTrajectoriesNine, databasePathNine);

   const char * sqlGetTrajectoriesTen = "SELECT * from TRAJECTORIES_0781 ORDER BY TRACK_ID, TIME;";
   char databasePathTen[] = "./db/intsc_data_781.db";
   getData(sqlGetTrajectoriesTen, databasePathTen);

   const char * sqlGetTrajectoriesEleven = "SELECT * from TRAJECTORIES_0782 ORDER BY TRACK_ID, TIME;";
   char databasePathEleven[] = "./db/intsc_data_782.db";
   getData(sqlGetTrajectoriesEleven, databasePathEleven);

   const char * sqlGetTrajectoriesTwelve = "SELECT * from TRAJECTORIES_0783 ORDER BY TRACK_ID, TIME;";
   char databasePathTwelve[] = "./db/intsc_data_783.db";
   getData(sqlGetTrajectoriesTwelve, databasePathTwelve);

   const char * sqlGetTrajectoriesThirteen = "SELECT * from TRAJECTORIES_0784 ORDER BY TRACK_ID, TIME;";
   char databasePathThirteen[] = "./db/intsc_data_784.db";
   getData(sqlGetTrajectoriesThirteen, databasePathThirteen);

   /* Output Statistics */
   ARGUMENTS.statisticsOutput ? printf("STATISTICS FOR THE WHOLE SAMPLE:\n") : NO_OUTPUT;
   ARGUMENTS.statisticsOutput ? printf(" -> Number of vehicles to pass green light: %d\n",
                                       totalGreen + totalLeftEncounter + totalPermissiveLeft) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  ->  Number of vehicles to pass green light via permissive left turn: %d\n",
                                       totalPermissiveLeft) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  ->  Number of vehicles to pass green light illegally by left turn: %d\n",
                                       totalLeftEncounter) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf(" ->  Number of vehicles to stop at a red light: %d\n",
                                       totalRedLightStops) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf(" -> Number of vehicles to encounter red lights: %d\n",
                                       totalRedLights + totalRedLightPasses + totalRedLightPassesNoStop) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  -> Number of vehicles to violate red lights: %d\n",
                                       totalRedLights) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  -> Number of vehicles to pass red lights: %d\n",
                                       totalRedLightPasses) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  -> Number of vehicles to pass red lights without stopping: %d\n",
                                       totalRedLightPassesNoStop) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  -> Number of vehicles causing delays of vehicles going straight from the left: %d\n",
                                       totalRedLightPassesWithSlowdown) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf(" ->  Number of vehicles to stop at a yellow light: %d\n",
                                       totalYellowLightStops) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf(" -> Number of vehicles to pass yellow light: %d\n",
                                       totalYellowFail + totalYellowPass) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  ->  Number of vehicles to pass yellow light illegally: %d\n",
                                       totalYellowFail) : NO_OUTPUT;
                                       //END printf
   ARGUMENTS.statisticsOutput ? printf("  ->  Number of vehicles to pass yellow light legally as the vehicle was moving too fast: %d\n",
                                       totalYellowPass) : NO_OUTPUT;
                                       //END printf
} //END main
