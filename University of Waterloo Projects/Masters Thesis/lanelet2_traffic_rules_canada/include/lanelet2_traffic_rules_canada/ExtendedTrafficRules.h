//includes the current source file once in a single compilation
#pragma once

//include lanelet2 core functions
#include <lanelet2_core/primitives/Lanelet.h>

//include lanelet2 routing functions
#include <lanelet2_routing/RoutingGraph.h>

//include traffic rule class to build ExtendedTrafficRules with
#include <lanelet2_traffic_rules/GenericTrafficRules.h>

//include optional 
#include <boost/optional.hpp>

/*
Determines how we find the end yaw of a vehicle moving across an intersection:
  IntersectionProjection - Finds the end yaw of the vehicle once it leaves the intersectional or yield lanelets and exits the following lanelets.
                           Problems can arise if Lanelet2 pinpoints the vehicle as being nearest to the wrong lanelet.
  Dataset - Finds the end yaw of the vehicle using the final reported yaw from the interaction datasets momentary intervals.
            Assumes that the interval represents the movement of a vehicle across some specific lanelet.
            If multiple intersections are covered by the interval, then the method will not work correctly.
            It is a lot faster and more accurate to use the Dataset method if possible.
*/
enum YawChecking {IntersectionProjection = 0, Dataset = 1};

/*
Determines if we Should Check for a Specific Stop Sign:
  NoSignCheck - Do not check for a mounted stop sign for the stop line to be considered valid.
  SignCheck - Check for a specific stop sign for the stop line to be considered valid.
*/
enum StopSignReference {NoSignCheck = 1, SignCheck = 2};

/*
Represents the Mode Used to Check Vehicle Stopping Behaviour:
  SingleStop = 1 - Vehicle has a single stop in the intersection
  MultiStop = 2 - Vehicle has multiple stops in the intersection
*/
enum StopDirection {SingleStop = 1, MultiStop = 2};

/*
Boundary Classes for Inspecting Vehicle Position:
  Full = 100 - Percentage of Vehicle length/width to consider from the centerpoint.
  RemoveFrame = 90 - Percentage of Vehicle length/width to consider from the centerpoint.
  RemoveChassis = 80 - Percentage of Vehicle length/width to consider from the centerpoint.
  RemoveHalf = 50 - Percentage of Vehicle length/width to consider from the centerpoint.
  Centre = 0 - Percentage of Vehicle length/width to consider from the centerpoint.
*/
enum BoundaryValues {Full = 100, RemoveFrame = 90, RemoveChassis = 80, RemoveHalf = 50, Centre = 0};

/*
Used to Signal the Mode for Checking Speed Violations:
  UpperLimit - Signified the Value is Intended as an Upper Limit.
  TargetLimit - Signifies that the Value is Intended as a Upper and Lower Limit Within a Specified Threshold.
*/
enum SpeedCheckMode {UpperLimit = 0, TargetLimit = 1};

/*
Contains Directions for Vehicle Turns:
  Left = 0
  Right = 1
  Straight = 2
  Error = -1
*/
enum Direction {Left = 0, Right = 1, Straight = 2, Error = -1}; //END Direction

/*
Represents Bounds for Either the Front or the Back of a Vehicle:
  Front = 0
  Back = 2
  TopLeft = 3
  BottomRight = 4
  TopRight = 5
  BottomLeft = 6
*/
enum Bounds {Front = 0, Back = 2, TopLeft = 3, BottomRight = 4, TopRight = 5, BottomLeft = 6}; //END Bounds

/*
Contains Colours for Traffic Lights:
  Red = 0
  Yellow = 1
  Green = 2
*/
enum Colour {Red = 0, Yellow = 1, Green = 2}; //END Colour

/*
Contains Output Modes for isTailgating:
  All = 1
  StandstillOnly = 2
  DrivingOnly = 3
*/
enum TailgatingMode {All = 1, StandstillOnly = 2, DrivingOnly = 3}; //END TailgatingMode

/*
Contains Default Values and Initializers:
  None = -1
  NO_LINE_ON_LANELET = 1000
*/
enum Default {None = -1, NO_LINE_ON_LANELET = 1000}; //END Default

/*
Used to encode if a point is inside, outside, or at the entry for an intersection:
  IntersectionalEntry - Denotes the entry to an intersection.
  IntersectionInterior - Denotes the interior of the intersection.
  RoadLane - denotes a regular road lane, including the lanes exiting an intersections interior.
*/
enum LaneletPosition {IntersectionEntry = 1, IntersectionInterior = 2, RoadLane = 3};

/*
Contains Values for Different Traffic Rules:
  RI = 1
  RIII = 3
  RIV = 4
  RV = 5
  Tailgating = 6
  SpeedLimit = 7
  TargetSpeed = 8
  WayPoint = 9
  Collision = 10
  OffRoad = 11
  StopSignAndNoOffroad = 12
  GoalWayPointAndTargetSpeed = 12
*/
enum Rules {RI = 1, RIII = 3, RIV = 4, RV = 5, Tailgating = 6, SpeedLimit = 7,
            TargetSpeed = 8, WayPoint = 9, Collision = 10, OffRoad = 11,
            StopSignAndNoOffroad = 12, GoalWayPointAndTargetSpeed = 12};
            //END Rules

/*
Contains Identifiers for Different Traffic Signs by Origin Country:
  STOP_SIGN_VALUE = 1
  ALL_WAY_STOP_VALUE = 1
  YEILD_VALUE = 2
*/
enum GeneralRules {STOP_SIGN_VALUE = 1, ALL_WAY_STOP_VALUE = 1, YEILD_VALUE = 2}; //END GeneralRules

/*
Contains Identifiers for Different Traffic Signs by Origin Country:
  German = 1
  American = 2
  Canadian = 3
*/
enum TrafficSignOrigin {German = 1, American = 2, Canadian = 3}; //END TrafficSignOrigin

/*
struct TrafficSignsCanadian:
  Contains string representations as they appear in standard formatted OSM files.
  Contains priority values for Canadian signs.
*/
struct TrafficSignsCanadian {
  //adds string representations for the signs
  const std::string STOP = "caRA-1";
  const std::string ALL_WAY_STOP = "caRA-1T";
  const std::string YEILD = "caRA-2";
  const std::string YEILD_TAB = "caRA-2T";

  //add priority values for the sign types
  const int STOP_VALUE = 1;
  const int YEILD_VALUE = 2;
}; //END TrafficSignsCanadian

/*
struct TrafficSignsAmerican:
  Contains string representations as they appear in standard formatted OSM files.
  Contains priority values for American signs.
*/
struct TrafficSignsAmerican {
  //adds string representations for the signs
  const std::string STOP = "usR1-1";
  const std::string YEILD = "usR1-2";
  const std::string YEILD_TAB = "usR1-2";
  const std::string MULTI_WAY_STOP = "usR1-3";

  //add priority values for the sign types
  const int STOP_VALUE = 1;
  const int YEILD_VALUE = 2;
}; //END TrafficSignsAmerican

/*
struct TrafficSignsGerman:
  Contains string representations as they appear in standard formatted OSM files.
  Contains priority values for German signs based on turn direction {left, straight, right}.
*/
struct TrafficSignsGerman {
  //adds string representations for the signs
  const std::string STOP = "de206";
  const std::string PRIORITY_UNTIL = "de306";
  const std::string PRIORITY_NEXT_INTERSECTION = "de301"; 
  const std::string YEILD = "de205";
  const std::string RIGHT_TRAFFIC_PRIORITY = "de102";
  const std::string RIGHT_TURN_ARROW = "de720";
  const std::string LEFT_TURN_PRIORITY_FOUR_WAY = "de1002-10";
  const std::string LEFT_TURN_PRIORITY_THREE_WAY_VERTICAL = "de1002-12";
  const std::string LEFT_TURN_PRIORITY_THREE_WAY_HORIZONTAL = "de1002-13";
  const std::string RIGHT_TURN_PRIORITY_FOUR_WAY = "de1002-20";
  const std::string RIGHT_TURN_PRIORITY_THREE_WAY_VERTICAL = "de1002-22";
  const std::string RIGHT_TURN_PRIORITY_THREE_WAY_HORIZONTAL = "de1002-23";
  const std::string NO_TURN_PRIORITY_RIGHT_FOUR_WAY = "de1002-11";
  const std::string NO_TURN_PRIORITY_RIGHT_THREE_WAY = "de1002-14";
  const std::string NO_TURN_PRIORITY_LEFT_FOUR_WAY = "de1002-21";
  const std::string NO_TURN_PRIORITY_LEFT_THREE_WAY = "de1002-24";

  //add priority values for the signs based on turn direction {left, straight, right}
  const int INTERSECTION10[3] = {5, 4, 4};
  const int INTERSECTION12[3] = {5, 4, 0};
  const int INTERSECTION13[3] = {5, 0, 4};
  const int INTERSECTION20[3] = {4, 4, 5};
  const int INTERSECTION22[3] = {0, 4, 5};
  const int INTERSECTION23[3] = {4, 0, 5};
  const int INTERSECTION11[3] = {2, 2, 2};
  const int INTERSECTION14[3] = {2, 2, 0};
  const int INTERSECTION21[3] = {2, 2, 2};
  const int INTERSECTION24[3] = {0, 2, 2};
  const int PRIORITY306[3] = {4, 5, 4};
  const int PRIORITY301[3] = {4, 5, 4};
  const int YEILD205[3] = {2, 2, 2};
  const int STOP206[3] = {1, 1, 1};
  const int TRAFFICROY102[3] = {3, 3, 3};
  const int RIGHTAFTERSTOP720[3] = {0, 0, 0};
}; //END TrafficSignsGerman

/*
struct tailgatingMoment - Struct of Tailgated Vehicles (Victim) Information:
  int victimID - The ID of the tailgated vehicle.
  int timestamp - The moment where the vehicle was tailgated.
  double xPos, yPos, angle - Positioning of the victim.
  bool stopped - Signals if the vehicle was tailgated while stopped, or while moving.
*/
struct tailgatingMoment {
  //contains the ID and the current time moment for the victim vehicle
  int victimID;
  int timestamp;

  //position of the victim
  double xPos, yPos, angle;

  //determines if tailgating detection occurred between stopped vehicles or not
  bool stopped;

  //position of the violating vehicle
  double xPosViolator, yPosViolator, angleViolator;

  //determines if tailgating detection occurred between stopped vehicles or not (for the violator)
  bool stoppedViolator;
}; //END tailgatingMoment

/*
struct directionVector - Contains the Direction Vector for a Lanelet.
  double x - Contains the x position from the origin for the lanelet.
  double y - Contains the y position from the origin for the lanelet.
  double angle - Contains the angle from the x-axis from the origin point for the vector.
*/
struct directionVector {
  //contains x and y position for the vector coordinates
  double x;
  double y;

  //contains the angle from the x-axis from the origin point for the vector
  double angle;
}; //END directionVector

/*
struct tailgatingInformation - Contains List of Tailgating Vehicles:
  int violatingVehicleID - ID of the vehicle tailgating other vehicles.
  double xPos, yPos, angle - Position of the vehicle.
  bool stopped - Determines if tailgating detection occurred between stopped vehicles.
  std::vector<struct tailgatingMoment> tailgatedVehicle - ID of the vehicles that were tailgated.
*/
struct tailgatingInformation {
  //ID of the vehicle tailgating other vehicles
  int violatingVehicleID;

  //ID of the vehicles that were tailgated
  std::vector<struct tailgatingMoment> tailgatedVehicle;
}; //END tailgatingInformation

/*
struct vehicleStatistics - Used to Contains Vehicle Statistical Data:
  int trackID - Vehicle ID.
  std::string agentType - Vehicle Type.
  double length - Vehicle Length.
  double width - Vehicle Width.
  std::vector<int> frameID - Current frame at moment.
  std::vector<double> timestamp - Current time at moment.
  std::vector<double> xCoord - Current x coordinate at moment.
  std::vector<double> yCoord - Current y coordinate at moment.
  std::vector<double> xVelocity - Current x velocity at moment.
  std::vector<double> yVelocity - Current y velocity at moment.
  std::vector<double> yaw - Current yaw at moment.
  double timeRange, timeMedian, timeMean, timeMode, timeStandardDeviation - Time statistical information.
  double xRange, xMedian, xMean, xMode, xStandardDeviation - X-position statistical information.
  double yRange, yMedian, yMean, yMode, yStandardDeviation - Y-position statistical information.
  double xVelocityRange, xVelocityMedian, xVelocityMean, xVelocityMode, xVelocityStandardDeviation - X-velocity statistical information.
  double yVelocityRange, yVelocityMedian, yVelocityMean, yVelocityMode, yVelocityStandardDeviation - Y-velocity statistical information.
  double yawRange, yawMedian, yawMean, yawMode, yawStandardDeviation - Yaw statistical information.
*/
struct vehicleStatistics {
  //contains vehicle data
  int trackID;
  std::string agentType;
  double length;
  double width;

  //contains time interval momentary data
  std::vector<int> frameID;
  std::vector<double> timestamp;
  std::vector<double> xCoord;
  std::vector<double> yCoord;
  std::vector<double> xVelocity;
  std::vector<double> yVelocity;
  std::vector<double> yaw;

  //contains statistics data
  double timeRange, timeMedian, timeMean, timeMode, timeStandardDeviation;
  double xRange, xMedian, xMean, xMode, xStandardDeviation;
  double yRange, yMedian, yMean, yMode, yStandardDeviation;
  double xVelocityRange, xVelocityMedian, xVelocityMean, xVelocityMode, xVelocityStandardDeviation;
  double yVelocityRange, yVelocityMedian, yVelocityMean, yVelocityMode, yVelocityStandardDeviation;
  double yawRange, yawMedian, yawMean, yawMode, yawStandardDeviation;
}; //END vehicleStatistics

/*
struct stopLineData - Used for lanelet stop line data:
  int index - index of the regulatory element that is triggered.
  double lowestDist - lowest distance to the line.
  bool greenArrow - signals if a green right turn arrow is active.
  double greenArrowDist - distance to the green right turn arrow stop line.
*/
struct stopLineData {
  //index of the regulatory element that is triggered
  int index;
  //lowest distance to the line
  double lowestDist;
  //signals if a green right turn arrow is active
  bool greenArrow;
  //distance to the green right turn arrow stop line
  double greenArrowDist;
}; //END stopLineData

/*
struct interval - Interval of Data per Moment of Time in the Interaction Dataset:
  int id - Vehicle ID for interval.
  int timestamp - Current moment during interval.
  double velocityX - Current x-velocity at moment.
  double velocityY - Current y-velocity at moment.
  double yaw - Current yaw at moment.
  double xPos - Current x-position at moment.
  double yPos - Current y-position at moment.
*/
struct interval {
  int id;
  int timestamp;
  double velocityX;
  double velocityY;
  double yaw;
  double xPos;
  double yPos;
}; //END interval

/*
struct stopLineData - Contains Vehicles Information Along with all Intervals Related to the Vehicle:
  //contains vehicle information
  double xStart, xEnd - Contains vehicle starting and ending x-position for the interaction dataset recording.
  double yStart, yEnd - Contains vehicle starting and ending y-position for the interaction dataset recording.
  double yawStart, yawEnd - Contains vehicle starting and ending yaw for the interaction dataset recording.
  double length, width - Contains vehicle length and width.
  std::vector<struct interval> intervals - Contains all vehicle intervals.
*/
struct vehicles {
  //contains vehicle information
  double xStart, xEnd;
  double yStart, yEnd;
  double yawStart, yawEnd;
  double length, width;

  //contains vehicle intervals
  std::vector<struct interval> intervals;
}; //END vehicles

/*
struct commands - Contains specific commands for specific test cases.
  velocityThreshold and distanceThreshold are required inputs.
  responseTime, maxBreakAccFront, maxAccelAccRear, minBreakAccRear are used for tailgating checks.
  Finally, the remaining values listed below are for d_min distribution checks when checking tailgating:
    responseVariation;
    maxBreakAccFrontVariation;
    maxAccelAccRearVariation;
    minBreakAccRearVariation;
  Note that the following values are used to determine the number of divisions associated with the variation,
  with a value of 1 indicating a simple +/-/0 relationship (i.e. responseTime + ((+/-/0)tick / responseVariation)).
  We can see that a tick of 1 would result in three outputs:
    {responseTime - responseVariation, responseTime, responseTime + responseVariation}
  The tick values can be found below:
    responseVariationTick;
    maxBreakAccFrontVariationTick;
    maxAccelAccRearVariationTick;
    minBreakAccRearVariationTick;
*/
struct commands {
  //contains the velocity and distance threshold for vehicle stopping
  double velocityThreshold;
  double distanceThreshold;

  //contains driver coefficients if applicable
  boost::optional<double> responseTime;
  boost::optional<double> maxBreakAccFront;
  boost::optional<double> maxAccelAccRear;
  boost::optional<double> minBreakAccRear;

  //contains max driver coefficients if applicable
  boost::optional<double> responseTimeMax;
  boost::optional<double> maxBreakAccFrontMax;
  boost::optional<double> maxAccelAccRearMax;
  boost::optional<double> minBreakAccRearMax;

  //contains min driver coefficients if applicable
  boost::optional<double> responseTimeMin;
  boost::optional<double> maxBreakAccFrontMin;
  boost::optional<double> maxAccelAccRearMin;
  boost::optional<double> minBreakAccRearMin;

  //contains distribution values if applicable
  boost::optional<double> responseVariation;
  boost::optional<double> maxBreakAccFrontVariation;
  boost::optional<double> maxAccelAccRearVariation;
  boost::optional<double> minBreakAccRearVariation;
  boost::optional<int> responseVariationTick;
  boost::optional<int> maxBreakAccFrontVariationTick;
  boost::optional<int> maxAccelAccRearVariationTick;
  boost::optional<int> minBreakAccRearVariationTick;
}; //END commands

/*
struct commandInput - Contains Coefficient Information Along for Test Cases:
  Contains coefficients for the various inputs associated with the rules being evaluated, for examples:
    stop, leftBeforeRight, laneletPriority, leftTurnPriority, tailgating, dMinDistribution;
*/
struct commandInput {
  //contains information for stop checking
  struct commands stop;

  //contains information for turns after stopping at stop regulated intersections
  struct commands leftBeforeRight, laneletPriority, leftTurnPriority;

  //contains information for tailgate checking
  struct commands tailgating;

  //contains information for d_min distribution checking
  struct commands dMinDistribution;
}; //END commandInput

/*
struct stopLineData - Contains Data Values for Rule Violated Output:
  const int vehicles - Contains the number of vehicles recorded.
  const int nonViolators - Contains the number of vehicles that did not violate any rules
  const int shouldStop - Contains the number of vehicles that did not stop at the stop regulated intersection.
  const int leftBeforeRight - Contains the number of vehicles that failed to turn right-before-left when applicable.
  const int hasPriority - Contains the number of vehicles that failed consider lanelet priority.
  const int turningLeft - Contains the number of vehicles that failed to maintain left-turning priority when applicable.
  const int tailgating - Contains the number of vehicles that engaged in tailgating.
  const std::string PATH - Contains the path to the output CSV file.
  const std::string title - Contains the headers for the CSV files columns.
*/
struct ruleViolationStatistics {
  //contains the number of vehicles
  const int vehicles;

  //contains the number of vehicles that did not violate any rules
  const int nonViolators;

  //contains the number of vehicles that violated the specified rules
  const int shouldStop;
  const int leftBeforeRight;
  const int hasPriority;
  const int turningLeft;
  const int tailgating;
  const int speedLimit;
  const int noCollisions;

  //contains the path to the output CSV file
  const std::string PATH;

  //contains the headers for the CSV files columns
  const std::string title;

  //contains the total number of stops vehicles needed to complete
  const int stopsNeeded;
  const int yieldsOneNeeded;
  const int yieldsTwoNeeded;
}; //END ruleViolationStatistics

/*
struct stopLineData - Contains for storing traffic rule violations:
  Rules rule - Represents the rule being considered.
  bool ruleStatus - Represents if the rule was violated.
  int referenceID - Contains the ID of the reference vehicle.
  boost::optional<int> timestamp - Contains the time of the offensive if applicable.
  boost::optional<double> magnitude - Contains a magnitude measuring how off the maneuver is.
  boost::optional<struct directionVector> vector - Contains a vector for the direction of the offensive.
*/
struct maneuver {
  Rules rule;
  bool ruleStatus;
  int referenceID;
  boost::optional<int> timestamp;
  boost::optional<double> magnitude = false;
  boost::optional<struct directionVector> vector;
}; //END maneuver

/*
struct stopLineData - Contains for storing traffic rule violations:
  Rules rule - Represents the rule being considered.
  bool ruleStatus - Represents if the rule was violated.
  int referenceID - Contains the ID of the reference vehicle.
  boost::optional<int> otherID - Contains the ID of another vehicle involved if applicable.
  boost::optional<int> timestamp - Contains the time of the offensive if applicable.
  boost::optional<double> xPos - Contains the vehicle coordinates at the location of the rule violation.
  boost::optional<double> yPos - Contains the vehicle coordinates at the location of the rule violation.
  boost::optional<double> yaw - Contains the vehicle orientation at the location of the rule violation.
  boost::optional<int> endTimestamp - Contains the ending time for the associated rule violation (such as a turn).
  boost::optional<double> magnitude - Contains the magnitude associated with the rule violation if applicable.
  boost::optional<double> turnDirection - Contains the direction of a turn associated with the rule violation if applicable.
*/
struct violation {
  Rules rule;
  bool ruleStatus;
  int referenceID;
  boost::optional<int> otherID;
  boost::optional<int> timestamp;
  boost::optional<double> xPos;
  boost::optional<double> yPos;
  boost::optional<double> yaw;
  boost::optional<int> endTimestamp;
  boost::optional<double> magnitude;
  boost::optional<double> turnDirection;
  boost::optional<int> iteration;
  boost::optional<bool> neededToStop;
}; //END violation

/*
struct stopLineData - Contains all rule violations for a given vehicle:
  struct violation ruleI, ruleIStrictVelocity, ruleIHardVelocity, ruleIStrictDistance, ruleIHardDistance
                   - Contains the results of checks for stops at stop regulated intersections.
  struct violation ruleIII - Contains the results of checks for right-before-left turning at stop regulated intersections.
  struct violation ruleIV - Contains the results of checks for lanelet priority at stop regulated intersections.
  struct violation ruleV - Contains the results of checks for left turning priority at stop regulated intersections.
  struct tailgatingInformation tailgating, tailgatingHighResponse, tailgatingHighBreak, tailgatingHighAccel, 
                               tailgatingHighMinAccel, tailgatingLowResponse, tailgatingLowBreak, 
                               tailgatingLowAccel, tailgatingLowMinAccel
                               - Contains the results of checks for tailgating at stop regulated intersections.
  std::tuple<std::vector<struct maneuver>, double, double> speedLimitViolations, speedLimitViolationsStrict,
                                                           speedLimitViolationsSoft, speedLimitViolationsHard;
                                                           - Contains the results of speed limit violations.
  std::vector<struct violation> offroadViolations, offroadViolationsFull, offroadViolationsFrame,
                                offroadViolationsChassis, offroadViolationsCenter;
                                - Contains the results of off-road violations
*/
struct violations {
  struct violation ruleI, ruleIStrictVelocity, ruleIHardVelocity, ruleIStrictDistance, ruleIHardDistance;
  struct violation ruleIII;
  struct violation ruleIV;
  struct violation ruleV;
  struct tailgatingInformation tailgating, tailgatingHighResponse, tailgatingHighBreak, tailgatingHighAccel, 
                               tailgatingHighMinAccel, tailgatingLowResponse, tailgatingLowBreak, 
                               tailgatingLowAccel, tailgatingLowMinAccel;
  std::tuple<std::vector<struct maneuver>, double, double> speedLimitViolations, speedLimitViolationsStrict,
                                                           speedLimitViolationsSoft, speedLimitViolationsHard;
  std::vector<struct violation> offroadViolations, offroadViolationsFull, offroadViolationsFrame,
                                offroadViolationsChassis, offroadViolationsCenter;
}; //END violations

//using lanelet2 functions
namespace lanelet {

//using traffic rules for lanelet2
namespace traffic_rules {

//expression for Canadian vehicles
static constexpr const char Canada[] = "ca";

//expression for American vehicles
static constexpr const char America[] = "us";

//expression for German vehicles
static constexpr const char Germany[] = "de";

//expression for German vehicles
static constexpr const char China[] = "cn";

//! This class generically implements the TrafficRules class in the sense of the tagging specification. It is designed
//! to make sense for most countries and participants. Country specific details (traffic signs, speed limits
//! regulations, ...) must be implemented by inheriting classes.
class ExtendedTrafficRules : public GenericTrafficRules {  // NOLINT
 public:
  //define a smart pointer for the generic traffic rules class
  using GenericTrafficRules::GenericTrafficRules;

  //declare can pass from the previous GenericTrafficRules class
  bool canPass(const lanelet::ConstLanelet & lanelet) const override;

  //helper functions for traffic rules
  struct stopLineData getTrafficLightStopLine(const lanelet::ConstLanelet & lanelet, const lanelet::Point3d vehicle) const;
  struct stopLineData getSignStopLine(const lanelet::ConstLanelet & lanelet, const lanelet::Point3d vehicle) const;
  struct stopLineData getAllWayStopLine(const lanelet::ConstLanelet & lanelet, const lanelet::Point3d vehicle) const;
  Direction getDirection(const double distance, const double length) const;
  int priorityValue(const lanelet::ConstLanelet & egoLanelet, const Direction turnType) const;
  bool isIncomingLeftOf(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet,
                        const lanelet::Lanelets mapData, const Direction egoTurn) const;
                        //END isIncomingLeftOf
  bool oncomingLanes(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet, const lanelet::Lanelets mapData,
                     const Direction egoTurn) const;
                     //END oncomingLanes
  bool hasPriority(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet, const Direction turnTypeEgo,
                   const Direction turnTypeOther) const;
                   //END hasPriority
  bool hasPriority(const int egoLanelet, const lanelet::ConstLanelet & otherLanelet, const Direction turnTypeOther) const;
  bool hasPriority(const lanelet::ConstLanelet & egoLanelet, const Direction turnTypeEgo, const int otherLanelet) const;
  bool hasPriority(const int egoLanelet, const int otherLanelet) const;
  bool samePriority(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet, const Direction turnTypeEgo,
                    const Direction turnTypeOther) const;
                    //END samePriority
  bool canStandstillAtLights(const lanelet::ConstLanelet & lanelet, const double travelledDistanceTillStandstill,
                             const struct stopLineData index, const lanelet::Point3d vehicle) const;
                             //END canStandstillAtLights
  bool inStandstill(const double projectedV, const double errorV) const;
  bool standstillSign(const lanelet::ConstLanelet & lanelet, const double travelledDistanceTillStandstill,
                      const int index, const lanelet::Point3d vehicle) const;
                      //END standstillSign
  bool sameIntersection(const lanelet::LaneletMapPtr map, const lanelet::BasicPoint2d originPoint,
                        const lanelet::routing::RoutingGraphConstPtr routingGraph,
                        const lanelet::BasicPoint2d currentPoint, const lanelet::BasicPoint2d otherPoint) const;
                        //END sameIntersection
  LaneletPosition intersectionSegment(const double xPos, const double yPos, const std::string mapPath,
                                      const lanelet::BasicPoint2d originPoint, const char * vehicleLocation) const;

  //traffic rule functions
  bool shouldStop(const lanelet::ConstLanelet & lanelet, const lanelet::Point3d vehicle) const;
  bool shouldWaitAtTrafficLight(const lanelet::ConstLanelet & lanelet, const Direction turn, const Direction light, const Colour colour,
                                const double travelledDistanceTillStandstill, const lanelet::Point3d vehicle) const;
                                //END shouldWaitAtTrafficLight
  bool shouldTurnRightBeforeLeft(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet, const Direction egoTurn,
                                 const Direction otherTurn, const lanelet::Lanelets mapData, const lanelet::Point3d egoPos,
                                 const lanelet::Point3d otherPos) const;
                                 //END shouldTurnRightBeforeLeft
  bool shouldHavePriority(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet, const Direction egoTurn,
                          const Direction otherTurn, const lanelet::Lanelets mapData,
                          const lanelet::Point3d egoPos, const lanelet::Point3d otherPos) const;
                          //END shouldHavePriority
  bool shouldBeTurningLeft(const lanelet::ConstLanelet & egoLanelet, const lanelet::ConstLanelet & otherLanelet, const Direction egoTurn,
                           const Direction otherTurn, const lanelet::Lanelets mapData,
                           const lanelet::Point3d egoPos, const lanelet::Point3d otherPos) const;
                           //END shouldBeTurningLeft

  //CSV file reader and formatted for interaction dataset
  std::vector<struct vehicles> getVehicleData(const std::string dataPath) const;
  std::vector<std::pair<int, int>> standstillIntervals(const std::vector<struct interval> intervalData, const std::string mapPath,
                                                       const double velocityThreshold) const;
                                                       //END standstillIntervals
  int getCurrentLanelet(const std::string mapPath, const lanelet::BasicPoint2d originPoint, const lanelet::BasicPoint2d startPoint) const;

  //vehicle data extractors
  std::vector<struct vehicleStatistics> getVehicleStats(const std::string dataPath) const;
  Direction turnDirection(const double yawStart, const double yawEnd) const;
  double getStopLineDist(const lanelet::LaneletMapPtr map, const lanelet::BasicPoint2d originPoint,
                         const lanelet::BasicPoint2d startPoint, const lanelet::routing::RoutingGraphConstPtr routingGraph,
                         std::vector<int> exception = {}) const;
                         //END getStopLineDist
  double getStopLineDist(const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                         const lanelet::BasicPoint2d startPoint, const char * vehicleLocation,
                         std::vector<int> exception = {}) const;
                         //END getStopLineDist
  double getRadians(const double orientation) const;
  double getOrientation(const double radians) const;
  double endAngleFinder(const lanelet::LaneletMapPtr map, const lanelet::BasicPoint2d originPoint,
                        const lanelet::routing::RoutingGraphConstPtr routingGraph,
                        const std::vector<interval> moments, const int startMoment,
                        const double velocityThreshold, const double stoppingThreshold) const;
                        //END endAngleFinder

  //tailgating functions
  std::vector<struct tailgatingInformation> getDminDistribution(const std::vector<struct interval> intervalData, const double referenceLength,
                                                                const std::vector<struct vehicles> allVehicles, const std::string mapPath,
                                                                const lanelet::BasicPoint2d origin, const double speedThreshold,
                                                                const double stopDistanceThreshold, const double responseTime,
                                                                const double maxBreakAccFront, const double maxAccelAccRear,
                                                                const double minBreakAccRear, const int mode,
                                                                const double responseVariation, const double maxBreakAccFrontVariation,
                                                                const double maxAccelAccRearVariation, const double minBreakAccRearVariation,
                                                                const int responseVariationTick, const int maxBreakAccFrontVariationTick,
                                                                const int maxAccelAccRearVariationTick,
                                                                const int minBreakAccRearVariationTick, const char PATH[]) const;
                                                                //END getDminDistribution
  struct tailgatingInformation isTailgatingAtMoment(const std::vector<struct interval> intervalData, const double referenceLength,
                                                    const std::vector<struct vehicles> allVehicles, const std::string mapPath,
                                                    const lanelet::BasicPoint2d origin, const double speedThreshold,
                                                    const double stopDistanceThreshold, const double responseTime,
                                                    const double maxBreakAccFront, const double maxAccelAccRear,
                                                    const double minBreakAccRear, const int mode, const char * vehicleLocation) const;
                                                    //END isTailgatingAtMoment
  bool closeProximity(const lanelet::BasicPoint2d referenceRear, const lanelet::BasicPoint2d otherFront,
                      const double referenceOrientation, const double otherOrientation, const double distThreshold) const;
                      //END closeProximity
  double findMinDist(const double responseTime, const double rearVelocity, const double frontVelocity,
                     const double maxBreakAccFront, const double maxAccelAccRear, const double minBreakAccRear) const;
                     //END findMinDist
  void resetTailgatingResults(const char PATH[]) const;
  void resetTailgatingResultsLineByLine(const char PATH[]) const;
  void resetTailgatingOccurances(const char PATH[]) const;
  void appendTailgatingResults(const struct tailgatingInformation tailgatingResult, const double responseTimeChange,
                               const double maxBreakAccFrontChange, const double maxAccelAccRearChange,
                               const double minBreakAccRearChange, const char PATH[]) const;
                               //END appendTailgatingResults
  void appendTailgatingCounts(const std::string filename,
                              const int numTailgatingDefault, const int numTailgatingHighResponse,
                              const int numTailgatingHighBreak, const int numTailgatingHighAccel,
                              const int numTailgatingHighMinAccel, const int numTailgatingLowResponse,
                              const int numTailgatingLowBreak, const int numTailgatingLowAccel,
                              const int numTailgatingLowMinAccel, const char PATH[]) const;
                              //END appendTailgatingCounts
  void appendTailgatingResultsLineByLine(const struct tailgatingInformation tailgatingResult, const char PATH[]) const;
  void resetOutputDataToExcel(const std::string PATH, const char HEADER[]) const;
  void outputDataToExcel(const std::string PATH, const std::string data) const;
  std::string noOffroadString(const std::vector<violation> data, const double length, const double width, const BoundaryValues ratio,
                              const std::string mapPath, const lanelet::BasicPoint2d originPoint, const char * vehicleLocation) const;
                              //END noOffroadString

  //vehicle statistics functions
  std::vector<double> sortVector(std::vector<double> vector) const;
  double findRange(std::vector<double> vector) const;
  double findMax(std::vector<double> vector) const;
  double findMin(std::vector<double> vector) const;
  double findMean(std::vector<double> vector) const;
  double findMedian(std::vector<double> vector) const;
  double findMode(std::vector<double> vector) const;
  double findStandardDeviation(std::vector<double> vector) const;
  void writeToCSV(const std::vector<struct vehicleStatistics> data, const std::string appendName, const std::string fileName) const;
  void outputToResultsLog(const struct ruleViolationStatistics) const;
  void clearOutput(const std::string path, const char header[]) const;
  void clearVehicleStats(const std::string path, const char header[]) const;

  //helper functions for interaction dataset
  std::vector<std::string> stringSplit(const std::string data, const char delim) const;
  lanelet::BasicPoint2d getVehicleBounds(const double diameter, const double angle, const double referenceX,
                                         const double referenceY, const Bounds mode = Bounds::Front, const double width = 0,
                                         const BoundaryValues bounds = BoundaryValues::RemoveFrame) const;
                                         //END getVehicleBounds
  lanelet::ConstLanelet getLanelet(const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                                   const lanelet::BasicPoint2d startPoint) const;
                                   //END getLanelet
  struct directionVector getLaneletDirectionVector(const lanelet::ConstLanelet & lanelet) const;
  double vectorAngle(const double x, const double y) const;
  void outputToFile(const std::string data) const;
  void outputNoOffroad(const std::string PATH, const std::vector<violation> data, const double length, const double width) const;
  bool incomingLeftOf(const lanelet::Lanelet otherLanelet, const lanelet::Lanelet currentLanelet, const double otherOrientation,
                      const double currentOrientation, const lanelet::routing::RoutingGraphConstPtr routingGraph) const;
                      //END incomingLeftOf

  //STL formulas for interaction dataset - rule violation detection system
  struct violation stopSignWithNoOffroad(const std::vector<struct interval> intervalData, const std::string mapPath,
                                         const lanelet::BasicPoint2d originPoint, const double stoppingThreshold,
                                         const double velocityThreshold, const double length, const double width,
                                         const char * vehicleLocation, std::vector<int> exception = {}) const;
                                         //END stopSignWithNoOffroad
  std::vector<struct violation> noOffroad(const std::vector<struct interval> intervalData,  const double length, const double width,
                                          const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                                          const char * vehicleLocation, const BoundaryValues bounds = BoundaryValues::RemoveFrame) const;
                                          //END noOffroad
  std::vector<struct violation> goalWayPointWithTargetSpeed(const std::vector<struct interval> intervalData,
                                                            const lanelet::BasicPoint2d goalPoint, const double epsilonWaypoint,
                                                            const double targetSpeedValue, const double epsilonSpeed, 
                                                            const SpeedCheckMode mode = SpeedCheckMode::UpperLimit) const;
                                                            //END goalWayPointWithTargetSpeed

  //STL formulas for interaction dataset - control planning system
  std::tuple<std::vector<struct maneuver>, double, double> checkSpeedLimit(const std::vector<struct interval> intervalData, const std::string mapPath,
                                               const lanelet::BasicPoint2d originPoint, const double epsilon,
                                               const char * vehicleLocation, const double velocityThreshold) const;
                                               //END checkSpeedLimit
  std::vector<struct maneuver> goalWaypoint(const std::vector<struct interval> intervalData, const lanelet::BasicPoint2d goalPoint,
                                            const double epsilon) const;
                                            //END goalWaypoint
  std::vector<struct maneuver> targetSpeed(const std::vector<struct interval> intervalData, const double target,
                                           const double epsilon, const SpeedCheckMode mode = SpeedCheckMode::UpperLimit) const;
                                           //END targetSpeed
  std::vector<struct maneuver> noCollision(const std::vector<struct interval> intervalData, const double referenceLength,
                                           const lanelet::BasicPoint2d collisionPoint, const double epsilon) const;
                                           //END noCollision

  //traffic rule functions for interaction dataset
  const struct violation checkStopAtSign(const std::vector<struct interval> intervalData, const std::string mapPath,
                                         const lanelet::BasicPoint2d originPoint, const double stoppingThreshold,
                                         const double velocityThreshold, const char * vehicleLocation,
                                         const StopDirection stopType = StopDirection::SingleStop, const int timeLimit = 0,
                                         std::vector<int> exception = {}) const;
                                         //END checkStopAtSign
  const struct violation checkLeftBeforeRight(const std::vector<struct vehicles> vehicleData, const struct vehicles ego,
                                              const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                                              const double stoppingThreshold, const double velocityThreshold,
                                              const char * vehicleLocation, const YawChecking yawCheck = YawChecking::Dataset) const;
                                              //END checkLeftBeforeRight
  const struct violation checkHasPriority(const std::vector<struct vehicles> vehicleData, const struct vehicles ego,
                                          const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                                          const double stoppingThreshold, const double velocityThreshold,
                                          const char * vehicleLocation, const std::vector<std::pair<int, int>> overridePriority = {},
                                          const YawChecking yawCheck = YawChecking::Dataset) const;
                                          //END checkHasPriority
  const struct violation checkShouldBeTurningLeft(const std::vector<struct vehicles> vehicleData, const struct vehicles ego,
                                                  const std::string mapPath, const lanelet::BasicPoint2d originPoint,
                                                  const double stoppingThreshold, const double velocityThreshold,
                                                  const char * vehicleLocation, const YawChecking yawCheck = YawChecking::Dataset) const;
                                                  //END checkShouldBeTurningLeft
}; //END ExtendedTrafficRules
}  // namespace traffic_rules
}  // namespace lanelet
