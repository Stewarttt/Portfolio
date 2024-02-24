//import c and c++ libraries
#include <string>
#include <vector>
#include <fstream>
#include <cstdio>

//include standard libraries
#include <bits/stdc++.h>

//include traffic rules system for lanelet2
#include "lanelet2_traffic_rules/TrafficRulesFactory.h"
#include "lanelet2_traffic_rules/Exceptions.h"

//include traffic rule system for Canadian rules
#include "lanelet2_traffic_rules_canada/ExtendedTrafficRules.h"

//include lanelet2 core elements for lanelets and geometry
#include <lanelet2_core/geometry/Area.h>
#include <lanelet2_core/geometry/Lanelet.h>
#include <lanelet2_core/primitives/RegulatoryElement.h>
#include <lanelet2_core/utility/Units.h>
#include <lanelet2_core/LaneletMap.h>
#include <lanelet2_core/primitives/BasicRegulatoryElements.h>
#include <lanelet2_core/primitives/Lanelet.h>
#include <lanelet2_core/primitives/Area.h>
#include <lanelet2_core/primitives/LineString.h>
#include <lanelet2_core/primitives/Point.h>
#include <lanelet2_core/primitives/Polygon.h>

//include lanelet2 map elements
#include <lanelet2_projection/UTM.h>
#include <lanelet2_projection/LocalCartesian.h>

//include lanelet2 input-output elements
#include <lanelet2_io/Io.h>

//include lanelet2 routing system
#include "lanelet2_routing/Route.h"
#include "lanelet2_routing/RoutingGraph.h"
#include "lanelet2_routing/RoutingCost.h"
#include <lanelet2_routing/RoutingGraphContainer.h>

//include matching system for lanelet2
#include <lanelet2_matching/LaneletMatching.h>

//allow assert statements to work in release mode
#undef NDEBUG

#pragma GCC diagnostic ignored "-Wunused-but-set-variable"
#pragma GCC diagnostic ignored "-Wunused-variable"

//compare floats a couple of times and know this is save in this context
#pragma GCC diagnostic ignored "-Wfloat-equal"

//define runtime constants
float PI = 3.14159;

namespace lanelet {
namespace traffic_rules {

/*
Input:
  const lanelet::ConstLanelet & lanelet - Contains the address of the lanelet being checked.
Output:
  bool canPass - Determines if a vehicle can pass lanes.
Method:
  Calls the predefined method from the GenericTrafficRules class associated with the Lanelet2 package.
Description:
  Determines if a vehicle on the given lanelet can pass lanes.
*/
bool ExtendedTrafficRules::canPass(const lanelet::ConstLanelet & lanelet) const {
  return GenericTrafficRules::canPass(lanelet);
} //END canPass

/*
Input:
  const struct ruleViolationStatistics data - struct containing data for output. Contains the following elements:
    const int data.vehicles - Contains the number of vehicles iterated over.
    const int data.nonViolators - Contains the number of vehicles that violated any rule.
    const int data.shouldStop - Contains the number of times stop signs were violated.
    const int data.leftBeforeRight - Contains the number of times left before right stop priority was violated.
    const int data.hasPriority - Contains the number of times lanelet stop priority was violated.
    const int data.turningLeft - Contains the number of times turning left stop priority was violated.
    const int data.tailgating - Contains the number of times tailgating priority was violated.
    const std::string data.PATH - Contains the directory path and filename to the output CSV file.
    const std::string data.title - Contains headers for the output columns in the CSV.
Output:
  std::ofstream - Output consists of output printed into a CSV file at a specified directory path.
Methods:
  Prints data using std C++ I/O functions.
Description:
  Formats and prints output for the rule violation tests preformed by the program.
*/
void ExtendedTrafficRules::outputToResultsLog(const struct ruleViolationStatistics data) const {
  //stores file pointer
  std::ofstream outputPointer;

  //open a file with the specified path
  outputPointer.open(data.PATH, std::ios_base::app);

  //write the header to the file
  outputPointer << data.title << std::endl;

  //write to the file and append a newline character
  outputPointer << "," << data.vehicles << "," << data.nonViolators << "," << data.shouldStop << "," << data.stopsNeeded << ","
                << data.leftBeforeRight << "," << data.hasPriority << "," << data.turningLeft << "," << data.yieldsOneNeeded << ","
                << data.yieldsTwoNeeded << "," << data.tailgating << "," << data.speedLimit << "," << data.noCollisions << std::endl;
                //END outputPointer

  //close the file
  outputPointer.close();
} //END outputToResultsLog

/*
Input:
  const std::vector<violation> data - Contains the off-road rule violations.
  const double length - Gives the length of the corresponding vehicle.
  const double width - Gives the width of the corresponding vehicle.
  const BoundaryValues ratio - Gives the ratio from the centerpoint to the edges of the corresponding vehicle.
  const std::string mapPath - Gives the path to the OSM the vehicle was within.
  const lanelet::BasicPoint2d originPoint - Gives the origin point for the map.
  const char * vehicleLocation - Gives the two character representation for the nation the vehicle was driving within.
Output:
  std::string - Represents the output string for the given no off-road rule violation vector in a CSV format.
Description:
  Used to format CSV output for no off-road violations for any given vehicle.
*/
std::string ExtendedTrafficRules::noOffroadString(const std::vector<violation> data, const double length, const double width,
                                                  const BoundaryValues ratio, const std::string mapPath,
                                                  const lanelet::BasicPoint2d originPoint, const char * vehicleLocation) const {
                                                  //END noOffroadString
  //container for the output string
  std::string outputString = "";

  //iterate through the vehicle violations
  for (size_t i = 0; i < data.size(); i++) {
    outputString += std::to_string(data[i].referenceID) + "," + std::to_string(*data[i].xPos) + "," + std::to_string(*data[i].yPos) + ",";
    outputString += std::to_string(*data[i].timestamp) + "," + std::to_string(length) + "," + std::to_string(width) + ",";
    outputString += std::to_string(*data[i].yaw) + "," + std::to_string(ratio) + ",";
    outputString += std::to_string((int) intersectionSegment(*data[i].xPos, *data[i].yPos, mapPath, originPoint, vehicleLocation)) + '\n';
  } //END FOR

  //close the file
  return outputString;
} //END noOffroadString

/*
Input:
  const std::string PATH - Determines the path for the given output file.
  const std::vector<violation> data - Determines the data associated with rule violations for the given vehicle.
  const double length - Determines the length of the given vehicle.
  const double width - Determines the width of the given vehicle.
Output:
  Stores off-road rule violations for a given vehicle to the specified file given a vector of raw violation objects.
Description:
  Appends data to the specified output file.
*/
void ExtendedTrafficRules::outputNoOffroad(const std::string PATH, const std::vector<violation> data,
                                           const double length, const double width) const {
                                           //END outputNoOffroad
  //stores file pointer
  std::ofstream outputPointer;

  //open a file with the specified path
  outputPointer.open(PATH.c_str(), std::ios_base::app);

  //iterate through the vehicle violations
  for (size_t i = 0; i < data.size(); i++) {
    //write to the file and append a newline character
    outputPointer << data[i].referenceID << "," << *data[i].xPos << "," << *data[i].yPos << "," << *data[i].timestamp << ",";
    outputPointer << length << "," << width << "," << *data[i].yaw << std::endl;
  } //END FOR

  //close the file
  outputPointer.close();
} //END outputNoOffroad

/*
Input:
  const std::string path - Path to the specified file to be cleared.
  const char header[] - Column headers data for the specified CSV information.
Output:
  std::ofstream - Output consists of a blank file with the specified path and column header information for the first row.
Methods:
  Clears data using std C++ I/O functions.
Description:
  Clears output in the CSV file containing rule violations.
*/
void ExtendedTrafficRules::clearVehicleStats(const std::string path, const char header[]) const {
  //delete the output file
  remove(path.c_str());

  //stores file pointer
  std::ofstream outputPointer;

  //open a file with the specified path
  outputPointer.open(path, std::ios_base::app);

  //write the header to the file
  outputPointer << header << std::endl;

  //close the file
  outputPointer.close();
} //END clearOutput

/*
Input:
  const std::string path - Path to the specified file to be cleared.
  const char header[] - Column headers data for the specified CSV information.
Output:
  std::ofstream - Output consists of a blank file with the specified path and column header information for the first row.
Methods:
  Clears data using std C++ I/O functions.
Description:
  Clears output in the CSV file containing rule violations.
*/
void ExtendedTrafficRules::clearOutput(const std::string path, const char header[]) const {
  //delete the output file
  remove(path.c_str());

  //stores file pointer
  std::ofstream outputPointer;

  //open a file with the specified path
  outputPointer.open(path, std::ios_base::app);

  //write the header to the file
  outputPointer << header << std::endl;

  //close the file
  outputPointer.close();
} //END clearOutput

/*
Input:
  const std::string data - Output to be printed into a file.
Output:
  std::ofstream - Output is placed text file.
Methods:
  Prints debug data using std C++ I/O functions.
Description:
  Used for printing data for debugging purposes.
*/
void ExtendedTrafficRules::outputToFile(const std::string data) const {
  //stores the path to the output file
  char PATH[] = "./Output/consoleOutput.txt";

  //stores file pointer
  std::ofstream outputPointer;

  //open a file with the specified path and append data
  outputPointer.open(PATH, std::ios_base::app);

  //write to the file and append a newline character
  outputPointer << data << std::endl;

  //close the file
  outputPointer.close();
} //END outputToFile

/*
Input:
  std::vector<double> vector - Contains the vector data to be sorted.
Output:
  std::vector<double> sortVector - Contains a vector with the input data sorted.
Methods:
  Uses the built in sort function.
Description:
  Sorts the elements within the vector and returns a vector with the sorted data.
*/
std::vector<double> ExtendedTrafficRules::sortVector(std::vector<double> vector) const {
  //sort the vector given the starting and ending positions
  sort(vector.begin(), vector.end());

  //return the sorted vector
  return vector;
} //END sortVector

/*
Input:
  std::vector<double> vector - Contains a sorted vector to be inspected.
Output:
  double findRange - Contains the range between the largest and smallest element.
Methods:
  Takes the largest element and subtracts by the smallest
Description:
  Finds the range between the largest and smallest value in the vector assuming the vector is sorted.
*/
double ExtendedTrafficRules::findRange(std::vector<double> vector) const {
  //subtract the largest from the smallest value
  return vector.back() - vector.front();
} //END findRange

/*
Input:
  std::vector<double> vector - Contains a sorted vector to be inspected.
Output:
  double findMax - Contains the largest element in the list.
Methods:
  Takes the last element from the vector.
Description:
  Gets the largest value in the vector, given the vector is sorted.
*/
double ExtendedTrafficRules::findMax(std::vector<double> vector) const {
  //returns the largest value
  return vector.back();
} //END findMax

/*
Input:
  std::vector<double> vector - Contains a sorted vector to be inspected.
Output:
  double findMin - Contains the smallest element in the list.
Methods:
  Takes the first element from the vector.
Description:
  Gets the smallest value in the vector, given the vector is sorted.
*/
double ExtendedTrafficRules::findMin(std::vector<double> vector) const {
  //returns the smallest value
  return vector.front();
} //END findMin

/*
Input:
  std::vector<double> vector - Contains a sorted vector to be inspected.
Output:
  double findMedian - Contains the median of the list.
Methods:
  Takes the middle element from the vector if the vector has an odd number of elements.
  Takes the average between the two middle numbers in the vector if the vector has an even number of elements.
  Assumes the vector is sorted.
Description:
  Gets the median value in the vector, given the vector is sorted.
*/
double ExtendedTrafficRules::findMedian(std::vector<double> vector) const {
  //if there are an odd number of elements
  if (vector.size() % 2 == 1) {
    //return the middle element from the vector
    return vector[(vector.size()-1) / 2 + 1];
  } //END IF

  //if there are an even number of elements return the average of the two middle elements
  return (vector[vector.size() / 2] + vector[vector.size() / 2 + 1]) / 2;
} //END findMedian

/*
Input:
  std::vector<double> vector - Contains a vector to be inspected.
Output:
  double findMedian - Contains the mean of the list.
Methods:
  Takes the sum of all elements in the vector and divides the sum by the number of elements in the vector.
Description:
  Gets the average value of the vector.
*/
double ExtendedTrafficRules::findMean(std::vector<double> vector) const {
  //contains the sum of all values in the vector
  double totalSum = 0;

  //gets the sum of the vector elements
  for (size_t i = 0; i < vector.size(); i++) {
    //add the sum to the summation container
    totalSum += vector[i];
  } //END FOR

  //return the total sum divided by the size of the vector
  return totalSum / vector.size();
} //END findMean

/*
Input:
  std::vector<double> vector - Contains a sorted vector to be inspected.
Output:
  double findMode - Contains the mode of the vector.
Methods:
  Counts the list to find the most common element and returns it to the user.
  Lists smaller then three elements are excluded.
Description:
  Gets the mode value in a vector with all values rounded to the nearest digit, given a sorted vector
*/
double ExtendedTrafficRules::findMode(std::vector<double> vector) const {
  //check if a mode is appropriate given the size of the vector
  if (vector.size() < 3) {
    //the mode means nothing, just return the first value
    return vector[0];
  } //END IF

  //gets the maximum and minimum values in the vector
  int max = findMax(vector);
  int min = findMin(vector);

  //gets the previously iterated value
  int previousValue = max;

  //stores the mode value
  int mode;

  //stores the number of occurrences for the most frequently encountered integer
  int maxcount = 0;

  //stores the number of occurrences some currently iterated integer
  int currcount = 0;

  //determines if the dataset contains duplicates
  bool duplicates = false;

  //for every element in the vector
  for (size_t i = 0; i < vector.size(); i++) {
    //checks if the currently iterated value falls within the range for the previous value
    if (std::round(vector[i]) == previousValue) {
      //count the current occurrence of the value
      currcount += 1;

      //checks if the current count is larger then the previously recorded maximum count
      if (currcount > maxcount) {
        //duplicate integers exist
        duplicates = true;

        //gets the new most commonly encountered integer
        maxcount = currcount;

        //gets the new most commonly encountered integer
        mode = std::round(vector[i]);
      } //END IF
    }
    else {
      //if a new integer is encountered stores the number of times it has occurred
      currcount = 1;
    } //END IF

    //contains the value of the previously encountered integer
    previousValue = std::round(vector[i]);
  } //END FOR

  //given a dataset without duplicate integers
  if (!duplicates) {
    //get the mode as the first value in the dataset
    return vector[0];
  } //END IF

  //returns the mode for the dataset given duplicates
  return mode;
} //END findMode

/*
Input:
  std::vector<double> vector - Contains a vector to be inspected.
Output:
  double findMode - Contains the mode of the vector.
Methods:
  Counts the list to find the most common element and returns it to the user.
  Lists smaller then three elements are excluded.
Description:
  Gets the (sample) standard deviation given a vector and the mean of the elements within the vector
*/
double ExtendedTrafficRules::findStandardDeviation(std::vector<double> vector) const {
  //contains the sum of the vector
  double sum = 0;

  //contains the size of the vector
  const int size = vector.size();

  //contains the mean of the vector
  const double mean = findMean(vector);

  //iterate through the vector
  for (size_t i = 0; i < vector.size(); i++) {
    //obtains the sum of all elements in the vector minus the mean squared
    sum += (vector[i] - mean)*(vector[i] - mean);
  } //END FOR

  //divided the summation of the vector by the number of elements in the vector minus one to get the sample standard deviation squared
  sum /= (size - 1);

  //return the sample standard deviation
  return sqrt(sum);
} //END findStandardDeviation

/*
Input:
  const double orientation - Contains a yaw angle in radians.
Output:
  double getRadians - Contains the yaw angle in terms of circular radians.
Methods:
  Converts from a yaw measurement system to a circular radian system.
Description:
  Convert yaw radians into circular radians.
*/
double ExtendedTrafficRules::getRadians(const double orientation) const {
  //if the yaw is less then zero
  if (orientation < 0) {
    //convert to circular radians by finding the magnitude past one-PI and adding it to PI
    return PI + (PI + orientation);
  } //END IF

  //the yaw orientation is the same as the orientation in circular radians
  return orientation;
} //END getRadians

/*
Input:
  const double radians - Contains a circular angle in radians.
Output:
  double getRadians - Contains the yaw angle in terms of yaw radians.
Methods:
  Converts from a circular radian measurement system to a yaw radian system.
Description:
  Convert circular radians into yaw radians.
*/
double ExtendedTrafficRules::getOrientation(const double radians) const {
  //if the yaw is less then zero
  if (radians > PI) {
    //convert to circular radians by finding the magnitude past one-PI and adding it to PI
    return (radians - PI) - PI;
  } //END IF

  //the yaw orientation is the same as the orientation in circular radians
  return radians;
} //END getRadians

/*
Input:
  const lanelet::BasicPoint2d referenceRear - Coordinates for the reference vehicles front point.
  const lanelet::BasicPoint2d otherFront - Coordinates for the other vehicles rear point.
  const double referenceOrientation - Reference vehicles yaw orientation.
  const double otherOrientation - Other vehicles yaw orientation.
  const double distThreshold - Distance threshold for the two vehicles.
Output:
  bool closeProximity - Signals if one vehicle is tailgating another vehicle due to its close proximity along a path.
Methods:
  Compares the distance between two points to find vehicle distance:
    Assumes one point is the back of one vehicle and the other is the front of another.
  Checks the yaw of both vehicles to determine if the vehicles are facing each other down a lanelet.
Description:
  Checks for vehicle tailgating by proximity between the back of one vehicle and the front of another.
*/
bool ExtendedTrafficRules::closeProximity(const lanelet::BasicPoint2d referenceRear, const lanelet::BasicPoint2d otherFront,
                                          const double referenceOrientation, const double otherOrientation, const double distThreshold) const {
                                          //END closeProximity
  //contains coordinate values
  double xDif, yDif, distance;

  //contains reference value in terms of radians
  double radReferenceOrientation, radOtherOrientation;

  //determine the x and y value difference between the two points
  xDif = otherFront.x() - referenceRear.x();
  yDif = otherFront.y() - referenceRear.y();

  //determine the distance between the two points in two-dimensions
  distance = sqrt(xDif*xDif + yDif*yDif);

  //check if the distance is less then the min-threshold for a close proximity vehicle
  if (distance < distThreshold) {
    //checks if the vehicles are moving in the same direction
    if (abs(referenceOrientation - otherOrientation) < PI/6) {
      return true; //the vehicles are in close proximity
    }
    //if the orientation is negative convert to radians
    else if (referenceOrientation <= 0 && otherOrientation >= 0) {
      //convert to radians
      radReferenceOrientation = getRadians(referenceOrientation);
      //checks if the vehicles are moving in the same direction
      if (abs(radReferenceOrientation - otherOrientation) < PI/6) {
        return true; //the vehicles are in close proximity
      } //END IF
    }
    //if the orientation is negative convert to radians
    else if (otherOrientation <= 0 && referenceOrientation >= 0) {
      //convert to radians
      radOtherOrientation = getRadians(otherOrientation);
      //checks if the vehicles are moving in the same direction
      if (abs(referenceOrientation - radOtherOrientation) < PI/6) {
        return true; //the vehicles are in close proximity
      } //END IF
    }
    //if the orientation is negative convert to radians
    else if (referenceOrientation <= 0 && otherOrientation <= 0)  {
      //convert to radians
      radReferenceOrientation = getRadians(referenceOrientation);
      radOtherOrientation = getRadians(otherOrientation);
      //checks if the vehicles are moving in the same direction
      if (abs(radReferenceOrientation - radOtherOrientation) < PI/6) {
        return true; //the vehicles are in close proximity
      } //END IF
    } //END IF
  } //END IF

  //if the vehicles are not moving in the same direction
  return false;
} //END closeProximity

/*
Input:
  const double responseTime - Represents the amount of time it takes the vehicle to respond to traffic.
  const double rearVelocity - Rear vehicles velocity.
  const double frontVelocity - Front vehicles velocity.
  const double maxBreakAccFront - The maximum breaking acceleration for the front vehicle.
  const double maxAccelAccRear - The maximum breaking acceleration for the rear vehicle.
  const double minBreakAccRear - The minimum breaking acceleration for the front vehicle.
Output:
  double findMinDist - Finds the minimum distance between two vehicles for tailgating not to occur.
Methods:
  We calculate d_min using Intel's Safe Longitudinal Distance - Same Direction distance formulation: https://arxiv.org/pdf/1708.06374.pdf
Formula:
  We use the following formula provided by the formulation under Lemma 2:
    d_min = max{[(v_r)(p) + (0.5)(a_max,accel)(p^2) + ((v_r + pa_max,accel)^2/(2a_min,brake)) - (v_f^2)/(2a_max,brake)], 0}
Descriptiosize_t  Gets the minimum distance between two vehicles for tailgating detection.
*/
double ExtendedTrafficRules::findMinDist(const double responseTime, const double rearVelocity, const double frontVelocity,
                                         const double maxBreakAccFront, const double maxAccelAccRear, const double minBreakAccRear) const {
                                         //END findMinDist
  //determine the minimum distance between the vehicles
  return std::max((rearVelocity * responseTime) + (0.5 * maxBreakAccFront * responseTime * responseTime) +
                  ((rearVelocity + responseTime*maxAccelAccRear) / (2*minBreakAccRear)) -
                  ((frontVelocity * frontVelocity) / (2 * maxBreakAccFront)), 0.0);
                  //END std::max
} //END findMinDist

/*
Input:
  const char PATH[] - Represents the path to the given output file.
Output:
  Output is stored in a file with the specified path as input.
Description:
  Used to store a pre-defined header to a specified file path after clearing its contents.
*/
void ExtendedTrafficRules::resetTailgatingResults(const char PATH[]) const {
  //delete the file
  remove(PATH);

  //open the file pointer using the given file path
  std::ofstream file;
  file.open(PATH);

  //write column headers to the file
  file << "Response Time,Maximum Front Breaking Acceleration,Maximum Rear Vehicle Acceleration,";
  file << "Minimum Rear Breaking Acceleration,Coordinates,Orientation,";
  file << "Vehicle/Victim ID,Timestamp,Coordinates,Orientation,Stopped" << std::endl;

  //close the current file
  file.close();
} //END resetTailgatingResults

/*
Input:
  const char PATH[] - Represents the path to the given output file.
Output:
  Output is stored in a file with the specified path as input.
Description:
  Used to store a pre-defined header to a specified file path after clearing its contents.
*/
void ExtendedTrafficRules::resetTailgatingOccurances(const char PATH[]) const {
  //delete the file
  remove(PATH);

  //open the file pointer using the given file path
  std::ofstream file;
  file.open(PATH);

  //write column headers to the file
  file << "FILE,Default,HighResponse,HighBreak,HighAccel,HighMinAccel,LowResponse,LowBreak,LowAccel,LowMinAccel" << std::endl;

  //close the current file
  file.close();
} //END resetTailgatingResults

/*
Input:
  const char PATH[] - Represents the path to the given output file.
Output:
  Output is stored in a file with the specified path as input.
Description:
  Used to store a pre-defined header to a specified file path after clearing its contents.
*/
void ExtendedTrafficRules::resetTailgatingResultsLineByLine(const char PATH[]) const {
  //delete the file
  remove(PATH);

  //open the file pointer using the given file path
  std::ofstream file;
  file.open(PATH);

  //write column headers to the file
  file << "Offending Vehicle ID,Victim Vehicle ID,X-Position,Y-Position,";
  file << "timestamp,Offending Yaw-Angle,Victim Yaw-Angle,Vehicle Stopped" << std::endl;

  //close the current file
  file.close();
} //END resetTailgatingResults

/*
Input:
  const std::string PATH - Represents the path to the given output file.
  const char HEADER[] - Represents the header for the given output file.
Output:
  Output is stored in a file with the specified path as input.
Description:
  Used to store a pre-defined header to a specified file path after clearing its contents.
*/
void ExtendedTrafficRules::resetOutputDataToExcel(const std::string PATH, const char HEADER[]) const {
  //delete the file
  remove(PATH.c_str());

  //open the file pointer using the given file path.c_str()
  std::ofstream file;
  file.open(PATH.c_str(), std::fstream::out);

  //write column headers to the file
  file << HEADER << std::endl;

  //close the current file
  file.close();
} //END outputDataToExcel

/*
Input:
  const std::string PATH - Represents the path to the given output file.
  const std::string data - Represents the data to be stored in a given output file.
Output:
  Output is stored in a file with the specified path as input.
Description:
  Used to output pre-formatted data to a pre-defined output file.
*/
void ExtendedTrafficRules::outputDataToExcel(const std::string PATH, const std::string data) const {
  //open the file pointer using the given file path
  std::ofstream file;
  file.open(PATH.c_str(), std::ios_base::app);

  //write column headers to the file
  file << data << std::endl;

  //close the current file
  file.close();
} //END outputDataToExcel

/*
Input:
  const struct tailgatingInformation tailgatingResult - Struct containing the results of tailgating detection.
  const double responseTimeChange - Contains the response time change this iteration.
  const double maxBreakAccFrontChange - Contains the maximum breaking acceleration change this iteration.
  const double maxAccelAccRearChange - Contains the maximum vehicle acceleration this iteration.
  const double minBreakAccRearChange - Contains the minimum breaking acceleration this iteration.
  const char PATH[] - Character array representing a path in the file-system to the given output file.
Output:
  Prints results in a predefined format to the designated output file.
Method:
  Formats and outputs data to a file container using a CSV format system.
Description:
  Appends the results of iterated tailgating detection to the designated output file.
*/
void ExtendedTrafficRules::appendTailgatingResults(const struct tailgatingInformation tailgatingResult, const double responseTimeChange,
                                                   const double maxBreakAccFrontChange, const double maxAccelAccRearChange,
                                                   const double minBreakAccRearChange, const char PATH[]) const {
                                                   //END appendTailgatingResults

  //check if the number of tailgated vehicles by moment is equal to zero
  if (tailgatingResult.tailgatedVehicle.size() == 0) {
    //return early since no vehicles were tailgated
    return;
  } //END IF

  //open the file pointer using the given file path
  std::ofstream file;
  file.open(PATH, std::ios_base::app);

  //for every victim vehicle being tailgated
  for (size_t i = 0; i < tailgatingResult.tailgatedVehicle.size(); i++) {
    //write data to the file for the offending vehicles
    file << responseTimeChange << "," << maxBreakAccFrontChange << ",";
    file << maxAccelAccRearChange << "," << minBreakAccRearChange << ",";
    file << tailgatingResult.tailgatedVehicle[i].yPosViolator << "\"" << ",";
    file << "\"" << tailgatingResult.tailgatedVehicle[i].xPosViolator << ",";
    file << tailgatingResult.tailgatedVehicle[i].angleViolator << "," << tailgatingResult.violatingVehicleID << ",";
    file << "NA,NA," << ((tailgatingResult.tailgatedVehicle[i].stoppedViolator) ? "true" : "false");
    file << std::endl;
    //for each victim output the tailgating results to the file
    file << ",,,,,," << tailgatingResult.tailgatedVehicle[i].victimID << ",";
    file << tailgatingResult.tailgatedVehicle[i].timestamp << ",";
    file << "\"" << tailgatingResult.tailgatedVehicle[i].xPos << ",";
    file << tailgatingResult.tailgatedVehicle[i].yPos << "\"" << ",";
    file << tailgatingResult.tailgatedVehicle[i].angle << ",";
    file << ((tailgatingResult.tailgatedVehicle[i].stopped) ? "true" : "false");
    file << std::endl;
  } //END FOR

  //close the current file
  file.close();
} //END appendTailgatingResults

/*
Input: 
  const std::string filename - Contains the filename for the output file.
  const int numTailgatingDefault - Determines the number of default tailgating detentions.
  const int numTailgatingHighResponse - Determines the number of high response tailgating detentions.
  const int numTailgatingHighBreak - Determines the number of high break tailgating detentions.
  const int numTailgatingHighAccel - Determines the number of high acceleration tailgating detentions.
  const int numTailgatingHighMinAccel - Determines the number of high minimum tailgating detentions.
  const int numTailgatingLowResponse - Determines the number of low response tailgating detentions.
  const int numTailgatingLowBreak - Determines the number of low break tailgating detentions.
  const int numTailgatingLowAccel - Determines the number of low acceleration tailgating detentions.
  const int numTailgatingLowMinAccel - Determines the number of minimum acceleration tailgating detentions.
  const char PATH[] - Character array representing a path in the file-system to the given output file.
Output:
  Prints results in a predefined format to the designated output file.
Method:
  Formats and outputs data to a file container using a CSV format system.
Description:
  Appends the results of iterated tailgating detection to the designated output file.
*/
void ExtendedTrafficRules::appendTailgatingCounts(const std::string filename,
                                                  const int numTailgatingDefault, const int numTailgatingHighResponse,
                                                  const int numTailgatingHighBreak, const int numTailgatingHighAccel,
                                                  const int numTailgatingHighMinAccel, const int numTailgatingLowResponse,
                                                  const int numTailgatingLowBreak, const int numTailgatingLowAccel,
                                                  const int numTailgatingLowMinAccel, const char PATH[]) const {
                                                  //END appendTailgatingCounts

  //open the file pointer using the given file path
  std::ofstream file;
  file.open(PATH, std::ios_base::app);

  //for each victim output the tailgating results to the file
  file << filename << "," << numTailgatingDefault << "," << numTailgatingHighResponse << "," << numTailgatingHighBreak << ",";
  file << numTailgatingHighAccel << "," << numTailgatingHighMinAccel << "," << numTailgatingLowResponse << "," << numTailgatingLowBreak << ",";
  file << numTailgatingLowAccel << "," << numTailgatingLowMinAccel << std::endl;

  //close the current file
  file.close();
} //END appendTailgatingResults

/*
Input:
  const struct tailgatingInformation tailgatingResult - Struct containing the results of tailgating detection.
  const char PATH[] - Character array representing a path in the file-system to the given output file.
Output:
  Prints results in a predefined format to the designated output file.
Method:
  Formats and outputs data to a file container using a CSV format system.
Description:
  Appends the results of tailgating detection to the designated output file.
*/
void ExtendedTrafficRules::appendTailgatingResultsLineByLine(const struct tailgatingInformation tailgatingResult, const char PATH[]) const {
  //check if the number of tailgated vehicles by moment is equal to zero
  if (tailgatingResult.tailgatedVehicle.size() == 0) {
    //return early since no vehicles were tailgated
    return;
  } //END IF

  //open the file pointer using the given file path
  std::ofstream file;
  file.open(PATH, std::ios_base::app);

  //for every victim vehicle being tailgated
  for (size_t i = 0; i < tailgatingResult.tailgatedVehicle.size(); i++) {
    //for each victim output the tailgating results to the file
    file << tailgatingResult.violatingVehicleID << "," << tailgatingResult.tailgatedVehicle[i].victimID << ",";
    file << tailgatingResult.tailgatedVehicle[i].xPosViolator << "," << tailgatingResult.tailgatedVehicle[i].yPosViolator << ",";
    file << tailgatingResult.tailgatedVehicle[i].timestamp << ",";
    file << tailgatingResult.tailgatedVehicle[i].angleViolator << "," << tailgatingResult.tailgatedVehicle[i].angle << ",";
    file << ((tailgatingResult.tailgatedVehicle[i].stoppedViolator) ? "true" : "false") << std::endl;
  } //END FOR

  //close the current file
  file.close();
} //END appendTailgatingResults

/*
Input:
  const std::vector<struct interval> intervalData - Contains all intervals for the reference vehicle.
  const double referenceLength - Contains the length of the given reference vehicle.
  const std::vector<struct vehicles> allVehicles - Contains all intervals for all other vehicles.
  const std::string mapPath - Contains a string representation for the path to the map file.
  const lanelet::BasicPoint2d origin - Contains the coordinates of the origin point for the OSM file.
  const double speedThreshold - Contains the threshold value for the minimum speed for a vehicle to be considered stopped.
  const double stopDistanceThreshold - Contains the minimum distance threshold value for a stopped vehicle to be considered tailgating another vehicle.
  const double responseTime - Represents the amount of time it takes the vehicle to respond to traffic.
  const double maxBreakAccFront - The maximum breaking acceleration for the front vehicle.
  const double maxAccelAccRear - The maximum breaking acceleration for the rear vehicle.
  const double minBreakAccRear - The minimum breaking acceleration for the front vehicle.
  const int mode - The operation mode for the tailgating function:
    All = 1: Both vehicles that are stopped at intersections and moving vehicles are checked.
    StandstillOnly = 2: Only vehicles that are stopped at intersections are checked for tailgating.
    DrivingOnly = 3: Only vehicles that are driving are checked for tailgating.
  const double responseVariation - Represents the variation in the response time for the vehicle.
  const double maxBreakAccFrontVariation - Represents the variation in the max front vehicle break acceleration.
  const double maxAccelAccRearVariation - Represents the variation in the max rear vehicle acceleration.
  const double minBreakAccRearVariation - Represents the variation in the min rear vehicle acceleration.
  const double responseVariationTick - Number of times to loop through the range.
  const double maxBreakAccFrontVariationTick - Number of times to loop through the range.
  const double maxAccelAccRearVariationTick - Number of times to loop through the range.
  const double minBreakAccRearVariationTick - Number of times to loop through the range.
  const char PATH[] - Character array representing a path in the file-system to the given output file.
Output:
  std::vector<struct tailgatingInformation> getDminDistribution - Contains all the tailgating data for the given record.
Method:
  Runs the isTailgatingAtMoment function for every interval provided to generate statistics.
Description:
  Runs isTailgatingAtMoment with various constraints and outputs the results to a log file.
*/
std::vector<struct tailgatingInformation> ExtendedTrafficRules::getDminDistribution(const std::vector<struct interval> intervalData, const double referenceLength,
                                                                                    const std::vector<struct vehicles> allVehicles, const std::string mapPath,
                                                                                    const lanelet::BasicPoint2d origin, const double speedThreshold,
                                                                                    const double stopDistanceThreshold, const double responseTime,
                                                                                    const double maxBreakAccFront, const double maxAccelAccRear,
                                                                                    const double minBreakAccRear, const int mode,
                                                                                    const double responseVariation, const double maxBreakAccFrontVariation,
                                                                                    const double maxAccelAccRearVariation, const double minBreakAccRearVariation,
                                                                                    const int responseVariationTick, const int maxBreakAccFrontVariationTick,
                                                                                    const int maxAccelAccRearVariationTick,
                                                                                    const int minBreakAccRearVariationTick, const char PATH[]) const {
                                                                                    //END getDminDistribution

  //contains the results of tailgating checks
  std::vector<struct tailgatingInformation> tailgatingResults;

  //finds the new value of vehicle coefficients
  double responseTimeChange, maxBreakAccFrontChange, maxAccelAccRearChange, minBreakAccRearChange;

  //Get the +/- value of responseTime with responseVariation based on the number of loop iterations
  for (size_t i = -responseVariationTick; i <= responseVariationTick; i++) {
    responseTimeChange = responseTime + (i / responseVariationTick)*(responseVariation);

    //Get the +/- value of maxBreakAccFront with maxBreakAccFrontVariation based on the number of loop iterations
    for (size_t k = -maxBreakAccFrontVariationTick; k <= maxBreakAccFrontVariationTick; k++) {
      maxBreakAccFrontChange = maxBreakAccFront + (k / maxBreakAccFrontVariationTick)*(maxBreakAccFrontVariation);

      //Get the +/- value of maxAccelAccRear with maxAccelAccRearVariation based on the number of loop iterations
      for (size_t j = -maxAccelAccRearVariationTick; j <= maxAccelAccRearVariationTick; j++) {
        maxAccelAccRearChange = maxAccelAccRear + (j / maxAccelAccRearVariationTick)*(maxAccelAccRearVariation);

        //Get the +/- value of minBreakAccRear with minBreakAccRearVariation based on the number of loop iterations
        for (size_t n = -minBreakAccRearVariationTick; n <= minBreakAccRearVariationTick; n++) {
          minBreakAccRearChange = minBreakAccRear + (n / minBreakAccRearVariationTick)*(minBreakAccRearVariation);

          //Gets the result of vehicle tailgating checks
          appendTailgatingResults(isTailgatingAtMoment(intervalData, referenceLength, allVehicles, mapPath, origin, speedThreshold, stopDistanceThreshold,
                                                       responseTimeChange, maxBreakAccFrontChange, maxAccelAccRearChange, minBreakAccRearChange, mode, "us"),
                                                      //END isTailgatingAtMoment
                                  responseTimeChange, maxBreakAccFrontChange, maxAccelAccRearChange, minBreakAccRearChange, PATH);
                                  //END appendTailgatingResults
        } //END FOR
      } //END FOR
    } //END FOR
  } //END FOR

  //return the tailgating results to the caller
  return tailgatingResults;
} //END getDminDistribution

/*
Input:
  const std::vector<struct interval> intervalData - Contains all intervals for the reference vehicle.
  const double referenceLength - Gets the length of the reference vehicle.
  const std::vector<struct vehicles> allVehicles - Contains all intervals for all other vehicles.
  const std::string mapPath - Contains a string representation for the path to the map file.
  const lanelet::BasicPoint2d originPoint - Contains the coordinates of the origin point for the OSM file.
  const double speedThreshold - Contains the threshold value for the minimum speed for a vehicle to be considered stopped.
  const double stopDistanceThreshold - Contains the minimum distance threshold value for a stopped vehicle to be considered tailgating another vehicle.
  const double responseTime - Represents the amount of time it takes the vehicle to respond to traffic.
  const double maxBreakAccFront - The maximum breaking acceleration for the front vehicle.
  const double maxAccelAccRear - The maximum breaking acceleration for the rear vehicle.
  const double minBreakAccRear - The minimum breaking acceleration for the front vehicle.
  const int mode - The operation mode for the tailgating function:
    All = 1: Both vehicles that are stopped at intersections and moving vehicles are checked.
    StandstillOnly = 2: Only vehicles that are stopped at intersections are checked for tailgating.
    DrivingOnly = 3: Only vehicles that are driving are checked for tailgating.
Output:
  struct tailgatingInformation isTailgatingAtMoment - Returns a tailgating object with the following parameters:
    int violatingVehicleID - The ID of the tailgating (offending) vehicle.
    std::vector<struct tailgatingMoment> tailgatedVehicle - A list of tailgated vehicles (victim) information:
      int victimID - The ID of the tailgated vehicle.
      int timestamp - The moment where the vehicle was tailgated.
      double xPos, yPos, angle - Positioning of the victim.
      bool stopped - Signals if the vehicle was tailgated while stopped, or while moving.
Method:
  Tailgating is detected between two vehicles by taking the front point of a (rear) vehicle and comparing its distance with the back point of a (front) vehicle.
  Finding the distance between the two points and comparing it with d_min as is formulated in findMinDist determines if vehicles are tailgating.
  Comparing the orientation of the two vehicles as is formulated in closeProximity ensures the vehicles are driving down the same direction,
  and not perpendicular or diagonal to each other.
  All vehicles tailgated by the reference (ego) vehicle are returned in the tailgatingInformation struct.
Description:
  We check if one vehicle has tailgated another vehicle using the function.
  Information for the tailgated vehicles is returned in the form of a tailgatingInformation struct.
*/
struct tailgatingInformation ExtendedTrafficRules::isTailgatingAtMoment(const std::vector<struct interval> intervalData, const double referenceLength,
                                                                        const std::vector<struct vehicles> allVehicles, const std::string mapPath,
                                                                        const lanelet::BasicPoint2d origin, const double speedThreshold,
                                                                        const double stopDistanceThreshold, const double responseTime,
                                                                        const double maxBreakAccFront, const double maxAccelAccRear,
                                                                        const double minBreakAccRear, const int mode, const char * vehicleLocation) const {
                                                                        //END isTailgatingAtMoment
  //container for tailgating violations
  struct tailgatingInformation tailgating;

  //container for tailgating moment
  struct tailgatingMoment currentViolation;

  //container for other vehicles velocity
  double frontVelocity, dmin;

  //container for the the current time moment
  int moment;

  //container for the current velocity from x and y velocity vectors
  double rearVelocity;

  //container for basic points
  lanelet::BasicPoint2d vehicleHead, vehicleRear;

  //contains the projector for the map
  projection::UtmProjector projector(lanelet::Origin({origin[0], origin[1]}), true, false);

  //opens the map and gets the routing graph
  auto map = load(mapPath, projector);

  //container for ego and other lanelets
  lanelet::ConstLanelet egoLanelet, otherLanelet;

  //contains if a vehicle has stopped
  bool stoppedOne = false;
  bool stoppedTwo = false;

  //check that the vehicle has already been found tailgating
  bool isAlreadyTailgating = false;

  //contains the coordinate data for the vehicle
  double distance, xDif, yDif;

  //for every entry in the interval with the reference vehicle
  for (size_t i = 0; i < intervalData.size(); i++) {
    //contains the rear velocity for the vehicle
    rearVelocity = sqrt(intervalData[i].velocityX*intervalData[i].velocityX + intervalData[i].velocityY*intervalData[i].velocityY);

    //checks if the vehicle is currently in standstill
    if (inStandstill(rearVelocity, speedThreshold)) {
      stoppedOne = true;
    } //END IF

    //keeps track of the current moment
    moment = intervalData[i].timestamp;

    //get the front position of the reference vehicle
    vehicleHead = BasicPoint2d(intervalData[i].xPos, intervalData[i].yPos);
    vehicleHead = getVehicleBounds(referenceLength, intervalData[i].yaw, vehicleHead[0], vehicleHead[1], Front);

    //get the reference vehicles current lanelet
    egoLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, vehicleHead, 1)[0]);

    //for every vehicle in the set of all vehicle records
    for (size_t k = 0; k < allVehicles.size(); k++) {
      //for every moment entry in the set of all vehicle records
      for (size_t j = 0; j < allVehicles[k].intervals.size(); j++) {
        //if the vehicle ID equals that of the reference vehicle were comparing other vehicles to
        if (intervalData[i].id == allVehicles[k].intervals[j].id) {
          continue; //continue to the next entry
        } //END IF

        //if the current moment for the other vehicle equals the reference vehicles moment
        if (moment == allVehicles[k].intervals[j].timestamp) {
          //finds the back position of the other vehicle
          vehicleRear = BasicPoint2d(allVehicles[k].intervals[j].xPos, allVehicles[k].intervals[j].yPos);
          vehicleRear = getVehicleBounds(allVehicles[k].length, allVehicles[k].intervals[j].yaw, vehicleRear[0], vehicleRear[1], Back);

          //get the other vehicles current lanelet
          otherLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, vehicleRear, 1)[0]);

          //gets the velocity of the other vehicle
          frontVelocity = sqrt(allVehicles[k].intervals[j].velocityX * allVehicles[k].intervals[j].velocityX +
                               allVehicles[k].intervals[j].velocityY * allVehicles[k].intervals[j].velocityY);
          //END frontVelocity

          //if the other vehicle is currently in standstill
          if (inStandstill(frontVelocity, speedThreshold)) {
            stoppedTwo = true;
          } //END IF

          //if both vehicles are on the same lanelet
          if (egoLanelet.id() == otherLanelet.id()) {

            //if one of the vehicles are currently in standstill
            if ((stoppedOne && stoppedTwo) && (mode == All || mode == StandstillOnly)) {
              //find the x coordinate value between the two vehicles
              xDif = vehicleHead.x() - vehicleRear.x();
              yDif = vehicleHead.y() - vehicleRear.y();

              //find the distance between the two vehicles
              distance = sqrt(xDif*xDif + yDif*yDif);

              //if the distance is below a predefined threshold then return the offending vehicles ID
              if (distance < stopDistanceThreshold) {
                //reset the current moment
                currentViolation = tailgatingMoment();

                //report the victim vehicles ID and the current moments timestamp
                currentViolation.victimID = allVehicles[k].intervals[j].id;
                currentViolation.timestamp = allVehicles[k].intervals[j].timestamp;
                //report the positional information for the vehicle
                currentViolation.xPos = allVehicles[k].intervals[j].xPos;
                currentViolation.yPos = allVehicles[k].intervals[j].yPos;
                currentViolation.angle = allVehicles[k].intervals[j].yaw;
                //report that the vehicles were stopped at the given intersection
                currentViolation.stopped = stoppedTwo;
                currentViolation.stoppedViolator = stoppedOne;
                //report the positional information for the victim
                currentViolation.xPosViolator = intervalData[i].xPos;
                currentViolation.yPosViolator = intervalData[i].yPos;
                currentViolation.angleViolator = intervalData[i].yaw;

                //add the violating vehicles ID and append the current victims violation information
                tailgating.violatingVehicleID = intervalData[i].id;
                //add the victim to the vehicles list of tailgated vehicles
                tailgating.tailgatedVehicle.push_back(currentViolation);

		//remove the vehicle standstill trigger for the other vehicle
	        stoppedTwo = false;

		//the vehicle engaged in tailgating at the given moment
		isAlreadyTailgating = true;

		//exit the loop
                break;
              } //END IF
            } //END IF

            //gets the minimum distance before tailgating detection is triggered
            dmin = findMinDist(responseTime, rearVelocity, frontVelocity, maxBreakAccFront, maxAccelAccRear, minBreakAccRear);

            //check if the vehicles are in close enough proximity and have the right orientation (in terms of yaw) for tailgating detection to be triggered
            if ((mode == All || mode == DrivingOnly) && closeProximity(vehicleRear, vehicleHead, allVehicles[k].intervals[j].yaw, intervalData[i].yaw, dmin)) {
              //determine that the front vehicle is in front of the rear vehicle in terms of the lanelets geometric orientation
              if (lanelet::geometry::toArcCoordinates(egoLanelet.centerline2d(), vehicleRear).distance >
                  lanelet::geometry::toArcCoordinates(egoLanelet.centerline2d(), vehicleHead).distance) {
                //reset the current moment
                currentViolation = tailgatingMoment();

                //add the ID and the current moments timestamp for the victim vehicle
                currentViolation.victimID = allVehicles[k].intervals[j].id;
                currentViolation.timestamp = allVehicles[k].intervals[j].timestamp;
                //report the positional information for the vehicle
                currentViolation.xPos = allVehicles[k].intervals[j].xPos;
                currentViolation.yPos = allVehicles[k].intervals[j].yPos;
                currentViolation.angle = allVehicles[k].intervals[j].yaw;
                //report that one or both of the vehicles were moving at the given intersection
                currentViolation.stopped = stoppedTwo;
                currentViolation.stoppedViolator = stoppedOne;
                //report the positional information for the victim
                currentViolation.xPosViolator = intervalData[i].xPos;
                currentViolation.yPosViolator = intervalData[i].yPos;
                currentViolation.angleViolator = intervalData[i].yaw;

                //add the violating vehicles ID and append the current victims violation information
                tailgating.violatingVehicleID = intervalData[i].id;

                //add the victim to the vehicles list of tailgated vehicles
                tailgating.tailgatedVehicle.push_back(currentViolation);

		//remove the vehicle standstill trigger for the other vehicle
	        stoppedTwo = false;

		//the vehicle engaged in tailgating at the given moment
                isAlreadyTailgating = true;

		//exit the loop
		break;
              } //END IF
            } //END IF
          } //END IF
        } //END IF

        //remove the vehicle standstill trigger for the other vehicle
        stoppedTwo = false;
      } //END FOR

      //check if tailgating was already detected
      if (isAlreadyTailgating) {
	break;
      } //END IF
    } //END FOR

    //mark that we need to check for tailgating next time
    isAlreadyTailgating = false;

    //remove the vehicle standstill trigger for the reference vehicle
    stoppedOne = false;
  } //END FOR

  //return zero to indicate no tailgating has been detected
  return tailgating;
} //END isTailgatingAtMoment

/*
Input:
  const std::string data - contains a string to be split along a delimiter.
  const char delim - contains a character to split the given string along.
Output:
  std::vector<std::string> stringSplit - Returns a vector containing the elements of a string that has been split.
Method:
  Iterate over a string and push back all substring up-to a delimiter character.
  Push back the substring generated along the way into the substring vector.
  Continue until the string is empty, and push back the remainder into the substring vector.
Description:
  Gets the substring generated by delimiting a character from a string.
*/
std::vector<std::string> ExtendedTrafficRules::stringSplit(const std::string data, const char delim) const {
  //define a substring variable
  std::string substring;

  //contains a list of all parsed substrings
  std::vector<std::string> list;

  //Turn the given string into a string stream
  std::stringstream stream(data);

  //while the string stream is not empty, get each substring with delimiting characters removed
  while (std::getline(stream, substring, delim)) {
    //push back the delimited substring into the list
    list.push_back(substring);
  } //END WHILE

  //returns the list of substrings
  return list;
} //END stringSplit

/*
Input:
  const std::vector<struct vehicleStatistics> data - Contains statistics for all vehicles in the dataset.
  const std::string pathName - Contains the name of the output file we are storing output within.
  const std::string fileName - Contains the name of the interaction file that was checked.
Output:
  void writeToCSV - Prints a CSV file to the file-system containing the statistical results in the interaction dataset struct.
Method:
  Add all statistical elements to a CSV file containing statistical output.
  Column titles and filenames are incorporated into the output of the function.
Description:
  Record statistical results for an interaction dataset struct to an output file.
*/
void ExtendedTrafficRules::writeToCSV(const std::vector<struct vehicleStatistics> data, const std::string pathName, const std::string fileName) const {
  //open the output file in append mode
  std::ofstream file;
  file.open(pathName.c_str(), std::ios_base::app);

  //used to contain statistics for entire dataset
  double averageSpeed = 0;
  double averageDuration = 0;
  double averageXPosition = 0;
  double averageYPosition = 0;

  //add the file name to the first column
  file << fileName << std::endl;

  //for every vehicle recorded in the dataset
  for (size_t i = 0; i < data.size(); i++) {
    //append the line of data into the current row of the CSV file, using commas to delimit each entry
    file << ",";
    file << data[i].trackID;
    file << ",";
    file << data[i].agentType;
    file << ",";
    file << data[i].length;
    file << ",";
    file << data[i].width;
    file << ",";
    file << data[i].timeRange;
    file << ",";
    file << data[i].timeMedian;
    file << ",";
    file << data[i].timeMean;
    file << ",";
    file << data[i].timeMode;
    file << ",";
    file << data[i].timeStandardDeviation;
    file << ",";
    file << data[i].xRange;
    file << ",";
    file << data[i].xMedian;
    file << ",";
    file << data[i].xMean;
    file << ",";
    file << data[i].xMode;
    file << ",";
    file << data[i].xStandardDeviation;
    file << ",";
    file << data[i].yRange;
    file << ",";
    file << data[i].yMedian;
    file << ",";
    file << data[i].yMean;
    file << ",";
    file << data[i].yMode;
    file << ",";
    file << data[i].yStandardDeviation;
    file << ",";
    file << data[i].xVelocityRange;
    file << ",";
    file << data[i].xVelocityMedian;
    file << ",";
    file << data[i].xVelocityMean;
    file << ",";
    file << data[i].xVelocityMode;
    file << ",";
    file << data[i].xVelocityStandardDeviation;
    file << ",";
    file << data[i].yVelocityRange;
    file << ",";
    file << data[i].yVelocityMedian;
    file << ",";
    file << data[i].yVelocityMean;
    file << ",";
    file << data[i].yVelocityMode;
    file << ",";
    file << data[i].yVelocityStandardDeviation;
    file << ",";
    file << data[i].yawRange;
    file << ",";
    file << data[i].yawMedian;
    file << ",";
    file << data[i].yawMean;
    file << ",";
    file << data[i].yawMode;
    file << ",";
    file << data[i].yawStandardDeviation;

    //place an end-line character to move to the next row in the CSV file
    file << std::endl;

    //add averages to the sum for the entire dataset
    averageSpeed += sqrt(data[i].xVelocityMean*data[i].xVelocityMean + data[i].yVelocityMean*data[i].yVelocityMean);
    averageDuration += data[i].timeMean;
    averageXPosition += data[i].xMean;
    averageYPosition += data[i].yMean;
  } //END FOR

  //place an end-line character to move to the next row in the CSV file
  file << std::endl;

  //close the file
  file.close();
} //END writeToCSV

/*
Input:
  const std::string dataPath - Contains the path to the interaction dataset file to be checked.
Output:
  std::vector<struct vehicleStatistics> getVehicleStats - return a vector of vehicleStatistics structs containing statistics for each vehicle in the given dataset.
Method:
  Iterate through every line of the interaction-CSV file and run statistical tests:
    We find and record mean, median, mode, range, and population standard deviation measures for the timestamps, coordinates, velocity, and yaw values.
Description:
  Gets statistical information for the vehicles and stores it in a vector of vehicleStatistics structs.
*/
std::vector<struct vehicleStatistics> ExtendedTrafficRules::getVehicleStats(const std::string dataPath) const {
  //used to store information for each vehicle
  std::vector<struct vehicleStatistics> vehicles;

  //stores the raw delimited information from the CSV files line
  std::vector<std::string> vehicleRaw;

  //opens the interaction dataset csv file
  std::ifstream infile(dataPath);

  //contains a raw string for the CSV file without delimiting the data
  std::string line;

  //operating parameters for reading the CSV format
  bool firstRun, nextVehicle;

  //contains information related to the vehicle information along each line of the CSV data
  int track;

  //track index depends on the first entry in the file
  track = Default::None;

  //by default assume the first run through the data along with the vehicle data are yet to be run
  firstRun = true;

  //determines that the first vehicle encountered should be recorded
  nextVehicle = true;

  //create a struct to store vehicle information into
  struct vehicleStatistics newVehicle;

  //create a vector to store sorted information into
  std::vector<double> sortedVector;

  //for each line in the CSV file
  while (std::getline(infile, line, '\n')) {

    //skip the header since it does not contain any information
    if (firstRun) {
      //signal the the first line of data has been iterated through
      firstRun = false;

      //continue to the next entry in the dataset
      continue;
    } //END IF

    //delimit the line of the CSV file to get vehicle information
    boost::split(vehicleRaw, line, boost::is_any_of(","));

    //check if we have just started iterating through the code
    if (track == Default::None) {
      //get the initial track number and set it to be the track
      track = std::stoi(vehicleRaw[0]);
    } //END IF

    //if we have a new track then there is a new vehicle being encountered
    if (track != std::stoi(vehicleRaw[0])) {
      //record the starting and ending coordinates
      nextVehicle = true;

      //determines time based statistical values
      sortedVector = sortVector(newVehicle.timestamp);
      newVehicle.timeRange = findRange(sortedVector);
      newVehicle.timeMedian = findMedian(sortedVector);
      newVehicle.timeMean = findMean(sortedVector);
      newVehicle.timeMode = findMode(sortedVector);
      newVehicle.timeStandardDeviation = findStandardDeviation(sortedVector);

      //determines x position based statistical values
      sortedVector = sortVector(newVehicle.xCoord);
      newVehicle.xRange = findRange(sortedVector);
      newVehicle.xMedian = findMedian(sortedVector);
      newVehicle.xMean = findMean(sortedVector);
      newVehicle.xMode = findMode(sortedVector);
      newVehicle.xStandardDeviation = findStandardDeviation(sortedVector);

      //determines y position based statistical values
      sortedVector = sortVector(newVehicle.yCoord);
      newVehicle.yRange = findRange(sortedVector);
      newVehicle.yMedian = findMedian(sortedVector);
      newVehicle.yMean = findMean(sortedVector);
      newVehicle.yMode = findMode(sortedVector);
      newVehicle.yStandardDeviation = findStandardDeviation(sortedVector);

      //determines x velocity based statistical values
      sortedVector = sortVector(newVehicle.xVelocity);
      newVehicle.xVelocityRange = findRange(sortedVector);
      newVehicle.xVelocityMedian = findMedian(sortedVector);
      newVehicle.xVelocityMean = findMean(sortedVector);
      newVehicle.xVelocityMode = findMode(sortedVector);
      newVehicle.xVelocityStandardDeviation = findStandardDeviation(sortedVector);

      //determines y velocity based statistical values
      sortedVector = sortVector(newVehicle.yVelocity);
      newVehicle.yVelocityRange = findRange(sortedVector);
      newVehicle.yVelocityMedian = findMedian(sortedVector);
      newVehicle.yVelocityMean = findMean(sortedVector);
      newVehicle.yVelocityMode = findMode(sortedVector);
      newVehicle.yVelocityStandardDeviation = findStandardDeviation(sortedVector);

      //determines yaw based statistical values
      sortedVector = sortVector(newVehicle.yaw);
      newVehicle.yawRange = findRange(sortedVector);
      newVehicle.yawMedian = findMedian(sortedVector);
      newVehicle.yawMean = findMean(sortedVector);
      newVehicle.yawMode = findMode(sortedVector);
      newVehicle.yawStandardDeviation = findStandardDeviation(sortedVector);

      //store the current vehicle information into the array
      vehicles.push_back(newVehicle);

      //create a new container for vehicle information
      struct vehicleStatistics newVehicle;

      //remember the current track being considered
      track = std::stoi(vehicleRaw[0]);
    } //END IF

    //if a new vehicle in the dataset was encountered
    if (nextVehicle) {
      //mark that we have recorded the starting and ending interval data
      nextVehicle = false;

      //gets vehicle type information
      newVehicle.trackID = std::stoi(vehicleRaw[0]);
      newVehicle.agentType = vehicleRaw[3];
      newVehicle.length = std::stod(vehicleRaw[9]);
      newVehicle.width = std::stod(vehicleRaw[10]);
    } //END IF

    //gets vehicle momentary information
    newVehicle.frameID.push_back(std::stoi(vehicleRaw[1]));
    newVehicle.timestamp.push_back(std::stod(vehicleRaw[2]));
    newVehicle.xCoord.push_back(std::stod(vehicleRaw[4]));
    newVehicle.yCoord.push_back(std::stod(vehicleRaw[5]));
    newVehicle.xVelocity.push_back(std::stod(vehicleRaw[6]));
    newVehicle.yVelocity.push_back(std::stod(vehicleRaw[7]));
    newVehicle.yaw.push_back(std::stod(vehicleRaw[8]));

    //clear the line buffer for the next line of raw vehicle information
    vehicleRaw.clear();
  } //END WHILE

  //determines time based statistical values
  sortedVector = sortVector(newVehicle.timestamp);
  newVehicle.timeRange = findRange(sortedVector);
  newVehicle.timeMedian = findMedian(sortedVector);
  newVehicle.timeMean = findMean(sortedVector);
  newVehicle.timeMode = findMode(sortedVector);
  newVehicle.timeStandardDeviation = findStandardDeviation(sortedVector);

  //determines x position based statistical values
  sortedVector = sortVector(newVehicle.xCoord);
  newVehicle.xRange = findRange(sortedVector);
  newVehicle.xMedian = findMedian(sortedVector);
  newVehicle.xMean = findMean(sortedVector);
  newVehicle.xMode = findMode(sortedVector);
  newVehicle.xStandardDeviation = findStandardDeviation(sortedVector);

  //determines y position based statistical values
  sortedVector = sortVector(newVehicle.yCoord);
  newVehicle.yRange = findRange(sortedVector);
  newVehicle.yMedian = findMedian(sortedVector);
  newVehicle.yMean = findMean(sortedVector);
  newVehicle.yMode = findMode(sortedVector);
  newVehicle.yStandardDeviation = findStandardDeviation(sortedVector);

  //determines x velocity based statistical values
  sortedVector = sortVector(newVehicle.xVelocity);
  newVehicle.xVelocityRange = findRange(sortedVector);
  newVehicle.xVelocityMedian = findMedian(sortedVector);
  newVehicle.xVelocityMean = findMean(sortedVector);
  newVehicle.xVelocityMode = findMode(sortedVector);
  newVehicle.xVelocityStandardDeviation = findStandardDeviation(sortedVector);

  //determines y velocity based statistical values
  sortedVector = sortVector(newVehicle.yVelocity);
  newVehicle.yVelocityRange = findRange(sortedVector);
  newVehicle.yVelocityMedian = findMedian(sortedVector);
  newVehicle.yVelocityMean = findMean(sortedVector);
  newVehicle.yVelocityMode = findMode(sortedVector);
  newVehicle.yVelocityStandardDeviation = findStandardDeviation(sortedVector);

  //determines yaw based statistical values
  sortedVector = sortVector(newVehicle.yaw);
  newVehicle.yawRange = findRange(sortedVector);
  newVehicle.yawMedian = findMedian(sortedVector);
  newVehicle.yawMean = findMean(sortedVector);
  newVehicle.yawMode = findMode(sortedVector);
  newVehicle.yawStandardDeviation = findStandardDeviation(sortedVector);

  //store the current vehicle information into the array
  vehicles.push_back(newVehicle);

  //return the vector containing the vehicle information
  return vehicles;
} //END getVehicleStats

/*
Input:
  const std::string dataPath - Contains a path to the interaction dataset CSV being checked.
Output:
  std::vector<struct vehicles> getVehicleData - Contains a vector of vehicles structs containing formatted vehicle information:
    The vehicles struct contains the following information:
      double xStart, xEnd, yStart, yEnd - the (x, y) values for the starting and ending point for the intersection recording.
      double yawStart, yawEnd - the initial and ending yaw in terms of radians for the intersection recording.
      double length, width - the vehicles length and width values.
      std::vector<struct interval> intervals - All intervals related to the vehicle found in the intersection recording.
    The Interval struct contains the following information:
      int id;
      int timestamp;
      double velocityX, velocityY;
      double yaw;
      double xPos, yPos;
Method:
  Iterates through the dataset and extracts data into the relevant struct line by line.
  Interaction dataset formatting guidelines can be found here: https://interaction-dataset.com/details-and-format
    As can be seen, we see the following columns of data per every vehicle_tracks_xxx.csv file:
      track_id: column 1. For each vehicle_tracks_xxx.csv file, the track_id starts from 1, and represent the ID of the agent.
      frame_id: column 2. For each agent (per track_id), frame_id starts from 1 and represents the frames the agent appears in the video.
      timestamp_ms: column 3. For each agent (per track_id), timestamp_ms from 100ms, and represents the time the agent appears in the video. The unit is ms.
      agent_type: column 4. It represents the types of tracked agents. For example, it can be a car, a truck and so on.
      x: column 5, the x position of the agent at each frame. The unit is m.
      y: column 6, the y position of the agent at each frame. The unit is m.
      vx: column 7, the velocity of the agent along x-direction at each frame. The unit is m/s.
      vy: column 8, the velocity of the agent along y-direction at each frame. The unit is m/s.
      psi_rad: column 9, the yaw angle of the agent at each frame. The unit is rad.
      length: column 10, the length of the agent. The unit is m.
      width: column 11, the width of the agent. The unit is m.
Description:
  Gets vehicle data from the specified path for an interaction dataset.
*/
std::vector<struct vehicles> ExtendedTrafficRules::getVehicleData(const std::string dataPath) const {
  //contains vehicle information including vehicle start and ending information along with interval data at each moment the vehicle appeared in the dataset
  std::vector<struct vehicles> data;
  struct vehicles vehicle;
  struct interval vehicleInterval;

  //stores the raw delimited information from the CSV files line
  std::vector<std::string> vehicleRaw;

  //opens the interaction dataset csv file
  std::ifstream infile(dataPath);

  //contains a raw string for the CSV file without delimiting the data
  std::string line;

  //operating parameters for reading the CSV format
  bool firstRun, nextVehicle;

  //contains information related to the vehicle information along each line of the CSV data
  double xStart, yStart, xEnd, yEnd, length, width, yaw, velocityX, velocityY, yawStart;
  int timestamp, id, track;

  //track index depends on the first entry in the file
  track = Default::None;

  //by default assume the first run through the data along with the vehicle data are yet to be run
  firstRun = true;
  nextVehicle = true;

  //for each line in the CSV file
  while (std::getline(infile, line, '\n')) {
    //skip the header since it does not contain any information
    if (firstRun) {
      //signal the the first line of data has been iterated through
      firstRun = false;

      //continue to the next entry in the dataset
      continue;
    } //END IF

    //delimit the line of the CSV file to get vehicle information
    boost::split(vehicleRaw, line, boost::is_any_of(","));

    //check if we have just started iterating through the code
    if (track == Default::None) {
      //get the initial track number and set it to be the track
      track = std::stoi(vehicleRaw[0]);
    } //END IF

    //if we have a new track then there is a new vehicle being encountered
    if (track != std::stoi(vehicleRaw[0])) {

      //record the starting and ending coordinates
      nextVehicle = true;

      //remember the current track being considered
      track = std::stoi(vehicleRaw[0]);

      //record starting and ending vehicle information
      vehicle.xStart = xStart;
      vehicle.yStart = yStart;
      vehicle.xEnd = xEnd;
      vehicle.yEnd = yEnd;
      vehicle.yawStart = yawStart;
      vehicle.yawEnd = yaw;
      vehicle.length = length;
      vehicle.width = width;

      //save start-end vehicle information along with track information
      data.push_back(vehicle);

      //clear the vectors for the next vehicle
      vehicle = vehicles();
    } //END IF

    //if a new vehicle in the dataset was encountered
    if (nextVehicle) {
      //mark that we have recorded the starting and ending interval data
      nextVehicle = false;
      yawStart = std::stod(vehicleRaw[8]);
      xStart = std::stod(vehicleRaw[4]);
      yStart = std::stod(vehicleRaw[5]);
    } //END IF

    //get the information from the line in the CSV file
    id = std::stoi(vehicleRaw[0]);
    timestamp = std::stoi(vehicleRaw[2]);
    velocityX = std::stod(vehicleRaw[6]);
    velocityY = std::stod(vehicleRaw[7]);
    yaw = std::stod(vehicleRaw[8]);
    length = std::stod(vehicleRaw[9]);
    width = std::stod(vehicleRaw[10]);
    xEnd = std::stod(vehicleRaw[4]);
    yEnd = std::stod(vehicleRaw[5]);

    //record the current interval of vehicle data
    vehicleInterval.id = id;
    vehicleInterval.timestamp = timestamp;
    vehicleInterval.velocityX = velocityX;
    vehicleInterval.velocityY = velocityY;
    vehicleInterval.yaw = yaw;
    vehicleInterval.xPos = xEnd;
    vehicleInterval.yPos = yEnd;
    vehicle.intervals.push_back(vehicleInterval);

    //clear the interval struct to record the next moment
    vehicleInterval = interval();

    //clear the line buffer for the next line of raw vehicle information
    vehicleRaw.clear();
  } //END WHILE

  vehicle.xStart = xStart;
  vehicle.yStart = yStart;
  vehicle.xEnd = xEnd;
  vehicle.yEnd = yEnd;
  vehicle.yawStart = yawStart;
  vehicle.yawEnd = yaw;
  vehicle.length = length;
  vehicle.width = width;
  data.push_back(vehicle);

  //return the vehicle data back to the caller
  return data;
} //END getVehicleData

/*
Input:
  const lanelet::Lanelet otherLanelet - Contains the location of the other vehicles lanelet.
  const lanelet::Lanelet currentLanelet - Contains the location of the current vehicles lanelet.
  const double otherOrientation - Contains the current vehicles orientation.
  const double currentOrientation - Contains the other vehicles orientation.
  const lanelet::routing::RoutingGraphConstPtr routingGraph - Contains a routing graph for the given OSM location.
Output:
  Outputs if the current vehicle is incoming left of the other vehicle.
Description:
  Determines if a vehicle is incoming left of another vehicle or not.
*/
bool ExtendedTrafficRules::incomingLeftOf(const lanelet::Lanelet otherLanelet, const lanelet::Lanelet currentLanelet,
                                          const double otherOrientation, const double currentOrientation,
                                          const lanelet::routing::RoutingGraphConstPtr routingGraph) const {
                                          //END incomingLeftOf

  //check if the difference in orientation between the two vehicles indicates a leftward turn
  if (turnDirection(currentOrientation, otherOrientation) == Left) {
    //return that the current vehicle is incoming left to another vehicle
    return true;
  } //END IF

  //return that the current vehicle is not incoming left to another vehicle
  return false;
} //END incomingLeftOf

/*
Input:
  const double diameter - Contains the vehicle length which is interpreted as the diameter of a circle.
  const double angle - Contains the yaw angle of the vehicle,
  const double referenceX - Contains the reference-X position of the vehicle in terms of meters.
  const double referenceY - Contains the reference-Y position of the vehicle in terms of meters.
  const double width - Contains the width of the given vehicle.
  const Bounds mode = Front - Determines if we find the front or back point of the vehicle, the default is front.
Output:
  lanelet::BasicPoint2d getVehicleBounds - Contains the front/back point of the vehicle.
Method:
  Compares the vehicle to a circle and uses the length of the vehicle as a diameter and the yaw of the vehicle as an angle.
Formula:
  We use the formula to find the change in x and y position:
    Δx = rcos(θ)
    Δy = rcos(θ)
    Where:
      Front: (x + Δx, y + Δy)
      Back: (x - Δx, y - Δy)
Description:
  Determines the front of back point of the vehicle.
*/
lanelet::BasicPoint2d ExtendedTrafficRules::getVehicleBounds(const double diameter, const double angle, const double referenceX, const double referenceY,
                                                             const Bounds mode, const double width, const BoundaryValues bounds) const {
                                                             //END getVehicleBounds

  //used to contain the position and dimensions of the vehicle
  double xPos, yPos, radius, radAngle;

  //the centerpoint of the vehicle is used as reference, so half the length of the vehicle
  radius = diameter / 2;

  //get the yaw value in terms of circular radians
  radAngle = getRadians(angle);

  //check the mode to determine the correct output type
  if (mode == Bounds::Front) {
    //used to contain the x and y position of the vehicle
    xPos = radius * std::cos(angle);
    yPos = radius * std::sin(angle);

    //returns the front position of the vehicle
    return lanelet::BasicPoint2d(referenceX + xPos, referenceY + yPos);
  }
  else if (bounds == BoundaryValues::Centre) {
    //returns the centerpoint
    return lanelet::BasicPoint2d(referenceX, referenceY);
  }
  else if (mode == Bounds::Back) {
    //used to contain the x and y position of the vehicle
    xPos = radius * std::cos(angle);
    yPos = radius * std::sin(angle);

    //return the back position of the vehicle
    return lanelet::BasicPoint2d(referenceX - xPos, referenceY - yPos);
  }
  else if (mode == Bounds::TopLeft) {
    //used to contain the x and y position of the vehicle
    xPos = referenceX + (((width*(bounds/100.0)) / 2) * cos(radAngle)) - ((radius*(bounds/100.0)) * sin(radAngle));
    yPos = referenceY + (((width*(bounds/100.0)) / 2) * sin(radAngle)) + ((radius*(bounds/100.0)) * cos(radAngle));

    //return the top left corner of the vehicle
    return lanelet::BasicPoint2d(xPos, yPos);
  }
  else if (mode == Bounds::BottomLeft) {
    //used to contain the x and y position of the vehicle
    xPos = referenceX - (((width*(bounds/100.0)) / 2) * cos(radAngle)) - ((radius*(bounds/100.0)) * sin(radAngle));
    yPos = referenceY - (((width*(bounds/100.0)) / 2) * sin(radAngle)) + ((radius*(bounds/100.0)) * cos(radAngle));

    //return the top left corner of the vehicle
    return lanelet::BasicPoint2d(xPos, yPos);
  }
  else if (mode == Bounds::BottomRight) {
    //used to contain the x and y position of the vehicle
    xPos = referenceX - (((width*(bounds/100.0)) / 2) * cos(radAngle)) + ((radius*(bounds/100.0)) * sin(radAngle));
    yPos = referenceY - (((width*(bounds/100.0)) / 2) * sin(radAngle)) - ((radius*(bounds/100.0)) * cos(radAngle));

    //return the top left corner of the vehicle
    return lanelet::BasicPoint2d(xPos, yPos);
  }
  else if (mode == Bounds::TopRight) {
    //used to contain the x and y position of the vehicle
    xPos = referenceX + (((width*(bounds/100.0)) / 2) * cos(radAngle)) + ((radius*(bounds/100.0)) * sin(radAngle));
    yPos = referenceY + (((width*(bounds/100.0)) / 2) * sin(radAngle)) - ((radius*(bounds/100.0)) * cos(radAngle));

    //return the top left corner of the vehicle
    return lanelet::BasicPoint2d(xPos, yPos);
  } //END IF

  //return error state by default
  return lanelet::BasicPoint2d(-1, -1);
} //END getVehicleBounds

/*
Input:
  const double x - Represents the x position of the vector.
  const double y - Represents the y position of the vector.
Output:
  double vectorAngle - Angle in terms of radians from the x-axes.
Method:
  We get the lanelets angle by taking the position of the vector (x, y) and getting its angle from the origin (0, 0) x-axes.
Formula:
  We can get the angle from the origins x-axes using the following:
    θ = atan2(y, x)
Description:
  Gets the angle of a vector and converts it to yaw radians.
*/
double ExtendedTrafficRules::vectorAngle(const double x, const double y) const {
  //gets the angle of the vector in terms of radians
  double angleInRadians = std::atan2(y, x);

  //convert the circular radians to yaw radians
  if (angleInRadians > PI) {
    //convert radians past PI into yaw radians which are negative past PI in terms of circular radians
    angleInRadians = -(PI*2) + angleInRadians;
  } //END IF

  //returns the angle of the vector using yaw radians
  return angleInRadians;
} //END vectorAngle

/*
Input:
  const std::vector<struct interval> intervalData - Contains all intervals for the given vehicle.
  const double goalPoint - Represents the waypoint for the given vehicle.
  const double epsilonWaypoint - Contains the error margin value for the vehicle-waypoint distance.
  const double targetSpeedValue - Contains the target speed for the given vehicle.
  const double epsilonSpeed - Contains the error margin value for the target speed.
  const SpeedCheckMode mode - Contains the mode for the speed checking. Two modes are present:
    SpeedCheckMode::UpperLimit - The targetSpeedValue is used as a speed limit.
    SpeedCheckMode::TargetLimit - The targetSpeedValue is used as a speed target that's supposed to be within epsilonSpeed units.
Output:
  std::vector<struct violation> goalWayPointWithTargetSpeed - Contains a list of all moments the given rule was violated.
Method:
  Determines if a vehicle is within proximity of a given waypoint and is moving within range of the specified target speed.
Description:
  Checks all given vehicle intervals to determine if a rule violation has occurred.
*/
std::vector<struct violation> ExtendedTrafficRules::goalWayPointWithTargetSpeed(const std::vector<struct interval> intervalData,
                                                                                const lanelet::BasicPoint2d goalPoint, const double epsilonWaypoint,
                                                                                const double targetSpeedValue, const double epsilonSpeed,
                                                                                const SpeedCheckMode mode) const {
                                                                                //END goalWayPointWithTargetSpeed

  //container for the projected coordinate point for the vehicles position
  lanelet::BasicPoint2d vehiclePosition;

  //contains the difference in position between the vehicle and the waypoint
  double distanceDifference;

  //contains the projected vehicle velocity
  double projectedVelocity;

  //contains difference between vehicle and target velocities
  double velocityDifference, magnitudeVelocityDifference;

  //contains a list of all maneuvers
  std::vector<struct violation> waypointWithSpeedMoments;

  //contains a maneuver structure
  struct violation waypointWithSpeedResultsAtMoment;

  //obtains every moment for the given vehicle
  for (size_t i = 0; i < intervalData.size(); i++) {
    //gets the current position for the reference vehicle from x and y coordinate values
    vehiclePosition = lanelet::BasicPoint2d(intervalData[i].xPos, intervalData[i].yPos);

    //gets the current velocity for the reference vehicle from x and y velocity vectors
    projectedVelocity = sqrt(intervalData[i].velocityX * intervalData[i].velocityX +
                             intervalData[i].velocityY * intervalData[i].velocityY);
                             //END sqrt

    //gets the difference in speed between the vehicle and the given target speed
    velocityDifference = targetSpeedValue - projectedVelocity;

    //finds the magnitude of the velocity difference
    magnitudeVelocityDifference = abs(velocityDifference);

    //gets the difference in position between the vehicle and the given target position
    distanceDifference = sqrt(std::pow(vehiclePosition[0] - goalPoint[0], 2) + std::pow(vehiclePosition[1] - goalPoint[1], 2));

    //the vehicle has made it to the waypoint
    if (distanceDifference < epsilonWaypoint) {
      //check if the current speed is greater then the speed limit
      if ((magnitudeVelocityDifference < epsilonSpeed && mode == SpeedCheckMode::TargetLimit) ||
          (velocityDifference + epsilonSpeed < 0 && mode == SpeedCheckMode::UpperLimit)) {
        //create a new container for the violation
        waypointWithSpeedResultsAtMoment = violation();

        //record the violation event data
        waypointWithSpeedResultsAtMoment.rule = Rules::GoalWayPointAndTargetSpeed;
        waypointWithSpeedResultsAtMoment.ruleStatus = true;
        waypointWithSpeedResultsAtMoment.referenceID = intervalData[i].id;
        waypointWithSpeedResultsAtMoment.otherID = false;
        waypointWithSpeedResultsAtMoment.timestamp = intervalData[i].timestamp;

        //add the violation to the list of all violations for speed and waypoints
        waypointWithSpeedMoments.push_back(waypointWithSpeedResultsAtMoment);
      }
      //if the speed is off at the given point return a failure
      else {
        //create a new container for the violation
        waypointWithSpeedResultsAtMoment = violation();

        //record the violation event data
        waypointWithSpeedResultsAtMoment.rule = Rules::GoalWayPointAndTargetSpeed;
        waypointWithSpeedResultsAtMoment.ruleStatus = false;
        waypointWithSpeedResultsAtMoment.referenceID = intervalData[i].id;
        waypointWithSpeedResultsAtMoment.otherID = false;
        waypointWithSpeedResultsAtMoment.timestamp = intervalData[i].timestamp;

        //add the violation to the list of all violations for speed and waypoints
        waypointWithSpeedMoments.push_back(waypointWithSpeedResultsAtMoment);
      } //END IF
    } //END FOR
  } //END FOR

  //return the violations detected for target speed and waypoints
  return waypointWithSpeedMoments;
} //END goalWayPointWithTargetSpeed

/*
Input:
  const std::vector<struct interval> intervalData - Contains all intervals for the given vehicle.
  const std::string mapPath - Contains a string representation for the OSM file path for the dataset.
  const lanelet::BasicPoint2d originPoint - Contains the coordinates of the origin point for the OSM file.
  const double stoppingThreshold - Represents the threshold for the vehicle to be considered stopped before the stop line.
  const double velocityThreshold - Represents the threshold for the vehicle to be considered at rest.
  const double length - Represents the length of the given vehicle.
  const double width - Represents the width of the given vehicle.
  const char * vehicleLocation - Contains the vehicle location two-character array.
Output:
  struct violation stopSignWithNoOffroad - Contains a list of all moments the given rule was violated.
Method:
  Determines if the vehicle has stopped before a stop sign off the road.
Description:
  Checks all given vehicle intervals to determine if a rule violation has occurred.
*/
struct violation ExtendedTrafficRules::stopSignWithNoOffroad(const std::vector<struct interval> intervalData, const std::string mapPath,
                                                             const lanelet::BasicPoint2d originPoint, const double stoppingThreshold,
                                                             const double velocityThreshold, const double length, const double width,
                                                             const char * vehicleLocation, std::vector<int> exception) const {
                                                             //END stopSignWithNoOffroad

  //contains rule violation for stopping while off road
  struct violation stoppingOffroad;

  //contains the moment the vehicle encountered an offroad occurance
  int k;

  //contains the minimum distance between the vehicle and the nearest stopline
  double minDistance;

  //contains the projected velocity of the vehicle
  double projectedVelocity;

  //contans the vehicles corner points in terms of basic points
  lanelet::BasicPoint2d vehiclePositionTopLeft;
  lanelet::BasicPoint2d vehiclePositionTopRight;
  lanelet::BasicPoint2d vehiclePositionBottomRight;
  lanelet::BasicPoint2d vehiclePositionBottomLeft;

  //contains lanelets for the corners of the vehicle
  lanelet::Lanelet regionTopLeft;
  lanelet::Lanelet regionTopRight;
  lanelet::Lanelet regionBottomLeft;
  lanelet::Lanelet regionBottomRight;

  //used to contain reference traffic rules
  lanelet::traffic_rules::TrafficRulesPtr trafficRules;

  //used to contain the routing graph built using the map and traffic rules
  lanelet::routing::RoutingGraphConstPtr routingGraph;

  //contains the open street map file being projected
  lanelet::LaneletMapPtr map;

  //contains the projector for the open street map file
  projection::UtmProjector projector(lanelet::Origin({originPoint[0], originPoint[1]}), true, false);

  //open the map with the referenced origin point
  map = load(mapPath, projector);
  trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(vehicleLocation, Participants::Vehicle);
  routingGraph = lanelet::routing::RoutingGraph::build(*map, *trafficRules);

  //contains information for vehicle stopping at the given intersection
  const violation stopViolations = checkStopAtSign(intervalData, mapPath, originPoint, stoppingThreshold, velocityThreshold, vehicleLocation, StopDirection::MultiStop);

  //contains information for vehicle violations related to driving off-road
  const std::vector<struct violation> offroadViolations = noOffroad(intervalData, length, width, mapPath, originPoint, vehicleLocation, BoundaryValues::RemoveHalf);

  //check if the vehicle stopped successfully
  if (stopViolations.ruleStatus) {
    //search through all off-road violations recorded
    for (size_t i = 0; i < offroadViolations.size(); i++) {
      //check if the vehicle was off-road for the given violation
      if (!offroadViolations[i].ruleStatus) {
        //check if the current timestamp is equal to the stop moment for the given vehicle
        if (offroadViolations[i].timestamp == stopViolations.timestamp && stopViolations.referenceID == offroadViolations[i].referenceID) {
          //record and return that the vehicle stopped off the lanelet map
          return {Rules::StopSignAndNoOffroad, false, stopViolations.referenceID, false, stopViolations.timestamp};
        } //END IF
      } //END IF
    } //END FOR
  } //END IF

  //check if the vehicle failed to stop at the stop sign
  if (stopViolations.ruleStatus == false) {
    //search through all off-road violations recorded
    for (size_t i = 0; i < offroadViolations.size(); i++) {
      //check if the vehicle was off-road for the given violation
      if (!offroadViolations[i].ruleStatus) {
        //contains the current iteration for the vehicle
        k = *offroadViolations[i].iteration;

        //gets the corner points for the vehicle
        vehiclePositionTopLeft = getVehicleBounds(length, intervalData[k].yaw, intervalData[k].xPos, intervalData[k].yPos, Bounds::TopLeft, width, BoundaryValues::Full);
        vehiclePositionTopRight = getVehicleBounds(length, intervalData[k].yaw, intervalData[k].xPos, intervalData[k].yPos, Bounds::TopRight, width, BoundaryValues::Full);
        vehiclePositionBottomRight = getVehicleBounds(length, intervalData[k].yaw, intervalData[k].xPos, intervalData[k].yPos, Bounds::BottomRight, width, BoundaryValues::Full);
        vehiclePositionBottomLeft = getVehicleBounds(length, intervalData[k].yaw, intervalData[k].xPos, intervalData[k].yPos, Bounds::BottomLeft, width, BoundaryValues::Full);

        //determines the vehicles projected velocity at the given moment
        projectedVelocity = std::sqrt(intervalData[k].velocityX * intervalData[k].velocityX + intervalData[k].velocityY * intervalData[k].velocityY);

        //determines the distance between the vehicles corner and a stopline of the nearest lanelet to the corner
        minDistance = getStopLineDist(map, originPoint, vehiclePositionTopLeft, routingGraph, exception);

        //finds the minimum distance among the remaining corners
        std::min(minDistance, getStopLineDist(map, originPoint, vehiclePositionTopRight, routingGraph, exception));
        std::min(minDistance, getStopLineDist(map, originPoint, vehiclePositionBottomRight, routingGraph, exception));
        std::min(minDistance, getStopLineDist(map, originPoint, vehiclePositionBottomLeft, routingGraph, exception));

        //determine if the vehicle stopped near a stop line within threshold for the stop to be registered
        if (minDistance <= stoppingThreshold && projectedVelocity <= velocityThreshold) {
          return {Rules::StopSignAndNoOffroad, false, stopViolations.referenceID, false, intervalData[k].timestamp};
        }
      } //END IF
    } //END FOR
    //return that the vehicle has not stopped at all
    return {Rules::StopSignAndNoOffroad, false, stopViolations.referenceID, false, false};
  } //END IF

  //return that the vehicle has stopped on the road
  return {Rules::StopSignAndNoOffroad, true, stopViolations.referenceID, false, stopViolations.timestamp};
} //END stopSignWithNoOffroad

/*
Input:
  const std::vector<struct interval> intervalData - Contains all intervals for the given vehicle.
  const double length - Represents the length of the given vehicle.
  const double width - Represents the width of the given vehicle.
  const std::string mapPath - Contains a string representation for the OSM file path for the dataset.
  const lanelet::BasicPoint2d originPoint - Contains the coordinates of the origin point for the OSM file.
  const char * vehicleLocation - Contains the vehicle location two-character array.
  const BoundaryValues bounds - Represents the length/width bounds for the vehicle to be evaluated. Has five classes:
    Full = 100 - Percentage of Vehicle length/width to consider from the centerpoint.
    RemoveFrame = 90 - Percentage of Vehicle length/width to consider from the centerpoint.
    RemoveChassis = 80 - Percentage of Vehicle length/width to consider from the centerpoint.
    RemoveHalf = 50 - Percentage of Vehicle length/width to consider from the centerpoint.
    Centre = 0 - Percentage of Vehicle length/width to consider from the centerpoint.
Output:
  std::vector<struct violation> noOffroad - Contains a list of all moments the given rule was violated.
Method:
  Determines if the vehicle has moved off the road by checking if the current vehicle position can be projected into a lanelet layer region for the given map.
Description:
  Checks all given vehicle intervals to determine if a rule violation has occurred.
*/
std::vector<struct violation> ExtendedTrafficRules::noOffroad(const std::vector<struct interval> intervalData, const double length, const double width,
                                                              const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                                                              const char * vehicleLocation, const BoundaryValues bounds) const {
                                                              //END noOffroad

  //used to contain the map pointer generated by the program
  lanelet::LaneletMapPtr map;

  //used to contain reference traffic rules
  lanelet::traffic_rules::TrafficRulesPtr trafficRules;

  //used to contain the routing graph built using the map and traffic rules
  lanelet::routing::RoutingGraphConstPtr routingGraph;

  //contains the projector for the map
  projection::UtmProjector projector(lanelet::Origin({originPoint[0], originPoint[1]}), true, false);

  //container for the projected coordinate point for the vehicles corner position
  lanelet::BasicPoint2d vehiclePositionTopLeft;
  lanelet::BasicPoint2d vehiclePositionTopRight;
  lanelet::BasicPoint2d vehiclePositionBottomRight;
  lanelet::BasicPoint2d vehiclePositionBottomLeft;

  //container for the bounding box containing the vehicle position
  lanelet::BoundingBox2d vehicleAreaTopLeft, vehicleAreaTopRight, vehicleAreaBottomLeft, vehicleAreaBottomRight;

  //container for the regions that the vehicle corners are located within
  lanelet::Lanelets regionTopLeft, regionTopRight, regionBottomLeft, regionBottomRight;

  //determines consecutive moments
  std::vector<int> moments;

  //contains the difference in speed between the speed limit and the vehicles velocity
  double distanceDifference;

  //contains a list of all maneuvers
  std::vector<struct violation> violations;

  //contains a maneuver structure
  struct violation boundaryViolation, bufferOne, bufferTwo;

  //opens the map and gets the routing graph
  map = load(mapPath, projector);
  trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(vehicleLocation, Participants::Vehicle);
  routingGraph = lanelet::routing::RoutingGraph::build(*map, *trafficRules);

  //obtains every moment for the given vehicle
  for (size_t i = 0; i < intervalData.size(); i++) {
    //gets the corner points for the vehicle
    vehiclePositionTopLeft = getVehicleBounds(length, intervalData[i].yaw, intervalData[i].xPos, intervalData[i].yPos, Bounds::TopLeft, width, bounds);
    vehiclePositionTopRight = getVehicleBounds(length, intervalData[i].yaw, intervalData[i].xPos, intervalData[i].yPos, Bounds::TopRight, width, bounds);
    vehiclePositionBottomRight = getVehicleBounds(length, intervalData[i].yaw, intervalData[i].xPos, intervalData[i].yPos, Bounds::BottomRight, width, bounds);
    vehiclePositionBottomLeft = getVehicleBounds(length, intervalData[i].yaw, intervalData[i].xPos, intervalData[i].yPos, Bounds::BottomLeft, width, bounds);

    //creates a bounding box for the vehicles corners
    vehicleAreaTopLeft = lanelet::BoundingBox2d(vehiclePositionTopLeft, vehiclePositionTopLeft);
    vehicleAreaTopRight = lanelet::BoundingBox2d(vehiclePositionTopRight, vehiclePositionTopRight);
    vehicleAreaBottomLeft = lanelet::BoundingBox2d(vehiclePositionBottomRight, vehiclePositionBottomRight);
    vehicleAreaBottomRight = lanelet::BoundingBox2d(vehiclePositionBottomLeft, vehiclePositionBottomLeft);

    //find if the point is located on any lanelet
    lanelet::Lanelets regionTopLeft = map->laneletLayer.search(vehicleAreaTopLeft);
    lanelet::Lanelets regionTopRight = map->laneletLayer.search(vehicleAreaTopRight);
    lanelet::Lanelets regionBottomLeft = map->laneletLayer.search(vehicleAreaBottomLeft);
    lanelet::Lanelets regionBottomRight = map->laneletLayer.search(vehicleAreaBottomRight);

    //check if there are no objects that the vehicle is within
    if (regionTopLeft.empty() || regionTopRight.empty() || regionBottomLeft.empty() || regionBottomRight.empty()) {
      //make a new violation object
      boundaryViolation = violation();

      //fill the violation object
      boundaryViolation.rule = Rules::OffRoad;
      boundaryViolation.referenceID = intervalData[i].id;
      boundaryViolation.ruleStatus = false;
      boundaryViolation.otherID = false;
      boundaryViolation.timestamp = intervalData[i].timestamp;
      boundaryViolation.xPos = intervalData[i].xPos;
      boundaryViolation.yPos = intervalData[i].yPos;
      boundaryViolation.yaw = intervalData[i].yaw;
      boundaryViolation.endTimestamp = false;
      boundaryViolation.magnitude = intersectionSegment(intervalData[i].xPos, intervalData[i].yPos, mapPath, originPoint, vehicleLocation);
      boundaryViolation.iteration = i;

      //append the current violation to the list of boundary violations
      violations.push_back(boundaryViolation);

    } //END IF
  } //END FOR

  //return the boundary violations committed by the vehicle
  return violations;
} //END noOffroad

/*
Input:
  const std::vector<struct interval> intervalData - Contains all intervals for the given vehicle.
  const double referenceLength - Represents the length of the given vehicle.
  const lanelet::BasicPoint2d collisionPoint - Represents the collision point for the vehicle.
  const double epsilon - Contains the error margin for the target threshold.
Output:
  std::vector<struct maneuver> noCollision - Contains a list of all moments the given rule was violated, with a magnitude value for correcting the rule.
Method:
  Determines if a vehicle has collided with a given object based off distance from a collision point.
Description:
  Checks all given vehicle intervals to determine if a rule violation has occurred.
*/
std::vector<struct maneuver> ExtendedTrafficRules::noCollision(const std::vector<struct interval> intervalData, const double referenceLength,
                                                               const lanelet::BasicPoint2d collisionPoint, const double epsilon) const {
                                                               //END noCollision

  //container for the projected coordinate point for the vehicles front and back position
  lanelet::BasicPoint2d vehiclePositionFront, vehiclePositionBack;

  //contains the distance between the vehicle position point and the target point
  double distanceDifferenceFront, distanceDifferenceBack;

  //contains a list of all maneuvers
  std::vector<struct maneuver> maneuverPoints;

  //contains a maneuver structure
  struct maneuver maneuverMoment;

  //contains a direction vector
  struct directionVector vector;

  //check all given vehicle intervals
  for (size_t i = 0; i < intervalData.size(); i++) {
    //get the vehicles front position
    vehiclePositionFront = getVehicleBounds(referenceLength, intervalData[i].yaw, intervalData[i].xPos, intervalData[i].yPos, Front);
    //get the vehicles back position
    vehiclePositionBack = getVehicleBounds(referenceLength, intervalData[i].yaw, intervalData[i].xPos, intervalData[i].yPos, Back);

    //gets the difference in speed between the vehicle and the given target speed
    distanceDifferenceFront = sqrt(std::pow(vehiclePositionFront[0] - collisionPoint[0], 2) + std::pow(vehiclePositionFront[1] - collisionPoint[1], 2));

    //gets the difference in speed between the vehicle and the given target speed
    distanceDifferenceBack = sqrt(std::pow(vehiclePositionBack[0] - collisionPoint[0], 2) + std::pow(vehiclePositionBack[1] - collisionPoint[1], 2));

    //check if the vehicle is within bounds of the collision point
    if (std::min(distanceDifferenceFront, distanceDifferenceBack) <= epsilon) {
      //create a new violation container
      maneuverMoment = maneuver();

      //create a new direction vector
      vector = directionVector();

      //fill the direction vector using the vehicle front as the origin point
      vector.x = collisionPoint[0] - vehiclePositionFront[0];
      vector.y = collisionPoint[1] - vehiclePositionFront[1];
      vector.angle = vectorAngle(vector.x, vector.y);

      //contains rule violation information
      maneuverMoment.rule = Rules::WayPoint;
      maneuverMoment.ruleStatus = false;
      maneuverMoment.referenceID = intervalData[i].id;
      maneuverMoment.timestamp = intervalData[i].timestamp;
      maneuverMoment.magnitude = std::min(distanceDifferenceFront, distanceDifferenceBack);
      maneuverMoment.vector = vector;

      //add the rule violation to the list
      maneuverPoints.push_back(maneuverMoment);
    } //END IF
  } //END FOR

  //return if rules were violated at least once
  return maneuverPoints;
} //END noCollision

/*
Input:
  const std::vector<struct interval> intervalData - Contains all intervals for the given vehicle.
  const double goalPoint - Represents the waypoint for the given vehicle.
  const double epsilon - Contains the error margin for the target threshold.
Output:
  std::vector<struct maneuver> goalWaypoint - Contains a list of all moments the given rule was violated, with a magnitude value for correcting the rule.
Method:
  Compares the vehicles current position to some given target position.
Description:
  Checks all given vehicle intervals to determine if a rule violation has occurred.
*/
std::vector<struct maneuver> ExtendedTrafficRules::goalWaypoint(const std::vector<struct interval> intervalData, const lanelet::BasicPoint2d goalPoint,
                                                                const double epsilon) const {
                                                                //END goalWaypoint

  //container for the projected coordinate point for the vehicles position
  lanelet::BasicPoint2d vehiclePosition;

  //contains the difference in position between the vehicle and the waypoint
  double distanceDifference;

  //contains the projected vehicle velocity
  double projectedVelocity;

  //contains a list of all maneuvers
  std::vector<struct maneuver> maneuverPoints;

  //contains a maneuver structure
  struct maneuver maneuverMoment;

  //contains a direction vector
  struct directionVector vector;

  //obtains every moment for the given vehicle
  for (size_t i = 0; i < intervalData.size(); i++) {
    //gets the current position for the reference vehicle from x and y coordinate values
    vehiclePosition = lanelet::BasicPoint2d(intervalData[i].xPos, intervalData[i].yPos);

    //gets the current velocity for the reference vehicle from x and y velocity vectors
    projectedVelocity = sqrt(intervalData[i].velocityX * intervalData[i].velocityX +
                             intervalData[i].velocityY * intervalData[i].velocityY);
                             //END sqrt

    //gets the difference in position between the vehicle and the given target position
    distanceDifference = sqrt(std::pow(vehiclePosition[0] - goalPoint[0], 2) + std::pow(vehiclePosition[1] - goalPoint[1], 2));

    //check if the current distance is greater then the epsilon limit
    if (distanceDifference >= epsilon) {
      //create a new violation container
      maneuverMoment = maneuver();

      //create a new direction vector
      vector = directionVector();

      //fill the direction vector using the vehicle as the origin point
      vector.x = goalPoint[0] - vehiclePosition[0];
      vector.y = goalPoint[1] - vehiclePosition[1];
      vector.angle = vectorAngle(vector.x, vector.y);

      //contains rule violation information
      maneuverMoment.rule = Rules::WayPoint;
      maneuverMoment.ruleStatus = false;
      maneuverMoment.referenceID = intervalData[i].id;
      maneuverMoment.timestamp = intervalData[i].timestamp;
      maneuverMoment.magnitude = distanceDifference / projectedVelocity;
      maneuverMoment.vector = vector;

      //add the rule violation to the list
      maneuverPoints.push_back(maneuverMoment);
    }
    //the vehicle has made it to the waypoint
    else if (distanceDifference < epsilon) {
      //create a new violation container
      maneuverMoment = maneuver();

      //clear the container for the previous maneuvers
      maneuverPoints.clear();

      //contains rule violation information
      maneuverMoment.rule = Rules::WayPoint;
      maneuverMoment.ruleStatus = true;
      maneuverMoment.referenceID = intervalData[i].id;
      maneuverMoment.timestamp = intervalData[i].timestamp;
      maneuverMoment.magnitude = distanceDifference / projectedVelocity;

      //add the current maneuver
      maneuverPoints.push_back(maneuverMoment);

      //return current data for the rule status, the vehicle ID, and momentary information
      return maneuverPoints;
    } //END IF
  } //END FOR

  //return if rules were violated at least once
  return maneuverPoints;
} //END goalWaypoint

/*
Input:
  const std::vector<struct interval> intervalData - Contains all intervals for the given vehicle.
  const double target - Represents the target speed for the given vehicle.
  const double epsilon - Contains the error margin for the target threshold.
  const SpeedCheckMode mode - Contains the mode for the speed checking. Two modes are present:
    SpeedCheckMode::UpperLimit - The targetSpeedValue is used as a speed limit.
    SpeedCheckMode::TargetLimit - The targetSpeedValue is used as a speed target that's supposed to be within epsilonSpeed units.
Output:
  std::vector<struct maneuver> targetSpeed - Contains a list of all moments the given rule was violated, with a magnitude value for correcting the rule.
Method:
  Compares the vehicles current speed to a given target speed.
Description:
  Checks all given vehicle intervals to determine if a rule violation has occurred.
*/
std::vector<struct maneuver> ExtendedTrafficRules::targetSpeed(const std::vector<struct interval> intervalData, const double target,
                                                               const double epsilon, const SpeedCheckMode mode) const {
                                                               //END targetSpeed

  //container for the projected velocity for the vehicle over time
  double projectedVelocity;

  //contains the difference in speed between the speed limit and the vehicles velocity
  double velocityDifference, magnitudeVelocityDifference;

  //contains a list of all maneuvers
  std::vector<struct maneuver> maneuverPoints;

  //contains a maneuver structure
  struct maneuver maneuverMoment;

  //contains the standard vehicle acceleration in terms of m/s^2
  double vehicleAcc = 5.36448;

  //contains the standard vehicle breaking acceleration in terms of m/s^2
  double breakingAcc = 6.5704555; //0.67g

  //obtains every moment for the given vehicle
  for (size_t i = 0; i < intervalData.size(); i++) {
    //gets the current velocity for the reference vehicle from x and y velocity vectors
    projectedVelocity = sqrt(intervalData[i].velocityX * intervalData[i].velocityX +
                             intervalData[i].velocityY * intervalData[i].velocityY);
                             //END sqrt

    //gets the difference in speed between the vehicle and the given target speed
    velocityDifference = target - projectedVelocity;

    //finds the magnitude of the velocity difference
    magnitudeVelocityDifference = abs(velocityDifference);

    //check if the current speed is greater then the speed limit
    if ((magnitudeVelocityDifference < epsilon && mode == SpeedCheckMode::TargetLimit) ||
        (velocityDifference + epsilon < 0 && mode == SpeedCheckMode::UpperLimit)) {
      //create a new violation container
      maneuverMoment = maneuver();

      //check if we need to slow down or speed up
      if (velocityDifference > 0) {
        //we must slow down since we are going over the speed limit
        maneuverMoment.magnitude = magnitudeVelocityDifference / breakingAcc;
      }
      else if (velocityDifference < 0) {
        //we must speed up since we are going under the speed limit
        maneuverMoment.magnitude = magnitudeVelocityDifference / vehicleAcc;
      } //END IF

      //contains rule violation information
      maneuverMoment.rule = Rules::TargetSpeed;
      maneuverMoment.ruleStatus = false;
      maneuverMoment.referenceID = intervalData[i].id;
      maneuverMoment.timestamp = intervalData[i].timestamp;
      maneuverMoment.magnitude = velocityDifference;

      //add the rule violation to the list
      maneuverPoints.push_back(maneuverMoment);
    }
    else {
      //create a new violation container
      maneuverMoment = maneuver();

      //contains rule violation information
      maneuverMoment.rule = Rules::TargetSpeed;
      maneuverMoment.ruleStatus = true;
      maneuverMoment.referenceID = intervalData[i].id;
      maneuverMoment.timestamp = intervalData[i].timestamp;

      //add the rule violation to the list
      maneuverPoints.push_back(maneuverMoment);
    } //END IF
  } //END FOR

  //return if rules were violated at least once
  return maneuverPoints;
} //END targetSpeed

/*
Input:
  const std::vector<struct interval> intervalData - Contains all intervals for the given vehicle.
  const std::string mapPath - Contains a string representation for the OSM file path for the dataset.
  const lanelet::BasicPoint2d originPoint - Contains the coordinates of the origin point for the OSM file.
  const double epsilon - Contains the error margin for the target threshold.
  const char * vehicleLocation - Contains a two character string representing the nation that the vehicle is driving within.
  const double velocityThreshold - Represents the velocity threshold to be considered at motion.
Output:
  std::vector<struct violation> checkSpeedLimit - Contains a list of all moments the given rule was violated.
Method:
  Compares the speed limit set by the currently occupied lanelet with the vehicles projected speed.
  Vehicle speeding is only checked against the total vehicle-in-motion time for a driver.
    Please see equation (1) from the following paper: https://www.sciencedirect.com/science/article/pii/S0001457521002980
Description:
  Checks all given vehicle intervals to determine if a rule violation has occurred.
*/
std::tuple<std::vector<struct maneuver>, double, double> ExtendedTrafficRules::checkSpeedLimit(const std::vector<struct interval> intervalData,
                                                                   const std::string mapPath, const lanelet::BasicPoint2d originPoint, const double epsilon,
                                                                   const char * vehicleLocation, const double velocityThreshold) const {
                                                                   //END checkSpeedLimit

  //used to contain the map pointer generated by the program
  lanelet::LaneletMapPtr map;

  //gets the lanelet that the ego vehicle ends at
  lanelet::ConstLanelet currentLanelet;

  //used to contain reference traffic rules
  lanelet::traffic_rules::TrafficRulesPtr trafficRules;

  //used to contain the routing graph built using the map and traffic rules
  lanelet::routing::RoutingGraphConstPtr routingGraph;

  //contains the projector for the map
  projection::UtmProjector projector(lanelet::Origin({originPoint[0], originPoint[1]}), true, false);

  //container for the projected velocity for the vehicle over time
  double projectedVelocity;

  //contains the difference in speed between the speed limit and the vehicles velocity
  double velocityDifference;

  //contains a list of all violations
  std::vector<struct maneuver> speedViolations;

  //contains a violation structure
  struct maneuver speedViolation;

  //used to contain the lanelet speed limit information
  Optional<SpeedLimitInformation> speedLimitInfo;

  //contains the maximum speed in terms of miles per second
  double maxSpeedMetersPerSecond;

  //contains the number of time intervals where the vehicle was in motion
  int motionIntervals = 0;

  //contains the amount of time in ms an interval represents
  int intervalDuration = 100;

  //opens the map and gets the routing graph
  map = load(mapPath, projector);
  trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(vehicleLocation, Participants::Vehicle);
  routingGraph = lanelet::routing::RoutingGraph::build(*map, *trafficRules);

  //obtains every moment for the given vehicle
  for (size_t i = 0; i < intervalData.size(); i++) {
    //gets the current lanelet that the vehicle is on
    currentLanelet = getLanelet(mapPath, originPoint, BasicPoint2d(intervalData[i].xPos, intervalData[i].yPos));

    //gets the speed limit for the current lanelet by referencing the traffic rules
    speedLimitInfo = speedLimit(currentLanelet);

    //get the speed in terms of miles per second
    maxSpeedMetersPerSecond = (*speedLimitInfo).speedLimit.value();

    //gets the current velocity for the reference vehicle from x and y velocity vectors
    projectedVelocity = sqrt(intervalData[i].velocityX * intervalData[i].velocityX +
                             intervalData[i].velocityY * intervalData[i].velocityY);
                             //END sqrt

    //check if the vehicle is currently moving
    if (!inStandstill(projectedVelocity, velocityThreshold)) {
      //record the current interval as being an interval where the vehicle was in motion
      motionIntervals += 1;

      //check if the current speed is greater then the speed limit
      if (maxSpeedMetersPerSecond - projectedVelocity + epsilon < 0) {
        //create a new violation container
        speedViolation = maneuver();

        //contains rule violation information
        speedViolation.rule = Rules::SpeedLimit;
        speedViolation.ruleStatus = false;
        speedViolation.referenceID = intervalData[i].id;
        speedViolation.timestamp = intervalData[i].timestamp;
        speedViolation.magnitude = maxSpeedMetersPerSecond * 3.6;

        //add the rule violation to the list
        speedViolations.push_back(speedViolation);
      } //END IF
    } //END IF
  } //END FOR

  //return if rules were violated at least once
  return std::make_tuple(speedViolations, motionIntervals*intervalDuration, maxSpeedMetersPerSecond * 3.6);
} //END checkSpeedLimit

/*
Input:
  const lanelet::ConstLanelet & lanelet - Gets the lanelet we wish to build a vector from.
Output:
  std::vector<double> getLaneletDirectionVector - Returns a vector of doubles.
Method:
  We get the lanelets direction and angle by taking the front and back positions of the lanelet.
Description:
  Gets a vector representing the lanelets direction and angle.
*/
struct directionVector ExtendedTrafficRules::getLaneletDirectionVector(const lanelet::ConstLanelet & lanelet) const {
  //define a container for the lanelet vector
  struct directionVector laneletVector;

  //gets the right line for the lanelet, which is always pointing in the forward direction for the lanelets traffic flow
  lanelet::ConstLineString3d rightLine = lanelet.rightBound();

  //gets the front and back points for the lanelet, which are at the front and back of the lanelets traffic flow directions respectively
  lanelet::ConstPoint2d frontPoint = lanelet::utils::to2D(rightLine.front());
  lanelet::ConstPoint2d backPoint = lanelet::utils::to2D(rightLine.back());

  //gets the vector for the lanelet between the front and back point
  laneletVector.x = backPoint.x() - frontPoint.x();
  laneletVector.y = backPoint.y() - frontPoint.y();

  //gets the angle for the vector
  laneletVector.angle = vectorAngle(laneletVector.x, laneletVector.y);

  //return the vector
  return laneletVector;
} //END getLaneletDirectionVector

/*
Input:
  const double xPos - Contains the current X-coordinate position of the vehicle.
  const double yPos - Contains the current Y-coordinate position of the vehicle.
  const std::string mapPath - Contains the current path for the OSM map representation for the road the vehicle is driving within.
  const lanelet::BasicPoint2d originPoint - Contains the origin point for the current vehicle.
  const char * vehicleLocation - Contains the two-character string representation for the nation that the vehicle is driving within.
Output:
  LaneletPosition - Determines the location within the lanelet that the vehicle is driving within.
Description:
  Determines if a vehicle is within an intersection within a given OSM map.
*/
LaneletPosition ExtendedTrafficRules::intersectionSegment(const double xPos, const double yPos, const std::string mapPath,
                                                          const lanelet::BasicPoint2d originPoint, const char * vehicleLocation) const {
                                                          //END intersectionSegment
  //containers for regulatory elements
  std::vector<AllWayStop::Ptr> allWayStopRegelems;
  std::vector<RightOfWay::Ptr> rightOfWayRegelems;

  //contains the lanelets that the ego and other vehicle are on
  lanelet::Lanelet currentLanelet;

  //contains the current point that the vehicle is on
  lanelet::BasicPoint2d currentPoint;

  //used to contain reference traffic rules
  lanelet::traffic_rules::TrafficRulesPtr trafficRules;

  //used to contain the routing graph built using the map and traffic rules
  lanelet::routing::RoutingGraphConstPtr routingGraph;

  //used to contain the map pointer generated by the program
  lanelet::LaneletMapPtr map;

  //contains the projector for the map
  projection::UtmProjector projector(lanelet::Origin({originPoint[0], originPoint[1]}), true, false);

  //contains the following lanelets to the current lanelet
  lanelet::ConstLanelets followingLanelets, previousLanelets, yieldingLanelets;

  //opens the map and gets the routing graph
  map = load(mapPath, projector);
  trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(vehicleLocation, Participants::Vehicle);
  routingGraph = lanelet::routing::RoutingGraph::build(*map, *trafficRules);

  //get the current vehicles position
  currentPoint = lanelet::BasicPoint2d(xPos, yPos);

  //get the lanelet that the vehicle is on
  currentLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, currentPoint, 1)[0]);

  //get data for the all way stop representing the stop sign
  allWayStopRegelems = currentLanelet.regulatoryElementsAs<AllWayStop>();

  //check if there is a right of way element
  rightOfWayRegelems = currentLanelet.regulatoryElementsAs<RightOfWay>();

  //checks the following lanelets within the routing graph
  followingLanelets = routingGraph->following(currentLanelet);

  //if stop sign regulation is present on the given lanelet
  if (!rightOfWayRegelems.empty()) {
    //check if the current lanelet has the right of way over the other lanelets
    if (rightOfWayRegelems[0]->getManeuver(currentLanelet) == lanelet::ManeuverType::Yield) {
      //return that the vehicle is currently within an intersectional entry
      return LaneletPosition::IntersectionEntry;
    } //END IF
  }
  else if (!allWayStopRegelems.empty()) {
    //return that the vehicle is currently within an intersectional entry
    return LaneletPosition::IntersectionEntry;
  } //END IF

  //obtain the next lanelets within the intersection
  for (size_t i = 0; i < followingLanelets.size(); i++) {
    //if stop sign regulation is present on the given lanelet
    if (!followingLanelets[i].regulatoryElementsAs<RightOfWay>().empty()) {
      //check if the current lanelet has the right of way over the other lanelets
      if (followingLanelets[i].regulatoryElementsAs<RightOfWay>()[0]->getManeuver(currentLanelet) == lanelet::ManeuverType::Yield) {
        //return that the vehicle is currently within an intersectional entry
        return LaneletPosition::IntersectionEntry;
      } //END IF
    }
    else if (!followingLanelets[i].regulatoryElementsAs<AllWayStop>().empty()) {
      //return that the vehicle is currently within an intersectional entry
      return LaneletPosition::IntersectionEntry;
    } //END IF
  } //END FOR

  //checks the previous lanelets within the routing graph
  previousLanelets = routingGraph->previous(currentLanelet);

  //obtain the next lanelets within the intersection
  for (size_t i = 0; i < previousLanelets.size(); i++) {
    //if stop sign regulation is present on the given lanelet
    if (!previousLanelets[i].regulatoryElementsAs<RightOfWay>().empty()) {
      //check if the current lanelet has the right of way over the other lanelets
      if (previousLanelets[i].regulatoryElementsAs<RightOfWay>()[0]->getManeuver(currentLanelet) == lanelet::ManeuverType::Yield) {
        //return that the vehicle is currently within an intersectional entry
        return LaneletPosition::IntersectionInterior;
      } //END IF
    }
    else if (!previousLanelets[i].regulatoryElementsAs<AllWayStop>().empty()) {
      //return that the vehicle is currently within an intersectional entry
      return LaneletPosition::IntersectionInterior;
    } //END IF
  } //END FOR

  //return the point is on a road
  return LaneletPosition::RoadLane;
} //END intersectionSegment

/*
Input:
  const lanelet::LaneletMapPtr map - Contains the pointer representation for the current OSM object.
  const lanelet::BasicPoint2d originPoint - Contains the origin point for the current OSM.
  const lanelet::routing::RoutingGraphConstPtr routingGraph - Contains the routing graph for the current OSM.
  const std::vector<interval> moments - contains a set of moment for a given vehicle.
  const int startMoment - Contains the starting moment in question for when the vehicle arrives at an intersection.
  const double velocityThreshold - Determines the velocity threshold for the given vehicle to be considered at motion.
  const double stoppingThreshold - Determines the stopping threshold for the given vehicle to be considered stopping before a stop sign.
Output:
  Determines the angle of the lanelet at the end of the intersection for the given vehicle.
Description:
  Finds the geometric angle at the end of the intersection for the given vehicle upon leaving the intersection.
*/
double ExtendedTrafficRules::endAngleFinder(const lanelet::LaneletMapPtr map, const lanelet::BasicPoint2d originPoint,
                                            const lanelet::routing::RoutingGraphConstPtr routingGraph,
                                            const std::vector<interval> moments, const int startMoment,
                                            const double velocityThreshold, const double stoppingThreshold) const {
                                            //END endAngleFinder

  //contains the projected velocity of the vehicle
  double projectedVelocity;

  //contains the lanelet ID of the lanelet that the vehicle stopped on
  int currentLaneletID;

  //contains the current point of the vehicle
  lanelet::BasicPoint2d currentPoint, otherPoint;

  //contains the ego lanelet for the vehicle
  lanelet::Lanelet egoLanelet;

  //containers for regulatory elements
  std::vector<AllWayStop::Ptr> allWayStopRegelems;
  std::vector<RightOfWay::Ptr> rightOfWayRegelems;

  //contains the last yaw recorded by the vehicle
  double lastYaw = Default::None;

  //for every moment the vehicle has on record
  for (size_t i = 0; i < moments.size(); i++) {
    //gets the momentary point for the vehicle on the map
    otherPoint = BasicPoint2d(moments[i].xPos, moments[i].yPos);

    //gets the current velocity for the reference vehicle from x and y velocity vectors
    projectedVelocity = sqrt(moments[i].velocityX * moments[i].velocityX + moments[i].velocityY * moments[i].velocityY);

    //get the current yaw angle at the given moment
    lastYaw = moments[i].yaw;

    //checks to find if the current moment is the moment the vehicle stopped
    if (startMoment == moments[i].timestamp) {
      //gets the current point for the vehicle on the map
      currentPoint = BasicPoint2d(moments[i].xPos, moments[i].yPos);

      //finds the ID of the current lanelet
      currentLaneletID = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, currentPoint, 1)[0]).id();
    } //END IF

    //check future intervals past the stopping moment
    if (startMoment < moments[i].timestamp &&
         !sameIntersection(map, originPoint, routingGraph, currentPoint, otherPoint) &&
         currentLaneletID != std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, otherPoint, 1)[0]).id() && (
         !inStandstill(projectedVelocity, velocityThreshold) ||
         getStopLineDist(map, originPoint, currentPoint, routingGraph) > stoppingThreshold)) {

      //determines the lanelet the vehicle is currently on
      egoLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, currentPoint, 1)[0]);

      //get traffic sign regulatory elements
      rightOfWayRegelems = egoLanelet.regulatoryElementsAs<RightOfWay>();
      allWayStopRegelems = egoLanelet.regulatoryElementsAs<AllWayStop>();

      //check if the lanelet is not part of the intersection
      if (allWayStopRegelems.empty() && rightOfWayRegelems.empty()) {
        return getLaneletDirectionVector(egoLanelet).angle;
      } //END IF
    } //END IF
  } //END FOR

  //return the last reported yaw angle as the ending yaw angle at the end of the intersection
  return lastYaw;
} //END endAngleFinder

/*
Input:
  const lanelet::LaneletMapPtr map - Represents the map pointer for the given OSM representation of the driving environment.
  const lanelet::BasicPoint2d originPoint - Represents the origin point for the given OSM file.
  const lanelet::routing::RoutingGraphConstPtr routingGraph - Contains a routing graph for the given OSM driving environment.
  const lanelet::BasicPoint2d currentPoint - Contains the current point for the given reference vehicle.
  const lanelet::BasicPoint2d otherPoint - Contains the current point for the other vehicle.
Output:
  bool - Represents if two points lie on the same intersection of Right-Of-Way Yield segments
Method:
  Iterates through all valid intersectional and right-of-way lanelets for the ego vehicle and checks if the other vehicles given lanelet is contained within.
Description:
  Determines if two vehicles are along the same intersection.
*/
bool ExtendedTrafficRules::sameIntersection(const lanelet::LaneletMapPtr map, const lanelet::BasicPoint2d originPoint,
                                            const lanelet::routing::RoutingGraphConstPtr routingGraph,
                                            const lanelet::BasicPoint2d currentPoint, const lanelet::BasicPoint2d otherPoint) const {
                                            //END sameIntersection
  //containers for regulatory elements
  std::vector<AllWayStop::Ptr> allWayStopRegelems;
  std::vector<RightOfWay::Ptr> rightOfWayRegelems;

  //contains a list of all valid lanelets
  std::vector<int> yieldLanelets;

  //contains a list of all intersectional lanelets
  std::vector<int> intersectionalLanelets;

  //contains the lanelets that the ego and other vehicle are on
  lanelet::Lanelet egoLanelet, otherLanelet;

  //contains the lanelets along a given intersection
  lanelet::Lanelets intersectionalYieldingLanelets;

  //get the lanelet that the vehicle is on
  egoLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, currentPoint, 1)[0]);

  //get the lanelet that the vehicle is on
  otherLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, otherPoint, 1)[0]);

  //get data for the all way stop representing the stop sign
  allWayStopRegelems = egoLanelet.regulatoryElementsAs<AllWayStop>();

  //check if there is a right of way element
  rightOfWayRegelems = egoLanelet.regulatoryElementsAs<RightOfWay>();

  //contains the next lanelets in the route
  std::vector<lanelet::ConstLanelet> nextLanelets;

  //if an all way stop regulatory element is present
  if (!allWayStopRegelems.empty()) {
    //for every all way stop element
    for (size_t i = 0; i < allWayStopRegelems.size(); i++) {
      //obtain all the lanes along the intersection
      intersectionalYieldingLanelets = allWayStopRegelems[i]->lanelets();
      //for every lanelet along the intersection
      for (size_t k = 0; k < intersectionalYieldingLanelets.size(); k++) {
        //gets the yield lanelets from the intersection
        yieldLanelets.push_back(allWayStopRegelems[i]->lanelets()[k].id());

        //get the next lanelets in the route
        nextLanelets = routingGraph->following(allWayStopRegelems[i]->lanelets()[k]);

        //obtain the next lanelets within the intersection
        for (size_t j = 0; j < nextLanelets.size(); j++) {
          intersectionalLanelets.push_back(nextLanelets[j].id());
        } //END FOR
      } //END FOR
    } //END FOR
  } //END IF

  //check if there are elements inside
  if (!rightOfWayRegelems.empty()) {
    //for every all way stop element
    for (size_t i = 0; i < rightOfWayRegelems.size(); i++) {
      //obtain all the lanes along the intersection
      intersectionalYieldingLanelets = rightOfWayRegelems[i]->yieldLanelets();
      //for every lanelet along the intersection
      for (size_t k = 0; k < intersectionalYieldingLanelets.size(); k++) {
        //gets the yield lanelets from the intersection
        yieldLanelets.push_back(rightOfWayRegelems[i]->yieldLanelets()[k].id());

        //get the next lanelets in the route
        nextLanelets = routingGraph->following(rightOfWayRegelems[i]->yieldLanelets()[k]);

        //obtain the next lanelets within the intersection
        for (size_t j = 0; j < nextLanelets.size(); j++) {
          intersectionalLanelets.push_back(nextLanelets[j].id());
        } //END FOR
      } //END FOR
    } //END FOR
  } //END IF

  //check if any lanelets along the ego vehicles intersection contain the other vehicles lanelet ID value
  if (std::find(yieldLanelets.begin(), yieldLanelets.end(), otherLanelet.id()) != yieldLanelets.end() ||
      std::find(intersectionalLanelets.begin(), intersectionalLanelets.end(), otherLanelet.id()) != intersectionalLanelets.end()) {
    //return that both vehicles are along the same intersection
    return true;
  } //END IF

  //return that none of the lanelets along the ego vehicles intersection contain the other vehicles lanelet
  return false;
} //END sameIntersection

/*
Input:
  const std::vector<struct vehicles> vehicleData - Contains data for all the vehicles in the given interaction dataset.
  const struct vehicles ego - Contains data for the ego vehicle.
  const std::string mapPath - Contains a string representation for the OSM file path for the dataset.
  const lanelet::BasicPoint2d originPoint - Contains the coordinates of the origin point for the OSM file.
  const double stoppingThreshold - Contains the threshold for a vehicle to be considered stopped before the stop line.
  const double velocityThreshold - Contains the threshold for a vehicle to be considered as not moving.
  const char * vehicleLocation - Contains the vehicle location two-character array.
  const YawChecking yawCheck - Determines the type of yaw angle checking to use for the vehicle:
    IntersectionProjection - Finds the end yaw of the vehicle once it leaves the intersectional or yield lanelets and exits the following lanelets.
                             Problems can arise if Lanelet2 pinpoints the vehicle as being nearest to the wrong lanelet.
    Dataset - Finds the end yaw of the vehicle using the final reported yaw from the interaction datasets momentary intervals.
              Assumes that the interval represents the movement of a vehicle across some specific lanelet.
              If multiple intersections are covered by the interval, then the method will not work correctly.
              It is a lot faster and more accurate to use the Dataset method if possible.
Output:
  const struct violation checkLeftBeforeRight - Return a struct containing information about:
      Rules rule - The rule that was evaluated.
      bool ruleStatus - Whether the rule was broken or not.
      int referenceID - The ID of the reference vehicle being checked.
      boost::optional<int> otherID - The ID of the other vehicle involved if the rule was violated, if applicable.
      boost::optional<int> timestamp - The moment in time the rule was violated or passed, if applicable.
Method:
  We take a reference vehicle and compare it to all other vehicles during the time interval that it is in standstill.
  If any of the other vehicles are stopped during the interval at the correct lanelets along the intersection, we evaluate the rules.
  The rules show that if two vehicles stop at the same time along the intersection, then the vehicle that should stay stopped must meet (R-IN3).
Formulation:
  R-IN3 is formulated in Formalization of Intersection Traffic Rules in Temporal Logic:
    https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
  R-IN3:
    We notice the formulation is as follows:
      G((on_incoming_left_of(x_ego, x_o) ∧ ¬relevant_traffic_light(x_ego) ∧
        (turning_right(x_ego) ∧ turning_right(x_o) ∧ same_priority(x_ego, x_o, right, right)
            --- we skip some combinations of turning directions here ---
        ∨ turning_left(x_ego) ∧ going_straight(x_o) ∧ same_priority(x_ego, x_o, left, straight)))
          ==> (G(not_endanger_intersection(x_ego, x_o)) ∨ ¬(on_lanelet_with_type(x_ego, intersection)))
Description:
  Checks if the ego vehicle is incoming left of another vehicle and their paths are crossing an intersection.
  The ego vehicle is only allowed to enter the intersection if it does not endanger the other vehicle.
  Right before left does not apply if traffic lights regulate the intersection.
*/
const struct violation ExtendedTrafficRules::checkLeftBeforeRight(const std::vector<struct vehicles> vehicleData, const struct vehicles ego,
                                                                  const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                                                                  const double stoppingThreshold, const double velocityThreshold,
                                                                  const char * vehicleLocation, const YawChecking yawCheck) const {
                                                                  //END checkLeftBeforeRight
  //used to contain the velocity of the vehicle from x and y velocity vectors
  double projectedVelocity, projectedVelocityTwo;

  //contains the difference between vehicle orientations
  double difference;

  //contains angles for lanelet vectors
  double egoLaneletAngle, otherLaneletAngle, otherLaneletAngleEnd, egoLaneletAngleEnd;

  //used to contain stop switches
  bool intervalStart = false;
  bool hasStoppedAndStarted = false;
  bool stopSignResponse = false;
  bool needToStop = false;
  bool hasPassed = false;
  bool priorityStop = false;
  bool leftTurnStop = false;
  bool oncomingLeftStop = false;
  bool skip = true;
  bool blocked = false; //states that the current stop line is blocked
  bool mustYield = false;
  bool onIncomingLeftOf;
  bool sPriority;
  bool relevantTrafficLights;

  //contains traffic light data for both lanelets
  struct stopLineData trafficLightDataEgo, trafficLightDataOther;

  //the starting and ending interval for a stop
  int startTime, endTime;

  //contains final recorded data
  boost::optional<int> finalInterval, finalOtherID;

  //contains the ID for the vehicle that has passed the stop line
  int id = Default::None;

  //contains both vehicle points on the map
  lanelet::BasicPoint2d currentPoint, otherPoint, centerPoint, otherCenterPoint;

  //used to contain the turn types for the vehicles
  Direction otherTurn, egoTurn;

  //used to contain the turn direction for the completed turn
  boost::optional<double> direction = false;

  //used to contain the lanelets for both vehicles
  lanelet::Lanelet egoLanelet, otherLanelet;

  //used to contain reference traffic rules
  lanelet::traffic_rules::TrafficRulesPtr trafficRules;

  //used to contain the routing graph built using the map and traffic rules
  lanelet::routing::RoutingGraphConstPtr routingGraph;

  //used to contain the map pointer generated by the program
  lanelet::LaneletMapPtr map;

  //contains the projector for the map
  projection::UtmProjector projector(lanelet::Origin({originPoint[0], originPoint[1]}), true, false);

  //opens the map and gets the routing graph
  map = load(mapPath, projector);
  trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(vehicleLocation, Participants::Vehicle);
  routingGraph = lanelet::routing::RoutingGraph::build(*map, *trafficRules);

  //contains a list of vehicles which hold the right-of-way over the currently iterated vehicle
  std::vector<int> vehicles;

  //contains the id of the given lanelet
  int laneletID = Default::None;
  int currentID;

  //contains the vehicles position upon completing a stop-regulated turn
  boost::optional<double> xPos = false;
  boost::optional<double> yPos = false;
  boost::optional<double> yaw = false;

  //contains the final duration recorded for the vehicle
  boost::optional<int> intervalDuration = false;

  //contains the final distance to the stop line for the recorded vehicle
  boost::optional<double> finalDistance = false;

  //contains the results of the rule violation check
  struct violation results = violation();

  //for every entry in the interval with the reference vehicle
  for (size_t i = 0; i < ego.intervals.size(); i++) {
    //gets the current point for the vehicle on the map
    centerPoint = BasicPoint2d(ego.intervals[i].xPos, ego.intervals[i].yPos);
    currentPoint = getVehicleBounds(ego.length, ego.intervals[i].yaw, centerPoint[0], centerPoint[1], Front);

    //gets the ID of the current lanelet
    currentID = getCurrentLanelet(mapPath, originPoint, currentPoint);

    //gets the current velocity for the reference vehicle from x and y velocity vectors
    projectedVelocity = sqrt(ego.intervals[i].velocityX * ego.intervals[i].velocityX + ego.intervals[i].velocityY * ego.intervals[i].velocityY);

    //if the vehicle has stopped within a threshold and the vehicle was moving at the previous moment
    if (inStandstill(projectedVelocity, velocityThreshold) && !intervalStart) {
      //flag the interval as the start of the vehicles stopping period
      intervalStart = true;

      //set the current moment to be the start time of the stop
      startTime = ego.intervals[i].timestamp;
    }
    //if the vehicle has not stopped and the vehicle was not moving at the previous moment
    else if (!inStandstill(projectedVelocity, velocityThreshold) && intervalStart) {
      //flag the interval as the end of the vehicles stopping period
      intervalStart = false;

      //state that the vehicle has stopped and has started moving again meaning we should check for rule violations
      hasStoppedAndStarted = true;

      //set the moment to be the end time of the stop
      endTime = ego.intervals[i].timestamp;
    } //END IF

    //if we are at the start of a stopping interval and the distance between the reference vehicle and the stop line is within a specific threshold
    if (intervalStart &&
       (getStopLineDist(map, originPoint, currentPoint, routingGraph) <= stoppingThreshold ||
        getStopLineDist(map, originPoint, centerPoint, routingGraph) <= stoppingThreshold) &&
        currentID != laneletID) {
      //clear the list of vehicles with the right of way
      vehicles.clear();

      //flag that we need to stop the vehicle
      needToStop = true;

      //obtain the IDs of all vehicles that are within a stop line
      for (size_t j = 0; j < vehicleData.size(); j++) {
        for (size_t k = 0; k < vehicleData[j].intervals.size(); k++) {
          //gets the position of the other vehicle on the reference map
          otherCenterPoint = BasicPoint2d(vehicleData[j].intervals[k].xPos, vehicleData[j].intervals[k].yPos);
          otherPoint = getVehicleBounds(vehicleData[j].length, vehicleData[j].intervals[k].yaw, otherCenterPoint[0], otherCenterPoint[1], Front);

          //obtain the projected velocity of the other vehicle from its component x and y velocity vectors
          projectedVelocityTwo = sqrt(vehicleData[j].intervals[k].velocityX*vehicleData[j].intervals[k].velocityX +
                                      vehicleData[j].intervals[k].velocityY*vehicleData[j].intervals[k].velocityY);
                                      //END sqrt

          //if the other vehicle is at the stop line and stopped at the currently iterated moment
          if (ego.intervals[i].id != vehicleData[j].intervals[k].id &&
              vehicleData[j].intervals[k].timestamp == startTime &&
              (getStopLineDist(map, originPoint, otherPoint, routingGraph) <= stoppingThreshold ||
               getStopLineDist(map, originPoint, otherCenterPoint, routingGraph) <= stoppingThreshold) &&
              sameIntersection(map, originPoint, routingGraph, centerPoint, otherCenterPoint)) {

              //sets a new ID for the current lanelet
              laneletID = getCurrentLanelet(mapPath, originPoint, currentPoint);

              //gets the reference and other lanelet the vehicles are currently within
              egoLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, centerPoint, 1)[0]);
              otherLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, otherCenterPoint, 1)[0]);

              //get lanelet turn angles based on current lanelet geometry at the entrance of the intersection
              egoLaneletAngle = ego.intervals[i].yaw;
              otherLaneletAngle = vehicleData[j].intervals[k].yaw;

              //get lanelet turn angles based on final lanelet geometry at the end of the intersection
              otherLaneletAngleEnd = vehicleData[j].yawEnd;

              //gets the difference between vehicle orientation at the stop line and vehicle orientation leaving the intersection map
              difference = abs(egoLaneletAngle - otherLaneletAngle);

              //gets the ego turn and the other vehicles turn
              egoTurn = turnDirection(egoLaneletAngle, ego.yawEnd);
              otherTurn = turnDirection(otherLaneletAngle, otherLaneletAngleEnd);

              //checks if the reference vehicle is on the incoming left of the other vehicle being iterated over at the current moment interval
              onIncomingLeftOf = incomingLeftOf(otherLanelet, egoLanelet, vehicleData[j].intervals[k].yaw, ego.intervals[i].yaw, routingGraph);

              //checks if two vehicles share the same priority as one another on their respective lanelets
              sPriority = samePriority(egoLanelet, otherLanelet, egoTurn, otherTurn);
              //assume there is a relevant traffic light on the lanelet
              relevantTrafficLights = true;

              //checks if there are any traffic lights on the ego lane or the other vehicles lane
              trafficLightDataEgo = getTrafficLightStopLine(egoLanelet, lanelet::Point3d(lanelet::utils::getId(), currentPoint[0], currentPoint[1], 0));
              trafficLightDataOther = getTrafficLightStopLine(otherLanelet, lanelet::Point3d(lanelet::utils::getId(), otherPoint[0], otherPoint[1], 0));

              //checks if there are any traffic lights on the lanelets in consideration
              if (trafficLightDataEgo.index == Default::None && trafficLightDataOther.index == Default::None) {
                //determines there are no relevant traffic lights and that we can proceed
                relevantTrafficLights = false;
              } //END IF

	      //note that the vehicle needs to yield to another vehicle that is already at the intersection
	      if (onIncomingLeftOf && sPriority && !relevantTrafficLights && sameIntersection(map, originPoint, routingGraph, centerPoint, otherCenterPoint)) {
	        mustYield = true;
	      } //END IF

              //record the vehicles that are currently at the stop line
              vehicles.push_back(j);
          } //END IF
        } //END FOR
      } //END FOR
    } //END IF

    //if the vehicle has not stopped and started moving again, the vehicle is currently stopped, and the vehicle needs to stop at an upcoming stop line
    if (!hasStoppedAndStarted && intervalStart && needToStop) {
      //mark that the vehicle responded to the stop sign effectively
      stopSignResponse = true;
    } //END IF

    //if the vehicle has just exited a stopping state
    if (hasStoppedAndStarted) {
      //mark that the vehicle is no longer in a stopping state
      hasStoppedAndStarted = false;

      //if we are using the interaction datasets final reported values
      if (yawCheck == YawChecking::Dataset) {
        //contains the ending angle for the ego lanelet
        egoLaneletAngleEnd = ego.yawEnd;
      }
      //if we are using the intersections map projection
      else if (yawCheck == YawChecking::IntersectionProjection) {
        //find the ending angle of the vehicle upon leaving the projected intersection
        egoLaneletAngleEnd = endAngleFinder(map, originPoint, routingGraph, ego.intervals,
                                            ego.intervals[i].timestamp, velocityThreshold, stoppingThreshold);
                                            //END endAngleFinder
      } //END IF

      //for every other vehicle on the intersection
      for (size_t j = 0; j < vehicles.size(); j++) {
        lanelet::ConstLanelet otherEnd = getLanelet(mapPath, originPoint, BasicPoint2d(vehicleData[vehicles[j]].xEnd, vehicleData[vehicles[j]].yEnd));
        //for every moment in the other vehicles entry on the intersection
        for (size_t k = 0; k < vehicleData[vehicles[j]].intervals.size(); k++) {
          //checks if the other vehicle is not the ego vehicle
          if (vehicleData[vehicles[j]].intervals[k].timestamp != ego.intervals[i].timestamp) {
            continue;
          } //END IF

          //gets the position of the other vehicle on the reference map
          otherCenterPoint = BasicPoint2d(vehicleData[vehicles[j]].intervals[k].xPos, vehicleData[vehicles[j]].intervals[k].yPos);
          otherPoint = getVehicleBounds(vehicleData[vehicles[j]].length, vehicleData[vehicles[j]].intervals[k].yaw, otherCenterPoint[0], otherCenterPoint[1], Front);

          //obtain the projected velocity of the other vehicle from its component x and y velocity vectors
          projectedVelocityTwo = sqrt(vehicleData[vehicles[j]].intervals[k].velocityX*vehicleData[vehicles[j]].intervals[k].velocityX +
                                      vehicleData[vehicles[j]].intervals[k].velocityY*vehicleData[vehicles[j]].intervals[k].velocityY);
                                      //END sqrt

          //gets the reference and other lanelet the vehicles are currently within
          egoLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, centerPoint, 1)[0]);
          otherLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, otherCenterPoint, 1)[0]);

          //get lanelet turn angles based on current lanelet geometry at the entrance of the intersection
          egoLaneletAngle = ego.intervals[i].yaw;
          otherLaneletAngle = vehicleData[vehicles[j]].intervals[k].yaw;

          //if we are using the interaction datasets final reported values
          if (yawCheck == YawChecking::Dataset) {
            //get lanelet turn angles based on final lanelet geometry at the end of the intersection
            otherLaneletAngleEnd = vehicleData[vehicles[j]].yawEnd;
          }
          //if we are using the intersections map projection
          else if (yawCheck == YawChecking::IntersectionProjection) {
            //find the ending angle of the vehicle upon leaving the projected intersection
            otherLaneletAngleEnd = endAngleFinder(map, originPoint, routingGraph, vehicleData[vehicles[j]].intervals,
                                                  vehicleData[vehicles[j]].intervals[k].timestamp, velocityThreshold, stoppingThreshold);
                                                  //END endAngleFinder
          } //END IF

          //gets the difference between vehicle orientation at the stop line and vehicle orientation leaving the intersection map
          difference = abs(egoLaneletAngle - otherLaneletAngle);

          //gets the ego turn and the other vehicles turn
          egoTurn = turnDirection(egoLaneletAngle, egoLaneletAngleEnd);
          otherTurn = turnDirection(otherLaneletAngle, otherLaneletAngleEnd);

          //checks if the reference vehicle is on the incoming left of the other vehicle being iterated over at the current moment interval
          onIncomingLeftOf = incomingLeftOf(otherLanelet, egoLanelet, vehicleData[vehicles[j]].intervals[k].yaw, ego.intervals[i].yaw, routingGraph);

          //check if the reference vehicle is not on the incoming left of the other vehicle
          if (!onIncomingLeftOf) {
            continue; //continue through the loop to check the next vehicle on the map at the current moment
          } //END IF

          //the reference vehicle is on the incoming left of the other vehicle, so rule violation checks must pass
          skip = false;

          //checks if two vehicles share the same priority as one another on their respective lanelets
          sPriority = samePriority(egoLanelet, otherLanelet, egoTurn, otherTurn);
          //assume there is a relevant traffic light on the lanelet
          relevantTrafficLights = true;

          //checks if there are any traffic lights on the ego lane or the other vehicles lane
          trafficLightDataEgo = getTrafficLightStopLine(egoLanelet, lanelet::Point3d(lanelet::utils::getId(), currentPoint[0], currentPoint[1], 0));
          trafficLightDataOther = getTrafficLightStopLine(otherLanelet, lanelet::Point3d(lanelet::utils::getId(), otherPoint[0], otherPoint[1], 0));

          //checks if there are any traffic lights on the lanelets in consideration
          if (trafficLightDataEgo.index == Default::None && trafficLightDataOther.index == Default::None) {
            //determines there are no relevant traffic lights and that we can proceed
            relevantTrafficLights = false;
          } //END IF

          /*if the other vehicle is on the incoming left of the ego vehicle and the vehicles share the same priority on their respective lanelets
          and there are no relevant traffic lights on either vehicles lanelet*/
          if (onIncomingLeftOf && sPriority && !relevantTrafficLights) {
            //checks if the current moment for the other vehicle is equal to the moment the reference vehicle stopped and checks if the other vehicle is at the stop line
            if (inStandstill(projectedVelocityTwo, velocityThreshold) ||
                (getStopLineDist(map, originPoint, otherPoint, routingGraph) <= stoppingThreshold ||
                 getStopLineDist(map, originPoint, otherCenterPoint, routingGraph) <= stoppingThreshold) &&
                sameIntersection(map, originPoint, routingGraph, centerPoint, otherCenterPoint)) {
              //return that right-before-left applies to the ego vehicle and that it should not enter the intersection
              hasPassed = true;

              //store important information for return struct
              finalInterval = ego.intervals[i].timestamp;
              finalOtherID = vehicleData[vehicles[j]].intervals[k].id;

              //gets the final distance recorded between the ego vehicle and the stop line
              finalDistance = std::min(getStopLineDist(map, originPoint, otherPoint, routingGraph), getStopLineDist(map, originPoint, otherCenterPoint, routingGraph));

              //gets the amount of time it took the vehicle to turn since last moving
              intervalDuration = *finalInterval - startTime;

              //record the vehicle position
              xPos = ego.intervals[i].xPos;
              yPos = ego.intervals[i].yPos;
              yaw = ego.intervals[i].yaw;

	      //vehicle needs to yield
	      mustYield = true;

              //record the ego vehicle turn direction
              direction = egoTurn;
            } //END IF
          } //END IF
        } //END FOR
      } //END FOR
    } //END IF
  } //END FOR

  //fill in the results of the rule violation check for the given rule
  results.rule = Rules::RIII;
  //if hasPassed is true then the traffic rule applies to the vehicle and it failed by entering the intersection.
  //if skip is true then the vehicle has no vehicles that it is incoming left of.
  results.ruleStatus = !hasPassed || skip;
  //fill in vehicle information
  results.referenceID = ego.intervals[0].id;
  results.otherID = finalOtherID;
  results.neededToStop = mustYield;
  results.timestamp = finalInterval;
  results.xPos = xPos;
  results.yPos = yPos;
  results.yaw = yaw;
  results.endTimestamp = intervalDuration;
  results.magnitude = finalDistance;
  results.turnDirection = direction;

  //return the results container
  return results;
} //END checkIsLeftBeforeRight

/*
Input:
  const std::vector<struct vehicles> vehicleData - Contains data for all the vehicles in the given interaction dataset.
  const struct vehicles ego - Contains data for the ego vehicle.
  const std::string mapPath - Contains a string representation for the OSM file path for the dataset.
  const lanelet::BasicPoint2d originPoint - Contains the coordinates of the origin point for the OSM file.
  const double stoppingThreshold - Contains the threshold for a vehicle to be considered stopped before the stop line.
  const double velocityThreshold - Contains the threshold for a vehicle to be considered as not moving.
  const std::vector<std::pair<int, int>> overridePriority - Contains a vector of pairs for lanelet priority over-ride values:
    Override values are used to override given lanelet priority values to a preset user defined number.
    The first integer is used to to represent the lanelet ID, and the second to represent its new priority value.
  const YawChecking yawCheck - Determines the type of yaw angle checking to use for the vehicle:
    IntersectionProjection - Finds the end yaw of the vehicle once it leaves the intersectional or yield lanelets and exits the following lanelets.
                             Problems can arise if Lanelet2 pinpoints the vehicle as being nearest to the wrong lanelet.
    Dataset - Finds the end yaw of the vehicle using the final reported yaw from the interaction datasets momentary intervals.
              Assumes that the interval represents the movement of a vehicle across some specific lanelet.
              If multiple intersections are covered by the interval, then the method will not work correctly.
              It is a lot faster and more accurate to use the Dataset method if possible.
Output:
  const struct violation checkHasPriority - Return a struct containing information about:
      Rules rule - The rule that was evaluated.
      bool ruleStatus - Whether the rule was broken or not.
      int referenceID - The ID of the reference vehicle being checked.
      boost::optional<int> otherID - The ID of the other vehicle involved if the rule was violated, if applicable.
      boost::optional<int> timestamp - The moment in time the rule was violated or passed, if applicable.
Method:
  We take a reference vehicle and compare it to all other vehicles during the time interval that it is in standstill.
  If any of the other vehicles are stopped during the interval at the correct lanelets along the intersection, we evaluate the rules.
  The rules show that if two vehicles stop at the same time along the intersection, then the vehicle that should stay stopped must meet (R-IN4).
Formulation:
  R-IN4 is formulated in Formalization of Intersection Traffic Rules in Temporal Logic:
    https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
  R-IN4:
    G((turning_right(x_o) ∧ turning_right(x_ego) ∧ has_priority(x_o, x_ego, right, right)
          --- we skip some combinations of turning directions here ---
      ∨ turning_right(x_o) ∧ turning_left(x_ego) ∧ has_priority(x_o, x_ego, right, left) ∧ ¬on_oncom_of(x_o, x_ego)
      ∨ going_straight(x_o) ∧ turning_left(x_ego) ∧ has_priority(x_o, x_ego, straight, left) ∧ ¬on_oncom_of(x_o, x_ego))
        ==> (G(not_endanger_intersection(x_ego, x_o) ∨ ¬on_lanelet_with_type(x_ego, intersection)))
Description:
  Checks both the ego vehicles and the other vehicles priority to see if the other vehicle has priority at the intersection.
  The ego vehicle is only allowed to enter the intersection if it does not endanger the other vehicle.
*/
const struct violation ExtendedTrafficRules::checkHasPriority(const std::vector<struct vehicles> vehicleData, const struct vehicles ego,
                                                              const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                                                              const double stoppingThreshold, const double velocityThreshold,
                                                              const char * vehicleLocation,
                                                              const std::vector<std::pair<int, int>> overridePriority,
                                                              const YawChecking yawCheck) const {
  //used to contain the velocity of the vehicle from x and y velocity vectors
  double projectedVelocity, projectedVelocityTwo;

  //contains the difference between vehicle orientations
  double difference;

  //contains angles for lanelet vectors
  double egoLaneletAngle, egoLaneletAngleEnd, otherLaneletAngle, otherLaneletAngleEnd;

  //used to contain stop switches
  bool intervalStart = false;
  bool hasStoppedAndStarted = false;
  bool stopSignResponse = false;
  bool needToStop = false;
  bool isOncoming = false;
  bool hasPassed = false;
  bool skip = true;
  bool hPriority;

  //contains traffic light data for both lanelets
  struct stopLineData trafficLightDataEgo, trafficLightDataOther;

  //the starting and ending interval for a stop
  int startTime, endTime;

  //contains final recorded data
  boost::optional<int> finalInterval, finalOtherID;

  //contains the ID for the vehicle that has passed the stop line
  int id = Default::None;

  //contains both vehicle points on the map
  lanelet::BasicPoint2d currentPoint, otherPoint, centerPoint, centerPointOther;

  //used to contain the turn types for the vehicles
  Direction otherTurn, egoTurn;

  //used to contain the turn direction for the completed turn
  boost::optional<double> direction = false;

  //used to contain the lanelets for both vehicles
  lanelet::Lanelet egoLanelet, otherLanelet;

  //used to contain reference traffic rules
  lanelet::traffic_rules::TrafficRulesPtr trafficRules;

  //used to contain the routing graph built using the map and traffic rules
  lanelet::routing::RoutingGraphConstPtr routingGraph;

  //used to contain the map pointer generated by the program
  lanelet::LaneletMapPtr map;

  //contains the projector for the map
  projection::UtmProjector projector(lanelet::Origin({originPoint[0], originPoint[1]}), true, false);

  //opens the map and gets the routing graph
  map = load(mapPath, projector);
  trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(vehicleLocation, Participants::Vehicle);
  routingGraph = lanelet::routing::RoutingGraph::build(*map, *trafficRules);

  //gets the lanelet that the ego vehicle ends at
  lanelet::ConstLanelet egoEnd = getLanelet(mapPath, originPoint, BasicPoint2d(ego.xEnd, ego.yEnd));

  //contains a list of vehicles which hold the right-of-way over the currently iterated vehicle
  std::vector<int> vehicles;

  //contains the id of the given lanelet for the intersection
  int laneletID = Default::None;

  //contains the id of the current lanelet
  int currentID;

  //contains overridden ego and other lanelet values if applicable
  int egoValue = Default::None;
  int otherValue = Default::None;

  //contains the vehicles position upon completing a stop-regulated turn
  boost::optional<double> xPos = false;
  boost::optional<double> yPos = false;
  boost::optional<double> yaw = false;

  //contains the final duration recorded for the vehicle
  boost::optional<int> intervalDuration = false;

  //contains the final distance to the stop line for the recorded vehicle
  boost::optional<double> finalDistance = false;

  //contains the results of the rule violation check
  struct violation results = violation();

  //for every entry in the interval with the reference vehicle
  for (size_t i = 0; i < ego.intervals.size(); i++) {
    //gets the current point for the vehicle on the map
    centerPoint = BasicPoint2d(ego.intervals[i].xPos, ego.intervals[i].yPos);
    currentPoint = getVehicleBounds(ego.length, ego.intervals[i].yaw, centerPoint[0], centerPoint[1], Front);

    //gets the id of the current lanelet
    currentID = getCurrentLanelet(mapPath, originPoint, centerPoint);

    //gets the current velocity for the reference vehicle from x and y velocity vectors
    projectedVelocity = sqrt(ego.intervals[i].velocityX * ego.intervals[i].velocityX + ego.intervals[i].velocityY * ego.intervals[i].velocityY);

    //if the vehicle has stopped within a threshold and the vehicle was moving at the previous moment
    if (inStandstill(projectedVelocity, velocityThreshold) && !intervalStart) {
      //flag the interval as the start of the vehicles stopping period
      intervalStart = true;

      //set the current moment to be the start time of the stop
      startTime = ego.intervals[i].timestamp;
    }
    //if the vehicle has not stopped and the vehicle was not moving at the previous moment
    else if (!inStandstill(projectedVelocity, velocityThreshold) && intervalStart) {
      //flag the interval as the end of the vehicles stopping period
      intervalStart = false;

      //state that the vehicle has stopped and has started moving again meaning we should check for rule violations
      hasStoppedAndStarted = true;

      //set the moment to be the end time of the stop
      endTime = ego.intervals[i].timestamp;
    } //END IF

    //if we are at the start of a stopping interval and the distance between the reference vehicle and the stop line is within a specific threshold
    if (intervalStart && (getStopLineDist(map, originPoint, currentPoint, routingGraph) <= stoppingThreshold ||
                          getStopLineDist(map, originPoint, centerPoint, routingGraph) <= stoppingThreshold) &&
                          currentID != laneletID) {
      //clear the list of vehicles with the right of way
      vehicles.clear();

      //flag that we need to stop the vehicle
      needToStop = true;

      //obtain the IDs of all vehicles that are within a stop line
      for (size_t j = 0; j < vehicleData.size(); j++) {
        for (size_t k = 0; k < vehicleData[j].intervals.size(); k++) {
          //gets the position of the other vehicle on the reference map
          centerPointOther = BasicPoint2d(vehicleData[j].intervals[k].xPos, vehicleData[j].intervals[k].yPos);
          otherPoint = getVehicleBounds(vehicleData[j].length, vehicleData[j].intervals[k].yaw, centerPointOther[0], centerPointOther[1], Front);

          //obtain the projected velocity of the other vehicle from its component x and y velocity vectors
          projectedVelocityTwo = sqrt(vehicleData[j].intervals[k].velocityX*vehicleData[j].intervals[k].velocityX +
                                      vehicleData[j].intervals[k].velocityY*vehicleData[j].intervals[k].velocityY);
                                      //END sqrt

          //if the other vehicle is at the stop line and stopped at the currently iterated moment
          if (ego.intervals[i].id != vehicleData[j].intervals[k].id &&
              vehicleData[j].intervals[k].timestamp == startTime &&
              getStopLineDist(map, originPoint, otherPoint, routingGraph) <= stoppingThreshold &&
              sameIntersection(map, originPoint, routingGraph, centerPoint, centerPointOther)) {

              //sets the ID for the current intersection lanelet
              laneletID = getCurrentLanelet(mapPath, originPoint, currentPoint);

              //record the vehicles that are currently at the stop line
              vehicles.push_back(j);
          } //END IF
        } //END FOR
      } //END FOR
    } //END IF

    //if the vehicle has not stopped and started moving again, the vehicle is currently stopped, and the vehicle needs to stop at an upcoming stop line
    if (!hasStoppedAndStarted && intervalStart && needToStop) {
      //mark that the vehicle responded to the stop sign effectively
      stopSignResponse = true;
    } //END IF

    //if the vehicle has just exited a stopping state
    if (hasStoppedAndStarted) {
      //mark that the vehicle is no longer in a stopping state
      hasStoppedAndStarted = false;

      //if we are using the interaction datasets final reported values
      if (yawCheck == YawChecking::Dataset) {
        //get lanelet turn angles based on final lanelet geometry at the end of the intersection
        egoLaneletAngleEnd = ego.yawEnd;
      }
      //if we are using the intersections map projection
      else if (yawCheck == YawChecking::IntersectionProjection) {
        //find the ending angle of the vehicle upon leaving the projected intersection
        egoLaneletAngleEnd = endAngleFinder(map, originPoint, routingGraph, ego.intervals,
                                            ego.intervals[i].timestamp, velocityThreshold, stoppingThreshold);
                                            //END endAngleFinder
      } //END IF

      //for every other vehicle on the intersection
      for (size_t j = 0; j < vehicles.size(); j++) {
        //get the other vehicles lanelet
        lanelet::ConstLanelet otherEnd = getLanelet(mapPath, originPoint, BasicPoint2d(vehicleData[vehicles[j]].xEnd, vehicleData[vehicles[j]].yEnd));
        //for every moment in the other vehicles entry on the intersection
        for (size_t k = 0; k < vehicleData[vehicles[j]].intervals.size(); k++) {
          //checks if the other vehicle is not the ego vehicle
          if (vehicleData[vehicles[j]].intervals[k].timestamp != ego.intervals[i].timestamp) {
            //continue to the next moment for the vehicle
            continue;
          } //END IF

          //gets the position of the other vehicle on the reference map
          centerPointOther = BasicPoint2d(vehicleData[vehicles[j]].intervals[k].xPos, vehicleData[vehicles[j]].intervals[k].yPos);
          otherPoint = getVehicleBounds(vehicleData[vehicles[j]].length, vehicleData[vehicles[j]].intervals[k].yaw, centerPointOther[0], centerPointOther[1], Front);

          //obtain the projected velocity of the other vehicle from its component x and y velocity vectors
          projectedVelocityTwo = sqrt(vehicleData[vehicles[j]].intervals[k].velocityX*vehicleData[vehicles[j]].intervals[k].velocityX +
                                      vehicleData[vehicles[j]].intervals[k].velocityY*vehicleData[vehicles[j]].intervals[k].velocityY);
                                      //END sqrt

          //gets the reference and other lanelet the vehicles are currently within
          egoLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, centerPoint, 1)[0]);
          otherLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, centerPointOther, 1)[0]);

          //gets the angle of the current and other lanelet based on their respective geometries
          egoLaneletAngle = ego.intervals[i].yaw;
          otherLaneletAngle = vehicleData[vehicles[j]].intervals[k].yaw;

          //if we are using the interaction datasets final reported values
          if (yawCheck == YawChecking::Dataset) {
            //get lanelet turn angles based on final lanelet geometry at the end of the intersection
            otherLaneletAngleEnd = vehicleData[vehicles[j]].yawEnd;
          }
          //if we are using the intersections map projection
          else if (yawCheck == YawChecking::IntersectionProjection) {
            //find the ending angle of the vehicle upon leaving the projected intersection
            otherLaneletAngleEnd = endAngleFinder(map, originPoint, routingGraph, vehicleData[vehicles[j]].intervals,
                                                  vehicleData[vehicles[j]].intervals[k].timestamp, velocityThreshold, stoppingThreshold);
                                                  //END endAngleFinder
          } //END IF

          //gets the difference between the angles for the current and other lanelet
          difference = abs(egoLaneletAngle - otherLaneletAngle);

          //gets the ego turn and the other vehicles turn
          egoTurn = turnDirection(egoLaneletAngle, egoLaneletAngleEnd);
          otherTurn = turnDirection(otherLaneletAngle, otherLaneletAngleEnd);

          //checks if the reference vehicle is on the incoming left of the other vehicle being iterated over at the current moment interval
          isOncoming = (difference > 2.84159 && difference < 3.44159);

          //the reference vehicle is on the incoming left of the other vehicle, so rule violation checks must pass
          skip = false;

          //check if priority values have been overridden
          for (size_t j = 0; j < overridePriority.size(); j++) {
            if (std::get<0>(overridePriority[j]) == egoLanelet.id()) {
              egoValue = std::get<1>(overridePriority[j]);
            } //END IF
            if (std::get<0>(overridePriority[j]) == otherLanelet.id()) {
              otherValue = std::get<1>(overridePriority[j]);
            } //END IF
          } //END FOR

          //check if priority values have been overridden by user input
          if (egoValue != Default::None && otherValue != Default::None) {
            //checks if two vehicles share the same priority as one another given overridden priority values
            hPriority = hasPriority(otherValue, egoValue);
            egoValue = Default::None;
            otherValue = Default::None;
          }
          else if (egoValue != Default::None) {
            //checks if two vehicles share the same priority as one another on a respective lanelet and overridden priority value
            hPriority = hasPriority(otherLanelet, otherTurn, egoValue);
            egoValue = Default::None;
          }
          else if (otherValue != Default::None) {
            //checks if two vehicles share the same priority as one another on a respective lanelet and overridden priority value
            hPriority = hasPriority(otherValue, egoLanelet, egoTurn);
            otherValue = Default::None;
          }
          else {
            //checks if two vehicles share the same priority as one another on their respective lanelets
            hPriority = hasPriority(otherLanelet, egoLanelet, otherTurn, egoTurn);
          } //END IF

          //if the ego vehicle is turning left and the other vehicle is turning straight
          if (egoTurn == Left && otherTurn == Straight) {
            //if the ego vehicles does not have priority over the other vehicle and the other vehicle is on the oncoming lane of the ego vehicle
            if ((hPriority && !isOncoming) && inStandstill(projectedVelocityTwo, velocityThreshold) ||
                (getStopLineDist(map, originPoint, otherPoint, routingGraph) <= stoppingThreshold ||
                 getStopLineDist(map, originPoint, centerPointOther, routingGraph) <= stoppingThreshold) &&
                sameIntersection(map, originPoint, routingGraph, centerPoint, centerPointOther)) {
              //return that priority applies and that the ego vehicle should not enter the intersection
              hasPassed = true;

              //store important information for return struct
              finalInterval = ego.intervals[i].timestamp;
              finalOtherID = vehicleData[vehicles[j]].intervals[k].id;

              //gets the final distance recorded between the ego vehicle and the stop line
              finalDistance = std::min(getStopLineDist(map, originPoint, otherPoint, routingGraph), getStopLineDist(map, originPoint, centerPointOther, routingGraph));

              //gets the amount of time it took the vehicle to turn since last moving
              intervalDuration = *finalInterval - startTime;

              //record the vehicle position
              xPos = ego.intervals[i].xPos;
              yPos = ego.intervals[i].yPos;
              yaw = ego.intervals[i].yaw;

              //record the ego vehicle turn direction
              direction = egoTurn;
            } //END IF
          }
          else if (egoTurn == Left && otherTurn == Right) {
            //if the ego vehicles does not have priority over the other vehicle and the other vehicle is on the oncoming lane of the ego vehicle
            if ((hPriority && !isOncoming) && inStandstill(projectedVelocityTwo, velocityThreshold) ||
                (getStopLineDist(map, originPoint, otherPoint, routingGraph) <= stoppingThreshold ||
                 getStopLineDist(map, originPoint, centerPointOther, routingGraph) <= stoppingThreshold) &&
                sameIntersection(map, originPoint, routingGraph, centerPoint, centerPointOther)) {
              //return that priority applies and that the ego vehicle should not enter the intersection
              hasPassed = true;

              //store important information for return struct
              finalInterval = ego.intervals[i].timestamp;
              finalOtherID = vehicleData[vehicles[j]].intervals[k].id;

              //gets the final distance recorded between the ego vehicle and the stop line
              finalDistance = std::min(getStopLineDist(map, originPoint, otherPoint, routingGraph), getStopLineDist(map, originPoint, centerPointOther, routingGraph));

              //gets the amount of time it took the vehicle to turn since last moving
              intervalDuration = *finalInterval - startTime;

              //record the vehicle position
              xPos = ego.intervals[i].xPos;
              yPos = ego.intervals[i].yPos;
              yaw = ego.intervals[i].yaw;

              //record the ego vehicle turn direction
              direction = egoTurn;
            } //END IF
          }
          else if (hPriority) {
            //return that priority applies and that the ego vehicle should not enter the intersection
            hasPassed = true;

            //store important information for return struct
            finalInterval = ego.intervals[i].timestamp;
            finalOtherID = vehicleData[vehicles[j]].intervals[k].id;

            //gets the final distance recorded between the ego vehicle and the stop line
            finalDistance = std::min(getStopLineDist(map, originPoint, otherPoint, routingGraph), getStopLineDist(map, originPoint, centerPointOther, routingGraph));

            //gets the amount of time it took the vehicle to turn since last moving
            intervalDuration = *finalInterval - startTime;

            //record the vehicle position
            xPos = ego.intervals[i].xPos;
            yPos = ego.intervals[i].yPos;
            yaw = ego.intervals[i].yaw;

            //record the ego vehicle turn direction
            direction = egoTurn;
          } //END IF
        } //END FOR
      } //END FOR
    } //END IF
  } //END FOR

  //fill in the results of the rule violation check for the given rule
  results.rule = Rules::RIV;
  //if hasPassed is true then the traffic rule applies to the vehicle and it failed by entering the intersection.
  //if skip is true then the vehicle has no vehicles that it is incoming left of.
  results.ruleStatus = !hasPassed || skip;
  //fill in vehicle information
  results.referenceID = ego.intervals[0].id;
  results.otherID = finalOtherID;
  results.timestamp = finalInterval;
  results.xPos = xPos;
  results.yPos = yPos;
  results.yaw = yaw;
  results.endTimestamp = intervalDuration;
  results.magnitude = finalDistance;
  results.turnDirection = direction;

  //return the results container
  return results;
} //END checkShouldBeTurningLeft

/*
Input:
  const std::vector<struct vehicles> vehicleData - Contains data for all the vehicles in the given interaction dataset.
  const struct vehicles ego - Contains data for the ego vehicle.
  const std::string mapPath - Contains a string representation for the OSM file path for the dataset.
  const lanelet::BasicPoint2d originPoint - Contains the coordinates of the origin point for the OSM file.
  const double stoppingThreshold - Contains the threshold for a vehicle to be considered stopped before the stop line.
  const double velocityThreshold - Contains the threshold for a vehicle to be considered as not moving.
  const char * vehicleLocation - Contains a two-character representation for the nation the vehicle is currently driving within.
  const YawChecking yawCheck - Determines the type of yaw angle checking to use for the vehicle:
    IntersectionProjection - Finds the end yaw of the vehicle once it leaves the intersectional or yield lanelets and exits the following lanelets.
                             Problems can arise if Lanelet2 pinpoints the vehicle as being nearest to the wrong lanelet.
    Dataset - Finds the end yaw of the vehicle using the final reported yaw from the interaction datasets momentary intervals.
              Assumes that the interval represents the movement of a vehicle across some specific lanelet.
              If multiple intersections are covered by the interval, then the method will not work correctly.
              It is a lot faster and more accurate to use the Dataset method if possible.
Output:
  const struct violation checkShouldBeTurningLeft - Return a struct containing information about:
      Rules rule - The rule that was evaluated.
      bool ruleStatus - Whether the rule was broken or not.
      int referenceID - The ID of the reference vehicle being checked.
      boost::optional<int> otherID - The ID of the other vehicle involved if the rule was violated, if applicable.
      boost::optional<int> timestamp - The moment in time the rule was violated or passed, if applicable.
Method:
  We take a reference vehicle and compare it to all other vehicles during the time interval that it is in standstill.
  If any of the other vehicles are stopped during the interval at the correct lanelets along the intersection, we evaluate the rules.
  The rules show that if two vehicles stop at the same time along the intersection, then the vehicle that should stay stopped must meet (R-IN5).
  Determines if the following vehicle should turn left by referencing the corresponding lanelet elements.
  Checks regulatory elements located on the lanelets to determine if the rules apply.
Formulation:
  R-IN5 is formulated in Formalization of Intersection Traffic Rules in Temporal Logic:
    https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
  R-IN5:
    G((turning_left(x_ego) ∧ (going_straight(x_o) ∧ ¬has_priority(x_ego, x_o, left, straight) ∧ on_oncom_of(x_o, x_ego))
      ∨ (turning_right(x_o) ∧ ¬has_priority(x_ego, x_o, left, right) ∧ on_oncom_of(x_o, x_ego)))
        ==> (G(not_endanger_intersection(x_ego, x_o)) ∨ ¬on_lanelet_with_type(x_ego, intersection)))
Description:
  Checks if a left turning ego vehicle lacks priority over oncoming or right turning vehicles.
  The other vehicle cannot be oncoming to the ego vehicle.
  The function is used only for static test cases defined in the test script.
*/
const struct violation ExtendedTrafficRules::checkShouldBeTurningLeft(const std::vector<struct vehicles> vehicleData, const struct vehicles ego,
                                                                  const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                                                                  const double stoppingThreshold, const double velocityThreshold,
                                                                  const char * vehicleLocation, const YawChecking yawCheck) const {
                                                                  //END checkShouldBeTurningLeft
  //used to contain the velocity of the vehicle from x and y velocity vectors
  double projectedVelocity, projectedVelocityTwo;

  //contains the difference between vehicle orientations
  double difference;

  //contains angles for lanelet vectors
  double egoLaneletAngle, egoLaneletAngleEnd, otherLaneletAngle, otherLaneletAngleEnd;

  //used to contain stop switches
  bool intervalStart = false;
  bool hasStoppedAndStarted = false;
  bool stopSignResponse = false;
  bool needToStop = false;
  bool isOncoming = false;
  bool hasPassed = false;
  bool skip = true;
  bool mustYield = false;
  bool hPriority;

  //contains traffic light data for both lanelets
  struct stopLineData trafficLightDataEgo, trafficLightDataOther;

  //the starting and ending interval for a stop
  int startTime, endTime;

  //contains final recorded data
  boost::optional<int> finalInterval, finalOtherID;

  //contains the ID for the vehicle that has passed the stop line
  int id = Default::None;

  //contains both vehicle points on the map
  lanelet::BasicPoint2d currentPoint, otherPoint, centerPoint, centerPointOther;

  //used to contain the turn types for the vehicles
  Direction otherTurn, egoTurn;

  //used to contain the turn direction for the completed turn
  boost::optional<double> direction = false;

  //used to contain the lanelets for both vehicles
  lanelet::Lanelet egoLanelet, otherLanelet;

  //used to contain reference traffic rules
  lanelet::traffic_rules::TrafficRulesPtr trafficRules;

  //used to contain the routing graph built using the map and traffic rules
  lanelet::routing::RoutingGraphConstPtr routingGraph;

  //used to contain the map pointer generated by the program
  lanelet::LaneletMapPtr map;

  //contains the projector for the map
  projection::UtmProjector projector(lanelet::Origin({originPoint[0], originPoint[1]}), true, false);

  //opens the map and gets the routing graph
  map = load(mapPath, projector);
  trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(vehicleLocation, Participants::Vehicle);
  routingGraph = lanelet::routing::RoutingGraph::build(*map, *trafficRules);

  //gets the lanelet that the ego vehicle ends at
  lanelet::ConstLanelet egoEnd = getLanelet(mapPath, originPoint, BasicPoint2d(ego.xEnd, ego.yEnd));

  //contains a list of vehicles which hold the right-of-way over the currently iterated vehicle
  std::vector<int> vehicles;

  //contains the id of the given lanelet for the intersection
  int laneletID = Default::None;

  //contains the id of the current lanelet
  int currentID;

  //contains the vehicles position upon completing a stop-regulated turn
  boost::optional<double> xPos = false;
  boost::optional<double> yPos = false;
  boost::optional<double> yaw = false;

  //contains the final duration recorded for the vehicle
  boost::optional<int> intervalDuration = false;

  //contains the final distance to the stop line for the recorded vehicle
  boost::optional<double> finalDistance = false;

  //contains the results of the rule violation check
  struct violation results = violation();

  //for every entry in the interval with the reference vehicle
  for (size_t i = 0; i < ego.intervals.size(); i++) {
    //gets the current point for the vehicle on the map
    centerPoint = BasicPoint2d(ego.intervals[i].xPos, ego.intervals[i].yPos);
    currentPoint = getVehicleBounds(ego.length, ego.intervals[i].yaw, centerPoint[0], centerPoint[1], Front);

    //gets the id of the current lanelet
    currentID = getCurrentLanelet(mapPath, originPoint, centerPoint);

    //gets the current velocity for the reference vehicle from x and y velocity vectors
    projectedVelocity = sqrt(ego.intervals[i].velocityX * ego.intervals[i].velocityX + ego.intervals[i].velocityY * ego.intervals[i].velocityY);

    //if the vehicle has stopped within a threshold and the vehicle was moving at the previous moment
    if (inStandstill(projectedVelocity, velocityThreshold) && !intervalStart) {
      //flag the interval as the start of the vehicles stopping period
      intervalStart = true;

      //set the current moment to be the start time of the stop
      startTime = ego.intervals[i].timestamp;
    }
    //if the vehicle has not stopped and the vehicle was not moving at the previous moment
    else if (!inStandstill(projectedVelocity, velocityThreshold) && intervalStart) {
      //flag the interval as the end of the vehicles stopping period
      intervalStart = false;

      //state that the vehicle has stopped and has started moving again meaning we should check for rule violations
      hasStoppedAndStarted = true;

      //set the moment to be the end time of the stop
      endTime = ego.intervals[i].timestamp;
    } //END IF

    //if we are at the start of a stopping interval and the distance between the reference vehicle and the stop line is within a specific threshold
    if (intervalStart && (getStopLineDist(map, originPoint, currentPoint, routingGraph) <= stoppingThreshold ||
                          getStopLineDist(map, originPoint, centerPoint, routingGraph) <= stoppingThreshold) &&
                          currentID != laneletID) {
      //clear the list of vehicles with the right of way
      vehicles.clear();

      //flag that we need to stop the vehicle
      needToStop = true;

      //obtain the IDs of all vehicles that are within a stop line
      for (size_t j = 0; j < vehicleData.size(); j++) {
        for (size_t k = 0; k < vehicleData[j].intervals.size(); k++) {
          //gets the position of the other vehicle on the reference map
          centerPointOther = BasicPoint2d(vehicleData[j].intervals[k].xPos, vehicleData[j].intervals[k].yPos);
          otherPoint = getVehicleBounds(vehicleData[j].length, vehicleData[j].intervals[k].yaw, centerPointOther[0], centerPointOther[1], Front);

          //obtain the projected velocity of the other vehicle from its component x and y velocity vectors
          projectedVelocityTwo = sqrt(vehicleData[j].intervals[k].velocityX*vehicleData[j].intervals[k].velocityX +
                                      vehicleData[j].intervals[k].velocityY*vehicleData[j].intervals[k].velocityY);
                                      //END sqrt

          //if the other vehicle is at the stop line and stopped at the currently iterated moment
          if (ego.intervals[i].id != vehicleData[j].intervals[k].id &&
              vehicleData[j].intervals[k].timestamp == startTime &&
              getStopLineDist(map, originPoint, otherPoint, routingGraph) <= stoppingThreshold &&
              sameIntersection(map, originPoint, routingGraph, centerPoint, centerPointOther)) {

              //sets the ID for the current intersection lanelet
              laneletID = getCurrentLanelet(mapPath, originPoint, currentPoint);

              //gets the position of the other vehicle on the reference map
              centerPointOther = BasicPoint2d(vehicleData[j].intervals[k].xPos, vehicleData[j].intervals[k].yPos);
              otherPoint = getVehicleBounds(vehicleData[j].length, vehicleData[j].intervals[k].yaw, centerPointOther[0], centerPointOther[1], Front);

              //obtain the projected velocity of the other vehicle from its component x and y velocity vectors
              projectedVelocityTwo = sqrt(vehicleData[j].intervals[k].velocityX*vehicleData[j].intervals[k].velocityX +
                                          vehicleData[j].intervals[k].velocityY*vehicleData[j].intervals[k].velocityY);
                                          //END sqrt

              //gets the reference and other lanelet the vehicles are currently within
              egoLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, centerPoint, 1)[0]);
              otherLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, centerPointOther, 1)[0]);

              //gets the angle of the current and other lanelet based on their respective geometries
              egoLaneletAngle = ego.intervals[i].yaw;
              otherLaneletAngle = vehicleData[j].intervals[k].yaw;
              otherLaneletAngleEnd = vehicleData[j].yawEnd;

              //gets the difference between the angles for the current and other lanelet
              difference = abs(egoLaneletAngle - otherLaneletAngle);

              //gets the ego turn and the other vehicles turn
              egoTurn = turnDirection(egoLaneletAngle, ego.yawEnd);
              otherTurn = turnDirection(otherLaneletAngle, otherLaneletAngleEnd);

              //checks if the reference vehicle is on the incoming left of the other vehicle being iterated over at the current moment interval
              isOncoming = (difference > 2.84159 && difference < 3.44159);

	      //checks if two vehicles share the same priority as one another on their respective lanelets
              hPriority = hasPriority(egoLanelet, otherLanelet, egoTurn, otherTurn);

              //if the ego vehicle is turning left and the other vehicle is turning straight
              if (((egoTurn == Left && otherTurn == Straight) || (egoTurn == Left && otherTurn == Right)) && !hPriority && isOncoming && sameIntersection(map, originPoint, routingGraph, centerPoint, centerPointOther)) {
	        mustYield = true;
	      } //END IF

              //record the vehicles that are currently at the stop line
              vehicles.push_back(j);
          } //END IF
        } //END FOR
      } //END FOR
    } //END IF

    //if the vehicle has not stopped and started moving again, the vehicle is currently stopped, and the vehicle needs to stop at an upcoming stop line
    if (!hasStoppedAndStarted && intervalStart && needToStop) {
      //mark that the vehicle responded to the stop sign effectively
      stopSignResponse = true;
    } //END IF

    //if the vehicle has just exited a stopping state
    if (hasStoppedAndStarted) {
      //mark that the vehicle is no longer in a stopping state
      hasStoppedAndStarted = false;

      //if we are using the interaction datasets final reported values
      if (yawCheck == YawChecking::Dataset) {
        //get lanelet turn angles based on final lanelet geometry at the end of the intersection
        egoLaneletAngleEnd = ego.yawEnd;
      }
      //if we are using the intersections map projection
      else if (yawCheck == YawChecking::IntersectionProjection) {
        //find the ending angle of the vehicle upon leaving the projected intersection
        egoLaneletAngleEnd = endAngleFinder(map, originPoint, routingGraph, ego.intervals,
                                            ego.intervals[i].timestamp, velocityThreshold, stoppingThreshold);
                                            //END endAngleFinder
      } //END IF

      //for every other vehicle on the intersection
      for (size_t j = 0; j < vehicles.size(); j++) {
        //get the other vehicles lanelet
        lanelet::ConstLanelet otherEnd = getLanelet(mapPath, originPoint, BasicPoint2d(vehicleData[vehicles[j]].xEnd, vehicleData[vehicles[j]].yEnd));
        //for every moment in the other vehicles entry on the intersection
        for (size_t k = 0; k < vehicleData[vehicles[j]].intervals.size(); k++) {
          //checks if the other vehicle is not the ego vehicle
          if (vehicleData[vehicles[j]].intervals[k].timestamp != ego.intervals[i].timestamp) {
            //continue to the next moment for the vehicle
            continue;
          } //END IF

          //gets the position of the other vehicle on the reference map
          centerPointOther = BasicPoint2d(vehicleData[vehicles[j]].intervals[k].xPos, vehicleData[vehicles[j]].intervals[k].yPos);
          otherPoint = getVehicleBounds(vehicleData[vehicles[j]].length, vehicleData[vehicles[j]].intervals[k].yaw, centerPointOther[0], centerPointOther[1], Front);

          //obtain the projected velocity of the other vehicle from its component x and y velocity vectors
          projectedVelocityTwo = sqrt(vehicleData[vehicles[j]].intervals[k].velocityX*vehicleData[vehicles[j]].intervals[k].velocityX +
                                      vehicleData[vehicles[j]].intervals[k].velocityY*vehicleData[vehicles[j]].intervals[k].velocityY);
                                      //END sqrt

          //gets the reference and other lanelet the vehicles are currently within
          egoLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, centerPoint, 1)[0]);
          otherLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, centerPointOther, 1)[0]);

          //gets the angle of the current and other lanelet based on their respective geometries
          egoLaneletAngle = ego.intervals[i].yaw;
          otherLaneletAngle = vehicleData[vehicles[j]].intervals[k].yaw;

          //if we are using the interaction datasets final reported values
          if (yawCheck == YawChecking::Dataset) {
            //get lanelet turn angles based on final lanelet geometry at the end of the intersection
            otherLaneletAngleEnd = vehicleData[vehicles[j]].yawEnd;
          }
          //if we are using the intersections map projection
          else if (yawCheck == YawChecking::IntersectionProjection) {
            //find the ending angle of the vehicle upon leaving the projected intersection
            otherLaneletAngleEnd = endAngleFinder(map, originPoint, routingGraph, vehicleData[vehicles[j]].intervals,
                                                  vehicleData[vehicles[j]].intervals[k].timestamp, velocityThreshold, stoppingThreshold);
                                                  //END endAngleFinder
          } //END IF

          //gets the difference between the angles for the current and other lanelet
          difference = abs(egoLaneletAngle - otherLaneletAngle);

          //gets the ego turn and the other vehicles turn
          egoTurn = turnDirection(egoLaneletAngle, egoLaneletAngleEnd);
          otherTurn = turnDirection(otherLaneletAngle, otherLaneletAngleEnd);

          //checks if the reference vehicle is on the incoming left of the other vehicle being iterated over at the current moment interval
          isOncoming = (difference > 2.84159 && difference < 3.44159);

          //check if the reference vehicle is not on the incoming left of the other vehicle
          if (!isOncoming) {
            //continue through the loop to check the next vehicle on the map at the current moment
            continue;
          } //END IF

          //the reference vehicle is on the incoming left of the other vehicle, so rule violation checks must pass
          skip = false;

          //checks if two vehicles share the same priority as one another on their respective lanelets
          hPriority = hasPriority(egoLanelet, otherLanelet, egoTurn, otherTurn);

          //if the ego vehicle is turning left and the other vehicle is turning straight
          if (egoTurn == Left && otherTurn == Straight) {
            //if the ego vehicles does not have priority over the other vehicle and the other vehicle is on the oncoming lane of the ego vehicle
            if ((!hPriority && isOncoming) && inStandstill(projectedVelocityTwo, velocityThreshold) ||
                (getStopLineDist(map, originPoint, otherPoint, routingGraph) <= stoppingThreshold ||
                 getStopLineDist(map, originPoint, centerPointOther, routingGraph) <= stoppingThreshold) &&
                sameIntersection(map, originPoint, routingGraph, centerPoint, centerPointOther)) {
              //return that priority applies and that the ego vehicle should not enter the intersection
              hasPassed = true;

              //store important information for return struct
              finalInterval = ego.intervals[i].timestamp;
              finalOtherID = vehicleData[vehicles[j]].intervals[k].id;

              //gets the final distance recorded between the ego vehicle and the stop line
              finalDistance = std::min(getStopLineDist(map, originPoint, otherPoint, routingGraph), getStopLineDist(map, originPoint, centerPointOther, routingGraph));

              //gets the amount of time it took the vehicle to turn since last moving
              intervalDuration = *finalInterval - startTime;

              //record the vehicle position
              xPos = ego.intervals[i].xPos;
              yPos = ego.intervals[i].yPos;
              yaw = ego.intervals[i].yaw;

              //note the vehicle must yield
              mustYield = true;

              //record the ego vehicle turn direction
              direction = egoTurn;
            } //END IF
          }
          else if (egoTurn == Left && otherTurn == Right) {
            //if the ego vehicles does not have priority over the other vehicle and the other vehicle is on the oncoming lane of the ego vehicle
            if ((!hPriority && isOncoming) && inStandstill(projectedVelocityTwo, velocityThreshold) ||
                (getStopLineDist(map, originPoint, otherPoint, routingGraph) <= stoppingThreshold ||
                 getStopLineDist(map, originPoint, centerPointOther, routingGraph) <= stoppingThreshold) &&
                sameIntersection(map, originPoint, routingGraph, centerPoint, centerPointOther)) {
              //return that priority applies and that the ego vehicle should not enter the intersection
              hasPassed = true;

              //store important information for return struct
              finalInterval = ego.intervals[i].timestamp;
              finalOtherID = vehicleData[vehicles[j]].intervals[k].id;

              //gets the final distance recorded between the ego vehicle and the stop line
              finalDistance = std::min(getStopLineDist(map, originPoint, otherPoint, routingGraph), getStopLineDist(map, originPoint, centerPointOther, routingGraph));

              //gets the amount of time it took the vehicle to turn since last moving
              intervalDuration = *finalInterval - startTime;

              //record the vehicle position
              xPos = ego.intervals[i].xPos;
              yPos = ego.intervals[i].yPos;
              yaw = ego.intervals[i].yaw;

	      //note the vehicle must yield
	      mustYield = true;

              //record the ego vehicle turn direction
              direction = egoTurn;
            } //END IF
          } //END IF
        } //END FOR
      } //END FOR
    } //END IF
  } //END FOR

  //fill in the results of the rule violation check for the given rule
  results.rule = Rules::RV;
  //if hasPassed is true then the traffic rule applies to the vehicle and it failed by entering the intersection.
  //if skip is true then the vehicle has no vehicles that it is incoming left of.
  results.ruleStatus = !hasPassed || skip;
  //fill in vehicle information
  results.referenceID = ego.intervals[0].id;
  results.otherID = finalOtherID;
  results.neededToStop = mustYield;
  results.timestamp = finalInterval;
  results.xPos = xPos;
  results.yPos = yPos;
  results.yaw = yaw;
  results.endTimestamp = intervalDuration;
  results.magnitude = finalDistance;
  results.turnDirection = direction;

  //return the results container
  return results;
} //END checkShouldBeTurningLeft

/*
Input:
  const double yawStart - Vehicles initial yaw angle in terms of radians.
  const double yawEnd - Vehicles ending yaw angle in terms of radians.
Output:
  Direction turnDirection - We return an enumeration representing the direction the vehicle has turned.
Method:
  We compare yaw radians between a vehicles start and end orientation and determine if a turn is left, right, or straight based on the result.
Description:
  Determines the turn direction of the vehicle based off its change in yaw angle.
*/
Direction ExtendedTrafficRules::turnDirection(const double yawStart, const double yawEnd) const {
  //contains the starting and ending radians
  double yawStartRad, yawEndRad;

  //get the angle in terms of yaw radians
  yawStartRad = getOrientation(yawStart);
  yawEndRad = getOrientation(yawEnd);

  //checks if the vehicle turned left or right, or if it went straight by comparing vehicle orientations
  if ((yawEndRad - yawStartRad > 0.7 && yawEndRad - yawStartRad < 2.27) || (yawEndRad - yawStartRad < -5*PI/4 && yawEndRad - yawStartRad > -7*PI/4)) {
    return Direction::Left;
  }
  else if ((yawEndRad - yawStartRad < -0.7 && yawEndRad - yawStartRad > -2.27) ||
           (yawEndRad - yawStartRad > 4 && yawEndRad - yawStartRad < 5.58)) {

    return Direction::Right;
  }
  else if (std::fmod(yawEndRad - yawStartRad, 2*PI) < 0.7 || (std::fmod(yawEndRad - yawStartRad, 2*PI) > 5.6)) {
    return Direction::Straight;
  } //END IF

  //default turn direction
  return Direction::Right;
} //END turnDirection

/*
Input:
  const std::string mapPath - Gets a string representation of path to the open street map file location.
  const lanelet::BasicPoint2d originPoint - Contains the origin point for the given open street map.
  const lanelet::BasicPoint2d startPoint - Contains the location of the vehicles position.
Output:
  int getCurrentLanelet - Returns the ID of the lanelet the vehicle is on.
Method:
  We take a vehicles position and compare it with the lanelet map to find the nearest lanelet to the vehicle.
Description:
  Gets the ID of the current lanelet the vehicle is on.
*/
int ExtendedTrafficRules::getCurrentLanelet(const std::string mapPath, const lanelet::BasicPoint2d originPoint, const lanelet::BasicPoint2d startPoint) const {
  //open the map with the given origin point for the projection
  projection::UtmProjector projector(lanelet::Origin({originPoint[0], originPoint[1]}), true, false);
  lanelet::LaneletMapPtr map = load(mapPath, projector);

  //return the lanelet given
  return std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, startPoint, 1)[0]).id();
} //END getCurrentLanelet

/*
Input:
  const std::string mapPath - Gets a string representation of path to the open street map file location.
  const lanelet::BasicPoint2d originPoint - Contains the origin point for the given open street map.
  const lanelet::BasicPoint2d startPoint - Contains the location of the vehicles position.
Output:
  lanelet::ConstLanelet getLanelet - Returns a lanelet::ConstLanelet representation of the lanelet the vehicle is on.
Method:
  We take a vehicles position and compare it with the lanelet map to find the nearest lanelet to the vehicle.
Description:
  Gets the current lanelet the vehicle is on.
*/
lanelet::ConstLanelet ExtendedTrafficRules::getLanelet(const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                                                       const lanelet::BasicPoint2d startPoint) const {
                                                       //END getLanelet

  //open the map with the given origin point for the projection
  projection::UtmProjector projector(lanelet::Origin({originPoint[0], originPoint[1]}), true, false);
  lanelet::LaneletMapPtr map = load(mapPath, projector);

  //return the lanelet given
  return std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, startPoint, 1)[0]);
} //END getLanelet

/*
Input:
  const std::string mapPath - Gets a string representation of path to the open street map file location.
  const lanelet::BasicPoint2d originPoint - Contains the origin point for the given open street map.
  const lanelet::BasicPoint2d startPoint - Contains the location of the vehicles position.
  const char * vehicleLocation - Contains a two-character representation for the nation the current vehicle is driving within.
  std::vector<int> exception - Contains the ID values of any lanelets excepted from the stop detection requirements.
Output:
  double getStopLineDist - Returns the distance between the vehicle and the stop line.
Method:
  Initializes the getStopLineDist function for function calls where the lanelet map pointer is not initialized for the given map.
  Returns the result of the standard getStopLineDist function.
Description:
  Gets the distance to the stop line given a point representing a vehicles location.
*/
double ExtendedTrafficRules::getStopLineDist(const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                                             const lanelet::BasicPoint2d startPoint, const char * vehicleLocation,
                                             std::vector<int> exception) const {
                                             //END getStopLineDist

  //open the map with the given origin point for the projection
  projection::UtmProjector projector(lanelet::Origin({originPoint[0], originPoint[1]}), true, false);
  lanelet::LaneletMapPtr map = load(mapPath, projector);

  //obtain a routing graph
  lanelet::traffic_rules::TrafficRulesPtr trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(vehicleLocation, Participants::Vehicle);
  lanelet::routing::RoutingGraphConstPtr routingGraph = lanelet::routing::RoutingGraph::build(*map, *trafficRules);

  //return the distance to the stop line
  return getStopLineDist(map, originPoint, startPoint, routingGraph, exception);
} //END getStopLineDist

/*
Input:
  const lanelet::LaneletMapPtr map - Contains a pointer to the map representation of the OSM file.
  const lanelet::BasicPoint2d originPoint - Contains the origin point for the given open street map.
  const lanelet::BasicPoint2d startPoint - Contains the location of the vehicles position.
  const lanelet::routing::RoutingGraphConstPtr routingGraph - Contains a routing graph pointer for the OSM file.
  std::vector<int> exception - Contains the ID values of any lanelets excepted from the stop detection requirements.
Output:
  double getStopLineDist - Returns the distance between the vehicle and the stop line.
Method:
  Finds the ArcCoordinate distance between the stop line position as a 2D line and the vehicle position as a 2D point.
Description:
  Gets the distance to the stop line given a point representing a vehicles location.
*/
double ExtendedTrafficRules::getStopLineDist(const lanelet::LaneletMapPtr map, const lanelet::BasicPoint2d originPoint,
                                             const lanelet::BasicPoint2d startPoint, const lanelet::routing::RoutingGraphConstPtr routingGraph,
                                             std::vector<int> exception) const {
                                             //END getStopLineDist

  //define containers for stop line data
  lanelet::Optional<lanelet::ConstLineString3d> stopLineCur;
  lanelet::Optional<lanelet::ConstLineString3d> stopLineNxt;
  lanelet::Optional<lanelet::ConstLineString3d> trafficSignLine;

  //get the next lanelets in the route
  std::vector<lanelet::ConstLanelet> nextLanelets;

  //containers for regulatory elements
  std::vector<AllWayStop::Ptr> allWayStopRegelems;
  std::vector<RightOfWay::Ptr> rightOfWayRegelems;

  //contains candidates for the distance between the vehicle and the stop line
  double distanceOne = Default::NO_LINE_ON_LANELET;
  double distanceTwo = Default::NO_LINE_ON_LANELET;

  //contains the distance between the vehicle and the stop line
  double vehicleDist = Default::NO_LINE_ON_LANELET;

  //get the lanelet that the vehicle is on
  lanelet::Lanelet egoLanelet = std::get<1>(lanelet::geometry::findNearest(map->laneletLayer, startPoint, 1)[0]);

  //create a point3d to represent the vehicle position
  lanelet::Point3d vehiclePos(lanelet::utils::getId(), startPoint[0], startPoint[1], 0);

  //check if any lanelets are set to be exempt from stop-sign regulation
  if (std::find(exception.begin(), exception.end(), egoLanelet.id()) != exception.end()) {
    //return the default value to the caller
    return vehicleDist;
  } //END IF

  //get the next lanelets in the route
  nextLanelets = routingGraph->following(egoLanelet);

  //get data for the all way stop representing the stop sign
  allWayStopRegelems = egoLanelet.regulatoryElementsAs<AllWayStop>();

  //check if there is a right of way element
  rightOfWayRegelems = egoLanelet.regulatoryElementsAs<RightOfWay>();

  //if an all way stop regulatory element is present
  if (!allWayStopRegelems.empty()) {
    //for every all way stop element
    for (size_t i = 0; i < allWayStopRegelems.size(); i++) {
      //get the current and next stop lines if they exist
      stopLineCur = allWayStopRegelems[i]->getStopLine(egoLanelet);

      //if the current lanelet has a stop line
      if (stopLineCur) {
        //find the distance between the vehicle and the stop line
        distanceOne = lanelet::geometry::toArcCoordinates(lanelet::utils::to2D(*stopLineCur), lanelet::utils::to2D(vehiclePos)).distance;
      } //END IF
    } //END FOR
  } //END IF
  //check if there are elements inside
  if (!rightOfWayRegelems.empty()) {
    //for the number of elements contained in the right of way representation
    for (size_t i = 0; i < rightOfWayRegelems.size(); i++) {
      //get the traffic line if it is available
      trafficSignLine = rightOfWayRegelems[i]->stopLine();

      //gets the maneuver from the given lanelet
      lanelet::ManeuverType maneuverType = rightOfWayRegelems[i]->getManeuver(egoLanelet);

      //if the next lanelet has a stop line and the lanelet must yield the right of way
      if (trafficSignLine && maneuverType == ManeuverType::Yield) {
        //find the distance between the vehicle and the stop line
        distanceTwo = lanelet::geometry::toArcCoordinates(lanelet::utils::to2D(*trafficSignLine), lanelet::utils::to2D(vehiclePos)).distance;
      } //END IF
    } //END FOR
  } //END IF

  //gets the lowest candidate distance
  vehicleDist = std::min(abs(distanceOne), abs(distanceTwo));

  //if there are no next lanelets OR the vehicle distance is not the default value
  if (nextLanelets.size() == 0 || vehicleDist != Default::NO_LINE_ON_LANELET) {
    //return the distance to the stop line
    return abs(vehicleDist);
  } //END IF

  //get data for the all way stop representing the stop sign
  auto nxtAllWayStopRegelems = nextLanelets[0].regulatoryElementsAs<AllWayStop>();

  //check if there is a right of way element
  auto nxtRightOfWayRegelems = nextLanelets[0].regulatoryElementsAs<RightOfWay>();

  //if there are sign regulatory elements in the vector
  if (!nxtAllWayStopRegelems.empty()) {
    //for every all way stop element
    for (size_t i = 0; i < nxtAllWayStopRegelems.size(); i++) {
      //get the current and next stop lines if they exist
      stopLineNxt = nxtAllWayStopRegelems[i]->getStopLine(nextLanelets[0]);

      //if the next lanelet has a stop line
      if (stopLineNxt) {
        //find the distance between the vehicle and the stop line
        distanceOne = lanelet::geometry::toArcCoordinates(lanelet::utils::to2D(*stopLineNxt), lanelet::utils::to2D(vehiclePos)).distance;
      } //END IF
    } //END FOR
  } //END IF
  //check if there are elements inside
  if (!nxtRightOfWayRegelems.empty()) {
    //for the number of elements contained in the right of way representation
    for (size_t i = 0; i < nxtRightOfWayRegelems.size(); i++) {
      //get the traffic line if it is available
      trafficSignLine = nxtRightOfWayRegelems[i]->stopLine();

      //gets the maneuver from the given lanelet
      lanelet::ManeuverType maneuverType = (*nxtRightOfWayRegelems[i]).getManeuver(egoLanelet);

      //if the next lanelet has a stop line and the lanelet must yield the right of way
      if (trafficSignLine && maneuverType == ManeuverType::Yield) {
        //find the distance between the vehicle and the stop line
        distanceTwo = lanelet::geometry::toArcCoordinates(lanelet::utils::to2D(*trafficSignLine), lanelet::utils::to2D(vehiclePos)).distance;
      } //END IF
    } //END FOR
  } //END IF

  //gets the lowest candidate distance
  vehicleDist = std::min(abs(distanceOne), abs(distanceTwo));

  //return the distance between the vehicle and the stop line
  return abs(vehicleDist);
} //END getStopLine

/*
Input:
  const std::vector<struct interval> intervalData - Contains interval data for all the reference vehicle in the given interaction dataset.
  const std::string mapPath - Contains a string representation for the OSM file path for the dataset.
  const lanelet::BasicPoint2d originPoint - Contains the coordinates of the origin point for the OSM file.
  const double stoppingThreshold - Contains the threshold for a vehicle to be considered stopped before the stop line.
  const double velocityThreshold - Contains the threshold for a vehicle to be considered as not moving.
  const char * vehicleLocation - Contains the vehicle location two-character array.
  const StopDirection stopType - Determines if multiple stop signs down a single roadway regulate the intersection.
  const int timeLimit - The time limit for a stop to be registered as a complete stop, set to zero by default.
  std::vector<int> exception - Contains a list of lanelet IDs to be excepted from the stop-sign regulation if needed.
Output:
  const struct violation checkLeftBeforeRight - Return a struct containing information about:
      Rules rule - The rule that was evaluated.
      bool ruleStatus - Whether the rule was broken or not.
      int referenceID - The ID of the reference vehicle being checked.
      boost::optional<int> otherID - Not applicable in this test.
      boost::optional<int> timestamp - Not applicable in this test.
Method:
  Iterates through all the intervals provided to check if stop sign rules are violated at any point.
  Stop signs are located by searching the lanelet by taking the vehicles position along the intersection.
  Stop lines and sign locations are based off pre-defined positions in the OSM file.
Formulation:
  R-IN1 is formulated in Formalization of Intersection Traffic Rules in Temporal Logic:
    https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
  R-IN1:
    G((passing_stop_line(x_ego) ∧ at_traffic_sign(x_ego, stop) ∧ ¬relevant_traffic_light(x_ego))
      ==> (O(G_[0,t_s|w](stop_line_in_front(x_ego) ∧ in_standstill(x_ego))))
Description:
  The ego vehicle has to stop with respect to a stop sign before it enters the intersection at least for a duration of
  t_s|w in front of the associated stop line.
  Determines if a vehicle has failed to stop at the stop line appropriately.
*/
const struct violation ExtendedTrafficRules::checkStopAtSign(const std::vector<struct interval> intervalData, const std::string mapPath,
                                                             const lanelet::BasicPoint2d originPoint, const double stoppingThreshold,
                                                             const double velocityThreshold, const char * vehicleLocation,
                                                             const StopDirection stopType, const int timeLimit,
                                                             std::vector<int> exception) const {
                                                             //END checkStopAtSign

  //used to contain the velocity of the vehicle from x and y velocity vectors
  double projectedVelocity;

  //contains the distance between a vehicle and the stop line for the lanelet
  double distance;

  //used to contain stop switches
  bool intervalStart = false;
  bool hasStopped = false;
  bool needToStop = false;
  bool release = false;
  bool neededToStopAtAll = false;
  bool justPassed = true;

  //store the vehicle values at the moment of passing the stop sign test
  double xPosPass, yPosPass, yawPass, passVelocity;

  //the starting and ending interval for a stop
  int startTime, endTime;

  //contains the moment that the test passed if applicable
  boost::optional<int> passMoment, endMoment;

  //contains the location of the current position of the vehicle
  lanelet::BasicPoint2d currentPoint;

  //contains the open street map file being projected
  lanelet::LaneletMapPtr map;

  //contains the traffic rules and the routing graph being referenced
  lanelet::traffic_rules::TrafficRulesPtr trafficRules;
  lanelet::routing::RoutingGraphConstPtr routingGraph;

  //contains the projector for the open street map file
  projection::UtmProjector projector(lanelet::Origin({originPoint[0], originPoint[1]}), true, false);

  //contains the results of the stop violation test
  struct violation violationResults;

  //open the map with the referenced origin point
  map = load(mapPath, projector);

  //open a routing graph for the current map
  trafficRules = lanelet::traffic_rules::TrafficRulesFactory::create(vehicleLocation, Participants::Vehicle);
  routingGraph = lanelet::routing::RoutingGraph::build(*map, *trafficRules);

  //sets the default passing velocity
  passVelocity = -1;

  //for every entry in the vehicle interval dataset
  for (size_t i = 0; i < intervalData.size(); i++) {
    //get the vehicles current point
    currentPoint = BasicPoint2d(intervalData[i].xPos, intervalData[i].yPos);

    //calculate the projected velocity from velocity vectors
    projectedVelocity = sqrt(intervalData[i].velocityX * intervalData[i].velocityX +
                             intervalData[i].velocityY * intervalData[i].velocityY);
                             //END sqrt

    //check if the vehicle is standing still and if the vehicle was not standing still at the last iteration
    if (inStandstill(projectedVelocity, velocityThreshold) && !intervalStart) {
      //the vehicle has stopped
      intervalStart = true;
      //set the current moment as the start time for the stop interval
      startTime = intervalData[i].timestamp;
    }
    //check if the vehicle is not standing still and it was previously at rest during the previous iterations
    else if (!inStandstill(projectedVelocity, velocityThreshold) && intervalStart) {
      //the vehicle has started moving
      intervalStart = false;
      //set the current moment as the end time for the stop interval
      endTime = intervalData[i].timestamp;

      //check if the vehicle has passed the test
      if (hasStopped) {
        endMoment = endTime;
      } //END IF
    } //END IF

    //find the distance between the vehicle and the stop line
    distance = getStopLineDist(map, originPoint, currentPoint, routingGraph, exception);

    //check if the distance is ever less then the threshold to determine if we ever needed to stop at all
    if (distance <= stoppingThreshold) {
      neededToStopAtAll = true;
      violationResults.neededToStop = true;
    } //END IF

    //check if we should begin looking for another stop line in the vehicles path
    if (hasStopped && stopType == StopDirection::MultiStop && !release && distance > stoppingThreshold) {
      //note that we have passed a stop line and should start checking for the next stop line
      release = true;
    } //END IF

    if (neededToStopAtAll && !hasStopped && distance > stoppingThreshold && justPassed) {
      //note the vehicles current speed at the moment of violation
      passVelocity = sqrt(intervalData[i].velocityX * intervalData[i].velocityX +
                          intervalData[i].velocityY * intervalData[i].velocityY);
                          //END sqrt
      //note that the vehicle just failed the stop sign check
      justPassed = false;
    } //END IF

    //check if the distance to the stop line is within a given threshold
    if (distance <= stoppingThreshold && !hasStopped) {
      //flag that the vehicle needs to stop
      needToStop = true;
    }
    else if (distance == Default::NO_LINE_ON_LANELET) {
      //continue to the next iteration of the loop
      continue;
    }
    else if (distance <= stoppingThreshold && hasStopped && stopType == StopDirection::MultiStop && release) {
      //flag that we need to stop again and that hasStopped should be re-evaluated
      hasStopped = false;

      //note that the vehicle has not yet failed the stop sign check
      justPassed = true;

      //flag that the vehicle needs to stop
      needToStop = true;

      //update the release checker
      release = false;
    }
    else {
      //flag that the vehicle does not need to stop
      needToStop = false;
    } //END IF

    //checks if the vehicle has not stopped, if the vehicle is currently at rest, and if the vehicle needs to stop
    if (!hasStopped && intervalStart && needToStop && intervalData[i].timestamp - startTime >= timeLimit) {
      //flag that the vehicle has stopped as it was supposed to
      hasStopped = true;

      //record the current timestamp
      passMoment = intervalData[i].timestamp;

      //check velocity at moment of rule pass
      passVelocity = sqrt(intervalData[i].velocityX * intervalData[i].velocityX +
                          intervalData[i].velocityY * intervalData[i].velocityY);
                          //END sqrt

      //record the current passing position and orientation values
      xPosPass = intervalData[i].xPos;
      yPosPass = intervalData[i].yPos;
      yawPass = intervalData[i].yaw;
    } //END IF
  } //END FOR

  //store the results of the stop violation test
  violationResults.rule = Rules::RI;
  violationResults.ruleStatus = hasStopped || !neededToStopAtAll;
  violationResults.referenceID = intervalData[0].id;
  violationResults.otherID = false;
  violationResults.timestamp = passMoment;
  violationResults.xPos = xPosPass;
  violationResults.yPos = yPosPass;
  violationResults.yaw = yawPass;
  violationResults.magnitude = passVelocity;
  violationResults.endTimestamp = endMoment;

  //return if the vehicle stopped or not at the given intersections stop line
  return violationResults;
} //END checkStopAtSign

/*
Input:
  const std::vector<struct interval> intervalData - Contains interval data for all the reference vehicle in the given interaction dataset.
  const std::string mapPath - Contains a string representation for the OSM file path for the dataset.
  const double velocityThreshold - Contains the threshold for a vehicle to be considered as not moving.
Output:
  std::vector<std::pair<int, int>> standstillIntervals - Intervals <starttime, stoptime> where the vehicle is not moving.
Method:
  Determines if the vehicle has stopped and if it records the timestamp when the vehicle stopped.
  Determines when a vehicle has started moving again and records the timestamp when the vehicle has started moving again.
Description:
  Gets the time window where a vehicle is at standstill.
*/
std::vector<std::pair<int, int>> ExtendedTrafficRules::standstillIntervals(const std::vector<struct interval> intervalData, const std::string mapPath,
                                                                           const double velocityThreshold) const {
                                                                           //END standstillIntervals

  //contains vehicle velocity and time values
  double projectedVelocity;
  int startTime, endTime;

  //contains the stop intervals for the vehicle
  std::vector<std::pair<int, int>> stopIntervals;

  //contains the start interval for the vehicle
  bool intervalStart = false;

  //for each entry in the vehicles dataset
  for (size_t i = 0; i < intervalData.size(); i++) {
    //determine the projected velocity from the x and y velocity vectors
    projectedVelocity = sqrt(intervalData[i].velocityX*intervalData[i].velocityX + intervalData[i].velocityY*intervalData[i].velocityY);

    //if the vehicle has stopped and there is no current active stop interval
    if (inStandstill(projectedVelocity, velocityThreshold) && !intervalStart) {
      //record that the interval has begun
      intervalStart = true;

      //record the start time for the stop interval
      startTime = intervalData[i].timestamp;
    }
    //if the vehicle has stopped and there is a current active stop interval
    else if (!inStandstill(projectedVelocity, velocityThreshold) && intervalStart) {
      //note that the interval has concluded
      intervalStart = false;

      //record the end time for the stop interval
      endTime = intervalData[i].timestamp;

      //record the interval of the vehicle stopping
      stopIntervals.push_back(std::make_pair(startTime, endTime));
    } //END IF
  } //END FOR

  //if there are no recorded intervals where the vehicle has stopped
  if (stopIntervals.empty()) {
    //add an empty entry
    stopIntervals.push_back(std::make_pair(0,0));
  } //END IF

  //return the vehicle stop intervals
  return stopIntervals;
} //END standstillIntervals

/*
Input:
  const double projectedV - Represents the projected velocity of the vehicle.
  const double errorV - Represents the error velocity of the vehicle.
Output:
  bool inStandstill - Determines if the vehicle is in standstill.
Method:
  Checks if the vehicle velocity is within a specified range to determine if it is in standstill.
Formulation:
  Formulated in Formalization of Intersection Traffic Rules in Temporal Logic:
    https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
  See section C. Velocity and Breaking Elements in the paper.
Description:
  Gets the time window where a vehicle stands still
*/
bool ExtendedTrafficRules::inStandstill(const double projectedV, const double errorV) const {
  //determines if the vehicle has stopped within a margin of error
  if (-errorV <= projectedV && projectedV <= errorV) {
    return true;
  } //END IF
  return false;
} //END inStandstill

/*
Input:
  const lanelet::ConstLanelet & lanelet - Lanelet the given vehicle is on.
  const double travelledDistanceTillStandstill - Distance a vehicle can travel until it comes to a stop.
  const struct stopLineData index - Contains an index of important stop line data.
  const lanelet::Point3d vehicle - position of the vehicle on the map.
Output:
  bool canStandstillAtLights - Determines if the vehicle can stop at the lights.
Method:
  Checks if the vehicle can stop at lights by verifying if a vehicle can make the stop.
Description:
  Checks if a vehicle can stop at traffic lights given a lanelet.
  Used to check if breaking for a yellow light is possible or not.
*/
bool ExtendedTrafficRules::canStandstillAtLights(const lanelet::ConstLanelet & lanelet, const double travelledDistanceTillStandstill,
                                                 const struct stopLineData index, const lanelet::Point3d vehicle) const {
                                                 //END canStandstillAtLights

  //get the traffic light regulatory elements
  auto trafficLightRegelems = lanelet.regulatoryElementsAs<TrafficLight>();

  //get the stop line for the traffic light regulatory element if it exists
  lanelet::Optional<lanelet::ConstLineString3d> trafficLightOpt = trafficLightRegelems[index.index]->stopLine();

  //determine if there is a traffic light stop line and if stopping at it is possible
  if (trafficLightOpt && travelledDistanceTillStandstill <= index.lowestDist) {
    return true;
  } //END IF
  return false;
} //END canStandstillAtLights

/*
Input:
  const lanelet::ConstLanelet & egoLanelet - Lanelet the given vehicle is on.
  const Direction turnType - The turning direction of a given vehicle.
Output:
  int priorityValue - Determines the priority of the given lanelet based off its traffic signs.
Method:
  Sums the priority value of all traffic signs regulating the lanelet.
Description:
  Finds the priority value of the given lanelet given the vehicles turn direction.
*/
int ExtendedTrafficRules::priorityValue(const lanelet::ConstLanelet & egoLanelet, const Direction turnType) const {
  //get traffic sign regulatory elements
  auto trafficSignRegelems = egoLanelet.regulatoryElementsAs<TrafficSign>();
  auto rightOfWayRegelems = egoLanelet.regulatoryElementsAs<RightOfWay>();
  auto awsRegelems = egoLanelet.regulatoryElementsAs<AllWayStop>();

  //get the ID values that map German traffic signs to their corresponding string representation
  struct TrafficSignsGerman germanSignIDs;

  //get the ID values that map American traffic signs to their corresponding string representation
  struct TrafficSignsAmerican americanSignIDs;

  //get the ID values that map Canadian traffic signs to their corresponding string representation
  struct TrafficSignsCanadian canadianSignIDs;

  //priority value sum
  int totalSum = 0;

  //index for the specified turn type
  int index = Default::None;

  //obtain the sign type, make the default value German
  int signType = TrafficSignOrigin::German;

  //determine the priority value based on the turn type the vehicle is going to make
  if (turnType == Direction::Left) {
    index = 0;
  }
  else if (turnType == Direction::Straight) {
    index = 1;
  }
  else if (turnType == Direction::Right) {
    index = 2;
  } //END IF

  //if there is an all-way-stop without a target sign specified
  if (!awsRegelems.empty() || !rightOfWayRegelems.empty()) {
    //Add a stop sign for the stop regulated intersection
    totalSum += GeneralRules::STOP_SIGN_VALUE;
  } //END IF

  //for each traffic sign on the lanelet
  for (size_t i = 0; i < trafficSignRegelems.size(); i++) {
    //determine the nation the vehicle is driving in based on sign identity
    if (trafficSignRegelems[i]->type().substr(0, 2) == lanelet::traffic_rules::Germany) {
      signType = TrafficSignOrigin::German;
    }
    else if (trafficSignRegelems[i]->type().substr(0, 2) == lanelet::traffic_rules::America) {
      signType = TrafficSignOrigin::American;
    }
    else if (trafficSignRegelems[i]->type().substr(0, 2) == lanelet::traffic_rules::Canada) {
      signType = TrafficSignOrigin::Canadian;
    } //END IF

    //if the given traffic sign is of German origin, check for the following signs
    if (signType == TrafficSignOrigin::German) {
      //determine the sign type and add the priority value to the total sum
      if (trafficSignRegelems[i]->type() == germanSignIDs.STOP) {
        totalSum += germanSignIDs.STOP206[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.PRIORITY_UNTIL) {
        totalSum += germanSignIDs.PRIORITY306[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.PRIORITY_NEXT_INTERSECTION) {
        totalSum += germanSignIDs.PRIORITY301[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.YEILD) {
        totalSum += germanSignIDs.YEILD205[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.RIGHT_TRAFFIC_PRIORITY) {
        totalSum += germanSignIDs.TRAFFICROY102[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.RIGHT_TURN_ARROW) {
        totalSum += germanSignIDs.RIGHTAFTERSTOP720[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.LEFT_TURN_PRIORITY_FOUR_WAY) {
        totalSum += germanSignIDs.INTERSECTION10[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.LEFT_TURN_PRIORITY_THREE_WAY_VERTICAL) {
        totalSum += germanSignIDs.INTERSECTION12[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.LEFT_TURN_PRIORITY_THREE_WAY_HORIZONTAL) {
        totalSum += germanSignIDs.INTERSECTION13[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.RIGHT_TURN_PRIORITY_FOUR_WAY) {
        totalSum += germanSignIDs.INTERSECTION20[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.RIGHT_TURN_PRIORITY_THREE_WAY_VERTICAL) {
        totalSum += germanSignIDs.INTERSECTION22[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.RIGHT_TURN_PRIORITY_THREE_WAY_HORIZONTAL) {
        totalSum += germanSignIDs.INTERSECTION23[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.NO_TURN_PRIORITY_RIGHT_FOUR_WAY) {
        totalSum += germanSignIDs.INTERSECTION11[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.NO_TURN_PRIORITY_RIGHT_THREE_WAY) {
        totalSum += germanSignIDs.INTERSECTION14[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.NO_TURN_PRIORITY_LEFT_FOUR_WAY) {
        totalSum += germanSignIDs.INTERSECTION21[index];
      }
      else if (trafficSignRegelems[i]->type() == germanSignIDs.NO_TURN_PRIORITY_LEFT_THREE_WAY) {
        totalSum += germanSignIDs.INTERSECTION24[index];
      } //END IF
    }
    //if the given traffic sign is of American origin, check for the following signs
    else if (signType == TrafficSignOrigin::American) {
      //checks the type of sign and adds a priority value based off that score
      if (trafficSignRegelems[i]->type() == americanSignIDs.STOP) {
        totalSum += americanSignIDs.STOP_VALUE;
      }
      else if (trafficSignRegelems[i]->type() == americanSignIDs.YEILD) {
        totalSum += americanSignIDs.YEILD_VALUE;
      }
      else if (trafficSignRegelems[i]->type() == americanSignIDs.MULTI_WAY_STOP) {
        totalSum += americanSignIDs.STOP_VALUE;
      } //END IF
    }
    //if the given traffic sign is of Canadian origin, check for the following signs
    else if (signType == TrafficSignOrigin::Canadian) {
      //checks the type of sign and adds a priority value based off that score
      if (trafficSignRegelems[i]->type() == canadianSignIDs.STOP) {
        totalSum += canadianSignIDs.STOP_VALUE;
      }
      else if (trafficSignRegelems[i]->type() == canadianSignIDs.ALL_WAY_STOP) {
        totalSum += canadianSignIDs.STOP_VALUE;
      }
      else if (trafficSignRegelems[i]->type() == canadianSignIDs.YEILD) {
        totalSum += canadianSignIDs.YEILD_VALUE;
      }
      else if (trafficSignRegelems[i]->type() == canadianSignIDs.YEILD_TAB) {
        totalSum += canadianSignIDs.YEILD_VALUE;
      } //END IF
    } //END IF
  } //END IF

  //return the lanelets priority value
  return totalSum;
} //END priorityValue

/*
Input:
  const lanelet::ConstLanelet & egoLanelet - Contains the ego lanelet.
  const lanelet::ConstLanelet & otherLanelet - Contains the other lanelet.
  const Direction turnTypeEgo - Contains the ego vehicles turn direction.
  const Direction turnTypeOther - Contains the other vehicles turn direction.
Output:
  bool hasPriority - Determines if the ego lanelet has a higher priority then the other lanelet.
Method:
  Compares the priority between lanelets to see if the ego lanelet has a greater priority then the other lanelet.
Description:
  Checks if one vehicle has priority over another vehicle given their respective lanelets.
*/
bool ExtendedTrafficRules::hasPriority(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet,
                                       const Direction turnTypeEgo, const Direction turnTypeOther) const {
                                       //END hasPriority

  //determines if one vehicle has priority over another vehicle
  if (priorityValue(egoLanelet, turnTypeEgo) > priorityValue(otherLanelet, turnTypeOther)) {
    //the ego vehicle has priority over the other vehicle
    return true;
  } //END IF

  //the ego vehicle does not have priority over the other vehicle
  return false;
} //END hasPriority

/*
Input:
  const int egoLanelet - Contains the ego lanelets fixed priority value.
  const Direction turnTypeEgo - Contains the ego vehicles turn direction.
  const Direction turnTypeOther - Contains the other vehicles turn direction.
Output:
  bool hasPriority - Determines if the ego lanelet has a higher priority then the other lanelet.
Method:
  Compares the priority between lanelets to see if the ego lanelet has a greater priority then the other lanelet.
Description:
  Override for has priority given a fixed priority value for the reference lanelet.
  Checks if one vehicle has priority over another vehicle given their respective lanelets.
*/
bool ExtendedTrafficRules::hasPriority(const int egoLanelet, const lanelet::ConstLanelet & otherLanelet, const Direction turnTypeOther) const {
  //determines if one vehicle has priority over another vehicle
  if (egoLanelet > priorityValue(otherLanelet, turnTypeOther)) {
    //the ego vehicle has priority over the other vehicle
    return true;
  } //END IF

  //the ego vehicle does not have priority over the other vehicle
  return false;
} //END hasPriority

/*
Input:
  const lanelet::ConstLanelet & egoLanelet - Contains the ego lanelet.
  const lanelet::ConstLanelet & otherLanelet - Contains the other lanelet.
  const int otherLanelet - Contains the other lanelets fixed priority value.
Output:
  bool hasPriority - Determines if the ego lanelet has a higher priority then the other lanelet.
Method:
  Compares the priority between lanelets to see if the ego lanelet has a greater priority then the other lanelet.
Description:
  Override for has priority given a fixed priority value for the other lanelet.
  Checks if one vehicle has priority over another vehicle given their respective lanelets.
*/
bool ExtendedTrafficRules::hasPriority(const lanelet::ConstLanelet & egoLanelet, const Direction turnTypeEgo, const int otherLanelet) const {
  //determines if one vehicle has priority over another vehicle
  if (priorityValue(egoLanelet, turnTypeEgo) > otherLanelet) {
    //the ego vehicle has priority over the other vehicle
    return true;
  } //END IF

  //the ego vehicle does not have priority over the other vehicle
  return false;
} //END hasPriority

/*
Input:
  const int egoLanelet - Contains the ego lanelets fixed priority value.
  const int otherLanelet - Contains the other lanelets fixed priority value.
Output:
  bool hasPriority - Determines if the ego lanelet has a higher priority then the other lanelet.
Method:
  Compares the priority between lanelets to see if the ego lanelet has a greater priority then the other lanelet.
Description:
  Override for has priority given a fixed priority value for the reference lanelet and the other lanelet.
  Checks if one vehicle has priority over another vehicle given their respective lanelets.
*/
bool ExtendedTrafficRules::hasPriority(const int egoLanelet, const int otherLanelet) const {
  //determines if one vehicle has priority over another vehicle
  if (egoLanelet > otherLanelet) {
    //the ego vehicle has priority over the other vehicle
    return true;
  } //END IF

  //the ego vehicle does not have priority over the other vehicle
  return false;
} //END hasPriority

/*
Input:
  const lanelet::ConstLanelet & egoLanelet - Contains the ego lanelet.
  const lanelet::ConstLanelet & otherLanelet - Contains the other lanelet.
  const Direction turnTypeEgo - Contains the ego vehicles turn direction.
  const Direction turnTypeOther - Contains the other vehicles turn direction.
Output:
  bool hasPriority - Determines if the ego lanelet has the same priority then the other lanelet.
Method:
  Compares the priority between lanelets to see if the ego lanelet has the same priority then the other lanelet.
Description:
  Determines if one vehicle has the same priority as another vehicle.
*/
bool ExtendedTrafficRules::samePriority(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet,
                                        const Direction turnTypeEgo, const Direction turnTypeOther) const {
                                        //END samePriority

  //determines if two vehicles have the same priority
  if (priorityValue(egoLanelet, turnTypeEgo) == priorityValue(otherLanelet, turnTypeOther)) {
    //the ego vehicle has priority over the other vehicle
    return true;
  } //END IF

  //the ego vehicle does not have priority over the other vehicle
  return false;
} //END samePriority

/*
Input:
  const double distance - Represents the distance between the initial and final position..
  const double range - Represents the range between the initial and final position.
Output:
  Direction getDirection - Returns an enumeration value representing the direction of the vehicle.
Method:
  Uses an approximation formula to find the direction of the turn.
Description:
  Gets the direction of vehicle turns as an approximation.
  The function is used only for static test cases defined in the test script.
*/
Direction ExtendedTrafficRules::getDirection(const double distance, const double range) const {
  //used to approximate turn directions on a simple two-dimensional test-intersection
  if (abs(distance) > abs(range)*3) {
    return Straight;
  }
  else if (distance == 0) {
    return Straight;
  }
  else if (distance < 0) {
    return Left;
  }
  else if (distance > 0) {
    return Right;
  } //END IF

  //return out of range error
  return Error;
} //END getDirection

/*
Input:
  const lanelet::ConstLanelet & egoLanelet - Represents the lanelet for the ego vehicle.
  const lanelet::ConstLanelet & otherLanelet - Represents the lanelet for the other vehicle
  const lanelet::Lanelets mapData - Contains a Lanelets representation for the OSM map.
  const Direction egoTurn - Contains an enumeration value representing the turn direction of the ego vehicle.
Output:
  bool oncomingLanes - Determines if the other vehicle is oncoming to the ego vehicle.
Method:
  Determines if a vehicle is oncoming to another vehicle by referencing is incoming left of and changing the turn type and ordering of the ego and other lanelet.
Description:
  Determines if one lanelet is oncoming to another lanelet.
  The function is used only for static test cases defined in the test script.
*/
bool ExtendedTrafficRules::oncomingLanes(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet,
                                         const lanelet::Lanelets mapData, const Direction egoTurn) const {
                                         //END oncomingLanes

  //determines if one vehicle is on the oncoming lane of another vehicle
  return isIncomingLeftOf(otherLanelet, egoLanelet, mapData, egoTurn);
} //END oncomingLanes

/*
Input:
  const lanelet::ConstLanelet & egoLanelet - Represents the lanelet for the ego vehicle.
  const lanelet::ConstLanelet & otherLanelet - Represents the lanelet for the other vehicle
  const lanelet::Lanelets mapData - Contains a Lanelets representation for the OSM map.
  const Direction egoTurn - Contains an enumeration value representing the turn direction of the ego vehicle.
Output:
  bool isIncomingLeftOf - Determines if the other vehicle is incoming corresponding to the ego vehicles turn.
Method:
  Determines if one vehicle is incoming to another vehicle by referencing the ego vehicle turn and determining the direction between the oncoming
  vehicle and the other vehicle.
Description:
  Determines if one lanelet is incoming to another lanelet.
  The function is used only for static test cases defined in the test script.
*/
bool ExtendedTrafficRules::isIncomingLeftOf(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet,
                                            const lanelet::Lanelets mapData, const Direction egoTurn) const {
                                            //END isIncomingLeftOf

  //stores the direction of a vehicle turn
  Direction turnDirection;

  //contains data for vehicle geometry
  double distance, length;
  double lengthFrontToFront;
  double lengthFrontToBack;

  //contains data for lanelet geometry values
  lanelet::ConstLineString3d egoCenterline = egoLanelet.centerline();

  //contains the start and end point for the vehicle lanelets
  lanelet::ConstPoint3d startPoint = otherLanelet.centerline().front();
  lanelet::ConstPoint3d endPoint = otherLanelet.centerline().back();
  lanelet::ConstPoint3d frontmostPoint = egoCenterline.front();

  //converts the points to two-dimensional values
  lanelet::ConstPoint2d startPointFlat = lanelet::utils::to2D(startPoint);
  lanelet::ConstPoint2d endPointFlat = lanelet::utils::to2D(endPoint);
  lanelet::ConstPoint2d frontmostPointFlat = lanelet::utils::to2D(frontmostPoint);

  //obtain the distance between the centerline and the starting and ending points of the other lanelet
  lengthFrontToFront = lanelet::geometry::toArcCoordinates(lanelet::utils::to2D(egoCenterline), startPointFlat).distance;
  lengthFrontToBack = lanelet::geometry::toArcCoordinates(lanelet::utils::to2D(egoCenterline), endPointFlat).distance;

  //gets the length of the lanelet from the front of the reference lanelet to the front of the other lanelet
  lengthFrontToFront = (frontmostPointFlat.x() - startPointFlat.x())*(frontmostPointFlat.x() - startPointFlat.x()) +
                       (frontmostPointFlat.y() - startPointFlat.y())*(frontmostPointFlat.y() - startPointFlat.y());
  //gets the length of the lanelet from the back of the reference lanelet to the front of the other lanelet
  lengthFrontToBack =  (frontmostPointFlat.x() - endPointFlat.x())*(frontmostPointFlat.x() - endPointFlat.x()) +
                       (frontmostPointFlat.y() - endPointFlat.y())*(frontmostPointFlat.y() - endPointFlat.y());

  //checks if the lanelet is not reversed, if so it is not an incoming lane
  if (lengthFrontToFront < lengthFrontToBack) {
    return false;
  } //END IF

  //checks the distance between the reference lanelets centerline and the start of the other lanelet
  distance = lanelet::geometry::toArcCoordinates(lanelet::utils::to2D(egoCenterline), startPointFlat).distance;
  length = lanelet::geometry::toArcCoordinates(lanelet::utils::to2D(egoCenterline), startPointFlat).length;

  //approximates turn direction by using predefined four-way intersection geometry as an underlying assumption
  turnDirection = getDirection(distance, length);

  //if the turn direction of the vehicle is equal to the turn direction requested
  if (turnDirection == egoTurn) {
    //the reference and other lanelets are orientated as desired
    return true;
  } //END IF

  //the reference and other lanelet are not orientated such that the other lanelet is incoming to the reference lanelet
  return false;
} //END isIncomingLeftOf

/*
Input:
  const lanelet::ConstLanelet & lanelet - The given lanelet being referenced.
  const lanelet::Point3d vehicle - The position of the vehicle being referenced as a 3D point.
Output:
  struct stopLineData getTrafficLightStopLine - Contains information related to the stop line and its important properties.
Method:
  Checks the distance between the vehicle and the traffic light stop line.
  Extracts the distance between the vehicle and the stop line along with the index of the stop line in the TrafficLight
  regulatory elements vector and stored them in the struct.
Description:
  Gets the stop line elements for the traffic light.
  The function is used only for static test cases defined in the test script.
*/
struct stopLineData ExtendedTrafficRules::getTrafficLightStopLine(const lanelet::ConstLanelet & lanelet, const lanelet::Point3d vehicle) const {
  //gets the traffic light regulatory element data
  auto trafficLightRegelems = lanelet.regulatoryElementsAs<TrafficLight>();

  //contains the elements related to the regulatory elements
  struct stopLineData elements;

  //gets the index of relevant traffic light regulatory element data
  int index = Default::None;

  //contains distances between vehicles and relevant elements
  double stopLineDist = 0;
  double vehicleDist = 0;
  double totalDist = 0;
  double lowestDist = Default::None;

  //contains relevant lanelet geometry data
  lanelet::ConstPoint3d pointOne;
  lanelet::ConstPoint3d pointTwo;
  lanelet::ConstPoint3d left;
  lanelet::ConstPoint3d right;
  lanelet::Point2d centerPointFlat;
  lanelet::Point2d vehiclePosition;
  lanelet::LineString2d roadStartFlat;

  //contains the stopline for the traffic light regulatory element
  lanelet::Optional<lanelet::ConstLineString3d> trafficLightOpt;

  //obtains the number of traffic light regulatory elements
  for (size_t i=0; i < trafficLightRegelems.size(); i++) {
    //gets the currently iterated traffic light regulatory element
    trafficLightOpt = trafficLightRegelems[i]->stopLine();

    //the traffic light regulatory element must have a real stop line position for the rule formulation to apply
    if (trafficLightOpt) {
      //gets the two-dimensional position of the vehicle
      vehiclePosition = utils::to2D(vehicle);

      //gets the front and back points for the stop line
      pointOne = (*trafficLightOpt).front();
      pointTwo = (*trafficLightOpt).back();

      //gets the centerpoint for the stop line
      lanelet::Point3d centerPoint(lanelet::utils::getId(),
                                  (pointOne.x() + pointTwo.x())/2,
                                  (pointOne.y() + pointTwo.y())/2,
                                  (pointOne.z() + pointTwo.z())/2);
      //END centerPoint

      //gets the centerpoint for the stop line in two-dimensions
      centerPointFlat = utils::to2D(centerPoint);

      //gets the front points for the left and right lanelet bounds
      left = lanelet.leftBound().front();
      right = lanelet.rightBound().front();

      //gets the front of the lanelet as a line
      lanelet::Point3d leftPoint(lanelet::utils::getId(), left.x(), left.y(), left.z()),
                       rightPoint(lanelet::utils::getId(), right.x(), right.y(), right.z());
      lanelet::LineString3d roadStart(utils::getId(), {leftPoint, rightPoint});

      //gets the front of the lanelet as a two-dimensional line
      roadStartFlat = utils::to2D(roadStart);

      //gets the distance between the vehicle and the front the of the lanelet as magnitudes
      vehicleDist = abs(lanelet::geometry::toArcCoordinates(roadStartFlat, vehiclePosition).distance);
      stopLineDist = abs(lanelet::geometry::toArcCoordinates(roadStartFlat, centerPointFlat).distance);

      //finds the difference in the distance between the front of the lanelet and the vehicle along the lanelet
      totalDist = stopLineDist - vehicleDist;

      //if the distance is greater then zero then the vehicle is still on the lanelet
      if (totalDist >= 0) {
        //if the currently iterated stop line is closer to the vehicle then the previously iterated stop lines
        if (lowestDist < 0 || totalDist < lowestDist) {
          //track the distance between the vehicle and the stop line
          lowestDist = totalDist;

          //track the index of the regulatory element
          index = i;
        } //END IF
      } //END IF
    } //END IF
  } //END FOR

  //store the index of the traffic light regulatory element and the distance between the vehicle and the stop line for the regulatory element
  elements.index = index;
  elements.lowestDist = lowestDist;

  //return the traffic light elements
  return elements;
} //END getTrafficLightStopLine

/*
Input:
  const lanelet::ConstLanelet & lanelet - The given lanelet being referenced.
  const lanelet::Point3d vehicle - The position of the vehicle being referenced as a 3D point.
Output:
  struct stopLineData getSignStopLine - Contains information related to the stop line and its important properties.
Method:
  Checks the distance between the vehicle and the stop sign stop line.
  Extracts the distance between the vehicle and the stop line along with the index of the stop line in the TrafficSign
  regulatory elements vector and stored them in the struct.
Description:
  Gets the stop line for any stop signs on the given lanelet.
  The function is used only for static test cases defined in the test script.
*/
struct stopLineData ExtendedTrafficRules::getSignStopLine(const lanelet::ConstLanelet & lanelet, const lanelet::Point3d vehicle) const {
  //gets the sign regulatory elements
  auto signData = lanelet.regulatoryElementsAs<TrafficSign>();

  //used to contain stop sign elements
  struct stopLineData elements;

  //used to contain the index of the stop sign as a regulatory element
  int index = Default::None;

  //containers for regulatory element data
  double greenArrow = 0;
  double stopLineDist = 0;
  double vehicleDist = 0;
  double totalDist = 0;
  double lowestDist = Default::None;
  double greenArrowDist = Default::None;

  //containers for lanelet geometry data
  lanelet::ConstLineStrings3d trafficSignOpt;
  lanelet::ConstPoint3d pointOne;
  lanelet::ConstPoint3d pointTwo;
  lanelet::ConstPoint3d left;
  lanelet::ConstPoint3d right;
  lanelet::Point2d centerPointFlat;
  lanelet::Point2d position;
  lanelet::LineString2d roadStartFlat;

  //for every regulatory element present on the current lanelet
  for (size_t i=0; i < signData.size(); i++) {
    //obtain the regulatory element reference lines
    trafficSignOpt = signData[i]->refLines();

    //if there are no reference lines present, the formulated traffic rules do not apply
    if (trafficSignOpt.size() != 0) {
      //takes the front and back points for the lanelet
      pointOne = trafficSignOpt.front().front();
      pointTwo = trafficSignOpt.front().back();

      //calculates the centerpoint for the lanelet
      lanelet::Point3d centerPoint(lanelet::utils::getId(),
                                  (pointOne.x() + pointTwo.x())/2,
                                  (pointOne.y() + pointTwo.y())/2,
                                  (pointOne.z() + pointTwo.z())/2);
                                  //END centerPoint

      //gets the left and right front bounds for the lanelet
      left = lanelet.leftBound().front();
      right = lanelet.rightBound().front();

      //creates a line between the left and right front bound points
      lanelet::Point3d leftPoint(lanelet::utils::getId(), left.x(), left.y(), left.z()),
                      rightPoint(lanelet::utils::getId(), right.x(), right.y(), right.z());
      lanelet::LineString3d roadStart(utils::getId(), {leftPoint, rightPoint});

      //gets the front of the lanelet as a two-dimensional line
      roadStartFlat = utils::to2D(roadStart);

      //gets the lanelet centerpoint as a two-dimensional point
      centerPointFlat = utils::to2D(centerPoint);

      //gets the vehicle position as a two dimensional point
      position = utils::to2D(vehicle);

      //gets the distance between the vehicle and the front of the lanelet
      vehicleDist = abs(lanelet::geometry::toArcCoordinates(roadStartFlat, position).distance);
      stopLineDist = abs(lanelet::geometry::toArcCoordinates(roadStartFlat, centerPointFlat).distance);

      //finds distance between vehicle and the front of the lanelet
      totalDist = stopLineDist - vehicleDist;
    } //END IF

    //if the vehicle if not past the front of the lanelet
    if (totalDist >= 0) {
      //if there is a green turn arrow sign present on the road
      if (signData[i]->type() == "de720") {
        //flag the road as having a turn marker
        greenArrow = 1;

        //get the distance to the turn marker
        greenArrowDist = totalDist;
      } //END IF

      //if the vehicle is on the lanelet
      if (lowestDist < 0 || totalDist < lowestDist) {
        //finds the distance from the vehicle to the front of the lanelet
        lowestDist = totalDist;

        //gets the index of the regulatory element being considered
        index = i;
      } //END IF
    } //END IF
  } //END FOR

  //push back elements to the vector
  elements.index = index;
  elements.lowestDist = lowestDist;
  elements.greenArrow = greenArrow;
  elements.greenArrowDist = greenArrowDist;

  //returns regulatory element data to the user
  return elements;
} //END getSignStopLine

/*
Input:
  const lanelet::ConstLanelet & lanelet - The given lanelet being referenced.
  const lanelet::Point3d vehicle - The position of the vehicle being referenced as a 3D point.
Output:
  struct stopLineData getSignStopLine - Contains information related to the stop line and its important properties.
Method:
  Checks the distance between the vehicle and the AllWayStop stop line.
  Extracts the distance between the vehicle and the stop line along with the index of the stop line in the AllWayStop
  regulatory elements vector and stored them in the struct.
Description:
  Gets the stop line for any AllWayStop's on the given lanelet.
  The function is used only for static test cases defined in the test script.
*/
struct stopLineData ExtendedTrafficRules::getAllWayStopLine(const lanelet::ConstLanelet & lanelet, const lanelet::Point3d vehicle) const {
  //contains all way stop sign data
  auto signData = lanelet.regulatoryElementsAs<AllWayStop>();

  //contains the elements related to the all way stop data
  struct stopLineData elements;

  //contains the index of the all way stop that is relevant
  int index = Default::None;

  //contains attributes relevant to the all way stop
  double stopLineDist = 0;
  double vehicleDist = 0;
  double lowestDist = Default::None;

  //for every all way stop the given lanelet references
  for (size_t i = 0; i < signData.size(); i++) {
    //obtain the given stop line for the vehicle
    lanelet::Optional<lanelet::ConstLineString3d> stopLineLocation = signData[i]->getStopLine(lanelet);

    //if an all way stop line has a stop line referenced by the lanelet
    if (stopLineLocation) {
      //get the distance along the lanelet to the vehicle
      vehicleDist = lanelet::geometry::toArcCoordinates(lanelet::utils::to2D(lanelet.centerline()), lanelet::utils::to2D((*stopLineLocation).front())).distance;

      //get the distance along the lanelet to the stop line
      stopLineDist = lanelet::geometry::toArcCoordinates(lanelet::utils::to2D(lanelet.centerline()), lanelet::utils::to2D(vehicle)).distance;

      //get the distance along the lanelet to the vehicle and compare it with the distance along the lanelet to the stop line
      if (abs(stopLineDist) - abs(vehicleDist) >= 0) {
        index = i;
      } //END IF
    } //END IF
  } //END FOR

  //store the relevant elements in a vector
  elements.index = index;
  elements.lowestDist = lowestDist;

  //return the vector
  return elements;
} //END getAllWayStopLine

/*
Input:
  const lanelet::ConstLanelet & lanelet - The given lanelet being referenced.
  const lanelet::Point3d vehicle - The position of the vehicle being referenced as a 3D point.
Output:
  bool shouldStop - Determines if a vehicle should stop given its position on a lanelet.
Method:
  Determines if there is a stop sign referenced by the lanelet and the vehicle is behind the stop line.
  Alternatively determines there is an AllWayStopLine element referenced by the lanelet and the vehicle is behind the stop line.
  Alternatively determines there is an TrafficLight element referenced by the lanelet and the vehicle is behind the stop line.
Formulation:
  R-IN1 is formulated in Formalization of Intersection Traffic Rules in Temporal Logic:
    https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
  R-IN1:
    G((passing_stop_line(x_ego) ∧ at_traffic_sign(x_ego, stop) ∧ ¬relevant_traffic_light(x_ego))
      ==> (O(G_[0,t_s|w](stop_line_in_front(x_ego) ∧ in_standstill(x_ego))))
Description:
  The ego vehicle has to stop with respect to a stop sign (sign 206) before it enters the intersection at least for a duration of
  t_s|w in front of the associated stop line.
  Determines if a vehicle should stop given a lanelet and a reference point for a vehicle.
  The function is used only for static test cases defined in the test script.
*/
bool ExtendedTrafficRules::shouldStop(const lanelet::ConstLanelet & lanelet, const lanelet::Point3d vehicle) const {
  //get the traffic sign regulatory elements for the given lanelet
  auto trafficSignRegelems = lanelet.regulatoryElementsAs<TrafficSign>();

  //get data for stop signs and traffic lights
  struct stopLineData allWayStopData = getAllWayStopLine(lanelet, vehicle);
  struct stopLineData stopLineData = getSignStopLine(lanelet, vehicle);
  struct stopLineData trafficLightData = getTrafficLightStopLine(lanelet, vehicle);

  //if a stop sign is present on the lanelet and the vehicle is behind the stop signs stop line
  if (stopLineData.index != Default::None && (trafficLightData.lowestDist > stopLineData.lowestDist || trafficLightData.index == Default::None ||
      trafficLightData.lowestDist == Default::None)) {

    if (trafficSignRegelems[stopLineData.index]->type() == "de206") {
      //the vehicle should stop at the signs stop line
      return true;
    } //END IF
  } //END IF

  //if an all way stop segment is present and the vehicle is behind the stop line for the intersection
  if (allWayStopData.index != Default::None && (trafficLightData.lowestDist > allWayStopData.lowestDist || trafficLightData.index == Default::None ||
      trafficLightData.lowestDist == Default::None)) {

    //the vehicle should stop at the all way stop
    return true;
  } //END IF

  //the vehicle should not stop
  return false;
} //END shouldStop

/*
Input:
  const lanelet::ConstLanelet & lanelet - The given lanelet being referenced.
  const Direction turn - Represents the direction of the vehicle turn.
  const Direction light - Represents the traffic-direction of the light the vehicle is viewing.
  const Colour colour - Represents the colour of the light the vehicle is viewing.
  const double travelledDistanceTillStandstill - Represents the distance the vehicle can travel until needing to come to a stop.
  const lanelet::Point3d vehicle - The position of the vehicle being referenced as a 3D point.
Output:
  bool shouldWaitAtTrafficLight - Determines if a vehicle should stop at the given traffic light.
Method:
  Gather data for the given lanelet and compares it to TrafficLight regulatory elements to determine if
  the vehicle should stop at the light given the lights conditions.
Formulation:
  R-IN2 is formulated in Formalization of Intersection Traffic Rules in Temporal Logic:
    https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
  R-IN2:
    G((turning left(x_ego) ∧ at_traffic_light(x_ego, left, red) ∨ at_traffic_light(x_ego, left, yellow)
        ∧ (braking_intersection_possible(x_ego) S ¬at_traffic_light(x_ego, left, yellow)))
      ∨ (going straight(x_ego) ∧ (at_traffic_light(x_ego, straight, red) ∨ at_traffic_light(x_ego, straight, yellow)
        ∧ (braking_intersection_possible(x_ego) S ¬at_traffic_light(x_ego, straight, yellow)))
      ∨ turning right(x_ego) ∧ at_traffic_light(x_ego, right, red) ∨ at_traffic_light(x_ego, right, yellow)
        ∧ (braking_intersection_possible(x_ego) S ¬at_traffic_light(x_ego, right, yellow))) ∧ ¬at_traffic_sign(x_ego, greenArrow))
      ==> (on _lanelet_with_type(x_ego, intersection) ∧ ¬passing stop line(x_ego)))
Description:
  The ego vehicle is not allowed to cross a red traffic light. If the traffic light is yellow and the ego vehicle can come to
  a standstill in front of the intersection without falling below an acceleration threshold, the ego vehicle is not allowed to cross a yellow traffic light.
*/
bool ExtendedTrafficRules::shouldWaitAtTrafficLight(const lanelet::ConstLanelet & lanelet, const Direction turn, const Direction light, const Colour colour,
                                                    const double travelledDistanceTillStandstill, const lanelet::Point3d vehicle) const {
                                                    //END shouldWaitAtTrafficLight

  //containers for traffic sign and vehicle elements
  bool greenArrow, breaking;
  struct stopLineData arrowData, trafficLightData;

  //finds the traffic light regulatory elements
  auto trafficLightRegelems = lanelet.regulatoryElementsAs<TrafficLight>();

  //gets the position of traffic lights on the lane
  std::shared_ptr<const lanelet::TrafficLight> tlRegelem = trafficLightRegelems.front();
  lanelet::ConstLineStringOrPolygon3d lights = tlRegelem->trafficLights().front();

  //gets the position of any right turn green arrow and traffic light stop lines
  arrowData = getSignStopLine(lanelet, vehicle);
  trafficLightData = getTrafficLightStopLine(lanelet, vehicle);

  //checks if breaking is possible when trying to stop at a stop line
  breaking = canStandstillAtLights(lanelet, travelledDistanceTillStandstill, trafficLightData, vehicle);

  //holds information for right turn green arrow positions
  greenArrow = false;

  //if no traffic light data exists then exit with false
  if (trafficLightData.index) {
    //the vehicle does not need to break since there is not traffic light
    return false;
  } //END IF

  //check if a right turning green arrow is present and is in front of or at the stop line
  if (arrowData.greenArrow == 1 && arrowData.greenArrowDist <= trafficLightData.lowestDist) {
    greenArrow = true;
  } //END IF

  //if the vehicle is turning left and their is a red light or alternatively there is a yellow light and breaking is impossible
  if (turn == Direction::Left && ((colour == Colour::Red && light == Direction::Left) || ((colour == Colour::Yellow && light == Left) && breaking == true))) {
    return true;
  } //END IF

  //if the vehicle is turning straight and their is a red light or alternatively there is a yellow light and breaking is impossible
  if (turn == Direction::Straight && ((colour == Colour::Red && light == Direction::Straight) || ((colour == Colour::Yellow && light == Direction::Straight) && breaking == true))) {
    return true;
  } //END IF

  //if the vehicle is turning right and their is a red light or alternatively there is a yellow light and breaking is impossible
  if (turn == Direction::Right && ((colour == Colour::Red && light == Direction::Right) || ((colour == Colour::Yellow && light == Right) && (breaking == true && greenArrow == false)))) {
    return true;
  } //END IF

  //the vehicle does not need to break at the traffic light
  return false;
} //END shouldWaitAtTrafficLight

/*
Input:
  const lanelet::ConstLanelet & egoLanelet - The given ego lanelet being referenced.
  const lanelet::ConstLanelet & otherLanelet - The given other lanelet being referenced.
  const Direction egoTurn - Represents the direction of the ego vehicle turn.
  const Direction otherTurn - Represents the direction of the other vehicle turn.
  const lanelet::Lanelets mapData - Contains a Lanelets representation for the OSM map.
  const lanelet::Point3d egoPos - References the position of the ego vehicle on the OSM map.
  const lanelet::Point3d otherPos - References the position of the other vehicle on the OSM map.
Output:
  bool shouldTurnRightBeforeLeft - Signals if the following rule was violated by the ego vehicle.
Method:
  Determines if the following vehicle should turn right before left by referencing the corresponding lanelet elements.
  Checks regulatory elements located on the lanelets to determine if the rules apply.
Formulation:
  R-IN3 is formulated in Formalization of Intersection Traffic Rules in Temporal Logic:
    https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
  R-IN3:
    We notice the formulation is as follows:
      G((on_incoming_left_of(x_ego, x_o) ∧ ¬relevant_traffic_light(x_ego) ∧
        (turning_right(x_ego) ∧ turning_right(x_o) ∧ same_priority(x_ego, x_o, right, right)
            --- we skip some combinations of turning directions here ---
        ∨ turning_left(x_ego) ∧ going_straight(x_o) ∧ same_priority(x_ego, x_o, left, straight)))
          ==> (G(not_endanger_intersection(x_ego, x_o)) ∨ ¬(on_lanelet_with_type(x_ego, intersection)))
Description:
  Checks if the ego vehicle is incoming left of another vehicle and their paths are crossing an intersection.
  The ego vehicle is only allowed to enter the intersection if it does not endanger the other vehicle.
  Right before left does not apply if traffic lights regulate the intersection.
  The function is used only for static test cases defined in the test script.
*/
bool ExtendedTrafficRules::shouldTurnRightBeforeLeft(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet,
                                                     const Direction egoTurn, const Direction otherTurn, const lanelet::Lanelets mapData,
                                                     const lanelet::Point3d egoPos, const lanelet::Point3d otherPos) const {
                                                     //END shouldTurnRightBeforeLeft

  //contains elements related to signs and lanelet elements
  bool onIncomingLeftOf, sPriority, relevantTrafficLights;
  struct stopLineData trafficLightDataEgo, trafficLightDataOther;

  //checks if there is another vehicle incoming left of the current vehicle
  onIncomingLeftOf = isIncomingLeftOf(egoLanelet, otherLanelet, mapData, Left);
  //checks if two vehicles share the same priority as one another on their respective lanelets
  sPriority = samePriority(egoLanelet, otherLanelet, egoTurn, otherTurn);
  //assume there is a relevant traffic light on the lanelet
  relevantTrafficLights = true;

  //checks if there are any traffic lights on the ego lane or the other vehicles lane
  trafficLightDataEgo = getTrafficLightStopLine(egoLanelet, egoPos);
  trafficLightDataOther = getTrafficLightStopLine(otherLanelet, otherPos);

  //checks if there are any traffic lights on the lanelets in consideration
  if (trafficLightDataEgo.index == Default::None && trafficLightDataOther.index == Default::None) {
    //determines there are no relevant traffic lights and that we can proceed
    relevantTrafficLights = false;
  }

  /*if the other vehicle is on the incoming left of the ego vehicle and the vehicles share the same priority on their respective lanelets
  and there are no relevant traffic lights on either vehicles lanelet*/
  if (onIncomingLeftOf && sPriority && !relevantTrafficLights) {
    //return that right-before-left applies to the ego vehicle and that it should not enter the intersection
    return true;
  } //END IF

  //return that right-before-left does not apply to the ego vehicle
  return false;
} //END shouldTurnRightBeforeLeft

/*
Input:
  const lanelet::ConstLanelet & egoLanelet - The given ego lanelet being referenced.
  const lanelet::ConstLanelet & otherLanelet - The given other lanelet being referenced.
  const Direction egoTurn - Represents the direction of the ego vehicle turn.
  const Direction otherTurn - Represents the direction of the other vehicle turn.
  const lanelet::Lanelets mapData - Contains a Lanelets representation for the OSM map.
  const lanelet::Point3d egoPos - References the position of the ego vehicle on the OSM map.
  const lanelet::Point3d otherPos - References the position of the other vehicle on the OSM map.
Output:
  bool shouldTurnRightBeforeLeft - Signals if the following rule was violated by the ego vehicle.
Method:
  Determines if the following vehicle should turn right before left by referencing the corresponding lanelet elements.
  Checks regulatory elements located on the lanelets to determine if the rules apply.
Formulation:
  R-IN4 is formulated in Formalization of Intersection Traffic Rules in Temporal Logic:
    https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
  R-IN4:
    G((turning_right(x_o) ∧ turning_right(x_ego) ∧ has_priority(x_o, x_ego, right, right)
          --- we skip some combinations of turning directions here ---
      ∨ turning_right(x_o) ∧ turning_left(x_ego) ∧ has_priority(x_o, x_ego, right, left) ∧ ¬on_oncom_of(x_o, x_ego)
      ∨ going_straight(x_o) ∧ turning_left(x_ego) ∧ has_priority(x_o, x_ego, straight, left) ∧ ¬on_oncom_of(x_o, x_ego))
        ==> (G(not_endanger_intersection(x_ego, x_o) ∨ ¬on_lanelet_with_type(x_ego, intersection)))
Description:
  Checks both the ego vehicles and the other vehicles priority to see if the other vehicle has priority at the intersection.
  The ego vehicle is only allowed to enter the intersection if it does not endanger the other vehicle.
  The function is used only for static test cases defined in the test script.
*/
bool ExtendedTrafficRules::shouldHavePriority(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet, const Direction egoTurn,
                                              const Direction otherTurn, const lanelet::Lanelets mapData, const lanelet::Point3d egoPos,
                                              const lanelet::Point3d otherPos) const {
                                              //END shouldHavePriority

  //containers for lanelet elements
  bool hPriority, isOncoming;

  //checks if the other lanelet has priority over the ego lanelet given the turn type
  hPriority = hasPriority(otherLanelet, egoLanelet, otherTurn, egoTurn);

  //if the other lanelet has priority
  if (hPriority) {
    //if there is a left ego turn and right other vehicle turn
    if (egoTurn == Left && otherTurn == Right) {
      //check that the other vehicle is on the oncoming lane of the ego vehicle
      if (!oncomingLanes(egoLanelet, otherLanelet, mapData, Straight)) {
        //return that priority applies and that the ego vehicle should not enter the intersection
        return true;
      } //END IF
      //return that the priority rule does not apply
      return false;
    }
    //if there is a left ego turn and straight other vehicle turn
    else if (egoTurn == Left && otherTurn == Straight) {
      //check that the other vehicle is on the oncoming lane of the ego vehicle
      if (!oncomingLanes(egoLanelet, otherLanelet, mapData, Straight)) {
        //return that priority applies and that the ego vehicle should not enter the intersection
        return true;
      } //END IF
      //return that the priority rule does not apply
      return false;
    } //END IF
    //return that priority applies and that the ego vehicle should not enter the intersection
    return true;
  } //END IF
  //return that the priority rule does not apply
  return false;
} //END shouldHavePriority

/*
Input:
  const lanelet::ConstLanelet & egoLanelet - The given ego lanelet being referenced.
  const lanelet::ConstLanelet & otherLanelet - The given other lanelet being referenced.
  const Direction egoTurn - Represents the direction of the ego vehicle turn.
  const Direction otherTurn - Represents the direction of the other vehicle turn.
  const lanelet::Lanelets mapData - Contains a Lanelets representation for the OSM map.
  const lanelet::Point3d egoPos - References the position of the ego vehicle on the OSM map.
  const lanelet::Point3d otherPos - References the position of the other vehicle on the OSM map.
Output:
  bool shouldTurnRightBeforeLeft - Signals if the following rule was violated by the ego vehicle.
Method:
  Determines if the following vehicle should turn right before left by referencing the corresponding lanelet elements.
  Checks regulatory elements located on the lanelets to determine if the rules apply.
Formulation:
  R-IN5 is formulated in Formalization of Intersection Traffic Rules in Temporal Logic:
    https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
  R-IN5:
    G((turning_left(x_ego) ∧ (going_straight(x_o) ∧ ¬has_priority(x_ego, x_o, left, straight) ∧ on_oncom_of(x_o, x_ego))
      ∨ (turning_right(x_o) ∧ ¬has_priority(x_ego, x_o, left, right) ∧ on_oncom_of(x_o, x_ego)))
        ==> (G(not_endanger_intersection(x_ego, x_o)) ∨ ¬on_lanelet_with_type(x_ego, intersection)))
Description:
  Checks if a left turning ego vehicle lacks priority over oncoming or right turning vehicles.
  The other vehicle cannot be oncoming to the ego vehicle.
  The function is used only for static test cases defined in the test script.
*/
//determines if the should be turning left rule applies between the reference vehicle and the other vehicle given their positions and corresponding lanelets
bool ExtendedTrafficRules::shouldBeTurningLeft(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet, const Direction egoTurn,
                                               const Direction otherTurn, const lanelet::Lanelets mapData, const lanelet::Point3d egoPos,
                                               const lanelet::Point3d otherPos) const {
                                               //END shouldBeTurningLeft

  //container for lanelet elements
  bool hPriority, isOncoming;

  //checks if the ego vehicle has priority over the other vehicle on their respective lanelets
  hPriority = hasPriority(egoLanelet, otherLanelet, egoTurn, otherTurn);

  //if the ego vehicle is turning left and the other vehicle is turning straight
  if (egoTurn == Left && otherTurn == Straight) {
    //check if the other vehicle is on the oncoming lane of the ego vehicle
    isOncoming = oncomingLanes(egoLanelet, otherLanelet, mapData, Straight);

    //if the ego vehicles does not have priority over the other vehicle and the other vehicle is on the oncoming lane of the ego vehicle
    if (hPriority == false && isOncoming == true) {
      //the ego vehicle should not enter the intersection
      return true;
    } //END IF
  }
  else if (egoTurn == Left && otherTurn == Right) {
    //check if the other vehicle is on the oncoming lane of the ego vehicle
    isOncoming = oncomingLanes(egoLanelet, otherLanelet, mapData, Straight);

    //if the ego vehicles does not have priority over the other vehicle and the other vehicle is on the oncoming lane of the ego vehicle
    if (hPriority == false && isOncoming == true) {
      //the ego vehicle should not enter the intersection
      return true;
    } //END IF
  } //END IF

  //the turning left rule does not apply
  return false;
} //END shouldBeTurningLeft
}  // namespace traffic_rules
}  // namespace lanelet
