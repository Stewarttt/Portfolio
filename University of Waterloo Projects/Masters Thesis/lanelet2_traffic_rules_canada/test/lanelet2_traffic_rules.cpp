//include core lanelet2 functions
#include <lanelet2_core/primitives/BasicRegulatoryElements.h>
#include <lanelet2_core/primitives/Lanelet.h>
#include <lanelet2_core/utility/Units.h>
#include <lanelet2_core/LaneletMap.h>

//include system functions
#include <sys/types.h>
#include <sys/stat.h>

//include parallelization header
#include <omp.h>

//include vehicleID/O struct for reading directories
#include <dirent.h>

//Google C++ Testing and Mocking Framework
#include "gtest/gtest.h"

//Include the traffic rules builder utility
#include "lanelet2_traffic_rules/TrafficRulesFactory.h"

//include Canadian traffic rules as a header file
#include "lanelet2_traffic_rules_canada/CanadianTrafficRules.h"

//include American traffic rules as a header file
#include "lanelet2_traffic_rules_canada/AmericanTrafficRules.h"

//include German traffic rules as a header file
#include "lanelet2_traffic_rules_canada/GermanTrafficRules.h"

//include Chinese traffic rules as a header file
#include "lanelet2_traffic_rules_canada/ChineseTrafficRules.h"

//include traffic rules utilities from lanelet2
#include <lanelet2_traffic_rules/TrafficRules.h>

//define smart pointers for the given attributes in the testing space
using Attr = lanelet::AttributeName;
using AttrStr = lanelet::AttributeNamesString;
using Value = lanelet::AttributeValueString;
using lanelet::Participants;

//get the ID values that map German traffic signs to their corresponding string representation
struct TrafficSignsGerman germanSignIDs;

//define a struct containing coefficients for test cases
struct commandInput coefficients;

//gets the regulatory element pointer for a speed limit object
lanelet::RegulatoryElementPtr getSpeedLimit(const std::string &type, const lanelet::AttributeMap &attributes = {}) {
  //define two points for the sign position
  lanelet::Point3d p1{10, 0, -1};
  lanelet::Point3d p2{11, 0, -2};

  //place the sign with the given ID
  lanelet::LineString3d sign{7, {p1, p2}};

  //return a newly created speed limit regulatory element pointer
  return lanelet::SpeedLimit::make(5, attributes, {{sign}, type});
} //END getSpeedLimit

//define smart pointer for the given test space
//Canada
using CanadianVehiclePtr = std::shared_ptr<lanelet::traffic_rules::CanadianVehicle>;
using CanadianBicyclePtr = std::shared_ptr<lanelet::traffic_rules::CanadianBicycle>;
using CanadianPedestrianPtr = std::shared_ptr<lanelet::traffic_rules::CanadianPedestrian>;

//America
using AmericanVehiclePtr = std::shared_ptr<lanelet::traffic_rules::AmericanVehicle>;
using AmericanBicyclePtr = std::shared_ptr<lanelet::traffic_rules::AmericanBicycle>;
using AmericanPedestrianPtr = std::shared_ptr<lanelet::traffic_rules::AmericanPedestrian>;

//German
using GermanVehiclePtr = std::shared_ptr<lanelet::traffic_rules::GermanVehicle>;
using GermanBicyclePtr = std::shared_ptr<lanelet::traffic_rules::GermanBicycle>;
using GermanPedestrianPtr = std::shared_ptr<lanelet::traffic_rules::GermanPedestrian>;

//Chinese
using ChineseVehiclePtr = std::shared_ptr<lanelet::traffic_rules::ChineseVehicle>;
using ChineseBicyclePtr = std::shared_ptr<lanelet::traffic_rules::ChineseBicycle>;
using ChinesePedestrianPtr = std::shared_ptr<lanelet::traffic_rules::ChinesePedestrian>;

//gets the Canadian vehicle rules pointer for the given test space
ChineseVehiclePtr getChineseVehicleRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::China, Participants::Vehicle, {})};

  //return the generic pointer cast for the Canadian vehicle rules
  return std::static_pointer_cast<lanelet::traffic_rules::ChineseVehicle>(genericTrafficRulesPtr);
} //END getChineseVehicleRules

//gets the Canadian bicycle rules pointer for the given test space
ChineseBicyclePtr getChineseBicycleRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::China, Participants::Bicycle, {})};

  //return the generic pointer cast for the Canadian bicycle rules
  return std::static_pointer_cast<lanelet::traffic_rules::ChineseBicycle>(genericTrafficRulesPtr);
} //END getChineseBicycleRules

//gets the Canadian pedestrian rules pointer for the given test space
ChinesePedestrianPtr getChinesePedestrianRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::China, Participants::Pedestrian, {})};

  //return the generic pointer cast for the Canadian pedestrian rules
  return std::static_pointer_cast<lanelet::traffic_rules::ChinesePedestrian>(genericTrafficRulesPtr);
} //END getChinesePedestrianRules

//gets the Canadian vehicle rules pointer for the given test space
CanadianVehiclePtr getCanadianVehicleRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::Canada, Participants::Vehicle, {})};

  //return the generic pointer cast for the Canadian vehicle rules
  return std::static_pointer_cast<lanelet::traffic_rules::CanadianVehicle>(genericTrafficRulesPtr);
} //END getCanadianVehicleRules

//gets the Canadian bicycle rules pointer for the given test space
CanadianBicyclePtr getCanadianBicycleRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::Canada, Participants::Bicycle, {})};

  //return the generic pointer cast for the Canadian bicycle rules
  return std::static_pointer_cast<lanelet::traffic_rules::CanadianBicycle>(genericTrafficRulesPtr);
} //END getCanadianBicycleRules

//gets the Canadian pedestrian rules pointer for the given test space
CanadianPedestrianPtr getCanadianPedestrianRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::Canada, Participants::Pedestrian, {})};

  //return the generic pointer cast for the Canadian pedestrian rules
  return std::static_pointer_cast<lanelet::traffic_rules::CanadianPedestrian>(genericTrafficRulesPtr);
} //END getCanadianPedestrianRules

//gets the Canadian vehicle rules pointer for the given test space
GermanVehiclePtr getGermanVehicleRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::Germany, Participants::Vehicle, {})};

  //return the generic pointer cast for the Canadian vehicle rules
  return std::static_pointer_cast<lanelet::traffic_rules::GermanVehicle>(genericTrafficRulesPtr);
} //END getGermanVehicleRules

//gets the Canadian bicycle rules pointer for the given test space
GermanBicyclePtr getGermanBicycleRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::Germany, Participants::Bicycle, {})};

  //return the generic pointer cast for the Canadian bicycle rules
  return std::static_pointer_cast<lanelet::traffic_rules::GermanBicycle>(genericTrafficRulesPtr);
} //END getGermanBicycleRules

//gets the Canadian pedestrian rules pointer for the given test space
GermanPedestrianPtr getGermanPedestrianRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::Germany, Participants::Pedestrian, {})};

  //return the generic pointer cast for the Canadian pedestrian rules
  return std::static_pointer_cast<lanelet::traffic_rules::GermanPedestrian>(genericTrafficRulesPtr);
} //END getGermanPedestrianRules

//gets the American vehicle rules pointer for the given test space
AmericanVehiclePtr getAmericanVehicleRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::America, Participants::Vehicle, {})};

  //return the generic pointer cast for the American vehicle rules
  return std::static_pointer_cast<lanelet::traffic_rules::AmericanVehicle>(genericTrafficRulesPtr);
} //END getAmericanVehicleRules

//gets the American bicycle rules pointer for the given test space
AmericanBicyclePtr getAmericanBicycleRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::America, Participants::Bicycle, {})};

  //return the generic pointer cast for the American bicycle rules
  return std::static_pointer_cast<lanelet::traffic_rules::AmericanBicycle>(genericTrafficRulesPtr);
} //END getAmericanBicycleRules

//gets the American pedestrian rules pointer for the given test space
AmericanPedestrianPtr getAmericanPedestrianRules() {
  //convert from unique to shared pointer
  const lanelet::traffic_rules::TrafficRulesPtr genericTrafficRulesPtr{
      lanelet::traffic_rules::TrafficRulesFactory::create(lanelet::traffic_rules::America, Participants::Pedestrian, {})};

  //return the generic pointer cast for the American pedestrian rules
  return std::static_pointer_cast<lanelet::traffic_rules::AmericanPedestrian>(genericTrafficRulesPtr);
} //END getAmericanPedestrianRules

//define the static test case lanelet geometry for the test space
class TrafficRulesTest : public ::testing::Test {
  public:
    //define points to form the intersections geometry
    lanelet::Point3d
        p1{lanelet::utils::getId(), 0, 12, 0}, p2{lanelet::utils::getId(), 3, 12, 0}, p3{lanelet::utils::getId(), 6, 12, 0}, p4{lanelet::utils::getId(), 9, 12, 0},
        p5{lanelet::utils::getId(), 0, 15, 0}, p6{lanelet::utils::getId(), 3, 15, 0}, p7{lanelet::utils::getId(), 6, 15, 0}, p8{lanelet::utils::getId(), 9, 15, 0},
        p9{lanelet::utils::getId(), 0, 18, 0}, p10{lanelet::utils::getId(), 3, 18, 0}, p11{lanelet::utils::getId(), 6, 18, 0}, p12{lanelet::utils::getId(), 9, 18, 0},
        p14{lanelet::utils::getId(), 12, 12, 0}, p15{lanelet::utils::getId(), 15, 12, 0},
        p17{lanelet::utils::getId(), 12, 15, 0}, p18{lanelet::utils::getId(), 15, 15, 0},
        p20{lanelet::utils::getId(), 12, 18, 0}, p21{lanelet::utils::getId(), 15, 18, 0},
        p23{lanelet::utils::getId(), 15, 15, 0}, p24{lanelet::utils::getId(), 15, 18, 0},
        p25{lanelet::utils::getId(), 18, 12, 0}, p26{lanelet::utils::getId(), 21, 12, 0}, p27{lanelet::utils::getId(), 24, 12, 0},
        p28{lanelet::utils::getId(), 18, 15, 0}, p29{lanelet::utils::getId(), 21, 15, 0}, p30{lanelet::utils::getId(), 24, 15, 0},
        p31{lanelet::utils::getId(), 18, 18, 0}, p32{lanelet::utils::getId(), 21, 18, 0}, p33{lanelet::utils::getId(), 24, 18, 0},
        p34{lanelet::utils::getId(), 10, 0, 0}, p35{lanelet::utils::getId(), 12, 0, 0}, p36{lanelet::utils::getId(), 14, 0, 0},
        p37{lanelet::utils::getId(), 10, 3, 0}, p38{lanelet::utils::getId(), 12, 3, 0}, p39{lanelet::utils::getId(), 14, 3, 0},
        p40{lanelet::utils::getId(), 10, 6, 0}, p41{lanelet::utils::getId(), 12, 6, 0}, p42{lanelet::utils::getId(), 14, 6, 0},
        p43{lanelet::utils::getId(), 10, 9, 0}, p44{lanelet::utils::getId(), 12, 9, 0}, p45{lanelet::utils::getId(), 14, 9, 0},
        p46{lanelet::utils::getId(), 10, 21, 0}, p47{lanelet::utils::getId(), 12, 21, 0}, p48{lanelet::utils::getId(), 14, 21, 0},
        p49{lanelet::utils::getId(), 10, 24, 0}, p50{lanelet::utils::getId(), 12, 24, 0}, p51{lanelet::utils::getId(), 14, 24, 0},
        p52{lanelet::utils::getId(), 10, 27, 0}, p53{lanelet::utils::getId(), 12, 27, 0}, p54{lanelet::utils::getId(), 14, 27, 0};
    //END lanelet::Point3d

    //build linestrings from the points listed above for the intersection being constructed
    lanelet::LineString3d ls1{lanelet::utils::getId(), {p12, p9}}, ls2{lanelet::utils::getId(), {p21, p12}}, ls3{lanelet::utils::getId(), {p33, p21}},
        ls4{lanelet::utils::getId(), {p1, p4}}, ls5{lanelet::utils::getId(), {p4, p15}}, ls6{lanelet::utils::getId(), {p15, p27}},
        ls7{lanelet::utils::getId(), {p4, p43}}, ls8{lanelet::utils::getId(), {p43, p34}},
        ls9{lanelet::utils::getId(), {p36, p45}}, ls10{lanelet::utils::getId(), {p45, p15}},
        ls14{lanelet::utils::getId(), {p45, p12}},
        ls15{lanelet::utils::getId(), {p35, p44}}, ls16{lanelet::utils::getId(), {p44, p8}}, ls17{lanelet::utils::getId(), {p44, p18}},
        ls18{lanelet::utils::getId(), {p30, p18}}, ls19{lanelet::utils::getId(), {p18, p8}}, ls20{lanelet::utils::getId(), {p8, p5}},
        ls21{lanelet::utils::getId(), {p52, p46}}, ls22{lanelet::utils::getId(), {p47, p53}}, ls23{lanelet::utils::getId(), {p48, p54}},
        ls24{lanelet::utils::getId(), {p45, p48}}, ls25{lanelet::utils::getId(), {p44, p47}}, ls26{lanelet::utils::getId(), {p46, p43}},
        rightSideRightTurnLeft{lanelet::utils::getId(), {p18, p47}}, rightSideRightTurnRight{lanelet::utils::getId(), {p21, p48}},
        rightSideLeftTurnLeft{lanelet::utils::getId(), {p18, p44}}, rightSideLeftTurnRight{lanelet::utils::getId(), {p21, p43}},
        reverseRightTurnLeft{lanelet::utils::getId(), {p47, p8}}, reverseRightTurnRight{lanelet::utils::getId(), {p46, p12}},
        reverseLeftTurnLeft{lanelet::utils::getId(), {p47, p18}}, reverseLeftTurnRight{lanelet::utils::getId(), {p46, p15}},
        trafficLightOne{lanelet::utils::getId(), {p43, p45}},
        trafficLightTwo{lanelet::utils::getId(), {p15, p21}},
        trafficLightThree{lanelet::utils::getId(), {p48, p46}},
        trafficLightFour{lanelet::utils::getId(), {p12, p4}},
        stopLineOne{lanelet::utils::getId(), {p40, p41, p42}},
        stopLineTwo{lanelet::utils::getId(), {p25, p28, p31}},
        stopLineThree{lanelet::utils::getId(), {p51, p50, p49}},
        stopLineFour{lanelet::utils::getId(), {p11, p7, p3}};
    //END lanelet::LineString3d

    //set the vehicle and pedestrian attributes for the intersection map being constructed
    lanelet::AttributeMap vehicleAttr{{AttrStr::Subtype, Value::Road}, {AttrStr::Location, Value::Urban}};
    lanelet::AttributeMap pedestrianAttr{{AttrStr::Subtype, Value::Walkway}, {AttrStr::Location, Value::Urban}};

    //define a lanelet with the connections through the intersection to other lanelets
    lanelet::Lanelet lanelet{lanelet::utils::getId(), ls9, ls15, vehicleAttr},
                     leftTurn{lanelet::utils::getId(), ls16, ls14, vehicleAttr},
                     rightTurn{lanelet::utils::getId(), ls10, ls17, vehicleAttr},
                     nextTurn{lanelet::utils::getId(), ls24, ls25, vehicleAttr},
                     leftOfFront{lanelet::utils::getId(), ls8, ls15, vehicleAttr};
                     //END lanelet

    //define a lanelet with the connections through the intersection to other lanelets
    lanelet::Lanelet laneletRight{lanelet::utils::getId(), ls3, ls18, vehicleAttr},
                     nextRightTurn{lanelet::utils::getId(), ls2, ls19, vehicleAttr},
                     leftRightTurn{lanelet::utils::getId(), rightSideLeftTurnLeft, rightSideLeftTurnRight, vehicleAttr},
                     rightRightTurn{lanelet::utils::getId(), rightSideRightTurnLeft, rightSideRightTurnRight, vehicleAttr},
                     leftOfRight{lanelet::utils::getId(), ls6, ls18, vehicleAttr};
                     //END laneletRight

    //define a lanelet with the connections through the intersection to other lanelets
    lanelet::Lanelet laneletLeft{lanelet::utils::getId(), ls4, ls20, vehicleAttr},
                     nextLeftTurn{lanelet::utils::getId(), ls19, ls5, vehicleAttr},
                     leftLeftTurn{lanelet::utils::getId(), rightSideRightTurnLeft, rightSideRightTurnRight, vehicleAttr},
                     rightLeftTurn{lanelet::utils::getId(), ls7, ls16, vehicleAttr},
                     leftOfLeft{lanelet::utils::getId(), ls1, ls20, vehicleAttr};
                     //END laneletLeft

    //define a lanelet with the connections through the intersection to other lanelets
    lanelet::Lanelet laneletReverse{lanelet::utils::getId(), ls21, ls22, vehicleAttr},
                     nextReverseTurn{lanelet::utils::getId(), ls26, ls25, vehicleAttr},
                     leftReverseTurn{lanelet::utils::getId(), reverseLeftTurnLeft, reverseLeftTurnRight, vehicleAttr},
                     rightReverseTurn{lanelet::utils::getId(), reverseRightTurnLeft, reverseRightTurnRight, vehicleAttr},
                     leftOfReverse{lanelet::utils::getId(), ls23, ls22, vehicleAttr};
                     //END laneletReverse

    //define a lanelets structure containing the lanelet map arrangement
    lanelet::Lanelets lanelets{lanelet, leftTurn, rightTurn, nextTurn, leftOfFront,
                               laneletRight, nextRightTurn, leftRightTurn, rightRightTurn, leftOfRight,
                               laneletLeft, nextLeftTurn, leftLeftTurn, rightLeftTurn, leftOfLeft,
                               laneletReverse, nextReverseTurn, leftReverseTurn, rightReverseTurn, leftOfReverse};
                               //END lanelets

    //define the path to an interaction dataset OSM file for the full three-way intersection map
    std::string mapPathThreeWay = "./Maps/DR_USA_Intersection_EP1.osm";

    //define the paths to interaction dataset files for the given map
    std::string dataPathThreeWay = "./Data/DR_USA_Intersection_EP1/vehicle_tracks_000.csv";
    std::string dataPathThreeWayTwo = "./Data/DR_USA_Intersection_EP1/vehicle_tracks_001.csv";
    std::string dataPathThreeWayThree = "./Data/DR_USA_Intersection_EP1/vehicle_tracks_002.csv";
    std::string dataPathThreeWayFour = "./Data/DR_USA_Intersection_EP1/vehicle_tracks_003.csv";
    std::string dataPathThreeWayFive = "./Data/DR_USA_Intersection_EP1/vehicle_tracks_004.csv";
    std::string dataPathThreeWaySix = "./Data/DR_USA_Intersection_EP1/vehicle_tracks_005.csv";

    //define the path to an interaction dataset OSM file for the cut three-way intersection map
    std::string mapPathShortThreeWay = "./Maps/DR_USA_Intersection_EP0.osm";

    //define the paths to interaction dataset files for the given map
    std::string dataPathShortThreeWay = "./Data/DR_USA_Intersection_EP0/vehicle_tracks_000.csv";
    std::string dataPathShortThreeWayTwo = "./Data/DR_USA_Intersection_EP0/vehicle_tracks_001.csv";
    std::string dataPathShortThreeWayThree = "./Data/DR_USA_Intersection_EP0/vehicle_tracks_002.csv";
    std::string dataPathShortThreeWayFour = "./Data/DR_USA_Intersection_EP0/vehicle_tracks_003.csv";
    std::string dataPathShortThreeWayFive = "./Data/DR_USA_Intersection_EP0/vehicle_tracks_004.csv";
    std::string dataPathShortThreeWaySix = "./Data/DR_USA_Intersection_EP0/vehicle_tracks_005.csv";
    std::string dataPathShortThreeWaySeven = "./Data/DR_USA_Intersection_EP0/vehicle_tracks_006.csv";
    std::string dataPathShortThreeWayEight = "./Data/DR_USA_Intersection_EP0/vehicle_tracks_007.csv";

    //define the path to an interaction dataset OSM file for the (+)-shaped four-way intersection map
    std::string mapPath = "./Maps/DR_USA_Intersection_MA.osm";

    //define the path an interaction dataset file for the (+)-shaped four-way intersection
    std::string dataPath = "./Data/DR_USA_Intersection_MA/vehicle_tracks_000.csv";
    std::string dataPathTwo = "./Data/DR_USA_Intersection_MA/vehicle_tracks_001.csv";

    //define the path to an interaction dataset OSM file for the (x)-shaped diagonal four-way intersection map
    std::string mapPathDiamond = "./Maps/DR_USA_Intersection_GL.osm";

    //define the path an interaction dataset file for the diagonal four-way intersection
    std::string dataPathDiamond = "./Data/DR_USA_Intersection_GL/vehicle_tracks_000.csv";

    //define the path to the interaction dataset folders containing the interaction datasets for the given maps they reference
    std::string dataPathThreeWayDir = "./Data/DR_USA_Intersection_EP1";
    std::string dataPathShortThreeWayDir = "./Data/DR_USA_Intersection_EP0";
    std::string dataPathDir = "./Data/DR_USA_Intersection_MA";
    std::string dataPathDiamondDir = "./Data/DR_USA_Intersection_GL";
    std::string dataPathUSARoundaboutEPDir = "./Data/DR_USA_Roundabout_EP";
    std::string dataPathUSARoundaboutFTDir = "./Data/DR_USA_Roundabout_FT";
    std::string dataPathUSARoundaboutSRDir = "./Data/DR_USA_Roundabout_SR";
    std::string dataPathDEURoundaboutOFDir = "./Data/DR_DEU_Roundabout_OF";
    std::string dataPathCHNRoundaboutLNDir = "./Data/DR_CHN_Roundabout_LN";

    //define the path an interaction dataset file for the roundabout intersection
    std::string dataPathUSARoundaboutEP = "./Data/DR_USA_Roundabout_EP/vehicle_tracks_000.csv";
    std::string dataPathUSARoundaboutEPFour = "./Data/DR_USA_Roundabout_EP/vehicle_tracks_004.csv";
    std::string dataPathUSARoundaboutEPFive = "./Data/DR_USA_Roundabout_EP/vehicle_tracks_005.csv";

    //define the path an interaction dataset file for the roundabout intersection
    std::string dataPathRoundaboutSR = "./Data/DR_USA_Roundabout_SR/vehicle_tracks_000.csv";

    //define the path to an interaction dataset OSM file for the roundabout maps
    std::string mapPathCHNRoundabout = "./Maps/DR_CHN_Roundabout_LN.osm";
    std::string mapPathUSARoundaboutEP = "./Maps/DR_USA_Roundabout_EP.osm";
    std::string mapPathUSARoundaboutFT = "./Maps/DR_USA_Roundabout_FT.osm";
    std::string mapPathUSARoundaboutSR = "./Maps/DR_USA_Roundabout_SR.osm";
    std::string mapPathDEURoundaboutOF = "./Maps/DR_DEU_Roundabout_OF.osm";

    //define an enum for different nations
    enum Nations{America = 1, Germany = 2, China = 3, Canada = 4}; //END nations

    //define a vector of all data paths
    std::vector<std::string> allDataPaths = {dataPathDir, dataPathThreeWayDir, dataPathShortThreeWayDir, dataPathDiamondDir, dataPathUSARoundaboutEPDir,
                                             dataPathUSARoundaboutFTDir, dataPathUSARoundaboutSRDir, dataPathDEURoundaboutOFDir, dataPathCHNRoundaboutLNDir};
                                             //END allDataPaths

    //define a vector of all map paths
    std::vector<std::string> allMapPaths = {mapPath, mapPathThreeWay, mapPathShortThreeWay, mapPathDiamond, mapPathUSARoundaboutEP,
                                            mapPathUSARoundaboutFT, mapPathUSARoundaboutSR, mapPathDEURoundaboutOF, mapPathCHNRoundabout};
                                            //END allMapPaths

    //define a vector of all pointer types for the above files
    std::vector<Nations> vehiclePtr = {America, America, America, America, America, America, America, Germany, China};
    //END vehiclePtr

    //define runtime constants
    float PI = 3.14159;

    //define how to check yaw at multi-turn intersections
    YawChecking multiTurnYawChecking = YawChecking::Dataset;

    //determines if rule violation tests for interaction datasets are active
    bool fourWayActive = true;
    bool fourWayDiamondActive = true;
    bool threeWayActive = true;
    bool threeWayShortActive = true;
    bool threeWayRoundaboutActive = true;
    bool roundaboutSixWayActive = true;
    bool roundaboutFiveWayActive = true;
    bool roundaboutFourWayActive = true;
    bool roundaboutThreeWayActive = true;
    //determines if sub-tests for interaction datasets are active
    bool stopChecking = true;
    bool turnChecking = false;
    bool offroadChecking = false;
    bool tailgatingChecking = false;
    bool speedingChecking = false;

    //determines which static tests are active
    bool staticTests = false;

    //determines if driving rule tests are active
    bool ruleTests = false;

    //determines if IO tests are active
    bool fileReadTests = false;

    //determines if statistics tests are active
    bool statisticsStaticTests = false;
    bool statisticsOffroadTests = false;
    bool statisticsTailgatingTests = false;
    bool statisticsTailgatingDistributionTests = false;
    bool statisticsTailgatingParameterTests = false;
    bool statisticsGridTests = false;
    bool tailgatingOccuranceByPositionTest = false;

    //define runtime constants
    struct commands stop = {0.5, 6};
    struct commands stopStrictVelocity = {1, 6};
    struct commands stopHardVelocity = {0.25, 6};
    struct commands stopStrictDistance = {0.5, 4};
    struct commands stopHardDistance = {0.5, 8};
    struct commands leftBeforeRight = {0.95, 5};
    struct commands laneletPriority = {0.95, 4};
    struct commands leftTurnPriority = {0.95, 5};
    struct commands tailgating = {5, 2,
                                  2.3, 4.6, 2, 3.9,
                                  3, 6.1, 5.4, 4.9,
                                  0.5, 3, 1, 2.9};
    struct commands dMinDisribution = {5, 2,
                                       2.3, 4.6, 2, 3.9,
                                       3, 6.1, 5.4, 4.9,
                                       0.5, 3, 1, 2.9,
                                       0.25, 1.77, 1, 1.47,
                                       1, 1, 1, 1};

    //define runtime coefficient container
    struct commandInput runtimeCoefficients = {stop, leftBeforeRight, laneletPriority,
                                               leftTurnPriority, tailgating, dMinDisribution};
                                               //END runtimeCoefficients
}; //END TrafficRulesTest

//define rules for vehicles in the testing space
class ChineseTrafficRulesVehicleTest : public TrafficRulesTest {
  public:
    //initialize a pointer for the vehicle rules
    ChineseVehiclePtr chineseVehiclePtr{getChineseVehicleRules()};
};

//define rules for bicycles in the testing space
class ChineseTrafficRulesBicycle : public TrafficRulesTest {
  public:
    //initialize a pointer for the bicycle rules
    ChineseBicyclePtr ChineseBicycle{getChineseBicycleRules()};
};

//define rules for pedestrians in the testing space
class ChineseTrafficRulesPedestrian : public TrafficRulesTest {
  public:
    //define the Canadian traffic rules for pedestrians
    ChineseTrafficRulesPedestrian() {
      lanelet.attributes() = pedestrianAttr;
      rightTurn.attributes() = pedestrianAttr;
      nextTurn.attributes() = pedestrianAttr;
    } //END ChineseTrafficRulesPedestrian

    //initialize a pointer for the pedestrian rules
    ChinesePedestrianPtr ChinesePedestrian{getChinesePedestrianRules()};
}; //END ChineseTrafficRulesPedestrian

//define rules for vehicles in the testing space
class CanadianTrafficRulesVehicleTest : public TrafficRulesTest {
  public:
    //initialize a pointer for the vehicle rules
    CanadianVehiclePtr canadianVehiclePtr{getCanadianVehicleRules()};
};

//define rules for bicycles in the testing space
class CanadianTrafficRulesBicycle : public TrafficRulesTest {
  public:
    //initialize a pointer for the bicycle rules
    CanadianBicyclePtr CanadianBicycle{getCanadianBicycleRules()};
};

//define rules for pedestrians in the testing space
class CanadianTrafficRulesPedestrian : public TrafficRulesTest {
  public:
    //define the Canadian traffic rules for pedestrians
    CanadianTrafficRulesPedestrian() {
      lanelet.attributes() = pedestrianAttr;
      rightTurn.attributes() = pedestrianAttr;
      nextTurn.attributes() = pedestrianAttr;
    } //END CanadianTrafficRulesPedestrian

    //initialize a pointer for the pedestrian rules
    CanadianPedestrianPtr CanadianPedestrian{getCanadianPedestrianRules()};
}; //END CanadianTrafficRulesPedestrian

//define rules for vehicles in the testing space
class GermanTrafficRulesVehicleTest : public TrafficRulesTest {
  public:
    //initialize a pointer for the vehicle rules
    GermanVehiclePtr germanVehiclePtr{getGermanVehicleRules()};
};

//define rules for bicycles in the testing space
class GermanTrafficRulesBicycle : public TrafficRulesTest {
  public:
    //initialize a pointer for the bicycle rules
    GermanBicyclePtr GermanBicycle{getGermanBicycleRules()};
};

//define rules for pedestrians in the testing space
class GermanTrafficRulesPedestrian : public TrafficRulesTest {
  public:
    //define the Canadian traffic rules for pedestrians
    GermanTrafficRulesPedestrian() {
      lanelet.attributes() = pedestrianAttr;
      rightTurn.attributes() = pedestrianAttr;
      nextTurn.attributes() = pedestrianAttr;
    } //END GermanTrafficRulesPedestrian

    //initialize a pointer for the pedestrian rules
    GermanPedestrianPtr GermanPedestrian{getGermanPedestrianRules()};
}; //END GermanTrafficRulesPedestrian

//define rules for vehicles in the testing space
class AmericanTrafficRulesVehicleTest : public TrafficRulesTest {
  public:
    //initialize a pointer for the vehicle rules
    AmericanVehiclePtr americanVehiclePtr{getAmericanVehicleRules()};
};

//define rules for bicycles in the testing space
class AmericanTrafficRulesBicycle : public TrafficRulesTest {
  public:
    //initialize a pointer for the bicycle rules
    AmericanBicyclePtr AmericanBicycle{getAmericanBicycleRules()};
};

//define rules for pedestrians in the testing space
class AmericanTrafficRulesPedestrian : public TrafficRulesTest {
  public:
    //define the American traffic rules for pedestrians
    AmericanTrafficRulesPedestrian() {
      lanelet.attributes() = pedestrianAttr;
      rightTurn.attributes() = pedestrianAttr;
      nextTurn.attributes() = pedestrianAttr;
    } //END AmericanTrafficRulesPedestrian

    //initialize a pointer for the pedestrian rules
    AmericanPedestrianPtr AmericanPedestrian{getAmericanPedestrianRules()};
}; //END AmericanTrafficRulesPedestrian

//defines an initializer function
void initializer() {
  //do nothing
  NULL;
} //END initializer

/*
To Test Rules Related to Traffic Signs, Use the Following:
  -> Define stopline and sign locations as linestrings:
      lanelet::LineString3d stop = stopLineOne;
  -> Define a lanelet traffic sign pointer, assign a traffic sign stopline location, and sign value string while doing so:
      lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stop}, germanSignIDs.STOP});
  -> Add the traffic sign as a regulatory element:
      lanelet.addRegulatoryElement(signTwo);
  -> Define any other attributes associated with the lanelets:
      For example: lanelet.setAttribute(Attr::Subtype, Value::Road);
  -> Define the vehicle position or positions:
      lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
      lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 11, 25, 0);
  -> Obtain or use the pre-define lanelet::lanelets structure containing the list of all lanelets at the intersection.
  -> Finally, test rules associated with traffic signs:
      EXPECT_EQ(canadianVehiclePtr->priorityValue(lanelet, Right), 6);
      EXPECT_TRUE(canadianVehiclePtr->shouldStop(lanelet, vehiclePosition));
      EXPECT_TRUE(canadianVehiclePtr->shouldTurnRightBeforeLeft(lanelet, laneletOther, Right, Straight, lanelets, otherVehiclePosition, vehiclePosition));
      EXPECT_FALSE(canadianVehiclePtr->shouldHavePriority(lanelet, laneletOther, Right, Straight, lanelets, otherVehiclePosition, vehiclePosition));
      EXPECT_FALSE(canadianVehiclePtr->shouldBeTurningLeft(lanelet, laneletOther, Right, Straight, lanelets, otherVehiclePosition, vehiclePosition));

To Test Rules Related to Traffic Lights, Use the Following:
  -> Define stopline and sign locations as linestrings:
      lanelet::LineString3d stop = stopLineOne;
  -> Define traffic light attribute values for the given lanelet:
      light.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
  -> Define a lanelet traffic light pointer, assign a traffic light stopline location, and a traffic light position:
      lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {light}, {light});
  -> Add the traffic light as a regulatory element:
      lanelet.addRegulatoryElement(trafficLightRegelem);
  -> Define any other attributes associated with the lanelets:
      For example: lanelet.setAttribute(Attr::Subtype, Value::Road);
  -> Define the vehicle position or positions:
      lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
  -> Define a stopping distance for the given vehicle:
      int vehicleStoppingDistance = 4;
  -> Obtain or use the pre-define lanelet::lanelets structure containing the list of all lanelets at the intersection:
      EXPECT_TRUE(canadianVehiclePtr->shouldWaitAtTrafficLight(lanelet, Straight, Straight, Yellow, vehicleStoppingDistance, vehiclePosition));
  -> 
*/
namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestOne) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stop = stopLineOne;
    lanelet::LineString3d sign = trafficLightOne;

    //initialize the traffic light attribute value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stop}, germanSignIDs.STOP});
    //initialize the traffic light as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //initialize the traffic light attribute value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{sign}, germanSignIDs.RIGHT_TURN_PRIORITY_FOUR_WAY});
    //initialize the traffic light as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signTwo);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    /* Determine if the Lanelet Priority Matches our Anticipated Priority Value */
    EXPECT_EQ(canadianVehiclePtr->priorityValue(lanelet, Right), 6);
  } //END TestOne
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestOneTwo) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stop = stopLineTwo;
    lanelet::LineString3d sign = trafficLightTwo;

    //initialize the traffic light attribute value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stop}, germanSignIDs.STOP});
    //initialize the traffic light as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signOne);

    //initialize the traffic light attribute value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{sign}, germanSignIDs.RIGHT_TURN_PRIORITY_FOUR_WAY});
    //initialize the traffic light as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signTwo);

    //set the road type for the reference lanelet
    laneletRight.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority Matches our Anticipated Priority Value
    EXPECT_EQ(canadianVehiclePtr->priorityValue(laneletRight, Right), 6);
  } //END TestOneTwo
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestTwo) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stop = stopLineOne;
    lanelet::LineString3d sign = trafficLightOne;

    //initialize the traffic light attribute value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{sign}, germanSignIDs.STOP});
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    signOne->addRefLine(stop);
    //initialize the traffic light as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);

    //check if the vehicle should wait at the given stop sign given the stop sign and vehicles positions
    EXPECT_TRUE(canadianVehiclePtr->shouldStop(lanelet, vehiclePosition));
  } //END TestTwo
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestTwoTwo) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d signPos = trafficLightOne;

    //initialize the traffic light attribute value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{signPos}, germanSignIDs.STOP});
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    signOne->addRefLine(stopline);
    //initialize the traffic light as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 7, 0);

    //check if the vehicle should wait at the given stop sign given the stop sign and vehicles positions
    EXPECT_FALSE(canadianVehiclePtr->shouldStop(lanelet, vehiclePosition));
  } //END TestTwoTwo
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestTwoThree) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineTwo;
    lanelet::LineString3d signPos = trafficLightTwo;

    //initialize the traffic light attribute value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{signPos}, germanSignIDs.STOP});
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    signOne->addRefLine(stopline);
    //initialize the traffic light as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signOne);

    //set the road type for the reference lanelet
    laneletRight.setAttribute(Attr::Subtype, Value::Road);

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //check if the vehicle should wait at the given stop sign given the stop sign and vehicles positions
    EXPECT_TRUE(canadianVehiclePtr->shouldStop(laneletRight, vehiclePosition));
  } //END TestTwoThree
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestTwoFour) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineTwo;
    lanelet::LineString3d signPos = trafficLightTwo;

    //initialize the traffic light attribute value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{signPos}, germanSignIDs.STOP});
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    signOne->addRefLine(stopline);
    //initialize the traffic light as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signOne);

    //set the road type for the reference lanelet
    laneletRight.setAttribute(Attr::Subtype, Value::Road);

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 17, 16, 0);

    //check if the vehicle should wait at the given stop sign given the stop sign and vehicles positions
    EXPECT_FALSE(canadianVehiclePtr->shouldStop(laneletRight, vehiclePosition));
  } //END TestTwoFour
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestThree) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stop = stopLineOne;
    lanelet::LineString3d light = trafficLightOne;

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stop}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signOne->addRefLine(stop);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //initialize the traffic light attribute value
    light.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {light}, {light});
    //initialize the traffic light as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(trafficLightRegelem);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);

    //check if the vehicle should wait at the given stop sign given the stop sign and vehicles positions
    EXPECT_TRUE(canadianVehiclePtr->shouldStop(lanelet, vehiclePosition));
  } //END TestThree
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestThreeTwo) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stop = stopLineTwo;
    lanelet::LineString3d light = trafficLightTwo;

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stop}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signOne->addRefLine(stop);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signOne);

    //initialize the traffic light attribute value
    light.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {light}, {light});
    //initialize the traffic light as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(trafficLightRegelem);

    //set the road type for the reference lanelet
    laneletRight.setAttribute(Attr::Subtype, Value::Road);

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //check if the vehicle should wait at the given stop sign given the stop sign and vehicles positions
    EXPECT_TRUE(canadianVehiclePtr->shouldStop(laneletRight, vehiclePosition));
  } //END TestThreeTwo
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestFour) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d light = stopLineOne;
    lanelet::LineString3d sign = trafficLightOne;

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{sign}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signOne->addRefLine(sign);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //initialize the traffic light attribute value
    light.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {sign}, {light});
    //initialize the traffic light as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(trafficLightRegelem);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);

    //check if the vehicle should wait at the given stop sign given the stop sign and vehicles positions
    EXPECT_FALSE(canadianVehiclePtr->shouldStop(lanelet, vehiclePosition));
  } //END TestFour
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestFourTwo) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d light = stopLineTwo;
    lanelet::LineString3d sign = trafficLightTwo;

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{sign}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signOne->addRefLine(sign);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signOne);

    //initialize the traffic light attribute value
    light.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {sign}, {light});
    //initialize the traffic light as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(trafficLightRegelem);

    //set the road type for the reference lanelet
    laneletRight.setAttribute(Attr::Subtype, Value::Road);

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //check if the vehicle should wait at the given stop sign given the stop sign and vehicles positions
    EXPECT_FALSE(canadianVehiclePtr->shouldStop(laneletRight, vehiclePosition));
  } //END TestFourTwo
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestFive) {
    //define a stopping distance for the vehicle
    int vehicleStoppingDistance = 1;

    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d lightPos = trafficLightOne;

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.RIGHT_TURN_ARROW});
    //add a reference line for the traffic sign
    signOne->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //initialize the traffic light attribute value
    lightPos.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {lightPos}, {stopline});
    //initialize the traffic light as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(trafficLightRegelem);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //check if the vehicle should wait at the given traffic light given its current position and stopping distance
    EXPECT_TRUE(canadianVehiclePtr->shouldWaitAtTrafficLight(lanelet, Right, Right, Red, vehicleStoppingDistance, vehiclePosition));
  } //END TestFive
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestFiveTwo) {
    //define a stopping distance for the vehicle
    int vehicleStoppingDistance = 2;

    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d lightPos = trafficLightOne;

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{lightPos}, germanSignIDs.RIGHT_TURN_ARROW});
    //add a reference line for the traffic sign
    signOne->addRefLine(lightPos);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signOne);

    //initialize the traffic light attribute value
    stopline.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {stopline}, {stopline});
    //initialize the traffic light as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(trafficLightRegelem);

    //set the road type for the reference lanelet
    laneletRight.setAttribute(Attr::Subtype, Value::Road);

    //check if the vehicle should wait at the given traffic light given its current position and stopping distance
    EXPECT_TRUE(canadianVehiclePtr->shouldWaitAtTrafficLight(laneletRight, Right, Right, Red, vehicleStoppingDistance, vehiclePosition));
  } //END TestFiveTwo
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestSix) {
    //define a stopping distance for the vehicle
    int vehicleStoppingDistance = 1;

    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d lightPos = trafficLightOne;

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{lightPos}, germanSignIDs.RIGHT_TURN_ARROW});
    //add a reference line for the traffic sign
    signOne->addRefLine(lightPos);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signTwo);

    //initialize the traffic light attribute value
    lightPos.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {lightPos}, {lightPos});
    //initialize the traffic light as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(trafficLightRegelem);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //check if the vehicle should wait at the given traffic light given its current position and stopping distance
    EXPECT_FALSE(canadianVehiclePtr->shouldWaitAtTrafficLight(lanelet, Right, Right, Yellow, vehicleStoppingDistance, vehiclePosition));
  } //END TestSix
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestSixTwo) {
    //define a stopping distance for the vehicle
    int vehicleStoppingDistance = 2;

    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d lightPos = trafficLightOne;

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);

    //initialize the traffic light attribute value
    lightPos.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {lightPos}, {stopline});
    //initialize the traffic light as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(trafficLightRegelem);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //check if the vehicle should wait at the given traffic light given its current position and stopping distance
    EXPECT_FALSE(canadianVehiclePtr->shouldWaitAtTrafficLight(lanelet, Right, Right, Yellow, vehicleStoppingDistance, vehiclePosition));
  } //END TestSixTwo
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestSixThree) {
    //define a stopping distance for the vehicle
    int vehicleStoppingDistance = 1;

    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d lightPos = trafficLightOne;

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);

    //initialize the traffic light attribute value
    lightPos.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {lightPos}, {stopline});
    //initialize the traffic light as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(trafficLightRegelem);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //check if the vehicle should wait at the given traffic light given its current position and stopping distance
    EXPECT_TRUE(canadianVehiclePtr->shouldWaitAtTrafficLight(lanelet, Right, Right, Yellow, vehicleStoppingDistance, vehiclePosition));
  } //END TestSixThree
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestSixFour) {
    //define a stopping distance for the vehicle
    int vehicleStoppingDistance = 5;

    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineTwo;
    lanelet::LineString3d lightPos = trafficLightTwo;

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{lightPos}, germanSignIDs.RIGHT_TURN_ARROW});
    //add a reference line for the traffic sign
    signOne->addRefLine(lightPos);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signOne);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signTwo);

    //initialize the traffic light attribute value
    lightPos.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {lightPos}, {lightPos});
    //initialize the traffic light as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(trafficLightRegelem);

    //set the road type for the reference lanelet
    laneletRight.setAttribute(Attr::Subtype, Value::Road);

    //check if the vehicle should wait at the given traffic light given its current position and stopping distance
    EXPECT_FALSE(canadianVehiclePtr->shouldWaitAtTrafficLight(laneletRight, Straight, Straight, Yellow, vehicleStoppingDistance, vehiclePosition));
  } //END TestSixFour
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestSixFive) {
    //define a stopping distance for the vehicle
    int vehicleStoppingDistance = 4;

    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineTwo;
    lanelet::LineString3d lightPos = trafficLightTwo;

    //define positions for both the ego/reference vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{lightPos}, germanSignIDs.RIGHT_TURN_ARROW});
    //add a reference line for the traffic sign
    signOne->addRefLine(lightPos);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signOne);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signTwo);

    //initialize the traffic light attribute value
    lightPos.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {lightPos}, {lightPos});
    //initialize the traffic light as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(trafficLightRegelem);

    //set the road type for the reference lanelet
    laneletRight.setAttribute(Attr::Subtype, Value::Road);

    //check if the vehicle should wait at the given traffic light given its current position and stopping distance
    EXPECT_TRUE(canadianVehiclePtr->shouldWaitAtTrafficLight(laneletRight, Straight, Straight, Yellow, vehicleStoppingDistance, vehiclePosition));
  } //END TestSixFive
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestSeven) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d lightPos = trafficLightOne;

    //set the location of the stop lines as local variables
    lanelet::LineString3d stoplineTwo = stopLineTwo;
    lanelet::LineString3d lightPosTwo = trafficLightTwo;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signTwo);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signThree = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stoplineTwo}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signThree->addRefLine(stoplineTwo);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signThree);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority Matches our Anticipated Priority Value
    EXPECT_EQ(canadianVehiclePtr->priorityValue(lanelet, Right), canadianVehiclePtr->priorityValue(laneletRight, Straight));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_TRUE(canadianVehiclePtr->shouldTurnRightBeforeLeft(lanelet, laneletRight, Right, Straight, lanelets, vehiclePosition, otherVehiclePosition));
  } //END TestSeven
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestSevenTwo) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d lightPos = trafficLightOne;

    //set the location of the stop lines as local variables
    lanelet::LineString3d stoplineTwo = stopLineTwo;
    lanelet::LineString3d lightPosTwo = trafficLightTwo;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{lightPos}, germanSignIDs.RIGHT_TRAFFIC_PRIORITY});
    //add a reference line for the traffic sign
    signOne->addRefLine(lightPos);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signTwo);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signThree = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stoplineTwo}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signThree->addRefLine(stoplineTwo);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signThree);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority for the First Lanelet is Three Units Greater then the Second Lanelet
    EXPECT_EQ(canadianVehiclePtr->priorityValue(lanelet, Right), 3 + canadianVehiclePtr->priorityValue(laneletRight, Straight));

    //check Priority Rules for the Other Vehicle to Determine that it can Move, Whereas the Ego Vehicle Cannot
    EXPECT_TRUE(canadianVehiclePtr->shouldHavePriority(laneletRight, lanelet, Straight, Right, lanelets, otherVehiclePosition, vehiclePosition));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_FALSE(canadianVehiclePtr->shouldTurnRightBeforeLeft(lanelet, laneletRight, Right, Straight, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldHavePriority(lanelet, laneletRight, Right, Straight, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldBeTurningLeft(lanelet, laneletRight, Right, Straight, lanelets, vehiclePosition, otherVehiclePosition));
  } //END TestSevenTwo
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestSevenThree) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d lightPos = trafficLightOne;

    //set the location of the stop lines as local variables
    lanelet::LineString3d stoplineTwo = stopLineTwo;
    lanelet::LineString3d lightPosTwo = trafficLightTwo;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signTwo);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signThree = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stoplineTwo}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signThree->addRefLine(stoplineTwo);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signThree);

    //initialize the traffic light attribute value
    lightPos.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelem = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {lightPos}, {lightPos});
    //initialize the traffic light as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(trafficLightRegelem);

    //initialize the traffic light attribute value
    lightPosTwo.attributes()[lanelet::AttributeName::Type] = lanelet::AttributeValueString::TrafficLight;
    //define a traffic light regulatory element and place it in a predefined location with a predefined stop line
    lanelet::RegulatoryElementPtr trafficLightRegelemTwo = lanelet::TrafficLight::make(lanelet::utils::getId(), {}, {lightPosTwo}, {lightPosTwo});
    //initialize the traffic light as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(trafficLightRegelemTwo);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority Matches our Anticipated Priority Value
    EXPECT_EQ(canadianVehiclePtr->priorityValue(lanelet, Right), canadianVehiclePtr->priorityValue(laneletRight, Straight));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_FALSE(canadianVehiclePtr->shouldTurnRightBeforeLeft(lanelet, laneletRight, Right, Straight, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldHavePriority(lanelet, laneletRight, Right, Right, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldBeTurningLeft(lanelet, laneletRight, Right, Right, lanelets, vehiclePosition, otherVehiclePosition));
  } //END TestSevenThree
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestSevenFour) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineTwo;
    lanelet::LineString3d stoplineTwo = stopLineThree;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 11, 25, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopLineThree}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stopLineThree);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletReverse.addRegulatoryElement(signTwo);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signThree = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopLineTwo}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signThree->addRefLine(stopLineTwo);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signThree);

    //set the road type for the reference lanelet
    laneletReverse.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority Matches our Anticipated Priority Value
    EXPECT_EQ(canadianVehiclePtr->priorityValue(laneletRight, Right), canadianVehiclePtr->priorityValue(laneletReverse, Straight));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_TRUE(canadianVehiclePtr->shouldTurnRightBeforeLeft(laneletRight, laneletReverse, Right, Straight, lanelets, otherVehiclePosition, vehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldHavePriority(laneletRight, laneletReverse, Right, Straight, lanelets, otherVehiclePosition, vehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldBeTurningLeft(laneletRight, laneletReverse, Right, Straight, lanelets, otherVehiclePosition, vehiclePosition));
  } //END TestSevenFour
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestSevenFive) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineTwo;
    lanelet::LineString3d stoplineTwo = stopLineThree;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 11, 25, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopLineThree}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stopLineThree);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletReverse.addRegulatoryElement(signTwo);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signThree = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopLineOne}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signThree->addRefLine(stopLineOne);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signThree);

    //set the road type for the reference lanelet
    laneletReverse.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority Matches our Anticipated Priority Value
    EXPECT_EQ(canadianVehiclePtr->priorityValue(lanelet, Right), canadianVehiclePtr->priorityValue(laneletReverse, Straight));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_FALSE(canadianVehiclePtr->shouldTurnRightBeforeLeft(lanelet, laneletReverse, Right, Straight, lanelets, otherVehiclePosition, vehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldHavePriority(lanelet, laneletReverse, Right, Straight, lanelets, otherVehiclePosition, vehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldBeTurningLeft(lanelet, laneletReverse, Right, Straight, lanelets, otherVehiclePosition, vehiclePosition));
  } //END TestSevenFive
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestEight) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d stoplineTwo = stopLineTwo;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signOne->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stoplineTwo}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stoplineTwo);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signTwo);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signThree = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{trafficLightTwo}, germanSignIDs.PRIORITY_NEXT_INTERSECTION});
    //add a reference line for the traffic sign
    signThree->addRefLine(trafficLightTwo);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signThree);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority for the Second Lanelet is Four Units Greater then the First Lanelet
    EXPECT_EQ(4 + canadianVehiclePtr->priorityValue(lanelet, Right), canadianVehiclePtr->priorityValue(laneletRight, Right));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_FALSE(canadianVehiclePtr->shouldTurnRightBeforeLeft(lanelet, laneletRight, Right, Right, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_TRUE(canadianVehiclePtr->shouldHavePriority(lanelet, laneletRight, Right, Right, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldBeTurningLeft(lanelet, laneletRight, Right, Right, lanelets, vehiclePosition, otherVehiclePosition));
  } //END TestEight
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestEightTwo) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d stoplineTwo = stopLineThree;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 11, 25, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signOne->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stoplineTwo}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stoplineTwo);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletReverse.addRegulatoryElement(signTwo);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signThree = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{trafficLightThree}, germanSignIDs.PRIORITY_NEXT_INTERSECTION});
    //add a reference line for the traffic sign
    signThree->addRefLine(trafficLightThree);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletReverse.addRegulatoryElement(signThree);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority for the Second Lanelet is Five Units Greater then the First Lanelet
    EXPECT_EQ(5 + canadianVehiclePtr->priorityValue(lanelet, Right), canadianVehiclePtr->priorityValue(laneletReverse, Straight));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_FALSE(canadianVehiclePtr->shouldTurnRightBeforeLeft(lanelet, laneletReverse, Right, Straight, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_TRUE(canadianVehiclePtr->shouldHavePriority(lanelet, laneletReverse, Right, Straight, lanelets, vehiclePosition, otherVehiclePosition));
  } //END TestEightTwo
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestEightThree) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d stoplineThree = stopLineThree;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 11, 25, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signOne->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stoplineThree}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stoplineThree);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletReverse.addRegulatoryElement(signTwo);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signThree = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{trafficLightThree}, germanSignIDs.PRIORITY_NEXT_INTERSECTION});
    //add a reference line for the traffic sign
    signThree->addRefLine(trafficLightThree);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletReverse.addRegulatoryElement(signThree);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    /* Determine if the Lanelet Priority for the Second Lanelet is Five Units Greater then the First Lanelet */
    EXPECT_EQ(5 + canadianVehiclePtr->priorityValue(lanelet, Left), canadianVehiclePtr->priorityValue(laneletReverse, Straight));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_FALSE(canadianVehiclePtr->shouldTurnRightBeforeLeft(lanelet, laneletReverse, Left, Straight, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldHavePriority(lanelet, laneletReverse, Left, Straight, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_TRUE(canadianVehiclePtr->shouldBeTurningLeft(lanelet, laneletReverse, Left, Straight, lanelets, vehiclePosition, otherVehiclePosition));
  } //END TestEightThree
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestNine) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d stoplineTwo = stopLineThree;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 11, 25, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signOne->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stoplineTwo}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stoplineTwo);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletReverse.addRegulatoryElement(signTwo);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority Matches for both Lanelets
    EXPECT_EQ(canadianVehiclePtr->priorityValue(lanelet, Left), canadianVehiclePtr->priorityValue(laneletReverse, Straight));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_FALSE(canadianVehiclePtr->shouldTurnRightBeforeLeft(lanelet, laneletReverse, Left, Straight, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_TRUE(canadianVehiclePtr->shouldBeTurningLeft(lanelet, laneletReverse, Left, Straight, lanelets, vehiclePosition, otherVehiclePosition));
  } //END TestNine
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestNineTwo) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d stoplineTwo = stopLineThree;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 11, 25, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signOne->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stoplineTwo}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stoplineTwo);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletReverse.addRegulatoryElement(signTwo);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority Matches for both Lanelets
    EXPECT_EQ(canadianVehiclePtr->priorityValue(lanelet, Left), canadianVehiclePtr->priorityValue(laneletReverse, Right));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_FALSE(canadianVehiclePtr->shouldTurnRightBeforeLeft(lanelet, laneletReverse, Left, Right, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_TRUE(canadianVehiclePtr->shouldBeTurningLeft(lanelet, laneletReverse, Left, Right, lanelets, vehiclePosition, otherVehiclePosition));
  } //END TestNineTwo
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestNineThree) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineOne;
    lanelet::LineString3d stoplineTwo = stopLineTwo;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 19, 16, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signOne->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    lanelet.addRegulatoryElement(signOne);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stoplineTwo}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stoplineTwo);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signTwo);

    //set the road type for the reference lanelet
    lanelet.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority Matches for both Lanelets
    EXPECT_EQ(canadianVehiclePtr->priorityValue(lanelet, Left), canadianVehiclePtr->priorityValue(laneletRight, Right));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_TRUE(canadianVehiclePtr->shouldTurnRightBeforeLeft(lanelet, laneletRight, Left, Right, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldBeTurningLeft(lanelet, laneletRight, Left, Right, lanelets, vehiclePosition, otherVehiclePosition));
  } //END TestNineThree
} //END rules

namespace rules {
  TEST_F(CanadianTrafficRulesVehicleTest, TestNineFour) {
    //set the location of the stop lines as local variables
    lanelet::LineString3d stopline = stopLineFour;
    lanelet::LineString3d stoplineTwo = stopLineTwo;

    //define positions for both the ego/reference vehicle and the other vehicle
    lanelet::Point3d vehiclePosition(lanelet::utils::getId(), 13, 5, 0);
    lanelet::Point3d otherVehiclePosition(lanelet::utils::getId(), 5, 14, 0);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signOne = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stopline}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signOne->addRefLine(stopline);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletLeft.addRegulatoryElement(signOne);

    //define a traffic sign regulatory element, place it in a predefined location, and give it a STOP sign value
    lanelet::TrafficSign::Ptr signTwo = lanelet::TrafficSign::make(lanelet::utils::getId(), {}, {{stoplineTwo}, germanSignIDs.STOP});
    //add a reference line for the traffic sign
    signTwo->addRefLine(stoplineTwo);
    //initialize the traffic sign as a regulatory element for the lanelet
    laneletRight.addRegulatoryElement(signTwo);

    //set the road type for the reference lanelet
    laneletLeft.setAttribute(Attr::Subtype, Value::Road);

    //determine if the Lanelet Priority Matches for both Lanelets
    EXPECT_EQ(canadianVehiclePtr->priorityValue(laneletLeft, Left), canadianVehiclePtr->priorityValue(laneletRight, Right));

    //check Rules for Crossing the Stop Regulated Intersection after Stopping
    EXPECT_FALSE(canadianVehiclePtr->shouldTurnRightBeforeLeft(laneletLeft, laneletRight, Left, Right, lanelets, vehiclePosition, otherVehiclePosition));
    EXPECT_FALSE(canadianVehiclePtr->shouldBeTurningLeft(laneletLeft, laneletRight, Left, Right, lanelets, vehiclePosition, otherVehiclePosition));
  } //END TestNineFour
} //END rules

namespace vehicleData {
  TEST_F(CanadianTrafficRulesVehicleTest, fileReadTestOne) {
    if (fileReadTests) {
      //open and extract data from the given dataset
      std::vector<struct vehicles> vehicleData = canadianVehiclePtr->getVehicleData(dataPath);

      //obtain the starting and ending x-coordinate positions for the first vehicle in the given dataset
      EXPECT_EQ(vehicleData[0].xStart, 1013.972);
      EXPECT_EQ(vehicleData[0].xEnd, 1015.196);
    } //EMD IF
  } //END TEST_F(CanadianTrafficRulesVehicleTest, fileReadTestOne)
} //END vehicleData

namespace vehicleData {
  TEST_F(CanadianTrafficRulesVehicleTest, fileReadTestTwo) {
    if (fileReadTests) {
      //obtain a pair representing the standstill intervals, contains two integers to show when a vehicle stopped and when it started
      EXPECT_EQ(canadianVehiclePtr->standstillIntervals(canadianVehiclePtr->getVehicleData(dataPath)[0].intervals, mapPath, 2).size(), 1);

      //get the standstill intervals for the given vehicles, showing when the vehicle stopped and when it started again
      EXPECT_EQ(std::get<0>(canadianVehiclePtr->standstillIntervals(canadianVehiclePtr->getVehicleData(dataPath)[0].intervals, mapPath, 2)[0]), 3400);
      EXPECT_EQ(std::get<1>(canadianVehiclePtr->standstillIntervals(canadianVehiclePtr->getVehicleData(dataPath)[0].intervals, mapPath, 2)[0]), 10900);
    } //EMD IF
  } //END TEST_F(CanadianTrafficRulesVehicleTest, fileReadTestTwo)
} //END vehicleData

namespace vehicleData {
  TEST_F(CanadianTrafficRulesVehicleTest, fileReadTestThree) {
    if (fileReadTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = canadianVehiclePtr->getVehicleData(dataPath);

      // Test if we can Extract the Correct Vehicle IDs from the Given Dataset

      //entry 38 is vehicle 43
      EXPECT_EQ(vehicleData[38].intervals[0].id, 43);
      //entry 55 is vehicle 60
      EXPECT_EQ(vehicleData[55].intervals[0].id, 60);
      //entry 56 is vehicle 61
      EXPECT_EQ(vehicleData[56].intervals[0].id, 61);
      //entry 59 is vehicle 64
      EXPECT_EQ(vehicleData[59].intervals[0].id, 64);
      //entry 62 is vehicle 67
      EXPECT_EQ(vehicleData[62].intervals[0].id, 67);
      //entry 65 is vehicle 70
      EXPECT_EQ(vehicleData[65].intervals[0].id, 70);
      //entry 86 is vehicle 95
      EXPECT_EQ(vehicleData[86].intervals[0].id, 95);
      //entry 90 is vehicle 99
      EXPECT_EQ(vehicleData[90].intervals[0].id, 99);
      //entry 112 is vehicle 121
      EXPECT_EQ(vehicleData[112].intervals[0].id, 121);
      //entry 114 is vehicle 123
      EXPECT_EQ(vehicleData[114].intervals[0].id, 123);
      //entry 121 is vehicle 130
      EXPECT_EQ(vehicleData[121].intervals[0].id, 130);
      //entry 126 is vehicle 135
      EXPECT_EQ(vehicleData[126].intervals[0].id, 135);
      //entry 129 is vehicle 138
      EXPECT_EQ(vehicleData[129].intervals[0].id, 138);
      //entry 134 is vehicle 143
      EXPECT_EQ(vehicleData[134].intervals[0].id, 143);
    } //EMD IF
  } //END TEST_F(CanadianTrafficRulesVehicleTest, fileReadTestThree)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, shouldStopTestStatic) {
    if (staticTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(dataPath);
      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      // Perform Testing to see if Vehicles have Stopped Correctly

      //vehicle 43 preforms a rolling stop and did not come to a rest.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[38].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), false);
      //vehicle 59 preforms a rolling stop, not a correct stop.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[54].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), false);
      //vehicle 60 preforms a rolling stop, not a correct stop.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[55].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), false);
      //vehicle 61 stops correctly.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[56].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), true);
      //vehicle 64 stops correctly.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[59].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), true);
      //vehicle 70 stops correctly.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[65].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), true);
      //vehicle 92 stops past the stop line, but is still within bounds to be considered as having stopped correctly.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[83].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), true);
      //vehicle 95 stops too far before stop line and is not considered to have stopped correctly.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[86].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), false);
      //vehicle 96 stops correctly.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[87].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), true);
      //vehicle 97 stops past the stop line a little, but is still within bounds to be considered as having stopped correctly.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[88].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), true);
      //vehicle 99 stops correctly.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[90].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), true);
      //vehicle 133 stops correctly, and is on the lanelet to the right of the map.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[124].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), true);
      //vehicle 135 does not stop at the stop line.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[126].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), false);
      //vehicle 136 blows past stop line and did not stop at all.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[127].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), false);
      //vehicle 138 misses the stop line and is out of bounds.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[129].intervals, mapPath, originPoint, 6, 0.5, lanelet::traffic_rules::America).ruleStatus), false);
     } //EMD IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, shouldStopTestStatic)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, shouldStopTestStaticMulti) {
    if (staticTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(dataPathUSARoundaboutEP);
      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //verify that we are looking at vehicle 35 on the roundabout map
      EXPECT_EQ(vehicleData[27].intervals[0].id, 35);

      //verify that we are looking at vehicle 38 on the roundabout map
      EXPECT_EQ(vehicleData[30].intervals[0].id, 38);

      //vehicle 35 fails to stop at the second stop line.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[27].intervals, mapPathUSARoundaboutEP, originPoint, 4, 0.5,
                                                    lanelet::traffic_rules::America, MultiStop).ruleStatus), false);

      //vehicle 38 stops at both stop lines.
      EXPECT_EQ((americanVehiclePtr->checkStopAtSign(vehicleData[30].intervals, mapPathUSARoundaboutEP, originPoint, 4, 0.5,
                                                    lanelet::traffic_rules::America, MultiStop).ruleStatus), true);
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, shouldStopTestStaticMulti)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, laneletGeometryTestStatic) {
    if (staticTests) {
      // Define Point Positions on the Map

      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);
      //create a list of points to find the closest lanelet on the map to
      lanelet::BasicPoint2d leftPoint = lanelet::BasicPoint2d(977.979, 1004.738);
      lanelet::BasicPoint2d topPoint = lanelet::BasicPoint2d(1017.402, 1014.117);
      lanelet::BasicPoint2d bottomPoint = lanelet::BasicPoint2d(1032, 987);
      lanelet::BasicPoint2d rightPoint = lanelet::BasicPoint2d(1038.5, 1014.7);

      // Finds the Intersections ID and Verify it with EXPECT_EQ

      // Left Side of the Intersection - Lanelet Number 30021
      EXPECT_EQ((americanVehiclePtr->getCurrentLanelet(mapPath, originPoint, leftPoint)), 30021);
      // Top Side of the Intersection - Lanelet Number 30055
      EXPECT_EQ((americanVehiclePtr->getCurrentLanelet(mapPath, originPoint, topPoint)), 30055);
      // Bottom Side of the Intersection - Lanelet Number 30010
      EXPECT_EQ((americanVehiclePtr->getCurrentLanelet(mapPath, originPoint, bottomPoint)), 30010);
      // Right Side of the Intersection - Lanelet Number 30046
      EXPECT_EQ((americanVehiclePtr->getCurrentLanelet(mapPath, originPoint, rightPoint)), 30046);

      // Gets a Lanelet Based on the Predefined Points and Determines its Orientation in Terms of Radians

      //get the direction of lanelet number 30021
      EXPECT_TRUE(americanVehiclePtr->getLaneletDirectionVector(americanVehiclePtr->getLanelet(mapPath, originPoint, leftPoint)).angle - -0.0966401 < 0.0000001);
      //get the direction of lanelet number 30055
      EXPECT_TRUE(americanVehiclePtr->getLaneletDirectionVector(americanVehiclePtr->getLanelet(mapPath, originPoint, topPoint)).angle - -1.52416 < 0.00001);
      //get the direction of lanelet number 30010
      EXPECT_TRUE(americanVehiclePtr->getLaneletDirectionVector(americanVehiclePtr->getLanelet(mapPath, originPoint, bottomPoint)).angle - 1.59655 < 0.00001);
      //get the direction of lanelet number 30046
      EXPECT_TRUE(americanVehiclePtr->getLaneletDirectionVector(americanVehiclePtr->getLanelet(mapPath, originPoint, rightPoint)).angle - -2.97181 < 0.00001);
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, laneletGeometryTestStatic)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, turnDirectionTestStatic) {
    if (staticTests) {
      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      // Test Turn Directions on all Four Lanelet Orientations and also Attempt Reversing Yaw Angle around PI

      // Test from the Bottom Lanelet
      EXPECT_EQ(americanVehiclePtr->turnDirection(PI/2, PI), Left);
      EXPECT_EQ(americanVehiclePtr->turnDirection(PI/2, 0), Right);
      EXPECT_EQ(americanVehiclePtr->turnDirection(PI/2, -PI/2), Straight);
      //test with inverted yaw sign for the left-facing final orientation
      EXPECT_EQ(americanVehiclePtr->turnDirection(PI/2, -PI), Left);

      // Test from the Right Lanelet
      EXPECT_EQ(americanVehiclePtr->turnDirection(PI, -PI/2), Left);
      EXPECT_EQ(americanVehiclePtr->turnDirection(PI, PI/2), Right);
      EXPECT_EQ(americanVehiclePtr->turnDirection(PI, PI), Straight);
      //test with inverted yaw sign for the left-facing initial orientation
      EXPECT_EQ(americanVehiclePtr->turnDirection(-PI, -PI/2), Left);
      EXPECT_EQ(americanVehiclePtr->turnDirection(-PI, PI/2), Right);
      EXPECT_EQ(americanVehiclePtr->turnDirection(-PI, PI), Straight);
      //test with inverted yaw sign for the left-facing initial and final orientation
      EXPECT_EQ(americanVehiclePtr->turnDirection(-PI, -PI), Straight);

      // Test from the Top Lanelet
      EXPECT_EQ(americanVehiclePtr->turnDirection(-PI/2, 0), Left);
      EXPECT_EQ(americanVehiclePtr->turnDirection(-PI/2, PI), Right);
      EXPECT_EQ(americanVehiclePtr->turnDirection(-PI/2, -PI/2), Straight);
      //test with inverted yaw sign for the left-facing final orientation
      EXPECT_EQ(americanVehiclePtr->turnDirection(-PI/2, -PI), Right);

      // Test from the Left Lanelet
      EXPECT_EQ(americanVehiclePtr->turnDirection(0, PI/2), Left);
      EXPECT_EQ(americanVehiclePtr->turnDirection(0, -PI/2), Right);
      EXPECT_EQ(americanVehiclePtr->turnDirection(0, -0.1), Straight);
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, turnDirectionTestStatic)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, frontPointStatic) {
    if (staticTests) {
      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //define the initial position of the vehicle
      lanelet::BasicPoint2d vehicleInitialPosition = lanelet::BasicPoint2d(1000, 1000);

      //define the vehicle length and orientation values
      double vehicleLength = 2;
      double vehicleOrientation = 0;

      //get the front bounds from the vehicle in terms of its length and orientation (a yaw of 0 means vehicle is facing right)
      lanelet::BasicPoint2d vehicleFrontPosition = americanVehiclePtr->getVehicleBounds(vehicleLength, vehicleOrientation, vehicleInitialPosition[0], vehicleInitialPosition[1], Front);

      //resolve and verify the location of the front-most point of the vehicle in terms of its x and y coordinates
      EXPECT_EQ(vehicleFrontPosition[0], 1001);
      EXPECT_EQ(vehicleFrontPosition[1], 1000);
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, frontPointStatic)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, findSpeedViolations) {
    if (ruleTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(dataPath);
      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //contains an epsilon value for speed limit violations
      double epsilon = -3;

      //contains the results of the speed limit check
      std::tuple<std::vector<struct maneuver>, double, double> results;

      //run through the vehicles to check for vehicle speed violations
      for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
        //get all speed limit violations for the given vehicle
        results = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, mapPath, originPoint, epsilon, lanelet::traffic_rules::America, 2.2352);

        //print the results of the speed limit violation test as the vehicle ID and a proportion of the total time spend driving
        EXPECT_EQ(vehicleData[vehicleID].intervals[0].id, (std::get<0>(results).size() * 100.0) / std::get<1>(results));

        //print time intervals where speeding occured
        if (!std::get<0>(results).empty()) {
          for (size_t i = 0; i < std::get<0>(results).size(); i++) {
            EXPECT_EQ(*std::get<0>(results)[i].timestamp, -1);
          } //END FOR
        } //END IF
      } //END FOR
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, findSpeedViolations)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, targetSpeedViolations) {
    if (ruleTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(dataPath);
      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //contains an epsilon value for speed limit violations
      double epsilon = 2;

      //contains the speed limit for vehicles
      double limit = 6;

      //contains all maneuvers from the target speed checker
      std::vector<struct maneuver> targetSpeedResults;

      //run through the vehicles to check for vehicle speed violations
      for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
        //gets the target speed results for the currently iterated vehicle
        targetSpeedResults = americanVehiclePtr->targetSpeed(vehicleData[vehicleID].intervals, limit, epsilon, SpeedCheckMode::TargetLimit);

        //we check who did not break the rule, just to see if anyone hit at 30kmh as an example
        for (size_t k = 0; k < targetSpeedResults.size(); k++) {
          //check if the target speed was ever met at any time
          if (!targetSpeedResults[k].ruleStatus) {
            //print the vehicleID along with the current timestamp for the violation
            EXPECT_EQ(vehicleData[vehicleID].intervals[0].id, *targetSpeedResults[k].timestamp);
          } //END IF
        } //END IF
      } //END FOR
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, targetSpeedViolations)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, waypointViolations) {
    if (ruleTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(dataPathUSARoundaboutEP);
      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //contains an epsilon value for waypoint boundaries
      double epsilon = 12;

      //contains the speed limit for vehicles
      lanelet::BasicPoint2d waypoint = lanelet::BasicPoint2d(1067, 1013);

      //run through the vehicles to check for vehicle speed violations
      for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
        if (americanVehiclePtr->goalWaypoint(vehicleData[vehicleID].intervals, waypoint, epsilon).size() != 1) {
          EXPECT_EQ(vehicleData[vehicleID].intervals[0].id, -1);
        } //END IF
      } //END FOR
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, waypointViolations)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, collisonViolations) {
    if (ruleTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(dataPathUSARoundaboutEP);
      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //contains an epsilon value for collision recognition
      double epsilon = 2.2;

      //contains the speed limit for vehicles
      lanelet::BasicPoint2d collisionPoint = lanelet::BasicPoint2d(974.5, 1031.6);

      //run through the vehicles to check for vehicle speed violations
      for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
        if (americanVehiclePtr->noCollision(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, collisionPoint, epsilon).size() != 0) {
          EXPECT_EQ(vehicleData[vehicleID].intervals[0].id, -1);
        } //END IF
      } //END FOR
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, collisonViolations)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, stopSignWithNoOffroadViolations) {
    if (ruleTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(dataPathThreeWay);
      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //contains an epsilon value for stopping distance and stopping velocity thresholds
      double velocityEpsilon = 2;
      double stoppingDistanceEpsilon = 5;

      //run through the vehicles to check for vehicle speed violations
      for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
        //container for the maneuver object for the function results
        struct violation results = americanVehiclePtr->stopSignWithNoOffroad(vehicleData[vehicleID].intervals, mapPathThreeWay, originPoint, stoppingDistanceEpsilon,
                                                                             velocityEpsilon, vehicleData[vehicleID].length, vehicleData[vehicleID].width, lanelet::traffic_rules::America);

        struct violation stopResult = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, mapPathThreeWay, originPoint, stoppingDistanceEpsilon,
                                                                          velocityEpsilon, lanelet::traffic_rules::America);

        //the vehicle stopped but it did not stop on the lanelet
        if (!results.ruleStatus && results.timestamp) {
          EXPECT_EQ(vehicleData[vehicleID].intervals[0].id, -1);
        } //END IF
      } //END FOR
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, stopSignWithNoOffroadViolations)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, goalWayPointWithTargetSpeedViolations) {
    if (ruleTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(dataPath);
      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //contains the target speed
      double target = 5;

      //contains an epsilon value for the target speed
      double epsilonTargetSpeed = 4;

      //contains an epsilon value for the waypoint distance
      double epsilonWaypoint = 1;

      //contains the speed limit for vehicles
      lanelet::BasicPoint2d waypoint = lanelet::BasicPoint2d(1050, 1002);

      //contains all maneuvers from the target speed checker
      std::vector<struct violation> results;

      //run through the vehicles to check for vehicle speed violations
      for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
        //obtain the results to see if the target was reached with the desired speed
        results = americanVehiclePtr->goalWayPointWithTargetSpeed(vehicleData[vehicleID].intervals, waypoint, epsilonWaypoint, target, epsilonTargetSpeed);

        //check all the results to find if the waypoint and speed were ever satisfied
        for (size_t k = 0; k < results.size(); k++) {
          if (results[k].ruleStatus) {
            EXPECT_EQ(vehicleData[vehicleID].intervals[0].id, -1);
            break;
          } //END IF
        } //END FOR
      } //END FOR
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, goalWayPointWithTargetSpeedViolations)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, statisticsStatic) {
    if (statisticsStaticTests) {
      //container for the directories being checked
      DIR * dir;
      struct dirent * ent;

      //directory to the map path and data
      std::string workingMapPath = mapPathThreeWay;
      std::string workingDataDir = dataPathThreeWayDir;

      //get the name of the file from the path
      std::string last = workingDataDir.substr(workingDataDir.find_last_of("/") + 1);

      //build a name for the output file path
      std::string filePath = "./Output/vehicleStatistics-" + last + ".csv";

      //container for the header column information
      std::string columnHeaderString = "";

      //contains column headers for the first line of the CSV file containing title information
      columnHeaderString += "FILE,TRACK ID,AGENT TYPE,LENGTH (m),WIDTH (m),";
      columnHeaderString += "RECORDED TIME RANGE (ms),RECORDED TIME MEDIAN (ms),RECORDED TIME MEAN (ms),RECORDED TIME MODE (ms),RECORDED TIME STANDARD DEVIATION (ms),";
      columnHeaderString += "X POSITION RANGE (m),X POSITION MEDIAN (m),X POSITION MEAN (m),X POSITION MODE (m),X POSITION STANDARD DEVIATION (m),";
      columnHeaderString += "Y POSITION RANGE (m),Y POSITION MEDIAN (m),Y POSITION MEAN (m),Y POSITION MODE (m),Y POSITION STANDARD DEVIATION (m),";
      columnHeaderString += "X VELOCITY RANGE (m),X VELOCITY MEDIAN (m),X VELOCITY MEAN (m),X VELOCITY MODE (m),X VELOCITY STANDARD DEVIATION (m),";
      columnHeaderString += "Y VELOCITY RANGE (m),Y VELOCITY MEDIAN (m),Y VELOCITY MEAN (m),Y VELOCITY MODE (m),Y VELOCITY STANDARD DEVIATION (m),";
      columnHeaderString += "YAW RANGE (m),YAW MEDIAN (m),YAW MEAN (m),YAW MODE (m),YAW STANDARD DEVIATION (m)\n";

      //clear the output file
      americanVehiclePtr->clearVehicleStats(filePath.c_str(), columnHeaderString.c_str());

      //If the Given Directory Path can be Opened by the File System
      if ((dir = opendir(workingDataDir.c_str())) != NULL) {
        //For Every File in the Given Directory Path
        while ((ent = readdir (dir)) != NULL) {
          //formats the directory path into a string
          std::string path = workingDataDir + "/" + std::string(ent->d_name);

          //filters out file names too short to be CSV's like "." and ".."
          if (strlen(ent->d_name) < 4) {
            //move to the next iteration of the loop
            continue;
          } //END IF

          //extract the statistics data from the dataset
          std::vector<vehicleStatistics> statisticsData = americanVehiclePtr->getVehicleStats(path);

          //write the statistical data to the output dataset
          americanVehiclePtr->writeToCSV(statisticsData, filePath, ent->d_name);
        } //END WHILE
      } //END IF
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, statisticsStatic)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, findNoOffroadViolations) {
    if (statisticsOffroadTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(dataPathThreeWay);

      //obtain the working map path
      std::string workingMapPath = mapPathThreeWay;

      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //contains the file path for the output log
      std::string filePathFull = "./Output/noOffroadFull.csv";
      std::string filePathFrame = "./Output/noOffroadFrame.csv";
      std::string filePathChassis = "./Output/noOffroadChassis.csv";
      std::string filePathHalf = "./Output/noOffroadHalf.csv";
      std::string filePathCenter = "./Output/noOffroadCenter.csv";

      //contians the header for the columns in the output CSV file
      const char header[] = "Offender ID,X-Position,Y-Position,Timestamp,Length,Width,Yaw";

      //clears the output file
      americanVehiclePtr->clearOutput(filePathFull, header);

      //clears the output file
      americanVehiclePtr->clearOutput(filePathFrame, header);

      //clears the output file
      americanVehiclePtr->clearOutput(filePathChassis, header);

      //clears the output file
      americanVehiclePtr->clearOutput(filePathHalf, header);

      //clears the output file
      americanVehiclePtr->clearOutput(filePathCenter, header);

      //contains the collision results for the given vehicle
      std::vector<struct violation> resultsFull, resultsFrame, resultsChassis, resultsHalf, resultsCenter;

      //run through the vehicles to check for vehicle speed violations
      for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
        //get the results for the currently iterated vehicle
        resultsFull = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Full);

        //get the results for the currently iterated vehicle
        resultsFrame = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveFrame);

        //get the results for the currently iterated vehicle
        resultsChassis = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveChassis);

        //get the results for the currently iterated vehicle
        resultsHalf = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveHalf);

        //get the results for the currently iterated vehicle
        resultsCenter = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Centre);
      } //END FOR
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, findNoOffroadViolations)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, tailgatingTestStatistics) {
    if (statisticsTailgatingTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(dataPath);

      //initialize a struct for tailgating records
      struct tailgatingInformation tailgatingDefault, tailgatingHighResponse, tailgatingHighBreak, tailgatingHighAccel, tailgatingHighMinAccel, tailgatingLowResponse, tailgatingLowBreak, tailgatingLowAccel, tailgatingLowMinAccel;

      //get the name of the file from the path
      std::string last = dataPath.substr(dataPath.find_last_of("/") + 1);

      //build a name for the output file path
      std::string header = "./Output/tailgating-" + last;

      //build a name for the output file path
      std::string headerHRe = "./Output/tailgating-HRe-" + last;
      //build a name for the output file path
      std::string headerHBrFront = "./Output/tailgating-HBrFront-" + last;
      //build a name for the output file path
      std::string headerHAcc = "./Output/tailgating-HAcc-" + last;
      //build a name for the output file path
      std::string headerHBrRear = "./Output/tailgating-HBrRear-" + last;

      //build a name for the output file path
      std::string headerLRe = "./Output/tailgating-LRe-" + last;
      //build a name for the output file path
      std::string headerLBrFront = "./Output/tailgating-LBrFront-" + last;
      //build a name for the output file path
      std::string headerLAcc = "./Output/tailgating-LAcc-" + last;
      //build a name for the output file path
      std::string headerLBrRear = "./Output/tailgating-LBrRear-" + last;

      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingResultsLineByLine(header.c_str());

      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingResultsLineByLine(headerHRe.c_str());
      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingResultsLineByLine(headerHBrFront.c_str());
      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingResultsLineByLine(headerHAcc.c_str());
      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingResultsLineByLine(headerHBrRear.c_str());

      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingResultsLineByLine(headerLRe.c_str());
      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingResultsLineByLine(headerLBrFront.c_str());
      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingResultsLineByLine(headerLAcc.c_str());
      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingResultsLineByLine(headerLBrRear.c_str());

      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //for every vehicle in the interaction dataset
      for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
        //obtain tailgating records and place in the container
        tailgatingDefault = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, mapPath, originPoint,
                                                              runtimeCoefficients.tailgating.velocityThreshold,
                                                              runtimeCoefficients.tailgating.distanceThreshold,
                                                              runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                              runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                              //END isTailgatingAtMoment
        //obtain tailgating records and place in the container
        tailgatingHighResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, mapPath, originPoint,
                                                              runtimeCoefficients.tailgating.velocityThreshold,
                                                              runtimeCoefficients.tailgating.distanceThreshold,
                                                              runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                              runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                              //END isTailgatingAtMoment
        //obtain tailgating records and place in the container
        tailgatingHighBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, mapPath, originPoint,
                                                              runtimeCoefficients.tailgating.velocityThreshold,
                                                              runtimeCoefficients.tailgating.distanceThreshold,
                                                              runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                              runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                              //END isTailgatingAtMoment
        //obtain tailgating records and place in the container
        tailgatingHighAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, mapPath, originPoint,
                                                              runtimeCoefficients.tailgating.velocityThreshold,
                                                              runtimeCoefficients.tailgating.distanceThreshold,
                                                              runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0),
                                                              runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                              //END isTailgatingAtMoment
        //obtain tailgating records and place in the container
        tailgatingHighMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, mapPath, originPoint,
                                                              runtimeCoefficients.tailgating.velocityThreshold,
                                                              runtimeCoefficients.tailgating.distanceThreshold,
                                                              runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                              runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                              //END isTailgatingAtMoment
              //obtain tailgating records and place in the container
        tailgatingLowResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, mapPath, originPoint,
                                                              runtimeCoefficients.tailgating.velocityThreshold,
                                                              runtimeCoefficients.tailgating.distanceThreshold,
                                                              runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                              runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                              //END isTailgatingAtMoment
        //obtain tailgating records and place in the container
        tailgatingLowBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, mapPath, originPoint,
                                                              runtimeCoefficients.tailgating.velocityThreshold,
                                                              runtimeCoefficients.tailgating.distanceThreshold,
                                                              runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                              runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                              //END isTailgatingAtMoment
        //obtain tailgating records and place in the container
        tailgatingLowAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, mapPath, originPoint,
                                                              runtimeCoefficients.tailgating.velocityThreshold,
                                                              runtimeCoefficients.tailgating.distanceThreshold,
                                                              runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0),
                                                              runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                              //END isTailgatingAtMoment
        //obtain tailgating records and place in the container
        tailgatingLowMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, mapPath, originPoint,
                                                              runtimeCoefficients.tailgating.velocityThreshold,
                                                              runtimeCoefficients.tailgating.distanceThreshold,
                                                              runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                              runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                              runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                              //END isTailgatingAtMoment

        //append the results to a CSV file
        americanVehiclePtr->appendTailgatingResultsLineByLine(tailgatingDefault, header.c_str());
        //append the results to a CSV file
        americanVehiclePtr->appendTailgatingResultsLineByLine(tailgatingHighResponse, headerHRe.c_str());
        //append the results to a CSV file
        americanVehiclePtr->appendTailgatingResultsLineByLine(tailgatingHighBreak, headerHBrFront.c_str());
        //append the results to a CSV file
        americanVehiclePtr->appendTailgatingResultsLineByLine(tailgatingHighAccel, headerHAcc.c_str());
        //append the results to a CSV file
        americanVehiclePtr->appendTailgatingResultsLineByLine(tailgatingHighMinAccel, headerHBrRear.c_str());
        //append the results to a CSV file
        americanVehiclePtr->appendTailgatingResultsLineByLine(tailgatingLowResponse, headerLRe.c_str());
        //append the results to a CSV file
        americanVehiclePtr->appendTailgatingResultsLineByLine(tailgatingLowBreak, headerLBrFront.c_str());
        //append the results to a CSV file
        americanVehiclePtr->appendTailgatingResultsLineByLine(tailgatingLowAccel, headerLAcc.c_str());
        //append the results to a CSV file
        americanVehiclePtr->appendTailgatingResultsLineByLine(tailgatingLowMinAccel, headerLBrRear.c_str());
      } //END FOR
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, tailgatingTestStatistics)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, tailgatingTestStatisticalVariation) {
    if (statisticsTailgatingDistributionTests) {
      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(dataPathTwo);

      //initialize a struct for tailgating records
      struct tailgatingInformation tailgating;

      //get the name of the file from the path
      std::string last = dataPathTwo.substr(dataPathTwo.find_last_of("/") + 1);

      //build a name for the output file path
      std::string header = "./Output/tailgating-" + last;

      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingResults(header.c_str());

      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //for every vehicle in the interaction dataset
      for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
        //get distribution of results for variable d_min coefficients, and append them to the log file line by line
        americanVehiclePtr->getDminDistribution(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, mapPath, originPoint,
                                                runtimeCoefficients.tailgating.velocityThreshold,
                                                runtimeCoefficients.tailgating.distanceThreshold,
                                                runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly,
                                                runtimeCoefficients.dMinDistribution.responseVariation.get_value_or(0),
                                                runtimeCoefficients.dMinDistribution.maxBreakAccFrontVariation.get_value_or(0),
                                                runtimeCoefficients.dMinDistribution.maxAccelAccRearVariation.get_value_or(0),
                                                runtimeCoefficients.dMinDistribution.minBreakAccRearVariation.get_value_or(0),
                                                runtimeCoefficients.dMinDistribution.responseVariationTick.get_value_or(0),
                                                runtimeCoefficients.dMinDistribution.maxBreakAccFrontVariationTick.get_value_or(0),
                                                runtimeCoefficients.dMinDistribution.maxAccelAccRearVariationTick.get_value_or(0),
                                                runtimeCoefficients.dMinDistribution.minBreakAccRearVariationTick.get_value_or(0), header.c_str());
                                                //END getDminDistribution
      } //END FOR
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, tailgatingTestStatisticalVariation)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, tailgatingTestResultsInOneFile) {
    if (statisticsTailgatingParameterTests) {
      //container for the directories being checked
      DIR * dir;
      struct dirent * ent;

      //directory to the map path and data
      std::string workingMapPath = mapPath;
      std::string workingDataDir = dataPathDir;

      //get the name of the file from the path
      std::string last = workingDataDir.substr(workingDataDir.find_last_of("/") + 1);

      //build a name for the output file path
      std::string header = "./Output/tailgatingOccurences-" + last + ".csv";

      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingOccurances(header.c_str());

      //If the Given Directory Path can be Opened by the File System
      if ((dir = opendir(workingDataDir.c_str())) != NULL) {
        //For Every File in the Given Directory Path
        while ((ent = readdir (dir)) != NULL) {
          //formats the directory path into a string
          std::string path = workingDataDir + "/" + std::string(ent->d_name);

          //filters out file names too short to be CSV's like "." and ".."
          if (strlen(ent->d_name) < 4) {
            //move to the next iteration of the loop
            continue;
          } //END IF

          //extract data from the dataset
          std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(path);

          //initialize a struct for tailgating records
          struct tailgatingInformation tailgatingDefault, tailgatingHighResponse, tailgatingHighBreak, tailgatingHighAccel, tailgatingHighMinAccel, tailgatingLowResponse, tailgatingLowBreak, tailgatingLowAccel, tailgatingLowMinAccel;

          //count the number of occurrences
          int numTailgatingDefault = 0;
          int numTailgatingHighResponse = 0;
          int numTailgatingHighBreak = 0;
          int numTailgatingHighAccel = 0;
          int numTailgatingHighMinAccel = 0;
          int numTailgatingLowResponse = 0;
          int numTailgatingLowBreak = 0;
          int numTailgatingLowAccel = 0;
          int numTailgatingLowMinAccel = 0;

          //initialize the origin point for the map
          lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

          //for every vehicle in the interaction dataset
          for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
            //obtain tailgating records and place in the container
            tailgatingDefault = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            tailgatingHighResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            tailgatingHighBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            tailgatingHighAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            tailgatingHighMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
                  //obtain tailgating records and place in the container
            tailgatingLowResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            tailgatingLowBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            tailgatingLowAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            tailgatingLowMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment

            //count the number of tailgating occurrences
            numTailgatingDefault += tailgatingDefault.tailgatedVehicle.size();
            numTailgatingHighResponse += tailgatingHighResponse.tailgatedVehicle.size();
            numTailgatingHighBreak += tailgatingHighBreak.tailgatedVehicle.size();
            numTailgatingHighAccel += tailgatingHighAccel.tailgatedVehicle.size();
            numTailgatingHighMinAccel += tailgatingHighMinAccel.tailgatedVehicle.size();
            numTailgatingLowResponse += tailgatingLowResponse.tailgatedVehicle.size();
            numTailgatingLowBreak += tailgatingLowBreak.tailgatedVehicle.size();
            numTailgatingLowAccel += tailgatingLowAccel.tailgatedVehicle.size();
            numTailgatingLowMinAccel += tailgatingLowMinAccel.tailgatedVehicle.size();
          } //END FOR
          americanVehiclePtr->appendTailgatingCounts(path, numTailgatingDefault, numTailgatingHighResponse,
                                                    numTailgatingHighBreak, numTailgatingHighAccel,
                                                    numTailgatingHighMinAccel, numTailgatingLowResponse,
                                                    numTailgatingLowBreak, numTailgatingLowAccel,
                                                    numTailgatingLowMinAccel, header.c_str());
                                                    //END appendTailgatingResultsLineByLine
        } //END WHILE
      } //END IF
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, tailgatingTestResultsInOneFile)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, tailgatingTestTwoVariableVariation) {
    if (statisticsGridTests) {
      //container for the directories being checked
      DIR * dir;
      struct dirent * ent;

      //directory to the map path and data
      std::string workingMapPath = mapPath;
      std::string workingDataDir = dataPathTwo;

      //get the name of the file from the path
      std::string last = workingDataDir.substr(workingDataDir.find_last_of("/") + 1);

      //build a name for the output file path
      std::string header = "./Output/tailgatingOccurences-" + last;

      //clear the output log file and add header columns for the upcoming additions
      americanVehiclePtr->resetTailgatingOccurances(header.c_str());

      //extract data from the dataset
      std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(workingDataDir);

      //initialize a struct for tailgating records
      struct tailgatingInformation tailgatingResults;

      //count the number of occurrences
      int numTailgatingDefault = 0;
      int numTailgatingHighResponse = 0;
      int numTailgatingHighBreak = 0;
      int numTailgatingHighAccel = 0;
      int numTailgatingHighMinAccel = 0;
      int numTailgatingLowResponse = 0;
      int numTailgatingLowBreak = 0;
      int numTailgatingLowAccel = 0;
      int numTailgatingLowMinAccel = 0;

      //initialize the origin point for the map
      lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

      //set values for higher and lower tailgating coefficients
      double highResponse = runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0);
      double highBreakAcc = runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0);
      double highRearAcc = runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0);
      double highMinBreakAcc = runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0);
      double lowResponse = runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0);
      double lowBreakAcc = runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0);
      double lowRearAcc =  runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0);
      double lowMinBreakAcc = runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0);

      //set values for default tailgating coefficients
      double defaultResponse = runtimeCoefficients.tailgating.responseTime.get_value_or(0);
      double defaultBreak = runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0);
      double defaultRear = runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0);
      double defaultMinBreak = runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0);

      //set the current values per iteration regarding tailgating coefficients
      double currentResponse = defaultResponse;
      double currentBreak = defaultBreak;
      double currentRear = defaultRear;
      double currentMinBreak = defaultMinBreak;

      //contains the type of value being iterated over in terms of a string representation
      std::string rowValueCoefficientName;

      //array of integers to contain the tailgating results
      int results[9][9];

      //variation matrix for elements
      for (size_t j = 0; j < 9; j++) {
        for (size_t k = 0; k < 9; k++) {
          //for every vehicle in the interaction dataset
          for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
            //check the current iteration
            if (j == 0) {
              currentResponse = defaultResponse;
              currentBreak = defaultBreak;
              currentRear = defaultRear;
              currentMinBreak = defaultMinBreak;
              rowValueCoefficientName = "default";
            }
            else if (j == 1) {
              currentResponse = highResponse;
              currentBreak = defaultBreak;
              currentRear = defaultRear;
              currentMinBreak = defaultMinBreak;
              rowValueCoefficientName = "highResponse";
            }
            else if (j == 2) {
              currentResponse = defaultResponse;
              currentRear = defaultRear;
              currentMinBreak = defaultMinBreak;
              currentBreak = highBreakAcc;
              rowValueCoefficientName = "highBreakAcc";
            }
            else if (j == 3) {
              currentResponse = defaultResponse;
              currentBreak = defaultBreak;
              currentMinBreak = defaultMinBreak;
              currentRear = highRearAcc;
              rowValueCoefficientName = "highRearAcc";
            }
            else if (j == 4) {
              currentResponse = defaultResponse;
              currentBreak = defaultBreak;
              currentRear = defaultRear;
              currentMinBreak = highMinBreakAcc;
              rowValueCoefficientName = "highMinBreakAcc";
            }
            else if (j == 5) {
              currentBreak = defaultBreak;
              currentRear = defaultRear;
              currentMinBreak = defaultMinBreak;
              currentResponse = lowResponse;
              rowValueCoefficientName = "lowResponse";
            }
            else if (j == 6) {
              currentResponse = defaultResponse;
              currentRear = defaultRear;
              currentMinBreak = defaultMinBreak;
              currentBreak = lowBreakAcc;
              rowValueCoefficientName = "lowBreakAcc";
            }
            else if (j == 7) {
              currentResponse = defaultResponse;
              currentBreak = defaultBreak;
              currentMinBreak = defaultMinBreak;
              currentRear = lowRearAcc;
              rowValueCoefficientName = "lowRearAcc";
            }
            else if (j == 8) {
              currentResponse = defaultResponse;
              currentBreak = defaultBreak;
              currentRear = defaultRear;
              currentMinBreak = lowMinBreakAcc;
              rowValueCoefficientName = "lowMinBreakAcc";
            } //END IF

            if (k == 1) {
              currentResponse = highResponse;
            }
            else if (k == 2) {
              currentBreak = highBreakAcc;
            }
            else if (k == 3) {
              currentRear = highRearAcc;
            }
            else if (k == 4) {
              currentMinBreak = highMinBreakAcc;
            }
            else if (k == 5) {
              currentResponse = lowResponse;
            }
            else if (k == 6) {
              currentBreak = lowBreakAcc;
            }
            else if (k == 7) {
              currentRear = lowRearAcc;
            }
            else if (k == 8) {
              currentMinBreak = lowMinBreakAcc;
            } //END IF

            //obtain tailgating records and place in the container
            tailgatingResults = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                        runtimeCoefficients.tailgating.velocityThreshold,
                                                                        runtimeCoefficients.tailgating.distanceThreshold,
                                                                        currentResponse,
                                                                        currentBreak,
                                                                        currentRear,
                                                                        currentMinBreak, DrivingOnly, lanelet::traffic_rules::America);
                                                                        //END isTailgatingAtMoment
            //record the results
            results[j][k] = tailgatingResults.tailgatedVehicle.size();

            //check the current iteration
            if (k == 0) {
              numTailgatingDefault += tailgatingResults.tailgatedVehicle.size();
            }
            else if (k == 1) {
              numTailgatingHighResponse += tailgatingResults.tailgatedVehicle.size();
            }
            else if (k == 2) {
              numTailgatingHighBreak += tailgatingResults.tailgatedVehicle.size();
            }
            else if (k == 3) {
              numTailgatingHighAccel += tailgatingResults.tailgatedVehicle.size();
            }
            else if (k == 4) {
              numTailgatingHighMinAccel += tailgatingResults.tailgatedVehicle.size();
            }
            else if (k == 5) {
              numTailgatingLowResponse += tailgatingResults.tailgatedVehicle.size();
            }
            else if (k == 6) {
              numTailgatingLowBreak += tailgatingResults.tailgatedVehicle.size();
            }
            else if (k == 7) {
              numTailgatingLowAccel += tailgatingResults.tailgatedVehicle.size();
            }
            else if (k == 8) {
              numTailgatingLowMinAccel += tailgatingResults.tailgatedVehicle.size();
            } //END IF
          } //END FOR
        } //END FOR

        //appends the tailgating counts to the output file
        americanVehiclePtr->appendTailgatingCounts(rowValueCoefficientName, numTailgatingDefault, numTailgatingHighResponse,
                                                    numTailgatingHighBreak, numTailgatingHighAccel,
                                                    numTailgatingHighMinAccel, numTailgatingLowResponse,
                                                    numTailgatingLowBreak, numTailgatingLowAccel,
                                                    numTailgatingLowMinAccel, header.c_str());
                                                    //END appendTailgatingResultsLineByLine
        //reset the counters to their default values for the next iteration
        numTailgatingDefault = 0;
        numTailgatingHighResponse = 0;
        numTailgatingHighBreak = 0;
        numTailgatingHighAccel = 0;
        numTailgatingHighMinAccel = 0;
        numTailgatingLowResponse = 0;
        numTailgatingLowBreak = 0;
        numTailgatingLowAccel = 0;
        numTailgatingLowMinAccel = 0;
      } //END FOR
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, tailgatingTestTwoVariableVariation)
} //END vehicleData

namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, tailgatingTestPosition) {
    if (tailgatingOccuranceByPositionTest) {
      //define the working path for the data
      std::string workingDataPath;

      //define a vector of map paths
      std::string workingMapPath;

      //contains the given lanelet segments type
      LaneletPosition segmentType;

      //define the default pointer type for the files
      auto vehiclePointerType = americanVehiclePtr;

      //define the default nation index for the nation being used
      const char * nationIndex = lanelet::traffic_rules::America;

      //for every given file path in the list of all possible file paths and map paths
      for (size_t directory = 0; directory < allMapPaths.size(); directory++) {
        //sets the current working map and data paths
        workingDataPath = allDataPaths[directory];
        workingMapPath = allMapPaths[directory];

        //contains the pointer for the vehicle nation being used
        if (vehiclePtr[directory] == Nations::America) {
          auto vehiclePointerType = getAmericanVehicleRules();
          nationIndex = lanelet::traffic_rules::America;
        }
        else if (vehiclePtr[directory] == Nations::China) {
          auto vehiclePointerType = getChineseVehicleRules();
          nationIndex = lanelet::traffic_rules::China;
        }
        else if (vehiclePtr[directory] == Nations::Germany) {
          auto vehiclePointerType = getGermanVehicleRules();
          nationIndex = lanelet::traffic_rules::Germany;
        } //END IF

        //container for the directories being checked
        DIR * dir;
        struct dirent * ent;

        //initialize a struct for tailgating records
        struct tailgatingInformation tailgatingDefault;

        //initialize counters
        int entry, intersection, exit;

        //build a name for the output file path
        std::string filePathEntry = "./Output/tailgatingResultsPositionalEntry-" + workingDataPath.substr(workingDataPath.find_last_of("/") + 1) + ".csv";
        std::string filePathIntersection = "./Output/tailgatingResultsPositionalIntersection-" + workingDataPath.substr(workingDataPath.find_last_of("/") + 1) + ".csv";
        std::string filePathLanelet = "./Output/tailgatingResultsPositionalLanelet-" + workingDataPath.substr(workingDataPath.find_last_of("/") + 1) + ".csv";

        //container for output data
        std::string outputString;

        //build an output header for the tailgating data
        const char HEADER[] = "FILE,ID,TRIP DURATION,VIOLATION DURATION,VIOLATION PROPORTION";

        //clear the output log file and add header columns for the upcoming additions
        vehiclePointerType->resetOutputDataToExcel(filePathEntry, HEADER);
        vehiclePointerType->resetOutputDataToExcel(filePathIntersection, HEADER);
        vehiclePointerType->resetOutputDataToExcel(filePathLanelet, HEADER);

        //initialize the origin point for the map
        lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

        //if the Given Directory Path can be Opened by the File System
        if ((dir = opendir(workingDataPath.c_str())) != NULL) {
          //for Every File in the Given Directory Path
          while ((ent = readdir (dir)) != NULL) {
            //formats the directory path into a string
            std::string path = workingDataPath + "/" + std::string(ent->d_name);

            //filters out file names too short to be CSV's like "." and ".."
            if (strlen(ent->d_name) < 4) {
              //move to the next iteration of the loop
              continue;
            } //END IF

            //get and store the formatted vehicle data from the interaction dataset
            std::vector<struct vehicles> vehicleData = vehiclePointerType->getVehicleData(path);

            //for every vehicle extracted from the given interaction recording
            for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
              //reset the containers for the tailgating positional results
              entry = 0;
              intersection = 0;
              exit = 0;

              //obtain tailgating records and place in the container
              tailgatingDefault = vehiclePointerType->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                          runtimeCoefficients.tailgating.velocityThreshold,
                                                                          runtimeCoefficients.tailgating.distanceThreshold,
                                                                          runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                          runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                          runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                          runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, nationIndex);
                                                                          //END isTailgatingAtMoment

              //find the given lanelet position for the tailgating results
              for (size_t tailgatingInstance = 0; tailgatingInstance < tailgatingDefault.tailgatedVehicle.size(); tailgatingInstance++) {
                //obtains the segment type for the given vehicle position
                segmentType = vehiclePointerType->intersectionSegment(tailgatingDefault.tailgatedVehicle[tailgatingInstance].xPos,
                                                                      tailgatingDefault.tailgatedVehicle[tailgatingInstance].yPos, workingMapPath,
                                                                      originPoint, nationIndex);
                                                                      //END intersectionSegment

                //determine the tailgating instances current position
                if (segmentType == LaneletPosition::IntersectionEntry) {
                  //add the tailgating instance to the counter
                  entry += 1;
                }
                else if (segmentType == LaneletPosition::IntersectionInterior) {
                  //add the tailgating instance to the counter
                  intersection += 1;
                }
                else if (segmentType == LaneletPosition::RoadLane) {
                  //add the tailgating instance to the counter
                  exit += 1;
                } //END IF
              } //END FOR

              //format the output string for the given vehicle
              outputString = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                             std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," + std::to_string(entry * 100) + "," +
                             std::to_string((entry * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0));
                             //END outputString

              vehiclePointerType->outputDataToExcel(filePathEntry, outputString);

              //format the output string for the given vehicle
              outputString = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                             std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," + std::to_string(intersection * 100) + "," +
                             std::to_string((intersection * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0));
                             //END outputString

              vehiclePointerType->outputDataToExcel(filePathIntersection, outputString);

              //format the output string for the given vehicle
              outputString = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                             std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," + std::to_string(exit * 100) + "," +
                             std::to_string((exit * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0));
                             //END outputString

              vehiclePointerType->outputDataToExcel(filePathLanelet, outputString);
            } //END FOR
          } //END WHILE
        } //END IF
      } //END FOR
    } //END IF
  } //END TEST_F(AmericanTrafficRulesVehicleTest, tailgatingTestPosition)
} //END vehicleData

/*
The following test script runs through the interaction datasets in the specified folder and checks for any rule violations,
as they were specified in the paper Formalization of Intersection Traffic Rules in Temporal Logic by Sebastian Maierhofer, Paul Moosbrugger, and Matthias Althoff.
  A link to the paper can be found here: https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
Rule violations can be visually inspected by checking the terminal output and looking for any test failures:
  The terminal will reference a failure message for the current CSV file, and continue to show the failure for the current test.
  As an example, we see the following (Note that "3" can represent any vehicle ID and "LeftBeforeRight" can represent any rule violation)
    The terminal will say it expected the quality of these values:
      std::to_string(vehicleData[vehicleID][0])
        Which is: "3"
        "LeftBeforeRight"]]></failure>
    Where "3" is the vehicle ID violating the rule, and "LeftBeforeRight" is the rule being violated
We can visualize the interaction dataset in question and manually inspect offending vehicle by using the following tool from GitHub: https://github.com/interaction-dataset/interaction-dataset
  Firstly, opening the terminal and moving the working directory to the interaction-dataset/python folder is required
  Secondly, run: ./main_visualize_data.py .TestScenarioForScripts
    Where .TestScenarioForScripts (under interaction-dataset/recorded_trackfiles/.TestScenarioForScripts/) contains the correct interaction-CSV file, and is renamed to vehicle_tracks_000.csv
    And (interaction-dataset/maps/) contains the correct OSM map (the one corresponding to the interaction-CSV being analyzed) renamed to .TestScenarioForScripts.osm
*/
namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, threeWayIntersectionWithRoundabout) {
    //container for the directories being checked
    DIR * dir;
    struct dirent * ent;

    //container to determine if a vehicle is tailgating another
    struct tailgatingInformation tailgating;

    //create a struct object to contain all violations for a given vehicle
    struct violations vehicleViolations;

    //create a vector to contain all violations for all vehicles in an interaction dataset
    std::vector<struct violations> allVehicleViolations;

    //create a vector to contain all violations for all vehicles across interaction datasets
    std::vector<std::pair<std::vector<struct violations>, std::string>> allViolationsAllDatasets;

    //container for the number of people who did not violate any rule
    bool nonViolator;

    //contains the number of people not to violate any traffic rules
    int nonViolators = 0;
    int totalNonViolators = 0;

    //contains the statistics for all interaction datasets within a given test
    int totalRuns = 0;
    int totalShouldStop = 0;
    int totalLeftBeforeRight = 0;
    int totalHasPriority = 0;
    int totalTurningLeft = 0;
    int totalTailgating = 0;
    int totalSpeedLimit = 0;
    int totalNoOffroad = 0;
    int totalStops = 0;
    int totalYieldsOne = 0;
    int totalYieldsTwo = 0;

    //contains statistics for specific interaction datasets within a given test
    int runs = 0;
    int shouldStop = 0;
    int neededToStop = 0;
    int leftBeforeRight = 0;
    int hasPriority = 0;
    int turningLeft = 0;
    int isTailgating = 0;
    int speedLimit = 0;
    int noOffroad = 0;
    int stops = 0;
    int yieldsOne = 0;
    int yieldsTwo = 0;

    //contains the number of vehicles that needed to stop before a stop sign
    int neededToStopHighDist = 0;
    int neededToStopLowDist = 0;
    int neededToStopHighSpeed = 0;
    int neededToStopLowSpeed = 0;

    //used for exiting early
    int iteration = 0;
    int COUNTER = 0;

    //define the working map path
    std::string workingMapPath = mapPathUSARoundaboutEP;

    //define the working data path
    std::string workingDataPath = dataPathUSARoundaboutEPDir;

    //stores the path to the output file
    std::string PATH = "./Output/resultsLogThreeWayIntersectionWithRoundabout.csv";

    //stores the path to the output file and the new header for the new file
    const char HEADER[] = "File,Total Vehicles,Non-Rule-Violating Vehicles,Stop-Violators,Stops Needed,Left Before Right Violators,Lanelet Priority Violators,Should Turn Left Violators,Yields One Needed,Yields Two Needed,Vehicle Tailgating Violations,Speed Limit,No Offroad\n";

    //clear the output file
    if (threeWayRoundaboutActive) americanVehiclePtr->clearOutput(PATH, HEADER);

    //contains the speed limit epsilon value in terms of meters per second
    double speedingEpsilonStrict = 0;
    double speedingEpsilonSoft = 1.38889;
    double speedingEpsilonHard = 2.77778;

    //contains the results of the vehicle speed limit violation
    std::string vehicleSpeedingResults;

    //format header for the speeding results
    const char SPEEDING_HEADER[] = "FILE,ID,TRIP DURATION,SPEEDING DURATION,MOTION DURATION,TIME BASED PROPORTION OF SPEEDING,SPEED LIMIT,EPSILON";;

    //containers for the file paths to the speeding results spreadsheets
    std::string filePathSpeedingStrict = "./Output/SpeedingResultsStrict-ThreeWayIntersectionWithRoundabout.csv";
    std::string filePathSpeedingSoft = "./Output/SpeedingResultsSoft-ThreeWayIntersectionWithRoundabout.csv";
    std::string filePathSpeedingHard = "./Output/SpeedingResultsHard-ThreeWayIntersectionWithRoundabout.csv";

    //reset the output file for the speeding results
    if (threeWayRoundaboutActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingStrict, SPEEDING_HEADER);
    if (threeWayRoundaboutActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingSoft, SPEEDING_HEADER);
    if (threeWayRoundaboutActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingHard, SPEEDING_HEADER);

    //container for the output line for the vehicle stop analysis
    std::string stopResults;

    //format header for the stop violation results
    const char STOP_HEADER[] = "FILE,ID,Standard,Standard-Velocity,High Velocity,High-Velocity,Low Velocity,Low-Velocity,High Distance,Long-Velocity,Low Distance,Short-Velocity";

    //reset the output file for the stopping results
    if (threeWayRoundaboutActive && stopChecking) americanVehiclePtr->resetOutputDataToExcel("./Output/StoppingResultsStandard-ThreeWayIntersectionWithRoundabout.csv", STOP_HEADER);

    //contains the file path for the output log
    std::string filePathFull = "./Output/noOffroadFull-ThreeWayIntersectionWithRoundabout.csv";

    //contains the header for the output file
    const char OFFROAD_HEADER[] = "FILE,ID,TRIP DURATION,"
                                  "OFFROAD DURATION (FULL),TIME BASED PROPORTION OF OFFROAD (FULL),"
                                  "OFFROAD DURATION (FRAME),TIME BASED PROPORTION OF OFFROAD (FRAME),"
                                  "OFFROAD DURATION (CHASSIS),TIME BASED PROPORTION OF OFFROAD (CHASSIS),"
                                  "OFFROAD DURATION (HALF),TIME BASED PROPORTION OF OFFROAD(HALF),"
                                  "OFFROAD DURATION (CENTER),TIME BASED PROPORTION OF OFFROAD (CENTER),"
                                  "INTERSECTION POSITION";

    //clears the output file
    if (threeWayRoundaboutActive && offroadChecking) americanVehiclePtr->resetOutputDataToExcel(filePathFull, OFFROAD_HEADER);

    //contains the collision results for the given vehicle
    std::vector<struct violation> resultsFull, resultsFrame, resultsChassis, resultsHalf, resultsCenter;

    //container for the results string for off-road violation detection
    std::string noOffroadResults;

    //contains the file path for the turning violation results
    std::string turnViolations = "./Output/turnViolationResults-ThreeWayIntersectionWithRoundabout.csv";

    //contains the header for the output file
    const char TURN_VIOLATION_HEADER[] = "FILE,Offender ID,X-Position,Y-Position,Timestamp,Stop Duration,Length,Width,Yaw,Direction,Trip Duration,Proportion";

    //clears the output file
    if (threeWayRoundaboutActive && turnChecking) americanVehiclePtr->resetOutputDataToExcel(turnViolations, TURN_VIOLATION_HEADER);

    //container for the results string for turn violation detection
    std::string turnViolationResults = "";

    //container for the tailgating results
    std::string tailgatingResults;

    //container for the spreadsheet for tailgating violations
    std::string tailgatingViolations = "./Output/tailgatingViolations-ThreeWayIntersectionWithRoundabout.csv";

    //contains the header for the output file
    const char TAILGATING_VIOLATION_HEADER[] = "FILE,ID,TRIP DURATION,"
                                               "TAILGATING DURATION (Default),TIME BASED PROPORTION OF TAILGATING (Default),"
                                               "TAILGATING DURATION (HighResponse),TIME BASED PROPORTION OF TAILGATING (HighResponse),"
                                               "TAILGATING DURATION (HighBreak),TIME BASED PROPORTION OF TAILGATING (HighBreak),"
                                               "TAILGATING DURATION (HighAccel),TIME BASED PROPORTION OF TAILGATING (HighAccel),"
                                               "TAILGATING DURATION (HighMinAccel),TIME BASED PROPORTION OF TAILGATING (HighMinAccel),"
                                               "TAILGATING DURATION (LowResponse),TIME BASED PROPORTION OF TAILGATING (LowResponse),"
                                               "TAILGATING DURATION (LowBreak),TIME BASED PROPORTION OF TAILGATING (LowBreak),"
                                               "TAILGATING DURATION (LowAccel),TIME BASED PROPORTION OF TAILGATING (LowAccel),"
                                               "TAILGATING DURATION (LowMinAccel),TIME BASED PROPORTION OF TAILGATING (LowMinAccel)";

    //reset the output file for tailgating violations
    if (threeWayRoundaboutActive && tailgatingChecking) americanVehiclePtr->resetOutputDataToExcel(tailgatingViolations, TAILGATING_VIOLATION_HEADER);

    //if the Given Directory Path can be Opened by the File System
    if ((dir = opendir(workingDataPath.c_str())) != NULL) {
      //for Every File in the Given Directory Path
      while ((ent = readdir (dir)) != NULL) {
        if (!threeWayRoundaboutActive) break;
        //formats the directory path into a string
        std::string path = workingDataPath + "/" + std::string(ent->d_name);

        //filters out file names too short to be CSV's like "." and ".."
        if (strlen(ent->d_name) < 4) {
          //move to the next iteration of the loop
          continue;
        }
        else if (iteration != COUNTER) {
          iteration += 1;
          continue;
        } //END IF

        //get and store the formatted vehicle data from the interaction dataset
        std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(path);

        //contains the origin point for the open street map file
        lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

        //output the file name
        EXPECT_EQ("file: ", path);

        //contains statistics for the vehicles being scanned
        runs = 0;
        nonViolators = 0;
        shouldStop = 0;
        leftBeforeRight = 0;
        hasPriority = 0;
        turningLeft = 0;
        isTailgating = 0;
        speedLimit = 0;
        noOffroad = 0;
        stops = 0;
        yieldsOne = 0;
	yieldsTwo = 0;

        //tests every vehicle, and reports those that fail with their ID
        for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
          //re-build a violations struct for the next vehicle encountered
          vehicleViolations = violations();

          //count the iteration through the FOR loop as a run
          runs += 1;

          //signals if the given vehicle NOT has violated any rules, and is true by default
          nonViolator = true;

          //determines if stop checking is active or not
          if (stopChecking) {
            //check if the given vehicle failed to stop at a stop regulated intersection
            vehicleViolations.ruleI = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                          runtimeCoefficients.stop.distanceThreshold,
                                                                          runtimeCoefficients.stop.velocityThreshold,
                                                                          lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                          //END checkStopAtSign
            vehicleViolations.ruleIStrictVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictVelocity.distanceThreshold,
                                                                                        stopStrictVelocity.velocityThreshold,
                                                                                        lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardVelocity.distanceThreshold,
                                                                                      stopHardVelocity.velocityThreshold,
                                                                                      lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                                      //END checkStopAtSign
            vehicleViolations.ruleIStrictDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictDistance.distanceThreshold,
                                                                                        stopStrictDistance.velocityThreshold,
                                                                                        lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardDistance.distanceThreshold,
                                                                                      stopHardDistance.velocityThreshold,
                                                                                      lanelet::traffic_rules::America, StopDirection::MultiStop);

            //records the stop results to the given string
            stopResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + ",";

            //check if the vehicle needs to stop
            stops += int(vehicleViolations.ruleI.neededToStop.get_value_or(0));

            //record the results of the stop sign checking
            if (vehicleViolations.ruleI.ruleStatus && vehicleViolations.ruleI.endTimestamp && vehicleViolations.ruleI.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleI.endTimestamp.get_value_or(0) - vehicleViolations.ruleI.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictVelocity.ruleStatus && vehicleViolations.ruleIStrictVelocity.endTimestamp && vehicleViolations.ruleIStrictVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardVelocity.ruleStatus && vehicleViolations.ruleIHardVelocity.endTimestamp && vehicleViolations.ruleIHardVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictDistance.ruleStatus && vehicleViolations.ruleIStrictDistance.endTimestamp && vehicleViolations.ruleIStrictDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictDistance.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardDistance.ruleStatus && vehicleViolations.ruleIHardDistance.endTimestamp && vehicleViolations.ruleIHardDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardDistance.magnitude.get_value_or(-1)) + ",";
            } //END IF

            //output the results of the stop sign detection to the output file
            americanVehiclePtr->outputDataToExcel("./Output/StoppingResultsStandard-ThreeWayIntersectionWithRoundabout.csv", stopResults);
          } //END IF

          if (tailgatingChecking) {
            //check if the given vehicle is tailgating other vehicles along the intersection
            vehicleViolations.tailgating = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                                    runtimeCoefficients.tailgating.velocityThreshold,
                                                                                    runtimeCoefficients.tailgating.distanceThreshold,
                                                                                    runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), All, lanelet::traffic_rules::America);
                                                                                    //END isTailgatingAtMoment

            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), All, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), All, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), All, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0), All, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), All, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), All, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), All, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0), All, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
          } //END if

          if (speedingChecking) {
            //check if the vehicle is violating the lanelets speed limit
            vehicleViolations.speedLimitViolationsStrict = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonStrict, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsSoft = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonSoft, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsHard = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonHard, lanelet::traffic_rules::America, 2.2352);
          } //END IF

          if (offroadChecking) {
            //check if the vehicle is violating the lanelets off-road boundaries
            vehicleViolations.offroadViolations = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveHalf);
            vehicleViolations.offroadViolationsFull = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Full);
            vehicleViolations.offroadViolationsFrame = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveFrame);
            vehicleViolations.offroadViolationsChassis = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveChassis);
            vehicleViolations.offroadViolationsCenter = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Centre);

            //contains the results of vehicle speeding detection
            noOffroadResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                              std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check if the results from the above tests are not empty
            if (!vehicleViolations.offroadViolationsFull.empty()) {
              //contains the results of vehicle speeding detection
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFull).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFull).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsFrame.empty()) {
              //contains the results of vehicle speeding detection
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFrame).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFrame).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsChassis.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsChassis).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsChassis).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolations.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolations).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolations).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsCenter.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsCenter).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsCenter).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results to the csv file
            americanVehiclePtr->outputDataToExcel(filePathFull, noOffroadResults);
          } //END IF

          /*
            If the Given Vehicle Stopped at the Intersections Stop-Line Given an AllWayStop or a Stop Sign Regulated Intersection,
            We can Check if the Vehicle Performed the Correct Intersectional Right-Of-Way Transfers Given Multiple Vehicles Stopped
            at the Same Time or Another Vehicle Stopped First and has the Intersectional Right-Of-Way.
            Essentially, Checks if the Reference Vehicle Passes the Intersection with the Right-Of-Way After Stopping:
              -> If multiple Vehicles Stop at Once, the Reference Vehicle Starts Once Given the Right-Of-Way
              -> If Another Vehicle Stops just Before the Reference Vehicle, the Reference Vehicle Waits to be Given the Right-Of-Way
          */
          if (stopChecking && turnChecking && vehicleViolations.ruleI.ruleStatus) {
            //check if Rules III, IV, or V were violated by the Currently Iterated Vehicle
            vehicleViolations.ruleIII = americanVehiclePtr->checkLeftBeforeRight(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                 runtimeCoefficients.leftBeforeRight.distanceThreshold,
                                                                                 runtimeCoefficients.leftBeforeRight.velocityThreshold,
                                                                                 lanelet::traffic_rules::America, multiTurnYawChecking);
                                                                                 //END checkLeftBeforeRight

            vehicleViolations.ruleV = americanVehiclePtr->checkShouldBeTurningLeft(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                   runtimeCoefficients.leftTurnPriority.distanceThreshold,
                                                                                   runtimeCoefficients.leftTurnPriority.velocityThreshold,
                                                                                   lanelet::traffic_rules::America, multiTurnYawChecking);
                                                                                   //END checkShouldBeTurningLeft

	    yieldsOne += int(vehicleViolations.ruleIII.neededToStop.get_value_or(0));
	    yieldsTwo += int(vehicleViolations.ruleV.neededToStop.get_value_or(0));


            //check for Violations Involving the Right-before-Left Rule (A Vehicle went LeftBeforeRight)
            if (!vehicleViolations.ruleIII.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleIII.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleIII.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)),
                        "LeftBeforeRight");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleIII.referenceID) + "," + std::to_string(vehicleViolations.ruleIII.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleIII.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.turnDirection.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";
              turnViolationResults += std::to_string(((double) vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left-right turn priority intersection stop violation
              leftBeforeRight += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF

            //check for Violations Involving the Turn Left Rule
            if (!vehicleViolations.ruleV.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleV.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleV.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)),
                        "ShouldBeTurningLeft");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleV.referenceID) + "," + std::to_string(vehicleViolations.ruleV.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleV.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.turnDirection.get_value_or(0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left turn intersection stop violation
              turningLeft += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          }
          //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
          else if (stopChecking && !vehicleViolations.ruleI.ruleStatus) {
            //record that a vehicle was detected engaging in a stopline intersection stop violation
            shouldStop += 1;
            //record that the vehicle has violated at least one rule
            nonViolator = false;
          } //END IF

          if (speedingChecking) {
            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsStrict).empty()) {
              //contains the results of vehicle speeding detection
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      (std::to_string(
                                        (std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100.0) /
                                        (std::get<1>(vehicleViolations.speedLimitViolationsStrict) * 1.0)
                                      )) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);

              //record that a vehicle was detected engaging in a stopline intersection stop violation
              speedLimit += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            }
            else {
              //contains the results of vehicle speeding detection
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsSoft).empty()) {
              //contains the results of vehicle speeding detection
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100.0) /
                                        std::get<1>(vehicleViolations.speedLimitViolationsSoft))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            }
            else {
              //contains the results of vehicle speeding detection
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsHard).empty()) {
              //contains the results of vehicle speeding detection
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsHard))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            }
            else {
              //contains the results of vehicle speeding detection
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } //END IF
          } //END IF

          if (offroadChecking) {
            //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
            if (vehicleViolations.offroadViolations.size() != 0) {
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              noOffroad += 1;
              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          } //END IF

          if (tailgatingChecking) {
            //contains the results of vehicle speeding detection
            tailgatingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check for Violations Involving Tailgating between Vehicles
            if (vehicleViolations.tailgating.tailgatedVehicle.size() != 0) {
              //record that a vehicle was detected tailgating
              isTailgating += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;

              tailgatingResults += std::to_string(vehicleViolations.tailgating.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgating.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            }
            else {
              //contains the results of vehicle speeding detection
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results of the vehicle speeding
            americanVehiclePtr->outputDataToExcel(tailgatingViolations, tailgatingResults);
          } //END IF

          //record all rule violations for the given vehicle by pushing it into the vector of all rule violations
          allVehicleViolations.push_back(vehicleViolations);

          //record that a non violating vehicle was recorded if no rules were broken
          if (nonViolator) {
            //no rules were violated
            nonViolators += 1;
          } //END IF
        } //END FOR

        //counts the total number of vehicles ran through by the loop
        totalRuns += runs;

        //counts the total number of non-rule violating vehicles encountered
        totalNonViolators += nonViolators;

        //counts the number of rule violating vehicle encountered in the given interaction dataset
        totalShouldStop += shouldStop;
        totalLeftBeforeRight += leftBeforeRight;
        totalHasPriority += hasPriority;
        totalTurningLeft += turningLeft;
        totalTailgating += isTailgating;
        totalSpeedLimit += speedLimit;
        totalNoOffroad += noOffroad;
        totalStops += stops;
	totalYieldsOne += yieldsOne;
	totalYieldsTwo += yieldsTwo;

        //store the results in a results statistics struct
        struct ruleViolationStatistics results = {runs, nonViolators, shouldStop, leftBeforeRight, hasPriority, turningLeft,
                                                  isTailgating, speedLimit, noOffroad, PATH, std::string(ent->d_name), stops, yieldsOne, yieldsTwo};

        //print the results struct to a CSV file
        americanVehiclePtr->outputToResultsLog(results);

        //append the vector and the interaction dataset path to the list of all rule violations
        allViolationsAllDatasets.push_back(std::make_pair(allVehicleViolations, path));

        //clear the given vector to get ready for the next interaction dataset
        allVehicleViolations.clear();
      } //END WHILE
    } //END IF

    //store the results in a results statistics struct
    struct ruleViolationStatistics results = {totalRuns, totalNonViolators, totalShouldStop, totalLeftBeforeRight, totalHasPriority, totalTurningLeft,
                                              totalTailgating, totalSpeedLimit, totalNoOffroad, PATH, std::string("All Files"), totalStops, totalYieldsOne, totalYieldsTwo};

    //print the results struct to a CSV file
    if (threeWayRoundaboutActive) americanVehiclePtr->outputToResultsLog(results);
  } //END TEST_F(AmericanTrafficRulesVehicleTest, threeWayIntersectionWithRoundabout)
} //END namespace vehicleData

/*
The following test script runs through the interaction datasets in the specified folder and checks for any rule violations,
as they were specified in the paper Formalization of Intersection Traffic Rules in Temporal Logic by Sebastian Maierhofer, Paul Moosbrugger, and Matthias Althoff.
  A link to the paper can be found here: https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
Rule violations can be visually inspected by checking the terminal output and looking for any test failures:
  The terminal will reference a failure message for the current CSV file, and continue to show the failure for the current test.
  As an example, we see the following (Note that "3" can represent any vehicle ID and "LeftBeforeRight" can represent any rule violation)
    The terminal will say it expected the quality of these values:
      std::to_string(vehicleData[vehicleID][0])
        Which is: "3"
        "LeftBeforeRight"]]></failure>
    Where "3" is the vehicle ID violating the rule, and "LeftBeforeRight" is the rule being violated
We can visualize the interaction dataset in question and manually inspect offending vehicle by using the following tool from GitHub: https://github.com/interaction-dataset/interaction-dataset
  Firstly, opening the terminal and moving the working directory to the interaction-dataset/python folder is required
  Secondly, run: ./main_visualize_data.py .TestScenarioForScripts 
    Where .TestScenarioForScripts (under interaction-dataset/recorded_trackfiles/.TestScenarioForScripts/) contains the correct interaction-CSV file, and is renamed to vehicle_tracks_000.csv
    And (interaction-dataset/maps/) contains the correct OSM map (the one corresponding to the interaction-CSV being analyzed) renamed to .TestScenarioForScripts.osm 
*/
namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, threeWayIntersectionShort) {
    //container for the directories being checked
    DIR * dir;
    struct dirent * ent;

    //container to determine if a vehicle is tailgating another
    struct tailgatingInformation tailgating;

    //create a struct object to contain all violations for a given vehicle
    struct violations vehicleViolations;

    //create a vector to contain all violations for all vehicles in an interaction dataset
    std::vector<struct violations> allVehicleViolations;

    //create a vector to contain all violations for all vehicles across interaction datasets
    std::vector<std::pair<std::vector<struct violations>, std::string>> allViolationsAllDatasets;

    //container for the number of people who did not violate any rule
    bool nonViolator;

    //contains the number of people not to violate any traffic rules
    int nonViolators = 0;
    int totalNonViolators = 0;

    //contains the statistics for all interaction datasets within a given test
    int totalRuns = 0;
    int totalShouldStop = 0;
    int totalLeftBeforeRight = 0;
    int totalHasPriority = 0;
    int totalTurningLeft = 0;
    int totalTailgating = 0;
    int totalSpeedLimit = 0;
    int totalNoOffroad = 0;
    int totalStops = 0;
    int totalYieldsOne = 0;
    int totalYieldsTwo = 0;

    //contains statistics for specific interaction datasets within a given test
    int runs = 0;
    int shouldStop = 0;
    int leftBeforeRight = 0;
    int hasPriority = 0;
    int turningLeft = 0;
    int isTailgating = 0;
    int speedLimit = 0;
    int noOffroad = 0;
    int stops = 0;
    int yieldsOne = 0;
    int yieldsTwo = 0;

    //used for exiting early
    int iteration = 0;
    int COUNTER = 0;

    //define the working map path
    std::string workingMapPath = mapPathShortThreeWay;

    //define the working data path
    std::string workingDataPath = dataPathShortThreeWayDir;

    //stores the path to the output file
    std::string PATH = "./Output/resultsLogThreeWayIntersectionShort.csv";

    //stores the path to the output file and the new header for the new file
    const char HEADER[] = "File,Total Vehicles,Non-Rule-Violating Vehicles,Stop-Violators,Stops Needed,Left Before Right Violators,Lanelet Priority Violators,Should Turn Left Violators,Yields One Needed,Yields Two Needed,Vehicle Tailgating Violations,Speed Limit,No Offroad\n";

    //clear the output file
    if (threeWayShortActive) americanVehiclePtr->clearOutput(PATH, HEADER);

    //contains the speed limit epsilon value in terms of meters per second
    double speedingEpsilonStrict = 0;
    double speedingEpsilonSoft = 1.38889;
    double speedingEpsilonHard = 2.77778;

    //contains the results of the vehicle speed limit violation
    std::string vehicleSpeedingResults;

    //format header for the speeding results
    const char SPEEDING_HEADER[] = "FILE,ID,TRIP DURATION,SPEEDING DURATION,MOTION DURATION,TIME BASED PROPORTION OF SPEEDING,SPEED LIMIT,EPSILON";;

    //containers for the file paths to the speeding results spreadsheets
    std::string filePathSpeedingStrict = "./Output/SpeedingResultsStrict-ThreeWayIntersectionShort.csv";
    std::string filePathSpeedingSoft = "./Output/SpeedingResultsSoft-ThreeWayIntersectionShort.csv";
    std::string filePathSpeedingHard = "./Output/SpeedingResultsHard-ThreeWayIntersectionShort.csv";

    //reset the output file for the speeding results
    if (threeWayShortActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingStrict, SPEEDING_HEADER);
    if (threeWayShortActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingSoft, SPEEDING_HEADER);
    if (threeWayShortActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingHard, SPEEDING_HEADER);

    //container for the output line for the vehicle stop analysis
    std::string stopResults;
    //format header for the stop violation results
    const char STOP_HEADER[] = "FILE,ID,Standard,Standard-Velocity,High Velocity,High-Velocity,Low Velocity,Low-Velocity,High Distance,Long-Velocity,Low Distance,Short-Velocity";
    //reset the output file for the stopping results
    if (threeWayShortActive && stopChecking) americanVehiclePtr->resetOutputDataToExcel("./Output/StoppingResultsStandard-ThreeWayIntersectionShort.csv", STOP_HEADER);

    //contains the file path for the output log
    std::string filePathFull = "./Output/noOffroadFull-ThreeWayIntersectionShort.csv";

    //contains the header for the output file
    const char OFFROAD_HEADER[] = "FILE,ID,TRIP DURATION,"
                                  "OFFROAD DURATION (FULL),TIME BASED PROPORTION OF OFFROAD (FULL),"
                                  "OFFROAD DURATION (FRAME),TIME BASED PROPORTION OF OFFROAD (FRAME),"
                                  "OFFROAD DURATION (CHASSIS),TIME BASED PROPORTION OF OFFROAD (CHASSIS),"
                                  "OFFROAD DURATION (HALF),TIME BASED PROPORTION OF OFFROAD(HALF),"
                                  "OFFROAD DURATION (CENTER),TIME BASED PROPORTION OF OFFROAD (CENTER)";

    //clears the output file
    if (threeWayShortActive && offroadChecking) americanVehiclePtr->resetOutputDataToExcel(filePathFull, OFFROAD_HEADER);

    //contains the collision results for the given vehicle
    std::vector<struct violation> resultsFull, resultsFrame, resultsChassis, resultsHalf, resultsCenter;

    //container for the results string for off-road violation detection
    std::string noOffroadResults;

    //contains the file path for the turning violation results
    std::string turnViolations = "./Output/turnViolationResults-ThreeWayIntersectionShort.csv";

    //contains the header for the output file
    const char TURN_VIOLATION_HEADER[] = "FILE,Offender ID,X-Position,Y-Position,Timestamp,Stop Duration,Length,Width,Yaw,Direction,Trip Duration,Proportion";

    //clears the output file
    if (threeWayShortActive && turnChecking) americanVehiclePtr->resetOutputDataToExcel(turnViolations, TURN_VIOLATION_HEADER);

    //container for the results string for turn violation detection
    std::string turnViolationResults = "";

    //container for the tailgating results
    std::string tailgatingResults;

    //container for the spreadsheet for tailgating violations
    std::string tailgatingViolations = "./Output/tailgatingViolations-ThreeWayIntersectionShort.csv";

    //contains the header for the output file
    const char TAILGATING_VIOLATION_HEADER[] = "FILE,ID,TRIP DURATION,"
                                               "TAILGATING DURATION (Default),TIME BASED PROPORTION OF TAILGATING (Default),"
                                               "TAILGATING DURATION (HighResponse),TIME BASED PROPORTION OF TAILGATING (HighResponse),"
                                               "TAILGATING DURATION (HighBreak),TIME BASED PROPORTION OF TAILGATING (HighBreak),"
                                               "TAILGATING DURATION (HighAccel),TIME BASED PROPORTION OF TAILGATING (HighAccel),"
                                               "TAILGATING DURATION (HighMinAccel),TIME BASED PROPORTION OF TAILGATING (HighMinAccel),"
                                               "TAILGATING DURATION (LowResponse),TIME BASED PROPORTION OF TAILGATING (LowResponse),"
                                               "TAILGATING DURATION (LowBreak),TIME BASED PROPORTION OF TAILGATING (LowBreak),"
                                               "TAILGATING DURATION (LowAccel),TIME BASED PROPORTION OF TAILGATING (LowAccel),"
                                               "TAILGATING DURATION (LowMinAccel),TIME BASED PROPORTION OF TAILGATING (LowMinAccel)";

    //reset the output file for tailgating violations
    if (threeWayShortActive && tailgatingChecking) americanVehiclePtr->resetOutputDataToExcel(tailgatingViolations, TAILGATING_VIOLATION_HEADER);

    //if the Given Directory Path can be Opened by the File System
    if ((dir = opendir(workingDataPath.c_str())) != NULL) {
      //for Every File in the Given Directory Path
      while ((ent = readdir (dir)) != NULL) {
        if (!threeWayShortActive) break;
        //formats the directory path into a string
        std::string path = workingDataPath + "/" + std::string(ent->d_name);

        //filters out file names too short to be CSV's like "." and ".."
        if (strlen(ent->d_name) < 4) {
          //move to the next iteration of the loop
          continue;
        }
        else if (iteration != COUNTER) {
          iteration += 1;
          continue;
        } //END IF

        //get and store the formatted vehicle data from the interaction dataset
        std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(path);

        //contains the origin point for the open street map file
        lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

        //output the file name
        EXPECT_EQ("file: ", path);

        //contains statistics for the vehicles being scanned
        runs = 0;
        nonViolators = 0;
        shouldStop = 0;
        leftBeforeRight = 0;
        hasPriority = 0;
        turningLeft = 0;
        isTailgating = 0;
        speedLimit = 0;
        noOffroad = 0;
        stops = 0;
        yieldsOne = 0;
	yieldsTwo = 0;

        //tests every vehicle, and reports those that fail with their ID
        for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
          //re-build a violations struct for the next vehicle encountered
          vehicleViolations = violations();

          //count the iteration through the FOR loop as a run
          runs += 1;

          //signals if the given vehicle NOT has violated any rules, and is true by default
          nonViolator = true;

          //determines if stop checking is active or not
          if (stopChecking) {
            //check if the given vehicle failed to stop at a stop regulated intersection
            vehicleViolations.ruleI = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                          runtimeCoefficients.stop.distanceThreshold,
                                                                          runtimeCoefficients.stop.velocityThreshold,
                                                                          lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                          //END checkStopAtSign
            vehicleViolations.ruleIStrictVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictVelocity.distanceThreshold,
                                                                                        stopStrictVelocity.velocityThreshold,
                                                                                        lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardVelocity.distanceThreshold,
                                                                                      stopHardVelocity.velocityThreshold,
                                                                                      lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                                      //END checkStopAtSign
            vehicleViolations.ruleIStrictDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictDistance.distanceThreshold,
                                                                                        stopStrictDistance.velocityThreshold,
                                                                                        lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardDistance.distanceThreshold,
                                                                                      stopHardDistance.velocityThreshold,
                                                                                      lanelet::traffic_rules::America, StopDirection::MultiStop);

            //records the stop results to the given string
            stopResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + ",";

            //check if the vehicle needs to stop
            stops += int(vehicleViolations.ruleI.neededToStop.get_value_or(0));

            //record the results of the stop sign checking
            if (vehicleViolations.ruleI.ruleStatus && vehicleViolations.ruleI.endTimestamp && vehicleViolations.ruleI.timestamp) {
              //adds the stop results to the current output string
              stopResults += (std::to_string((vehicleViolations.ruleI.endTimestamp.get_value_or(0) - vehicleViolations.ruleI.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0))) + "," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictVelocity.ruleStatus && vehicleViolations.ruleIStrictVelocity.endTimestamp && vehicleViolations.ruleIStrictVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardVelocity.ruleStatus && vehicleViolations.ruleIHardVelocity.endTimestamp && vehicleViolations.ruleIHardVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictDistance.ruleStatus && vehicleViolations.ruleIStrictDistance.endTimestamp && vehicleViolations.ruleIStrictDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictDistance.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardDistance.ruleStatus && vehicleViolations.ruleIHardDistance.endTimestamp && vehicleViolations.ruleIHardDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardDistance.magnitude.get_value_or(-1)) + ",";
            } //END IF

            //output the results of the stop sign detection to the output file
            americanVehiclePtr->outputDataToExcel("./Output/StoppingResultsStandard-ThreeWayIntersectionShort.csv", stopResults);
          } //END IF

          if (tailgatingChecking) {
            //check if the given vehicle is tailgating other vehicles along the intersection
            vehicleViolations.tailgating = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                                    runtimeCoefficients.tailgating.velocityThreshold,
                                                                                    runtimeCoefficients.tailgating.distanceThreshold,
                                                                                    runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                                    //END isTailgatingAtMoment

            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
          } //END IF

          if (speedingChecking) {
            //check if the vehicle is violating the lanelets speed limit
            vehicleViolations.speedLimitViolationsStrict = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonStrict, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsSoft = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonSoft, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsHard = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonHard, lanelet::traffic_rules::America, 2.2352);
          } //END IF

          if (offroadChecking) {
            //check if the vehicle is violating the lanelets off-road boundaries
            vehicleViolations.offroadViolations = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveHalf);
            vehicleViolations.offroadViolationsFull = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Full);
            vehicleViolations.offroadViolationsFrame = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveFrame);
            vehicleViolations.offroadViolationsChassis = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveChassis);
            vehicleViolations.offroadViolationsCenter = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Centre);

            //contains the results of vehicle speeding detection            
            noOffroadResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                              std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check if the results from the above tests are not empty
            if (!vehicleViolations.offroadViolationsFull.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFull).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFull).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsFrame.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFrame).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFrame).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsChassis.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsChassis).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsChassis).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolations.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolations).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolations).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsCenter.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsCenter).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsCenter).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results to the csv file
            americanVehiclePtr->outputDataToExcel(filePathFull, noOffroadResults);
          } //END IF

          /*
            If the Given Vehicle Stopped at the Intersections Stop-Line Given an AllWayStop or a Stop Sign Regulated Intersection,
            We can Check if the Vehicle Performed the Correct Intersectional Right-Of-Way Transfers Given Multiple Vehicles Stopped
            at the Same Time or Another Vehicle Stopped First and has the Intersectional Right-Of-Way.
            Essentially, Checks if the Reference Vehicle Passes the Intersection with the Right-Of-Way After Stopping:
              -> If multiple Vehicles Stop at Once, the Reference Vehicle Starts Once Given the Right-Of-Way
              -> If Another Vehicle Stops just Before the Reference Vehicle, the Reference Vehicle Waits to be Given the Right-Of-Way
          */
          if (stopChecking && turnChecking && vehicleViolations.ruleI.ruleStatus) {
            //check if Rules III, IV, or V were violated by the Currently Iterated Vehicle
            vehicleViolations.ruleIII = americanVehiclePtr->checkLeftBeforeRight(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                 runtimeCoefficients.leftBeforeRight.distanceThreshold,
                                                                                 runtimeCoefficients.leftBeforeRight.velocityThreshold,
                                                                                 lanelet::traffic_rules::America, multiTurnYawChecking);
                                                                                 //END checkLeftBeforeRight

            vehicleViolations.ruleV = americanVehiclePtr->checkShouldBeTurningLeft(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                   runtimeCoefficients.leftTurnPriority.distanceThreshold,
                                                                                   runtimeCoefficients.leftTurnPriority.velocityThreshold,
                                                                                   lanelet::traffic_rules::America, multiTurnYawChecking);
                                                                                   //END checkShouldBeTurningLeft

            yieldsOne += int(vehicleViolations.ruleIII.neededToStop.get_value_or(0));
            yieldsTwo += int(vehicleViolations.ruleV.neededToStop.get_value_or(0));

            //check for Violations Involving the Right-before-Left Rule (A Vehicle went LeftBeforeRight)
            if (!vehicleViolations.ruleIII.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleIII.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleIII.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)),
                        "LeftBeforeRight");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleIII.referenceID) + "," + std::to_string(vehicleViolations.ruleIII.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleIII.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.turnDirection.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";
              turnViolationResults += std::to_string(((double) vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left-right turn priority intersection stop violation
              leftBeforeRight += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF

            //check for Violations Involving the Turn Left Rule
            if (!vehicleViolations.ruleV.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleV.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleV.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)),
                        "ShouldBeTurningLeft");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleV.referenceID) + "," + std::to_string(vehicleViolations.ruleV.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleV.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.turnDirection.get_value_or(0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left turn intersection stop violation
              turningLeft += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          }
          //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
          else if (stopChecking && !vehicleViolations.ruleI.ruleStatus) {
            //record that a vehicle was detected engaging in a stopline intersection stop violation
            shouldStop += 1;
            //record that the vehicle has violated at least one rule
            nonViolator = false;
          } //END IF

          if (speedingChecking) {
            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsStrict).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsStrict))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);

              //record that a vehicle was detected engaging in a stopline intersection stop violation
              speedLimit += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsSoft).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsSoft))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            }
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsHard).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsHard))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            }
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } //END IF
          } //END IF

          if (offroadChecking) {
            //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
            if (vehicleViolations.offroadViolations.size() != 0) {
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              noOffroad += 1;
              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          } //END IF

          if (tailgatingChecking) {
            //contains the results of vehicle speeding detection            
            tailgatingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check for Violations Involving Tailgating between Vehicles
            if (vehicleViolations.tailgating.tailgatedVehicle.size() != 0) {
              //record that a vehicle was detected tailgating
              isTailgating += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;

              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgating.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgating.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            }
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results of the vehicle speeding
            americanVehiclePtr->outputDataToExcel(tailgatingViolations, tailgatingResults);
          } //END IF

          //record all rule violations for the given vehicle by pushing it into the vector of all rule violations
          allVehicleViolations.push_back(vehicleViolations);

          //record that a non violating vehicle was recorded if no rules were broken
          if (nonViolator) {
            //no rules were violated
            nonViolators += 1;
          } //END IF
        } //END FOR

        //counts the total number of vehicles ran through by the loop
        totalRuns += runs;

        //counts the total number of non-rule violating vehicles encountered
        totalNonViolators += nonViolators;

        //counts the number of rule violating vehicle encountered in the given interaction dataset
        totalShouldStop += shouldStop;
        totalLeftBeforeRight += leftBeforeRight;
        totalHasPriority += hasPriority;
        totalTurningLeft += turningLeft;
        totalTailgating += isTailgating;
        totalSpeedLimit += speedLimit;
        totalNoOffroad += noOffroad;
        totalStops += stops;
	totalYieldsOne += yieldsOne;
        totalYieldsTwo += yieldsTwo;

        //store the results in a results statistics struct
        struct ruleViolationStatistics results = {runs, nonViolators, shouldStop, leftBeforeRight, hasPriority, turningLeft,
                                                  isTailgating, speedLimit, noOffroad, PATH, std::string(ent->d_name), stops, yieldsOne, yieldsTwo};

        //print the results struct to a CSV file
        americanVehiclePtr->outputToResultsLog(results);

        //append the vector and the interaction dataset path to the list of all rule violations
        allViolationsAllDatasets.push_back(std::make_pair(allVehicleViolations, path));

        //clear the given vector to get ready for the next interaction dataset
        allVehicleViolations.clear();
      } //END WHILE
    } //END IF

    //store the results in a results statistics struct
    struct ruleViolationStatistics results = {totalRuns, totalNonViolators, totalShouldStop, totalLeftBeforeRight, totalHasPriority,
                                              totalTurningLeft, totalTailgating, totalSpeedLimit, totalNoOffroad, PATH, std::string("All Files"), totalStops, totalYieldsOne, totalYieldsTwo};

    //print the results struct to a CSV file
    if (threeWayShortActive) americanVehiclePtr->outputToResultsLog(results);
  } //END TEST_F(AmericanTrafficRulesVehicleTest, threeWayIntersectionShort)
} //END namespace vehicleData

/*
The following test script runs through the interaction datasets in the specified folder and checks for any rule violations,
as they were specified in the paper Formalization of Intersection Traffic Rules in Temporal Logic by Sebastian Maierhofer, Paul Moosbrugger, and Matthias Althoff.
  A link to the paper can be found here: https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
Rule violations can be visually inspected by checking the terminal output and looking for any test failures:
  The terminal will reference a failure message for the current CSV file, and continue to show the failure for the current test.
  As an example, we see the following (Note that "3" can represent any vehicle ID and "LeftBeforeRight" can represent any rule violation)
    The terminal will say it expected the quality of these values:
      std::to_string(vehicleData[vehicleID][0])
        Which is: "3"
        "LeftBeforeRight"]]></failure>
    Where "3" is the vehicle ID violating the rule, and "LeftBeforeRight" is the rule being violated
We can visualize the interaction dataset in question and manually inspect offending vehicle by using the following tool from GitHub: https://github.com/interaction-dataset/interaction-dataset
  Firstly, opening the terminal and moving the working directory to the interaction-dataset/python folder is required
  Secondly, run: ./main_visualize_data.py .TestScenarioForScripts 
    Where .TestScenarioForScripts (under interaction-dataset/recorded_trackfiles/.TestScenarioForScripts/) contains the correct interaction-CSV file, and is renamed to vehicle_tracks_000.csv
    And (interaction-dataset/maps/) contains the correct OSM map (the one corresponding to the interaction-CSV being analyzed) renamed to .TestScenarioForScripts.osm 
*/
namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, threeWayIntersection) {
    //container for the directories being checked
    DIR * dir;
    struct dirent * ent;

    //container to determine if a vehicle is tailgating another
    struct tailgatingInformation tailgating;

    //create a struct object to contain all violations for a given vehicle
    struct violations vehicleViolations;

    //create a vector to contain all violations for all vehicles in an interaction dataset
    std::vector<struct violations> allVehicleViolations;

    //create a vector to contain all violations for all vehicles across interaction datasets
    std::vector<std::pair<std::vector<struct violations>, std::string>> allViolationsAllDatasets;

    //container for the number of people who did not violate any rule
    bool nonViolator;

    //contains the number of people not to violate any traffic rules
    int nonViolators = 0;
    int totalNonViolators = 0;

    //contains the statistics for all interaction datasets within a given test
    int totalRuns = 0;
    int totalShouldStop = 0;
    int totalLeftBeforeRight = 0;
    int totalHasPriority = 0;
    int totalTurningLeft = 0;
    int totalTailgating = 0;
    int totalSpeedLimit = 0;
    int totalNoOffroad = 0;
    int totalStops = 0;
    int totalYieldsOne = 0;
    int totalYieldsTwo = 0;

    //contains statistics for specific interaction datasets within a given test
    int runs = 0;
    int shouldStop = 0;
    int leftBeforeRight = 0;
    int hasPriority = 0;
    int turningLeft = 0;
    int isTailgating = 0;
    int speedLimit = 0;
    int noOffroad = 0;
    int stops = 0;
    int yieldsOne = 0;
    int yieldsTwo = 0;

    //used for exiting early
    int iteration = 0;
    int COUNTER = 0;

    //define the working map path
    std::string workingMapPath = mapPathThreeWay;

    //define the working data path
    std::string workingDataPath = dataPathThreeWayDir;

    //stores the path to the output file
    std::string PATH = "./Output/resultsLogThreeWayIntersection.csv";

    //stores the path to the output file and the new header for the new file
    const char HEADER[] = "File,Total Vehicles,Non-Rule-Violating Vehicles,Stop-Violators,Stops Needed,Left Before Right Violators,Lanelet Priority Violators,Should Turn Left Violators,Yields One Needed,Yields Two Needed,Vehicle Tailgating Violations,Speed Limit,No Offroad\n";

    //clear the output file
    if (threeWayActive) americanVehiclePtr->clearOutput(PATH, HEADER);

    //contains the speed limit epsilon value in terms of meters per second
    double speedingEpsilonStrict = 0;
    double speedingEpsilonSoft = 1.38889;
    double speedingEpsilonHard = 2.77778;

    //contains the results of the vehicle speed limit violation
    std::string vehicleSpeedingResults;

    //format header for the speeding results
    const char SPEEDING_HEADER[] = "FILE,ID,TRIP DURATION,SPEEDING DURATION,MOTION DURATION,TIME BASED PROPORTION OF SPEEDING,SPEED LIMIT,EPSILON";;

    //containers for the file paths to the speeding results spreadsheets
    std::string filePathSpeedingStrict = "./Output/SpeedingResultsStrict-ThreeWayIntersection.csv";
    std::string filePathSpeedingSoft = "./Output/SpeedingResultsSoft-ThreeWayIntersection.csv";
    std::string filePathSpeedingHard = "./Output/SpeedingResultsHard-ThreeWayIntersection.csv";

    //reset the output file for the speeding results
    if (threeWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingStrict, SPEEDING_HEADER);
    if (threeWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingSoft, SPEEDING_HEADER);
    if (threeWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingHard, SPEEDING_HEADER);

    //container for the output line for the vehicle stop analysis
    std::string stopResults;
    //format header for the stop violation results
    const char STOP_HEADER[] = "FILE,ID,Standard,Standard-Velocity,High Velocity,High-Velocity,Low Velocity,Low-Velocity,High Distance,Long-Velocity,Low Distance,Short-Velocity";
    //reset the output file for the stopping results
    if (threeWayActive && stopChecking) americanVehiclePtr->resetOutputDataToExcel("./Output/StoppingResultsStandard-ThreeWayIntersection.csv", STOP_HEADER);

    //contains the file path for the output log
    std::string filePathFull = "./Output/noOffroadFull-ThreeWayIntersection.csv";

    //contains the header for the output file
    const char OFFROAD_HEADER[] = "FILE,ID,TRIP DURATION,"
                                  "OFFROAD DURATION (FULL),TIME BASED PROPORTION OF OFFROAD (FULL),"
                                  "OFFROAD DURATION (FRAME),TIME BASED PROPORTION OF OFFROAD (FRAME),"
                                  "OFFROAD DURATION (CHASSIS),TIME BASED PROPORTION OF OFFROAD (CHASSIS),"
                                  "OFFROAD DURATION (HALF),TIME BASED PROPORTION OF OFFROAD(HALF),"
                                  "OFFROAD DURATION (CENTER),TIME BASED PROPORTION OF OFFROAD (CENTER)";

    //clears the output file
    if (threeWayActive && offroadChecking) americanVehiclePtr->resetOutputDataToExcel(filePathFull, OFFROAD_HEADER);

    //contains the collision results for the given vehicle
    std::vector<struct violation> resultsFull, resultsFrame, resultsChassis, resultsHalf, resultsCenter;

    //container for the results string for off-road violation detection
    std::string noOffroadResults;

    //contains the file path for the turning violation results
    std::string turnViolations = "./Output/turnViolationResults-ThreeWayIntersection.csv";

    //contains the header for the output file
    const char TURN_VIOLATION_HEADER[] = "FILE,Offender ID,X-Position,Y-Position,Timestamp,Stop Duration,Length,Width,Yaw,Direction,Trip Duration,Proportion";

    //clears the output file
    if (threeWayActive && turnChecking) americanVehiclePtr->resetOutputDataToExcel(turnViolations, TURN_VIOLATION_HEADER);

    //container for the results string for turn violation detection
    std::string turnViolationResults = "";

    //container for the tailgating results
    std::string tailgatingResults;

    //container for the spreadsheet for tailgating violations
    std::string tailgatingViolations = "./Output/tailgatingViolations-ThreeWayIntersection.csv";

    //contains the header for the output file
    const char TAILGATING_VIOLATION_HEADER[] = "FILE,ID,TRIP DURATION,"
                                               "TAILGATING DURATION (Default),TIME BASED PROPORTION OF TAILGATING (Default),"
                                               "TAILGATING DURATION (HighResponse),TIME BASED PROPORTION OF TAILGATING (HighResponse),"
                                               "TAILGATING DURATION (HighBreak),TIME BASED PROPORTION OF TAILGATING (HighBreak),"
                                               "TAILGATING DURATION (HighAccel),TIME BASED PROPORTION OF TAILGATING (HighAccel),"
                                               "TAILGATING DURATION (HighMinAccel),TIME BASED PROPORTION OF TAILGATING (HighMinAccel),"
                                               "TAILGATING DURATION (LowResponse),TIME BASED PROPORTION OF TAILGATING (LowResponse),"
                                               "TAILGATING DURATION (LowBreak),TIME BASED PROPORTION OF TAILGATING (LowBreak),"
                                               "TAILGATING DURATION (LowAccel),TIME BASED PROPORTION OF TAILGATING (LowAccel),"
                                               "TAILGATING DURATION (LowMinAccel),TIME BASED PROPORTION OF TAILGATING (LowMinAccel)";

    //reset the output file for tailgating violations
    if (threeWayActive && tailgatingChecking) americanVehiclePtr->resetOutputDataToExcel(tailgatingViolations, TAILGATING_VIOLATION_HEADER);

    //if the Given Directory Path can be Opened by the File System
    if ((dir = opendir(workingDataPath.c_str())) != NULL) {
      //for Every File in the Given Directory Path
      while ((ent = readdir (dir)) != NULL) {
        if (!threeWayActive) break;
        //formats the directory path into a string
        std::string path = workingDataPath + "/" + std::string(ent->d_name);

        //filters out file names too short to be CSV's like "." and ".."
        if (strlen(ent->d_name) < 4) {
          //move to the next iteration of the loop
          continue;
        }
        else if (iteration != COUNTER) {
          iteration += 1;
          continue;
        } //END IF

        //get and store the formatted vehicle data from the interaction dataset
        std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(path);

        //contains the origin point for the open street map file
        lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

        //output the file name
        EXPECT_EQ("file: ", path);

        //contains statistics for the vehicles being scanned
        runs = 0;
        nonViolators = 0;
        shouldStop = 0;
        leftBeforeRight = 0;
        hasPriority = 0;
        turningLeft = 0;
        isTailgating = 0;
        speedLimit = 0;
        noOffroad = 0;
        stops = 0;
        yieldsOne = 0;
        yieldsTwo = 0;

        //tests every vehicle, and reports those that fail with their ID
        for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
          //re-build a violations struct for the next vehicle encountered
          vehicleViolations = violations();

          //count the iteration through the FOR loop as a run
          runs += 1;

          //signals if the given vehicle NOT has violated any rules, and is true by default
          nonViolator = true;

          //determines if stop checking is active or not
          if (stopChecking) {
            //check if the given vehicle failed to stop at a stop regulated intersection
            vehicleViolations.ruleI = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                          runtimeCoefficients.stop.distanceThreshold,
                                                                          runtimeCoefficients.stop.velocityThreshold,
                                                                          lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                          //END checkStopAtSign
            vehicleViolations.ruleIStrictVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictVelocity.distanceThreshold,
                                                                                        stopStrictVelocity.velocityThreshold,
                                                                                        lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardVelocity.distanceThreshold,
                                                                                      stopHardVelocity.velocityThreshold,
                                                                                      lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                                      //END checkStopAtSign
            vehicleViolations.ruleIStrictDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictDistance.distanceThreshold,
                                                                                        stopStrictDistance.velocityThreshold,
                                                                                        lanelet::traffic_rules::America, StopDirection::MultiStop);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardDistance.distanceThreshold,
                                                                                      stopHardDistance.velocityThreshold,
                                                                                      lanelet::traffic_rules::America, StopDirection::MultiStop);

            //records the stop results to the given string
            stopResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + ",";

            //check if the vehicle needs to stop
            stops += int(vehicleViolations.ruleI.neededToStop.get_value_or(0));

            //record the results of the stop sign checking
            if (vehicleViolations.ruleI.ruleStatus && vehicleViolations.ruleI.endTimestamp && vehicleViolations.ruleI.timestamp) {
              //adds the stop results to the current output string
              stopResults += (std::to_string((vehicleViolations.ruleI.endTimestamp.get_value_or(0) - vehicleViolations.ruleI.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0))) + "," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictVelocity.ruleStatus && vehicleViolations.ruleIStrictVelocity.endTimestamp && vehicleViolations.ruleIStrictVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardVelocity.ruleStatus && vehicleViolations.ruleIHardVelocity.endTimestamp && vehicleViolations.ruleIHardVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictDistance.ruleStatus && vehicleViolations.ruleIStrictDistance.endTimestamp && vehicleViolations.ruleIStrictDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictDistance.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardDistance.ruleStatus && vehicleViolations.ruleIHardDistance.endTimestamp && vehicleViolations.ruleIHardDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardDistance.magnitude.get_value_or(-1)) + ",";
            } //END IF

            //output the results of the stop sign detection to the output file
            americanVehiclePtr->outputDataToExcel("./Output/StoppingResultsStandard-ThreeWayIntersection.csv", stopResults);
          } //END IF

          if (tailgatingChecking) {
            //check if the given vehicle is tailgating other vehicles along the intersection
            vehicleViolations.tailgating = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                                    runtimeCoefficients.tailgating.velocityThreshold,
                                                                                    runtimeCoefficients.tailgating.distanceThreshold,
                                                                                    runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                                    //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
          } //END IF

          if (speedingChecking) {
            //check if the vehicle is violating the lanelets speed limit
            vehicleViolations.speedLimitViolationsStrict = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonStrict, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsSoft = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonSoft, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsHard = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonHard, lanelet::traffic_rules::America, 2.2352);
          } //END IF

          if (offroadChecking) {
            //check if the vehicle is violating the lanelets off-road boundaries
            vehicleViolations.offroadViolations = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveHalf);
            vehicleViolations.offroadViolationsFull = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Full);
            vehicleViolations.offroadViolationsFrame = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveFrame);
            vehicleViolations.offroadViolationsChassis = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveChassis);
            vehicleViolations.offroadViolationsCenter = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Centre);

            //contains the results of vehicle speeding detection            
            noOffroadResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                              std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check if the results from the above tests are not empty
            if (!vehicleViolations.offroadViolationsFull.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFull).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFull).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsFrame.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFrame).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFrame).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsChassis.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsChassis).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsChassis).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolations.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolations).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolations).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsCenter.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsCenter).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsCenter).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results to the csv file
            americanVehiclePtr->outputDataToExcel(filePathFull, noOffroadResults);
          } //END IF

          /*
            If the Given Vehicle Stopped at the Intersections Stop-Line Given an AllWayStop or a Stop Sign Regulated Intersection,
            We can Check if the Vehicle Performed the Correct Intersectional Right-Of-Way Transfers Given Multiple Vehicles Stopped
            at the Same Time or Another Vehicle Stopped First and has the Intersectional Right-Of-Way.
            Essentially, Checks if the Reference Vehicle Passes the Intersection with the Right-Of-Way After Stopping:
              -> If multiple Vehicles Stop at Once, the Reference Vehicle Starts Once Given the Right-Of-Way
              -> If Another Vehicle Stops just Before the Reference Vehicle, the Reference Vehicle Waits to be Given the Right-Of-Way
          */
          if (stopChecking && turnChecking && vehicleViolations.ruleI.ruleStatus) {
            //check if Rules III, IV, or V were violated by the Currently Iterated Vehicle
            vehicleViolations.ruleIII = americanVehiclePtr->checkLeftBeforeRight(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                 runtimeCoefficients.leftBeforeRight.distanceThreshold,
                                                                                 runtimeCoefficients.leftBeforeRight.velocityThreshold,
                                                                                 lanelet::traffic_rules::America, multiTurnYawChecking);
                                                                                 //END checkLeftBeforeRight

            vehicleViolations.ruleV = americanVehiclePtr->checkShouldBeTurningLeft(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                   runtimeCoefficients.leftTurnPriority.distanceThreshold,
                                                                                   runtimeCoefficients.leftTurnPriority.velocityThreshold,
                                                                                   lanelet::traffic_rules::America, multiTurnYawChecking);
                                                                                   //END checkShouldBeTurningLeft

            yieldsOne += int(vehicleViolations.ruleIII.neededToStop.get_value_or(0));
            yieldsTwo += int(vehicleViolations.ruleV.neededToStop.get_value_or(0));

            //check for Violations Involving the Right-before-Left Rule (A Vehicle went LeftBeforeRight)
            if (!vehicleViolations.ruleIII.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleIII.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleIII.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)),
                        "LeftBeforeRight");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleIII.referenceID) + "," + std::to_string(vehicleViolations.ruleIII.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleIII.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.turnDirection.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";
              turnViolationResults += std::to_string(((double) vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left-right turn priority intersection stop violation
              leftBeforeRight += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF

            //check for Violations Involving the Turn Left Rule
            if (!vehicleViolations.ruleV.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleV.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleV.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)),
                        "ShouldBeTurningLeft");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleV.referenceID) + "," + std::to_string(vehicleViolations.ruleV.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleV.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.turnDirection.get_value_or(0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left turn intersection stop violation
              turningLeft += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          }
          //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
          else if (stopChecking && !vehicleViolations.ruleI.ruleStatus) {
            //record that a vehicle was detected engaging in a stopline intersection stop violation
            shouldStop += 1;
            //record that the vehicle has violated at least one rule
            nonViolator = false;
          } //END IF

          if (speedingChecking) {
            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsStrict).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsStrict))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
              
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              speedLimit += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsSoft).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsSoft))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsHard).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsHard))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } //END IF
          } //END IF

          if (offroadChecking) {
            //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
            if (vehicleViolations.offroadViolations.size() != 0) {
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              noOffroad += 1;
              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          } //END IF

          if (tailgatingChecking) {
            //contains the results of vehicle speeding detection            
            tailgatingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check for Violations Involving Tailgating between Vehicles
            if (vehicleViolations.tailgating.tailgatedVehicle.size() != 0) {
              //record that a vehicle was detected tailgating
              isTailgating += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;

              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgating.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgating.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results of the vehicle speeding
            americanVehiclePtr->outputDataToExcel(tailgatingViolations, tailgatingResults);
          } //END IF

          //record all rule violations for the given vehicle by pushing it into the vector of all rule violations
          allVehicleViolations.push_back(vehicleViolations);

          //record that a non violating vehicle was recorded if no rules were broken
          if (nonViolator) {
            //no rules were violated
            nonViolators += 1;
          } //END IF
        } //END FOR

        //counts the total number of vehicles ran through by the loop
        totalRuns += runs;

        //counts the total number of non-rule violating vehicles encountered
        totalNonViolators += nonViolators;

        //counts the number of rule violating vehicle encountered in the given interaction dataset
        totalShouldStop += shouldStop;
        totalLeftBeforeRight += leftBeforeRight;
        totalHasPriority += hasPriority;
        totalTurningLeft += turningLeft;
        totalTailgating += isTailgating;
        totalSpeedLimit += speedLimit;
        totalNoOffroad += noOffroad;
        totalStops += stops;
        totalYieldsOne += yieldsOne;
        totalYieldsTwo += yieldsTwo;

        //store the results in a results statistics struct
        struct ruleViolationStatistics results = {runs, nonViolators, shouldStop, leftBeforeRight, hasPriority, turningLeft,
                                                  isTailgating, speedLimit, noOffroad, PATH, std::string(ent->d_name), stops, yieldsOne, yieldsTwo};

        //print the results struct to a CSV file
        americanVehiclePtr->outputToResultsLog(results);

        //append the vector and the interaction dataset path to the list of all rule violations
        allViolationsAllDatasets.push_back(std::make_pair(allVehicleViolations, path));

        //clear the given vector to get ready for the next interaction dataset
        allVehicleViolations.clear();
        
      } //END WHILE
    } //END IF

    //store the results in a results statistics struct
    struct ruleViolationStatistics results = {totalRuns, totalNonViolators, totalShouldStop, totalLeftBeforeRight, totalHasPriority, totalTurningLeft,
                                              totalTailgating, totalSpeedLimit, totalNoOffroad, PATH, std::string("All Files"), totalStops, totalYieldsOne, totalYieldsTwo};

    //print the results struct to a CSV file
    if (threeWayActive) americanVehiclePtr->outputToResultsLog(results);
  } //END TEST_F(AmericanTrafficRulesVehicleTest, threeWayIntersection)
} //END namespace vehicleData

/*
The following test script runs through the interaction datasets in the specified folder and checks for any rule violations,
as they were specified in the paper Formalization of Intersection Traffic Rules in Temporal Logic by Sebastian Maierhofer, Paul Moosbrugger, and Matthias Althoff.
  A link to the paper can be found here: https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
Rule violations can be visually inspected by checking the terminal output and looking for any test failures:
  The terminal will reference a failure message for the current CSV file, and continue to show the failure for the current test.
  As an example, we see the following (Note that "3" can represent any vehicle ID and "LeftBeforeRight" can represent any rule violation)
    The terminal will say it expected the quality of these values:
      std::to_string(vehicleData[vehicleID][0])
        Which is: "3"
        "LeftBeforeRight"]]></failure>
    Where "3" is the vehicle ID violating the rule, and "LeftBeforeRight" is the rule being violated
We can visualize the interaction dataset in question and manually inspect offending vehicle by using the following tool from GitHub: https://github.com/interaction-dataset/interaction-dataset
  Firstly, opening the terminal and moving the working directory to the interaction-dataset/python folder is required
  Secondly, run: ./main_visualize_data.py .TestScenarioForScripts 
    Where .TestScenarioForScripts (under interaction-dataset/recorded_trackfiles/.TestScenarioForScripts/) contains the correct interaction-CSV file, and is renamed to vehicle_tracks_000.csv
    And (interaction-dataset/maps/) contains the correct OSM map (the one corresponding to the interaction-CSV being analyzed) renamed to .TestScenarioForScripts.osm 
*/
namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, rulesFourWay) {
    //container for the directories being checked
    DIR * dir;
    struct dirent * ent;

    //container to determine if a vehicle is tailgating another
    struct tailgatingInformation tailgating;

    //create a struct object to contain all violations for a given vehicle
    struct violations vehicleViolations;

    //create a vector to contain all violations for all vehicles in an interaction dataset
    std::vector<struct violations> allVehicleViolations;

    //create a vector to contain all violations for all vehicles across interaction datasets
    std::vector<std::pair<std::vector<struct violations>, std::string>> allViolationsAllDatasets;

    //container for the number of people who did not violate any rule
    bool nonViolator;

    //contains the number of people not to violate any traffic rules
    int nonViolators = 0;
    int totalNonViolators = 0;

    //contains the statistics for all interaction datasets within a given test
    int totalRuns = 0;
    int totalShouldStop = 0;
    int totalLeftBeforeRight = 0;
    int totalHasPriority = 0;
    int totalTurningLeft = 0;
    int totalTailgating = 0;
    int totalSpeedLimit = 0;
    int totalNoOffroad = 0;
    int totalStops = 0;
    int totalYieldsOne = 0;
    int totalYieldsTwo = 0;

    //contains statistics for specific interaction datasets within a given test
    int runs = 0;
    int shouldStop = 0;
    int leftBeforeRight = 0;
    int hasPriority = 0;
    int turningLeft = 0;
    int isTailgating = 0;
    int speedLimit = 0;
    int noOffroad = 0;
    int stops = 0;
    int yieldsOne = 0;
    int yieldsTwo = 0;

    //used for exiting early
    int iteration = 0;
    int COUNTER = 0;

    //define the working map path
    std::string workingMapPath = mapPath;

    //define the working data path
    std::string workingDataPath = dataPathDir;

    //stores the path to the output file
    std::string PATH = "./Output/resultsLogRulesFourWay.csv";

    //stores the path to the output file and the new header for the new file
    const char HEADER[] = "File,Total Vehicles,Non-Rule-Violating Vehicles,Stop-Violators,Stops Needed,Left Before Right Violators,Lanelet Priority Violators,Should Turn Left Violators,Yields One Needed,Yields Two Needed,Vehicle Tailgating Violations,Speed Limit,No Offroad\n";

    //clear the output file
    if (fourWayActive) americanVehiclePtr->clearOutput(PATH, HEADER);

    //contains the speed limit epsilon value in terms of meters per second
    double speedingEpsilonStrict = 0;
    double speedingEpsilonSoft = 1.38889;
    double speedingEpsilonHard = 2.77778;
    
    //contains the results of the vehicle speed limit violation
    std::string vehicleSpeedingResults;

    //format header for the speeding results
    const char SPEEDING_HEADER[] = "FILE,ID,TRIP DURATION,SPEEDING DURATION,MOTION DURATION,TIME BASED PROPORTION OF SPEEDING,SPEED LIMIT,EPSILON";;

    //containers for the file paths to the speeding results spreadsheets
    std::string filePathSpeedingStrict = "./Output/SpeedingResultsStrict-RulesFourWay.csv";
    std::string filePathSpeedingSoft = "./Output/SpeedingResultsSoft-RulesFourWay.csv";
    std::string filePathSpeedingHard = "./Output/SpeedingResultsHard-RulesFourWay.csv";

    //reset the output file for the speeding results
    if (fourWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingStrict, SPEEDING_HEADER);
    if (fourWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingSoft, SPEEDING_HEADER);
    if (fourWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingHard, SPEEDING_HEADER);

    //container for the output line for the vehicle stop analysis
    std::string stopResults;
    //format header for the stop violation results
    const char STOP_HEADER[] = "FILE,ID,Standard,Standard-Velocity,High Velocity,High-Velocity,Low Velocity,Low-Velocity,High Distance,Long-Velocity,Low Distance,Short-Velocity";
    //reset the output file for the stopping results
    if (fourWayActive && stopChecking) americanVehiclePtr->resetOutputDataToExcel("./Output/StoppingResultsStandard-RulesFourWay.csv", STOP_HEADER);

    //contains the file path for the output log
    std::string filePathFull = "./Output/noOffroadFull-RulesFourWay.csv";

    //contains the header for the output file
    const char OFFROAD_HEADER[] = "FILE,ID,TRIP DURATION,"
                                  "OFFROAD DURATION (FULL),TIME BASED PROPORTION OF OFFROAD (FULL),"
                                  "OFFROAD DURATION (FRAME),TIME BASED PROPORTION OF OFFROAD (FRAME),"
                                  "OFFROAD DURATION (CHASSIS),TIME BASED PROPORTION OF OFFROAD (CHASSIS),"
                                  "OFFROAD DURATION (HALF),TIME BASED PROPORTION OF OFFROAD(HALF),"
                                  "OFFROAD DURATION (CENTER),TIME BASED PROPORTION OF OFFROAD (CENTER)";

    //clears the output file
    if (fourWayActive && offroadChecking) americanVehiclePtr->resetOutputDataToExcel(filePathFull, OFFROAD_HEADER);

    //contains the collision results for the given vehicle
    std::vector<struct violation> resultsFull, resultsFrame, resultsChassis, resultsHalf, resultsCenter;

    //container for the results string for off-road violation detection
    std::string noOffroadResults;

    //contains the file path for the turning violation results
    std::string turnViolations = "./Output/turnViolationResults-RulesFourWay.csv";

    //contains the header for the output file
    const char TURN_VIOLATION_HEADER[] = "FILE,Offender ID,X-Position,Y-Position,Timestamp,Stop Duration,Length,Width,Yaw,Direction,Trip Duration,Proportion";

    //clears the output file
    if (fourWayActive && turnChecking) americanVehiclePtr->resetOutputDataToExcel(turnViolations, TURN_VIOLATION_HEADER);

    //container for the results string for turn violation detection
    std::string turnViolationResults = "";

    //container for the tailgating results
    std::string tailgatingResults;

    //container for the spreadsheet for tailgating violations
    std::string tailgatingViolations = "./Output/tailgatingViolations-RulesFourWay.csv";

    //contains the header for the output file
    const char TAILGATING_VIOLATION_HEADER[] = "FILE,ID,TRIP DURATION,"
                                               "TAILGATING DURATION (Default),TIME BASED PROPORTION OF TAILGATING (Default),"
                                               "TAILGATING DURATION (HighResponse),TIME BASED PROPORTION OF TAILGATING (HighResponse),"
                                               "TAILGATING DURATION (HighBreak),TIME BASED PROPORTION OF TAILGATING (HighBreak),"
                                               "TAILGATING DURATION (HighAccel),TIME BASED PROPORTION OF TAILGATING (HighAccel),"
                                               "TAILGATING DURATION (HighMinAccel),TIME BASED PROPORTION OF TAILGATING (HighMinAccel),"
                                               "TAILGATING DURATION (LowResponse),TIME BASED PROPORTION OF TAILGATING (LowResponse),"
                                               "TAILGATING DURATION (LowBreak),TIME BASED PROPORTION OF TAILGATING (LowBreak),"
                                               "TAILGATING DURATION (LowAccel),TIME BASED PROPORTION OF TAILGATING (LowAccel),"
                                               "TAILGATING DURATION (LowMinAccel),TIME BASED PROPORTION OF TAILGATING (LowMinAccel)";

    //reset the output file for tailgating violations
    if (fourWayActive && tailgatingChecking) americanVehiclePtr->resetOutputDataToExcel(tailgatingViolations, TAILGATING_VIOLATION_HEADER);

    //if the Given Directory Path can be Opened by the File System
    if ((dir = opendir(workingDataPath.c_str())) != NULL) {
      //for Every File in the Given Directory Path
      while ((ent = readdir (dir)) != NULL) {
        if (!fourWayActive) break;
        //formats the directory path into a string
        std::string path = workingDataPath + "/" + std::string(ent->d_name);

        //filters out file names too short to be CSV's like "." and ".."
        if (strlen(ent->d_name) < 4) {
          //move to the next iteration of the loop
          continue;
        }
        else if (iteration != COUNTER) {
          iteration += 1;
          continue;
        } //END IF

        //get and store the formatted vehicle data from the interaction dataset
        std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(path);

        //contains the origin point for the open street map file
        lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

        //output the file name
        EXPECT_EQ("file: ", path);

        //contains statistics for the vehicles being scanned
        runs = 0;
        nonViolators = 0;
        shouldStop = 0;
        leftBeforeRight = 0;
        hasPriority = 0;
        turningLeft = 0;
        isTailgating = 0;
        speedLimit = 0;
        noOffroad = 0;
        stops = 0;
        yieldsOne = 0;
        yieldsTwo = 0;

        //tests every vehicle, and reports those that fail with their ID
        for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
          //re-build a violations struct for the next vehicle encountered
          vehicleViolations = violations();

          //count the iteration through the FOR loop as a run
          runs += 1;

          //signals if the given vehicle NOT has violated any rules, and is true by default
          nonViolator = true;

          //determines if stop checking is active or not
          if (stopChecking) {
            //check if the given vehicle failed to stop at a stop regulated intersection
            vehicleViolations.ruleI = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                          runtimeCoefficients.stop.distanceThreshold,
                                                                          runtimeCoefficients.stop.velocityThreshold,
                                                                          lanelet::traffic_rules::America);
                                                                          //END checkStopAtSign
            vehicleViolations.ruleIStrictVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictVelocity.distanceThreshold,
                                                                                        stopStrictVelocity.velocityThreshold,
                                                                                        lanelet::traffic_rules::America);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardVelocity.distanceThreshold,
                                                                                      stopHardVelocity.velocityThreshold,
                                                                                      lanelet::traffic_rules::America);
                                                                                      //END checkStopAtSign
            vehicleViolations.ruleIStrictDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictDistance.distanceThreshold,
                                                                                        stopStrictDistance.velocityThreshold,
                                                                                        lanelet::traffic_rules::America);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardDistance.distanceThreshold,
                                                                                      stopHardDistance.velocityThreshold,
                                                                                      lanelet::traffic_rules::America);

            //records the stop results to the given string
            stopResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + ",";

            //check if the vehicle needs to stop
            stops += int(vehicleViolations.ruleI.neededToStop.get_value_or(0));

            //record the results of the stop sign checking
            if (vehicleViolations.ruleI.ruleStatus && vehicleViolations.ruleI.endTimestamp && vehicleViolations.ruleI.timestamp) {
              //adds the stop results to the current output string
              stopResults += (std::to_string((vehicleViolations.ruleI.endTimestamp.get_value_or(0) - vehicleViolations.ruleI.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0))) + "," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictVelocity.ruleStatus && vehicleViolations.ruleIStrictVelocity.endTimestamp && vehicleViolations.ruleIStrictVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardVelocity.ruleStatus && vehicleViolations.ruleIHardVelocity.endTimestamp && vehicleViolations.ruleIHardVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictDistance.ruleStatus && vehicleViolations.ruleIStrictDistance.endTimestamp && vehicleViolations.ruleIStrictDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictDistance.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardDistance.ruleStatus && vehicleViolations.ruleIHardDistance.endTimestamp && vehicleViolations.ruleIHardDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardDistance.magnitude.get_value_or(-1)) + ",";
            } //END IF

            //output the results of the stop sign detection to the output file
            americanVehiclePtr->outputDataToExcel("./Output/StoppingResultsStandard-RulesFourWay.csv", stopResults);
          } //END IF

          if (tailgatingChecking) {
            //check if the given vehicle is tailgating other vehicles along the intersection
            vehicleViolations.tailgating = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                                    runtimeCoefficients.tailgating.velocityThreshold,
                                                                                    runtimeCoefficients.tailgating.distanceThreshold,
                                                                                    runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                                    //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
          } //END IF

          if (speedingChecking) {
            //check if the vehicle is violating the lanelets speed limit
            vehicleViolations.speedLimitViolationsStrict = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonStrict, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsSoft = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonSoft, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsHard = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonHard, lanelet::traffic_rules::America, 2.2352);
          } //END IF

          if (offroadChecking) {
            //check if the vehicle is violating the lanelets off-road boundaries
            vehicleViolations.offroadViolations = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveHalf);
            vehicleViolations.offroadViolationsFull = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Full);
            vehicleViolations.offroadViolationsFrame = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveFrame);
            vehicleViolations.offroadViolationsChassis = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveChassis);
            vehicleViolations.offroadViolationsCenter = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Centre);

            //contains the results of vehicle speeding detection            
            noOffroadResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                              std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check if the results from the above tests are not empty
            if (!vehicleViolations.offroadViolationsFull.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFull).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFull).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsFrame.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFrame).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFrame).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsChassis.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsChassis).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsChassis).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolations.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolations).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolations).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsCenter.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsCenter).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsCenter).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results to the csv file
            americanVehiclePtr->outputDataToExcel(filePathFull, noOffroadResults);
          } //END IF

          /*
            If the Given Vehicle Stopped at the Intersections Stop-Line Given an AllWayStop or a Stop Sign Regulated Intersection,
            We can Check if the Vehicle Performed the Correct Intersectional Right-Of-Way Transfers Given Multiple Vehicles Stopped
            at the Same Time or Another Vehicle Stopped First and has the Intersectional Right-Of-Way.
            Essentially, Checks if the Reference Vehicle Passes the Intersection with the Right-Of-Way After Stopping:
              -> If multiple Vehicles Stop at Once, the Reference Vehicle Starts Once Given the Right-Of-Way
              -> If Another Vehicle Stops just Before the Reference Vehicle, the Reference Vehicle Waits to be Given the Right-Of-Way
          */
          if (stopChecking && turnChecking && vehicleViolations.ruleI.ruleStatus) {
            //check if Rules III, IV, or V were violated by the Currently Iterated Vehicle
            vehicleViolations.ruleIII = americanVehiclePtr->checkLeftBeforeRight(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                 runtimeCoefficients.leftBeforeRight.distanceThreshold,
                                                                                 runtimeCoefficients.leftBeforeRight.velocityThreshold,
                                                                                 lanelet::traffic_rules::America);
                                                                                 //END checkLeftBeforeRight

            vehicleViolations.ruleV = americanVehiclePtr->checkShouldBeTurningLeft(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                   runtimeCoefficients.leftTurnPriority.distanceThreshold,
                                                                                   runtimeCoefficients.leftTurnPriority.velocityThreshold,
                                                                                   lanelet::traffic_rules::America);
                                                                                   //END checkShouldBeTurningLeft

            yieldsOne += int(vehicleViolations.ruleIII.neededToStop.get_value_or(0));
            yieldsTwo += int(vehicleViolations.ruleV.neededToStop.get_value_or(0));

            //check for Violations Involving the Right-before-Left Rule (A Vehicle went LeftBeforeRight)
            if (!vehicleViolations.ruleIII.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleIII.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleIII.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)),
                        "LeftBeforeRight");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleIII.referenceID) + "," + std::to_string(vehicleViolations.ruleIII.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleIII.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.turnDirection.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";
              turnViolationResults += std::to_string(((double) vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left-right turn priority intersection stop violation
              leftBeforeRight += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF

            //check for Violations Involving the Turn Left Rule
            if (!vehicleViolations.ruleV.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleV.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleV.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)),
                        "ShouldBeTurningLeft");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleV.referenceID) + "," + std::to_string(vehicleViolations.ruleV.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleV.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.turnDirection.get_value_or(0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left turn intersection stop violation
              turningLeft += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          }
          //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
          else if (stopChecking && !vehicleViolations.ruleI.ruleStatus) {
            EXPECT_EQ(std::to_string(vehicleViolations.ruleI.referenceID), "ShouldStop");
            //record that a vehicle was detected engaging in a stopline intersection stop violation
            shouldStop += 1;
            //record that the vehicle has violated at least one rule
            nonViolator = false;
          } //END IF

          if (speedingChecking) {
            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsStrict).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsStrict))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
              
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              speedLimit += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsSoft).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsSoft))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsHard).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsHard))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } //END IF
          } //END IF

          if (offroadChecking) {
            //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
            if (vehicleViolations.offroadViolations.size() != 0) {
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              noOffroad += 1;
              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          } //END IF

          if (tailgatingChecking) {
            //contains the results of vehicle speeding detection            
            tailgatingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check for Violations Involving Tailgating between Vehicles
            if (vehicleViolations.tailgating.tailgatedVehicle.size() != 0) {
              //record that a vehicle was detected tailgating
              isTailgating += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;

              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgating.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgating.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results of the vehicle speeding
            americanVehiclePtr->outputDataToExcel(tailgatingViolations, tailgatingResults);
          } //END IF

          //record all rule violations for the given vehicle by pushing it into the vector of all rule violations
          allVehicleViolations.push_back(vehicleViolations);

          //record that a non violating vehicle was recorded if no rules were broken
          if (nonViolator) {
            //no rules were violated
            nonViolators += 1;
          } //END IF
        } //END FOR

        //counts the total number of vehicles ran through by the loop
        totalRuns += runs;

        //counts the total number of non-rule violating vehicles encountered
        totalNonViolators += nonViolators;

        //counts the number of rule violating vehicle encountered in the given interaction dataset
        totalShouldStop += shouldStop;
        totalLeftBeforeRight += leftBeforeRight;
        totalHasPriority += hasPriority;
        totalTurningLeft += turningLeft;
        totalTailgating += isTailgating;
        totalSpeedLimit += speedLimit;
        totalNoOffroad += noOffroad;
        totalStops += stops;
        totalYieldsOne += yieldsOne;
        totalYieldsTwo += yieldsTwo;

        //store the results in a results statistics struct
        struct ruleViolationStatistics results = {runs, nonViolators, shouldStop, leftBeforeRight, hasPriority, turningLeft, isTailgating,
                                                  speedLimit, noOffroad, PATH, std::string(ent->d_name), stops, yieldsOne, yieldsTwo};

        //print the results struct to a CSV file
        americanVehiclePtr->outputToResultsLog(results);

        //append the vector and the interaction dataset path to the list of all rule violations
        allViolationsAllDatasets.push_back(std::make_pair(allVehicleViolations, path));

        //clear the given vector to get ready for the next interaction dataset
        allVehicleViolations.clear();
      } //END WHILE
    } //END IF

    //store the results in a results statistics struct
    struct ruleViolationStatistics results = {totalRuns, totalNonViolators, totalShouldStop, totalLeftBeforeRight, totalHasPriority, totalTurningLeft,
                                              totalTailgating, totalSpeedLimit, totalNoOffroad, PATH, std::string("All Files"), totalStops, totalYieldsOne, totalYieldsTwo};

    //print the results struct to a CSV file
    if (fourWayActive) americanVehiclePtr->outputToResultsLog(results);
  } //END TEST_F(AmericanTrafficRulesVehicleTest, rulesFourWay)
} //END namespace vehicleData

/*
The following test script runs through the interaction datasets in the specified folder and checks for any rule violations,
as they were specified in the paper Formalization of Intersection Traffic Rules in Temporal Logic by Sebastian Maierhofer, Paul Moosbrugger, and Matthias Althoff.
  A link to the paper can be found here: https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
Rule violations can be visually inspected by checking the terminal output and looking for any test failures:
  The terminal will reference a failure message for the current CSV file, and continue to show the failure for the current test.
  As an example, we see the following (Note that "3" can represent any vehicle ID and "LeftBeforeRight" can represent any rule violation)
    The terminal will say it expected the quality of these values:
      std::to_string(vehicleData[vehicleID][0])
        Which is: "3"
        "LeftBeforeRight"]]></failure>
    Where "3" is the vehicle ID violating the rule, and "LeftBeforeRight" is the rule being violated
We can visualize the interaction dataset in question and manually inspect offending vehicle by using the following tool from GitHub: https://github.com/interaction-dataset/interaction-dataset
  Firstly, opening the terminal and moving the working directory to the interaction-dataset/python folder is required
  Secondly, run: ./main_visualize_data.py .TestScenarioForScripts 
    Where .TestScenarioForScripts (under interaction-dataset/recorded_trackfiles/.TestScenarioForScripts/) contains the correct interaction-CSV file, and is renamed to vehicle_tracks_000.csv
    And (interaction-dataset/maps/) contains the correct OSM map (the one corresponding to the interaction-CSV being analyzed) renamed to .TestScenarioForScripts.osm 
*/
namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, rulesFourWayDiamond) {
    //container for the directories being checked
    DIR * dir;
    struct dirent * ent;

    //container to determine if a vehicle is tailgating another
    struct tailgatingInformation tailgating;

    //create a struct object to contain all violations for a given vehicle
    struct violations vehicleViolations;

    //create a vector to contain all violations for all vehicles in an interaction dataset
    std::vector<struct violations> allVehicleViolations;

    //create a vector to contain all violations for all vehicles across interaction datasets
    std::vector<std::pair<std::vector<struct violations>, std::string>> allViolationsAllDatasets;

    //container for the number of people who did not violate any rule
    bool nonViolator;

    //contains the number of people not to violate any traffic rules
    int nonViolators = 0;
    int totalNonViolators = 0;

    //contains the statistics for all interaction datasets within a given test
    int totalRuns = 0;
    int totalShouldStop = 0;
    int totalLeftBeforeRight = 0;
    int totalHasPriority = 0;
    int totalTurningLeft = 0;
    int totalTailgating = 0;
    int totalSpeedLimit = 0;
    int totalNoOffroad = 0;
    int totalStops = 0;
    int totalYieldsOne = 0;
    int totalYieldsTwo = 0;

    //contains statistics for specific interaction datasets within a given test
    int runs = 0;
    int shouldStop = 0;
    int leftBeforeRight = 0;
    int hasPriority = 0;
    int turningLeft = 0;
    int isTailgating = 0;
    int speedLimit = 0;
    int noOffroad = 0;
    int stops = 0;
    int yieldsOne = 0;
    int yieldsTwo = 0;

    //used for exiting early
    int iteration = 0;
    int COUNTER = 0;

    //define the working map path
    std::string workingMapPath = mapPathDiamond;

    //define the working data path
    std::string workingDataPath = dataPathDiamondDir;

    //stores the path to the output file
    std::string PATH = "./Output/resultsLogRulesFourWayDiamond.csv";

    //stores the path to the output file and the new header for the new file
    const char HEADER[] = "File,Total Vehicles,Non-Rule-Violating Vehicles,Stop-Violators,Stops Needed,Left Before Right Violators,Lanelet Priority Violators,Should Turn Left Violators,Yields One Needed,Yields Two Needed,Vehicle Tailgating Violations,Speed Limit,No Offroad\n";

    //clear the output file
    if (fourWayDiamondActive) americanVehiclePtr->clearOutput(PATH, HEADER);

    //contains the speed limit epsilon value in terms of meters per second
    double speedingEpsilonStrict = 0;
    double speedingEpsilonSoft = 1.38889;
    double speedingEpsilonHard = 2.77778;
    
    //contains the results of the vehicle speed limit violation
    std::string vehicleSpeedingResults;

    //format header for the speeding results
    const char SPEEDING_HEADER[] = "FILE,ID,TRIP DURATION,SPEEDING DURATION,MOTION DURATION,TIME BASED PROPORTION OF SPEEDING,SPEED LIMIT,EPSILON";;

    //containers for the file paths to the speeding results spreadsheets
    std::string filePathSpeedingStrict = "./Output/SpeedingResultsStrict-FourWayDiamond.csv";
    std::string filePathSpeedingSoft = "./Output/SpeedingResultsSoft-FourWayDiamond.csv";
    std::string filePathSpeedingHard = "./Output/SpeedingResultsHard-FourWayDiamond.csv";

    //reset the output file for the speeding results
    if (fourWayDiamondActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingStrict, SPEEDING_HEADER);
    if (fourWayDiamondActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingSoft, SPEEDING_HEADER);
    if (fourWayDiamondActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingHard, SPEEDING_HEADER);

    //container for the output line for the vehicle stop analysis
    std::string stopResults;
    //format header for the stop violation results
    const char STOP_HEADER[] = "FILE,ID,Standard,Standard-Velocity,High Velocity,High-Velocity,Low Velocity,Low-Velocity,High Distance,Long-Velocity,Low Distance,Short-Velocity";
    //reset the output file for the stopping results
    if (fourWayDiamondActive && stopChecking) americanVehiclePtr->resetOutputDataToExcel("./Output/StoppingResultsStandard-FourWayDiamond.csv", STOP_HEADER);

    //contains the file path for the output log
    std::string filePathFull = "./Output/noOffroadFull-FourWayDiamond.csv";

    //contains the header for the output file
    const char OFFROAD_HEADER[] = "FILE,ID,TRIP DURATION,"
                                  "OFFROAD DURATION (FULL),TIME BASED PROPORTION OF OFFROAD (FULL),"
                                  "OFFROAD DURATION (FRAME),TIME BASED PROPORTION OF OFFROAD (FRAME),"
                                  "OFFROAD DURATION (CHASSIS),TIME BASED PROPORTION OF OFFROAD (CHASSIS),"
                                  "OFFROAD DURATION (HALF),TIME BASED PROPORTION OF OFFROAD(HALF),"
                                  "OFFROAD DURATION (CENTER),TIME BASED PROPORTION OF OFFROAD (CENTER)";

    //clears the output file
    if (fourWayDiamondActive && offroadChecking) americanVehiclePtr->resetOutputDataToExcel(filePathFull, OFFROAD_HEADER);

    //contains the collision results for the given vehicle
    std::vector<struct violation> resultsFull, resultsFrame, resultsChassis, resultsHalf, resultsCenter;

    //container for the results string for off-road violation detection
    std::string noOffroadResults;

    //contains the file path for the turning violation results
    std::string turnViolations = "./Output/turnViolationResults-FourWayDiamond.csv";

    //contains the header for the output file
    const char TURN_VIOLATION_HEADER[] = "FILE,Offender ID,X-Position,Y-Position,Timestamp,Stop Duration,Length,Width,Yaw,Direction,Trip Duration,Proportion";

    //clears the output file
    if (fourWayDiamondActive && turnChecking) americanVehiclePtr->resetOutputDataToExcel(turnViolations, TURN_VIOLATION_HEADER);

    //container for the results string for turn violation detection
    std::string turnViolationResults = "";

    //container for the tailgating results
    std::string tailgatingResults;

    //container for the spreadsheet for tailgating violations
    std::string tailgatingViolations = "./Output/tailgatingViolations-FourWayDiamond.csv";

    //contains the header for the output file
    const char TAILGATING_VIOLATION_HEADER[] = "FILE,ID,TRIP DURATION,"
                                               "TAILGATING DURATION (Default),TIME BASED PROPORTION OF TAILGATING (Default),"
                                               "TAILGATING DURATION (HighResponse),TIME BASED PROPORTION OF TAILGATING (HighResponse),"
                                               "TAILGATING DURATION (HighBreak),TIME BASED PROPORTION OF TAILGATING (HighBreak),"
                                               "TAILGATING DURATION (HighAccel),TIME BASED PROPORTION OF TAILGATING (HighAccel),"
                                               "TAILGATING DURATION (HighMinAccel),TIME BASED PROPORTION OF TAILGATING (HighMinAccel),"
                                               "TAILGATING DURATION (LowResponse),TIME BASED PROPORTION OF TAILGATING (LowResponse),"
                                               "TAILGATING DURATION (LowBreak),TIME BASED PROPORTION OF TAILGATING (LowBreak),"
                                               "TAILGATING DURATION (LowAccel),TIME BASED PROPORTION OF TAILGATING (LowAccel),"
                                               "TAILGATING DURATION (LowMinAccel),TIME BASED PROPORTION OF TAILGATING (LowMinAccel)";

    //reset the output file for tailgating violations
    if (fourWayDiamondActive && tailgatingChecking) americanVehiclePtr->resetOutputDataToExcel(tailgatingViolations, TAILGATING_VIOLATION_HEADER);

    //if the Given Directory Path can be Opened by the File System
    if ((dir = opendir(workingDataPath.c_str())) != NULL) {
      //for Every File in the Given Directory Path
      while ((ent = readdir (dir)) != NULL) {
        if (!fourWayDiamondActive) break;
        //formats the directory path into a string
        std::string path = workingDataPath + "/" + std::string(ent->d_name);

        //filters out file names too short to be CSV's like "." and ".."
        if (strlen(ent->d_name) < 4) {
          //move to the next iteration of the loop
          continue;
        }
        else if (iteration != COUNTER) {
          iteration += 1;
          continue;
        } //END IF

        //get and store the formatted vehicle data from the interaction dataset
        std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(path);

        //contains the origin point for the open street map file
        lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

        //output the file name
        EXPECT_EQ("file: ", path);

        //contains statistics for the vehicles being scanned
        runs = 0;
        nonViolators = 0;
        shouldStop = 0;
        leftBeforeRight = 0;
        hasPriority = 0;
        turningLeft = 0;
        isTailgating = 0;
        speedLimit = 0;
        noOffroad = 0;
        stops = 0;
        yieldsOne = 0;
        yieldsTwo = 0;

        //tests every vehicle, and reports those that fail with their ID
        for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
          //re-build a violations struct for the next vehicle encountered
          vehicleViolations = violations();

          //count the iteration through the FOR loop as a run
          runs += 1;

          //signals if the given vehicle NOT has violated any rules, and is true by default
          nonViolator = true;

          //determines if stop checking is active or not
          if (stopChecking) {
            //check if the given vehicle failed to stop at a stop regulated intersection
            vehicleViolations.ruleI = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                          runtimeCoefficients.stop.distanceThreshold + 2,
                                                                          runtimeCoefficients.stop.velocityThreshold,
                                                                          lanelet::traffic_rules::America, StopDirection::SingleStop,
                                                                          0, {30082});
                                                                          //END checkStopAtSign
            vehicleViolations.ruleIStrictVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictVelocity.distanceThreshold,
                                                                                        stopStrictVelocity.velocityThreshold,
                                                                                        lanelet::traffic_rules::America, StopDirection::SingleStop);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardVelocity.distanceThreshold,
                                                                                      stopHardVelocity.velocityThreshold,
                                                                                      lanelet::traffic_rules::America, StopDirection::SingleStop);
                                                                                      //END checkStopAtSign
            vehicleViolations.ruleIStrictDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictDistance.distanceThreshold,
                                                                                        stopStrictDistance.velocityThreshold,
                                                                                        lanelet::traffic_rules::America, StopDirection::SingleStop);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardDistance.distanceThreshold,
                                                                                      stopHardDistance.velocityThreshold,
                                                                                      lanelet::traffic_rules::America, StopDirection::SingleStop);
                                                                                      //END checkStopAtSign

            //records the stop results to the given string
            stopResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + ",";

            //check if the vehicle needs to stop
            stops += int(vehicleViolations.ruleI.neededToStop.get_value_or(0));

            //record the results of the stop sign checking
            if (vehicleViolations.ruleI.ruleStatus && vehicleViolations.ruleI.endTimestamp && vehicleViolations.ruleI.timestamp) {
              //adds the stop results to the current output string
              stopResults += (std::to_string((vehicleViolations.ruleI.endTimestamp.get_value_or(0) - vehicleViolations.ruleI.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0))) + "," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictVelocity.ruleStatus && vehicleViolations.ruleIStrictVelocity.endTimestamp && vehicleViolations.ruleIStrictVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardVelocity.ruleStatus && vehicleViolations.ruleIHardVelocity.endTimestamp && vehicleViolations.ruleIHardVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictDistance.ruleStatus && vehicleViolations.ruleIStrictDistance.endTimestamp && vehicleViolations.ruleIStrictDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictDistance.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardDistance.ruleStatus && vehicleViolations.ruleIHardDistance.endTimestamp && vehicleViolations.ruleIHardDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardDistance.magnitude.get_value_or(-1)) + ",";
            } //END IF

            //output the results of the stop sign detection to the output file
            americanVehiclePtr->outputDataToExcel("./Output/StoppingResultsStandard-FourWayDiamond.csv", stopResults);
          } //END IF

          if (tailgatingChecking) {
            //check if the given vehicle is tailgating other vehicles along the intersection
            vehicleViolations.tailgating = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                                    runtimeCoefficients.tailgating.velocityThreshold,
                                                                                    runtimeCoefficients.tailgating.distanceThreshold,
                                                                                    runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                                    //END isTailgatingAtMoment

            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
          } //END IF

          if (speedingChecking) {
            //check if the vehicle is violating the lanelets speed limit
            vehicleViolations.speedLimitViolationsStrict = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonStrict, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsSoft = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonSoft, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsHard = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonHard, lanelet::traffic_rules::America, 2.2352);
          } //END IF

          if (offroadChecking) {
            //check if the vehicle is violating the lanelets off-road boundaries
            vehicleViolations.offroadViolations = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveHalf);
            vehicleViolations.offroadViolationsFull = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Full);
            vehicleViolations.offroadViolationsFrame = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveFrame);
            vehicleViolations.offroadViolationsChassis = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveChassis);
            vehicleViolations.offroadViolationsCenter = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Centre);

            //contains the results of vehicle speeding detection            
            noOffroadResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                              std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check if the results from the above tests are not empty
            if (!vehicleViolations.offroadViolationsFull.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFull).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFull).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsFrame.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFrame).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFrame).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsChassis.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsChassis).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsChassis).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolations.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolations).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolations).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsCenter.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsCenter).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsCenter).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results to the csv file
            americanVehiclePtr->outputDataToExcel(filePathFull, noOffroadResults);
          } //END IF

          /*
            If the Given Vehicle Stopped at the Intersections Stop-Line Given an AllWayStop or a Stop Sign Regulated Intersection,
            We can Check if the Vehicle Performed the Correct Intersectional Right-Of-Way Transfers Given Multiple Vehicles Stopped
            at the Same Time or Another Vehicle Stopped First and has the Intersectional Right-Of-Way.
            Essentially, Checks if the Reference Vehicle Passes the Intersection with the Right-Of-Way After Stopping:
              -> If multiple Vehicles Stop at Once, the Reference Vehicle Starts Once Given the Right-Of-Way
              -> If Another Vehicle Stops just Before the Reference Vehicle, the Reference Vehicle Waits to be Given the Right-Of-Way
          */
          if (stopChecking && turnChecking && vehicleViolations.ruleI.ruleStatus) {
            //check if Rules III, IV, or V were violated by the Currently Iterated Vehicle
            vehicleViolations.ruleIII = americanVehiclePtr->checkLeftBeforeRight(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                 runtimeCoefficients.leftBeforeRight.distanceThreshold,
                                                                                 runtimeCoefficients.leftBeforeRight.velocityThreshold,
                                                                                 lanelet::traffic_rules::America);
                                                                                 //END checkLeftBeforeRight

            vehicleViolations.ruleV = americanVehiclePtr->checkShouldBeTurningLeft(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                   runtimeCoefficients.leftTurnPriority.distanceThreshold,
                                                                                   runtimeCoefficients.leftTurnPriority.velocityThreshold,
                                                                                   lanelet::traffic_rules::America);
                                                                                   //END checkShouldBeTurningLeft

            yieldsOne += int(vehicleViolations.ruleIII.neededToStop.get_value_or(0));
            yieldsTwo += int(vehicleViolations.ruleV.neededToStop.get_value_or(0));

            //check for Violations Involving the Right-before-Left Rule (A Vehicle went LeftBeforeRight)
            if (!vehicleViolations.ruleIII.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleIII.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleIII.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)),
                        "LeftBeforeRight");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleIII.referenceID) + "," + std::to_string(vehicleViolations.ruleIII.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleIII.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.turnDirection.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";
              turnViolationResults += std::to_string(((double) vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left-right turn priority intersection stop violation
              leftBeforeRight += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF

            //check for Violations Involving the Turn Left Rule
            if (!vehicleViolations.ruleV.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleV.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleV.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)),
                        "ShouldBeTurningLeft");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleV.referenceID) + "," + std::to_string(vehicleViolations.ruleV.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleV.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.turnDirection.get_value_or(0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left turn intersection stop violation
              turningLeft += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          }
          //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
          else if (stopChecking && !vehicleViolations.ruleI.ruleStatus) {
            //record that a vehicle was detected engaging in a stopline intersection stop violation
            shouldStop += 1;
            //record that the vehicle has violated at least one rule
            nonViolator = false;
          } //END IF

          if (speedingChecking) {
            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsStrict).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsStrict))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
              
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              speedLimit += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsSoft).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsSoft))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsHard).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsHard))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } //END IF
          } //END IF

          if (offroadChecking) {
            //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
            if (vehicleViolations.offroadViolations.size() != 0) {
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              noOffroad += 1;
              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          } //END IF

          if (tailgatingChecking) {
            //contains the results of vehicle speeding detection            
            tailgatingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check for Violations Involving Tailgating between Vehicles
            if (vehicleViolations.tailgating.tailgatedVehicle.size() != 0) {
              //record that a vehicle was detected tailgating
              isTailgating += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;

              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgating.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgating.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results of the vehicle speeding
            americanVehiclePtr->outputDataToExcel(tailgatingViolations, tailgatingResults);
          } //END IF

          //record all rule violations for the given vehicle by pushing it into the vector of all rule violations
          allVehicleViolations.push_back(vehicleViolations);

          //record that a non violating vehicle was recorded if no rules were broken
          if (nonViolator) {
            //no rules were violated
            nonViolators += 1;
          } //END IF
        } //END FOR

        //counts the total number of vehicles ran through by the loop
        totalRuns += runs;

        //counts the total number of non-rule violating vehicles encountered
        totalNonViolators += nonViolators;

        //counts the number of rule violating vehicle encountered in the given interaction dataset
        totalShouldStop += shouldStop;
        totalLeftBeforeRight += leftBeforeRight;
        totalHasPriority += hasPriority;
        totalTurningLeft += turningLeft;
        totalTailgating += isTailgating;
        totalSpeedLimit += speedLimit;
        totalNoOffroad += noOffroad;
        totalStops += stops;
        totalYieldsOne += yieldsOne;
        totalYieldsTwo += yieldsTwo;

        //store the results in a results statistics struct
        struct ruleViolationStatistics results = {runs, nonViolators, shouldStop, leftBeforeRight, hasPriority, turningLeft,
                                                   isTailgating, speedLimit, noOffroad, PATH, std::string(ent->d_name), stops, yieldsOne, yieldsTwo};

        //print the results struct to a CSV file
        americanVehiclePtr->outputToResultsLog(results);

        //append the vector and the interaction dataset path to the list of all rule violations
        allViolationsAllDatasets.push_back(std::make_pair(allVehicleViolations, path));

        //clear the given vector to get ready for the next interaction dataset
        allVehicleViolations.clear();
      } //END WHILE
    } //END IF

    //store the results in a results statistics struct
    struct ruleViolationStatistics results = {totalRuns, totalNonViolators, totalShouldStop, totalLeftBeforeRight, totalHasPriority, totalTurningLeft,
                                              totalTailgating, totalSpeedLimit, totalNoOffroad, PATH, std::string("All Files"), totalStops, totalYieldsOne, totalYieldsTwo};

    //print the results struct to a CSV file
    if (fourWayDiamondActive) americanVehiclePtr->outputToResultsLog(results);
  } //END TEST_F(AmericanTrafficRulesVehicleTest, rulesFourWayDiamond)
} //END namespace vehicleData

/*
The following test script runs through the interaction datasets in the specified folder and checks for any rule violations,
as they were specified in the paper Formalization of Intersection Traffic Rules in Temporal Logic by Sebastian Maierhofer, Paul Moosbrugger, and Matthias Althoff.
  A link to the paper can be found here: https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
Rule violations can be visually inspected by checking the terminal output and looking for any test failures:
  The terminal will reference a failure message for the current CSV file, and continue to show the failure for the current test.
  As an example, we see the following (Note that "3" can represent any vehicle ID and "LeftBeforeRight" can represent any rule violation)
    The terminal will say it expected the quality of these values:
      std::to_string(vehicleData[vehicleID][0])
        Which is: "3"
        "LeftBeforeRight"]]></failure>
    Where "3" is the vehicle ID violating the rule, and "LeftBeforeRight" is the rule being violated
We can visualize the interaction dataset in question and manually inspect offending vehicle by using the following tool from GitHub: https://github.com/interaction-dataset/interaction-dataset
  Firstly, opening the terminal and moving the working directory to the interaction-dataset/python folder is required
  Secondly, run: ./main_visualize_data.py .TestScenarioForScripts 
    Where .TestScenarioForScripts (under interaction-dataset/recorded_trackfiles/.TestScenarioForScripts/) contains the correct interaction-CSV file, and is renamed to vehicle_tracks_000.csv
    And (interaction-dataset/maps/) contains the correct OSM map (the one corresponding to the interaction-CSV being analyzed) renamed to .TestScenarioForScripts.osm 
*/
namespace vehicleData {
  TEST_F(GermanTrafficRulesVehicleTest, roundaboutThreeWay) {
    //container for the directories being checked
    DIR * dir;
    struct dirent * ent;

    //container to determine if a vehicle is tailgating another
    struct tailgatingInformation tailgating;

    //create a struct object to contain all violations for a given vehicle
    struct violations vehicleViolations;

    //create a vector to contain all violations for all vehicles in an interaction dataset
    std::vector<struct violations> allVehicleViolations;

    //create a vector to contain all violations for all vehicles across interaction datasets
    std::vector<std::pair<std::vector<struct violations>, std::string>> allViolationsAllDatasets;

    //container for the number of people who did not violate any rule
    bool nonViolator;

    //contains the number of people not to violate any traffic rules
    int nonViolators = 0;
    int totalNonViolators = 0;

    //contains the statistics for all interaction datasets within a given test
    int totalRuns = 0;
    int totalShouldStop = 0;
    int totalLeftBeforeRight = 0;
    int totalHasPriority = 0;
    int totalTurningLeft = 0;
    int totalTailgating = 0;
    int totalSpeedLimit = 0;
    int totalNoOffroad = 0;
    int totalStops = 0;
    int totalYieldsOne = 0;
    int totalYieldsTwo = 0;

    //contains statistics for specific interaction datasets within a given test
    int runs = 0;
    int shouldStop = 0;
    int leftBeforeRight = 0;
    int hasPriority = 0;
    int turningLeft = 0;
    int isTailgating = 0;
    int speedLimit = 0;
    int noOffroad = 0;
    int stops = 0;
    int yieldsOne = 0;
    int yieldsTwo = 0;

    //used for exiting early
    int iteration = 0;
    int COUNTER = 0;

    //define the working map path
    std::string workingMapPath = mapPathDEURoundaboutOF;

    //define the working data path
    std::string workingDataPath = dataPathDEURoundaboutOFDir;

    //stores the path to the output file
    std::string PATH = "./Output/resultsLogRoundaboutThreeWay.csv";

    //stores the path to the output file and the new header for the new file
    const char HEADER[] = "File,Total Vehicles,Non-Rule-Violating Vehicles,Stop-Violators,Stops Needed,Left Before Right Violators,Lanelet Priority Violators,Should Turn Left Violators,Yields One Needed,Yields Two Needed,Vehicle Tailgating Violations,Speed Limit,No Offroad\n";

    //clear the output file
    if (roundaboutThreeWayActive) germanVehiclePtr->clearOutput(PATH, HEADER);

    //contains the speed limit epsilon value in terms of meters per second
    double speedingEpsilonStrict = 0;
    double speedingEpsilonSoft = 1.38889;
    double speedingEpsilonHard = 2.77778;
    
    //contains the results of the vehicle speed limit violation
    std::string vehicleSpeedingResults;

    //format header for the speeding results
    const char SPEEDING_HEADER[] = "FILE,ID,TRIP DURATION,SPEEDING DURATION,MOTION DURATION,TIME BASED PROPORTION OF SPEEDING,SPEED LIMIT,EPSILON";;

    //containers for the file paths to the speeding results spreadsheets
    std::string filePathSpeedingStrict = "./Output/SpeedingResultsStrict-RoundaboutThreeWay.csv";
    std::string filePathSpeedingSoft = "./Output/SpeedingResultsSoft-RoundaboutThreeWay.csv";
    std::string filePathSpeedingHard = "./Output/SpeedingResultsHard-RoundaboutThreeWay.csv";

    //reset the output file for the speeding results
    if (roundaboutThreeWayActive && speedingChecking) germanVehiclePtr->resetOutputDataToExcel(filePathSpeedingStrict, SPEEDING_HEADER);
    if (roundaboutThreeWayActive && speedingChecking) germanVehiclePtr->resetOutputDataToExcel(filePathSpeedingSoft, SPEEDING_HEADER);
    if (roundaboutThreeWayActive && speedingChecking) germanVehiclePtr->resetOutputDataToExcel(filePathSpeedingHard, SPEEDING_HEADER);

    //container for the output line for the vehicle stop analysis
    std::string stopResults;
    //format header for the stop violation results
    const char STOP_HEADER[] = "FILE,ID,Standard,Standard-Velocity,High Velocity,High-Velocity,Low Velocity,Low-Velocity,High Distance,Long-Velocity,Low Distance,Short-Velocity";
    //reset the output file for the stopping results
    if (roundaboutThreeWayActive && stopChecking) germanVehiclePtr->resetOutputDataToExcel("./Output/StoppingResultsStandard-RoundaboutThreeWay.csv", STOP_HEADER);

    //contains the file path for the output log
    std::string filePathFull = "./Output/noOffroadFull-RoundaboutThreeWay.csv";

    //contains the header for the output file
    const char OFFROAD_HEADER[] = "FILE,ID,TRIP DURATION,"
                                  "OFFROAD DURATION (FULL),TIME BASED PROPORTION OF OFFROAD (FULL),"
                                  "OFFROAD DURATION (FRAME),TIME BASED PROPORTION OF OFFROAD (FRAME),"
                                  "OFFROAD DURATION (CHASSIS),TIME BASED PROPORTION OF OFFROAD (CHASSIS),"
                                  "OFFROAD DURATION (HALF),TIME BASED PROPORTION OF OFFROAD(HALF),"
                                  "OFFROAD DURATION (CENTER),TIME BASED PROPORTION OF OFFROAD (CENTER)";

    //clears the output file
    if (roundaboutThreeWayActive && offroadChecking) germanVehiclePtr->resetOutputDataToExcel(filePathFull, OFFROAD_HEADER);

    //contains the collision results for the given vehicle
    std::vector<struct violation> resultsFull, resultsFrame, resultsChassis, resultsHalf, resultsCenter;

    //container for the results string for off-road violation detection
    std::string noOffroadResults;

    //contains the file path for the turning violation results
    std::string turnViolations = "./Output/turnViolationResults-RoundaboutThreeWay.csv";

    //contains the header for the output file
    const char TURN_VIOLATION_HEADER[] = "FILE,Offender ID,X-Position,Y-Position,Timestamp,Stop Duration,Length,Width,Yaw,Direction,Trip Duration,Proportion";

    //clears the output file
    if (roundaboutThreeWayActive && turnChecking) germanVehiclePtr->resetOutputDataToExcel(turnViolations, TURN_VIOLATION_HEADER);

    //container for the results string for turn violation detection
    std::string turnViolationResults = "";

    //container for the tailgating results
    std::string tailgatingResults;

    //container for the spreadsheet for tailgating violations
    std::string tailgatingViolations = "./Output/tailgatingViolations-RoundaboutThreeWay.csv";

    //contains the header for the output file
    const char TAILGATING_VIOLATION_HEADER[] = "FILE,ID,TRIP DURATION,"
                                               "TAILGATING DURATION (Default),TIME BASED PROPORTION OF TAILGATING (Default),"
                                               "TAILGATING DURATION (HighResponse),TIME BASED PROPORTION OF TAILGATING (HighResponse),"
                                               "TAILGATING DURATION (HighBreak),TIME BASED PROPORTION OF TAILGATING (HighBreak),"
                                               "TAILGATING DURATION (HighAccel),TIME BASED PROPORTION OF TAILGATING (HighAccel),"
                                               "TAILGATING DURATION (HighMinAccel),TIME BASED PROPORTION OF TAILGATING (HighMinAccel),"
                                               "TAILGATING DURATION (LowResponse),TIME BASED PROPORTION OF TAILGATING (LowResponse),"
                                               "TAILGATING DURATION (LowBreak),TIME BASED PROPORTION OF TAILGATING (LowBreak),"
                                               "TAILGATING DURATION (LowAccel),TIME BASED PROPORTION OF TAILGATING (LowAccel),"
                                               "TAILGATING DURATION (LowMinAccel),TIME BASED PROPORTION OF TAILGATING (LowMinAccel)";

    //reset the output file for tailgating violations
    if (roundaboutThreeWayActive && tailgatingChecking) germanVehiclePtr->resetOutputDataToExcel(tailgatingViolations, TAILGATING_VIOLATION_HEADER);

    //if the Given Directory Path can be Opened by the File System
    if ((dir = opendir(workingDataPath.c_str())) != NULL) {
      //for Every File in the Given Directory Path
      while ((ent = readdir (dir)) != NULL) {
        if (!roundaboutThreeWayActive) break;
        //formats the directory path into a string
        std::string path = workingDataPath + "/" + std::string(ent->d_name);

        //filters out file names too short to be CSV's like "." and ".."
        if (strlen(ent->d_name) < 4) {
          //move to the next iteration of the loop
          continue;
        }
        else if (iteration != COUNTER) {
          iteration += 1;
          continue;
        } //END IF

        //get and store the formatted vehicle data from the interaction dataset
        std::vector<struct vehicles> vehicleData = germanVehiclePtr->getVehicleData(path);

        //contains the origin point for the open street map file
        lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

        //output the file name
        EXPECT_EQ("file: ", path);

        //contains values for lanelet priority overrides
        std::vector<std::pair<int, int>> pOverride;

        //contains statistics for the vehicles being scanned
        runs = 0;
        nonViolators = 0;
        shouldStop = 0;
        leftBeforeRight = 0;
        hasPriority = 0;
        turningLeft = 0;
        isTailgating = 0;
        speedLimit = 0;
        noOffroad = 0;
        stops = 0;
        yieldsOne = 0;
        yieldsTwo = 0;

        //tests every vehicle, and reports those that fail with their ID
        for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
          //re-build a violations struct for the next vehicle encountered
          vehicleViolations = violations();

          //count the iteration through the FOR loop as a run
          runs += 1;

          //signals if the given vehicle NOT has violated any rules, and is true by default
          nonViolator = true;

          //determines if stop checking is active or not
          if (stopChecking) {
            //check if the given vehicle failed to stop at a stop regulated intersection
            vehicleViolations.ruleI = germanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                          runtimeCoefficients.stop.distanceThreshold,
                                                                          runtimeCoefficients.stop.velocityThreshold,
                                                                          lanelet::traffic_rules::Germany);
                                                                          //END checkStopAtSign
            vehicleViolations.ruleIStrictVelocity = germanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictVelocity.distanceThreshold,
                                                                                        stopStrictVelocity.velocityThreshold,
                                                                                        lanelet::traffic_rules::Germany);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardVelocity = germanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardVelocity.distanceThreshold,
                                                                                      stopHardVelocity.velocityThreshold,
                                                                                      lanelet::traffic_rules::Germany);
                                                                                      //END checkStopAtSign
            vehicleViolations.ruleIStrictDistance = germanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictDistance.distanceThreshold,
                                                                                        stopStrictDistance.velocityThreshold,
                                                                                        lanelet::traffic_rules::Germany);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardDistance = germanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardDistance.distanceThreshold,
                                                                                      stopHardDistance.velocityThreshold,
                                                                                      lanelet::traffic_rules::Germany);

            //records the stop results to the given string
            stopResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + ",";

            //check if the vehicle needs to stop
            stops += int(vehicleViolations.ruleI.neededToStop.get_value_or(0));

            //record the results of the stop sign checking
            if (vehicleViolations.ruleI.ruleStatus && vehicleViolations.ruleI.endTimestamp && vehicleViolations.ruleI.timestamp) {
              //adds the stop results to the current output string
              stopResults += (std::to_string((vehicleViolations.ruleI.endTimestamp.get_value_or(0) - vehicleViolations.ruleI.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0))) + "," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictVelocity.ruleStatus && vehicleViolations.ruleIStrictVelocity.endTimestamp && vehicleViolations.ruleIStrictVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardVelocity.ruleStatus && vehicleViolations.ruleIHardVelocity.endTimestamp && vehicleViolations.ruleIHardVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictDistance.ruleStatus && vehicleViolations.ruleIStrictDistance.endTimestamp && vehicleViolations.ruleIStrictDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictDistance.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardDistance.ruleStatus && vehicleViolations.ruleIHardDistance.endTimestamp && vehicleViolations.ruleIHardDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardDistance.magnitude.get_value_or(-1)) + ",";
            } //END IF

            //output the results of the stop sign detection to the output file
            germanVehiclePtr->outputDataToExcel("./Output/StoppingResultsStandard-RoundaboutThreeWay.csv", stopResults);
          } //END IF

          if (tailgatingChecking) {
            //check if the given vehicle is tailgating other vehicles along the intersection
            vehicleViolations.tailgating = germanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                                    runtimeCoefficients.tailgating.velocityThreshold,
                                                                                    runtimeCoefficients.tailgating.distanceThreshold,
                                                                                    runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::Germany);
                                                                                    //END isTailgatingAtMoment

            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighResponse = germanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::Germany);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighBreak = germanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::Germany);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighAccel = germanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold, 
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::Germany);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighMinAccel = germanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0), DrivingOnly, lanelet::traffic_rules::Germany);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowResponse = germanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::Germany);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowBreak = germanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::Germany);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowAccel = germanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::Germany);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowMinAccel = germanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0), DrivingOnly, lanelet::traffic_rules::Germany);
                                                                  //END isTailgatingAtMoment
          } //END IF
          
          if (speedingChecking) {
            //check if the vehicle is violating the lanelets speed limit
            vehicleViolations.speedLimitViolationsStrict = germanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonStrict, lanelet::traffic_rules::Germany, 2.2352);
            vehicleViolations.speedLimitViolationsSoft = germanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonSoft, lanelet::traffic_rules::Germany, 2.2352);
            vehicleViolations.speedLimitViolationsHard = germanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonHard, lanelet::traffic_rules::Germany, 2.2352);
          } //END IF

          if (offroadChecking) {
            //check if the vehicle is violating the lanelets off-road boundaries
            vehicleViolations.offroadViolations = germanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveHalf);
            vehicleViolations.offroadViolationsFull = germanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Full);
            vehicleViolations.offroadViolationsFrame = germanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveFrame);
            vehicleViolations.offroadViolationsChassis = germanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveChassis);
            vehicleViolations.offroadViolationsCenter = germanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Centre);

            //contains the results of vehicle speeding detection            
            noOffroadResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                              std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check if the results from the above tests are not empty
            if (!vehicleViolations.offroadViolationsFull.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFull).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFull).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsFrame.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFrame).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFrame).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsChassis.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsChassis).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsChassis).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolations.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolations).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolations).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsCenter.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsCenter).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsCenter).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results to the csv file
            germanVehiclePtr->outputDataToExcel(filePathFull, noOffroadResults);
          } //END IF

          /*
            If the Given Vehicle Stopped at the Intersections Stop-Line Given an AllWayStop or a Stop Sign Regulated Intersection,
            We can Check if the Vehicle Performed the Correct Intersectional Right-Of-Way Transfers Given Multiple Vehicles Stopped
            at the Same Time or Another Vehicle Stopped First and has the Intersectional Right-Of-Way.
            Essentially, Checks if the Reference Vehicle Passes the Intersection with the Right-Of-Way After Stopping:
              -> If multiple Vehicles Stop at Once, the Reference Vehicle Starts Once Given the Right-Of-Way
              -> If Another Vehicle Stops just Before the Reference Vehicle, the Reference Vehicle Waits to be Given the Right-Of-Way
          */
          if (stopChecking && turnChecking && vehicleViolations.ruleI.ruleStatus) {
            //check if Rules III, IV, or V were violated by the Currently Iterated Vehicle
            vehicleViolations.ruleIII = germanVehiclePtr->checkLeftBeforeRight(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                 runtimeCoefficients.leftBeforeRight.distanceThreshold,
                                                                                 runtimeCoefficients.leftBeforeRight.velocityThreshold,
                                                                                 lanelet::traffic_rules::Germany);
                                                                                 //END checkLeftBeforeRight

            vehicleViolations.ruleIV = germanVehiclePtr->checkHasPriority(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                            runtimeCoefficients.laneletPriority.distanceThreshold,
                                                                            runtimeCoefficients.laneletPriority.velocityThreshold,
                                                                            lanelet::traffic_rules::Germany, pOverride);
                                                                            //END checkHasPriority

            vehicleViolations.ruleV = germanVehiclePtr->checkShouldBeTurningLeft(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                   runtimeCoefficients.leftTurnPriority.distanceThreshold,
                                                                                   runtimeCoefficients.leftTurnPriority.velocityThreshold,
                                                                                   lanelet::traffic_rules::Germany);
                                                                                   //END checkShouldBeTurningLeft

            yieldsOne += int(vehicleViolations.ruleIII.neededToStop.get_value_or(0));
            yieldsTwo += int(vehicleViolations.ruleV.neededToStop.get_value_or(0));

            //check for Violations Involving the Right-before-Left Rule (A Vehicle went LeftBeforeRight)
            if (!vehicleViolations.ruleIII.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleIII.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleIII.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)),
                        "LeftBeforeRight");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleIII.referenceID) + "," + std::to_string(vehicleViolations.ruleIII.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleIII.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.turnDirection.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";
              turnViolationResults += std::to_string(((double) vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0));

              //add the turn violation to the output spreadsheet
              germanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left-right turn priority intersection stop violation
              leftBeforeRight += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF

            //check for Violations Involving the Turn Left Rule
            if (!vehicleViolations.ruleV.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleV.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleV.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)),
                        "ShouldBeTurningLeft");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleV.referenceID) + "," + std::to_string(vehicleViolations.ruleV.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleV.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.turnDirection.get_value_or(0));

              //add the turn violation to the output spreadsheet
              germanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left turn intersection stop violation
              turningLeft += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          }
          //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
          else if (stopChecking && !vehicleViolations.ruleI.ruleStatus) {
            //record that a vehicle was detected engaging in a stopline intersection stop violation
            shouldStop += 1;
            //record that the vehicle has violated at least one rule
            nonViolator = false;
          } //END IF

          if (speedingChecking) {
            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsStrict).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsStrict))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              germanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
              
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              speedLimit += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              germanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsSoft).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsSoft))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              germanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              germanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsHard).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsHard))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              germanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              germanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } //END IF
          } //END IF

          if (offroadChecking) {
            //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
            if (vehicleViolations.offroadViolations.size() != 0) {
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              noOffroad += 1;
              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          } //END IF

          if (tailgatingChecking) {
            //contains the results of vehicle speeding detection            
            tailgatingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check for Violations Involving Tailgating between Vehicles
            if (vehicleViolations.tailgating.tailgatedVehicle.size() != 0) {
              //record that a vehicle was detected tailgating
              isTailgating += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;

              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgating.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgating.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results of the vehicle speeding
            germanVehiclePtr->outputDataToExcel(tailgatingViolations, tailgatingResults);
          } //END IF

          //record all rule violations for the given vehicle by pushing it into the vector of all rule violations
          allVehicleViolations.push_back(vehicleViolations);

          //record that a non violating vehicle was recorded if no rules were broken
          if (nonViolator) {
            //no rules were violated
            nonViolators += 1;
          } //END IF
        } //END FOR

        //counts the total number of vehicles ran through by the loop
        totalRuns += runs;

        //counts the total number of non-rule violating vehicles encountered
        totalNonViolators += nonViolators;

        //counts the number of rule violating vehicle encountered in the given interaction dataset
        totalShouldStop += shouldStop;
        totalLeftBeforeRight += leftBeforeRight;
        totalHasPriority += hasPriority;
        totalTurningLeft += turningLeft;
        totalTailgating += isTailgating;
        totalSpeedLimit += speedLimit;
        totalNoOffroad += noOffroad;
        totalStops += stops;
        totalYieldsOne += yieldsOne;
        totalYieldsTwo += yieldsTwo;

        //store the results in a results statistics struct
        struct ruleViolationStatistics results = {runs, nonViolators, shouldStop, leftBeforeRight, hasPriority, turningLeft, isTailgating,
                                                  speedLimit, noOffroad, PATH, std::string(ent->d_name), stops, yieldsOne, yieldsTwo};

        //print the results struct to a CSV file
        germanVehiclePtr->outputToResultsLog(results);

        //append the vector and the interaction dataset path to the list of all rule violations
        allViolationsAllDatasets.push_back(std::make_pair(allVehicleViolations, path));

        //clear the given vector to get ready for the next interaction dataset
        allVehicleViolations.clear();
      } //END WHILE
    } //END IF

    //store the results in a results statistics struct
    struct ruleViolationStatistics results = {totalRuns, totalNonViolators, totalShouldStop, totalLeftBeforeRight, totalHasPriority, totalTurningLeft,
                                              totalTailgating, totalSpeedLimit, totalNoOffroad, PATH, std::string("All Files"), totalStops, totalYieldsOne, totalYieldsTwo};

    //print the results struct to a CSV file
    if (roundaboutThreeWayActive) germanVehiclePtr->outputToResultsLog(results);
  } //END TEST_F(GermanTrafficRulesVehicleTest, roundaboutThreeWay)
} //END namespace vehicleData

/*
The following test script runs through the interaction datasets in the specified folder and checks for any rule violations,
as they were specified in the paper Formalization of Intersection Traffic Rules in Temporal Logic by Sebastian Maierhofer, Paul Moosbrugger, and Matthias Althoff.
  A link to the paper can be found here: https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
Rule violations can be visually inspected by checking the terminal output and looking for any test failures:
  The terminal will reference a failure message for the current CSV file, and continue to show the failure for the current test.
  As an example, we see the following (Note that "3" can represent any vehicle ID and "LeftBeforeRight" can represent any rule violation)
    The terminal will say it expected the quality of these values:
      std::to_string(vehicleData[vehicleID][0])
        Which is: "3"
        "LeftBeforeRight"]]></failure>
    Where "3" is the vehicle ID violating the rule, and "LeftBeforeRight" is the rule being violated
We can visualize the interaction dataset in question and manually inspect offending vehicle by using the following tool from GitHub: https://github.com/interaction-dataset/interaction-dataset
  Firstly, opening the terminal and moving the working directory to the interaction-dataset/python folder is required
  Secondly, run: ./main_visualize_data.py .TestScenarioForScripts 
    Where .TestScenarioForScripts (under interaction-dataset/recorded_trackfiles/.TestScenarioForScripts/) contains the correct interaction-CSV file, and is renamed to vehicle_tracks_000.csv
    And (interaction-dataset/maps/) contains the correct OSM map (the one corresponding to the interaction-CSV being analyzed) renamed to .TestScenarioForScripts.osm 
*/
namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, roundaboutSixWay) {
    //container for the directories being checked
    DIR * dir;
    struct dirent * ent;

    //container to determine if a vehicle is tailgating another
    struct tailgatingInformation tailgating;

    //create a struct object to contain all violations for a given vehicle
    struct violations vehicleViolations;

    //create a vector to contain all violations for all vehicles in an interaction dataset
    std::vector<struct violations> allVehicleViolations;

    //create a vector to contain all violations for all vehicles across interaction datasets
    std::vector<std::pair<std::vector<struct violations>, std::string>> allViolationsAllDatasets;

    //container for the number of people who did not violate any rule
    bool nonViolator;

    //contains the number of people not to violate any traffic rules
    int nonViolators = 0;
    int totalNonViolators = 0;

    //contains the statistics for all interaction datasets within a given test
    int totalRuns = 0;
    int totalShouldStop = 0;
    int totalLeftBeforeRight = 0;
    int totalHasPriority = 0;
    int totalTurningLeft = 0;
    int totalTailgating = 0;
    int totalSpeedLimit = 0;
    int totalNoOffroad = 0;
    int totalStops = 0;
    int totalYieldsOne = 0;
    int totalYieldsTwo = 0;

    //contains statistics for specific interaction datasets within a given test
    int runs = 0;
    int shouldStop = 0;
    int leftBeforeRight = 0;
    int hasPriority = 0;
    int turningLeft = 0;
    int isTailgating = 0;
    int speedLimit = 0;
    int noOffroad = 0;
    int stops = 0;
    int yieldsOne = 0;
    int yieldsTwo = 0;

    //used for exiting early
    int iteration = 0;
    int COUNTER = 0;

    //define the working map path
    std::string workingMapPath = mapPathUSARoundaboutFT;

    //define the working data path
    std::string workingDataPath = dataPathUSARoundaboutFTDir;

    //stores the path to the output file
    std::string PATH = "./Output/resultsLogRoundaboutSixWay.csv";

    //stores the path to the output file and the new header for the new file
    const char HEADER[] = "File,Total Vehicles,Non-Rule-Violating Vehicles,Stop-Violators,Stops Needed,Left Before Right Violators,Lanelet Priority Violators,Should Turn Left Violators,Yields One Needed,Yields Two Needed,Vehicle Tailgating Violations,Speed Limit,No Offroad\n";

    //clear the output file
    if (roundaboutSixWayActive) americanVehiclePtr->clearOutput(PATH, HEADER);

    //contains the speed limit epsilon value in terms of meters per second
    double speedingEpsilonStrict = 0;
    double speedingEpsilonSoft = 1.38889;
    double speedingEpsilonHard = 2.77778;
    
    //contains the results of the vehicle speed limit violation
    std::string vehicleSpeedingResults;

    //format header for the speeding results
    const char SPEEDING_HEADER[] = "FILE,ID,TRIP DURATION,SPEEDING DURATION,MOTION DURATION,TIME BASED PROPORTION OF SPEEDING,SPEED LIMIT,EPSILON";;

    //containers for the file paths to the speeding results spreadsheets
    std::string filePathSpeedingStrict = "./Output/SpeedingResultsStrict-RoundaboutSixWay.csv";
    std::string filePathSpeedingSoft = "./Output/SpeedingResultsSoft-RoundaboutSixWay.csv";
    std::string filePathSpeedingHard = "./Output/SpeedingResultsHard-RoundaboutSixWay.csv";

    //reset the output file for the speeding results
    if (roundaboutSixWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingStrict, SPEEDING_HEADER);
    if (roundaboutSixWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingSoft, SPEEDING_HEADER);
    if (roundaboutSixWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingHard, SPEEDING_HEADER);

    //container for the output line for the vehicle stop analysis
    std::string stopResults;
    //format header for the stop violation results
    const char STOP_HEADER[] = "FILE,ID,Standard,Standard-Velocity,High Velocity,High-Velocity,Low Velocity,Low-Velocity,High Distance,Long-Velocity,Low Distance,Short-Velocity";
    //reset the output file for the stopping results
    if (roundaboutSixWayActive && stopChecking) americanVehiclePtr->resetOutputDataToExcel("./Output/StoppingResultsStandard-RoundaboutSixWay.csv", STOP_HEADER);

    //contains the file path for the output log
    std::string filePathFull = "./Output/noOffroadFull-RoundaboutSixWay.csv";

    //contains the header for the output file
    const char OFFROAD_HEADER[] = "FILE,ID,TRIP DURATION,"
                                  "OFFROAD DURATION (FULL),TIME BASED PROPORTION OF OFFROAD (FULL),"
                                  "OFFROAD DURATION (FRAME),TIME BASED PROPORTION OF OFFROAD (FRAME),"
                                  "OFFROAD DURATION (CHASSIS),TIME BASED PROPORTION OF OFFROAD (CHASSIS),"
                                  "OFFROAD DURATION (HALF),TIME BASED PROPORTION OF OFFROAD(HALF),"
                                  "OFFROAD DURATION (CENTER),TIME BASED PROPORTION OF OFFROAD (CENTER)";

    //clears the output file
    if (roundaboutSixWayActive && offroadChecking) americanVehiclePtr->resetOutputDataToExcel(filePathFull, OFFROAD_HEADER);

    //contains the collision results for the given vehicle
    std::vector<struct violation> resultsFull, resultsFrame, resultsChassis, resultsHalf, resultsCenter;

    //container for the results string for off-road violation detection
    std::string noOffroadResults;

    //contains the file path for the turning violation results
    std::string turnViolations = "./Output/turnViolationResults-RoundaboutSixWay.csv";

    //contains the header for the output file
    const char TURN_VIOLATION_HEADER[] = "FILE,Offender ID,X-Position,Y-Position,Timestamp,Stop Duration,Length,Width,Yaw,Direction,Trip Duration,Proportion";

    //clears the output file
    if (roundaboutSixWayActive && turnChecking) americanVehiclePtr->resetOutputDataToExcel(turnViolations, TURN_VIOLATION_HEADER);

    //container for the results string for turn violation detection
    std::string turnViolationResults = "";

    //container for the tailgating results
    std::string tailgatingResults;

    //container for the spreadsheet for tailgating violations
    std::string tailgatingViolations = "./Output/tailgatingViolations-RoundaboutSixWay.csv";

    //contains the header for the output file
    const char TAILGATING_VIOLATION_HEADER[] = "FILE,ID,TRIP DURATION,"
                                               "TAILGATING DURATION (Default),TIME BASED PROPORTION OF TAILGATING (Default),"
                                               "TAILGATING DURATION (HighResponse),TIME BASED PROPORTION OF TAILGATING (HighResponse),"
                                               "TAILGATING DURATION (HighBreak),TIME BASED PROPORTION OF TAILGATING (HighBreak),"
                                               "TAILGATING DURATION (HighAccel),TIME BASED PROPORTION OF TAILGATING (HighAccel),"
                                               "TAILGATING DURATION (HighMinAccel),TIME BASED PROPORTION OF TAILGATING (HighMinAccel),"
                                               "TAILGATING DURATION (LowResponse),TIME BASED PROPORTION OF TAILGATING (LowResponse),"
                                               "TAILGATING DURATION (LowBreak),TIME BASED PROPORTION OF TAILGATING (LowBreak),"
                                               "TAILGATING DURATION (LowAccel),TIME BASED PROPORTION OF TAILGATING (LowAccel),"
                                               "TAILGATING DURATION (LowMinAccel),TIME BASED PROPORTION OF TAILGATING (LowMinAccel)";

    //reset the output file for tailgating violations
    if (roundaboutSixWayActive && tailgatingChecking) americanVehiclePtr->resetOutputDataToExcel(tailgatingViolations, TAILGATING_VIOLATION_HEADER);

    //if the Given Directory Path can be Opened by the File System
    if ((dir = opendir(workingDataPath.c_str())) != NULL) {
      //for Every File in the Given Directory Path
      while ((ent = readdir (dir)) != NULL) {
        if (!roundaboutSixWayActive) break;
        //formats the directory path into a string
        std::string path = workingDataPath + "/" + std::string(ent->d_name);

        //filters out file names too short to be CSV's like "." and ".."
        if (strlen(ent->d_name) < 4) {
          //move to the next iteration of the loop
          continue;
        }
        else if (iteration != COUNTER) {
          iteration += 1;
          continue;
        } //END IF

        //get and store the formatted vehicle data from the interaction dataset
        std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(path);

        //contains the origin point for the open street map file
        lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

        //output the file name
        EXPECT_EQ("file: ", path);

        //contains statistics for the vehicles being scanned
        runs = 0;
        nonViolators = 0;
        shouldStop = 0;
        leftBeforeRight = 0;
        hasPriority = 0;
        turningLeft = 0;
        isTailgating = 0;
        speedLimit = 0;
        noOffroad = 0;
        stops = 0;
        yieldsOne = 0;
        yieldsTwo = 0;

        //tests every vehicle, and reports those that fail with their ID
        for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
          //re-build a violations struct for the next vehicle encountered
          vehicleViolations = violations();

          //count the iteration through the FOR loop as a run
          runs += 1;

          //signals if the given vehicle NOT has violated any rules, and is true by default
          nonViolator = true;

          //determines if stop checking is active or not
          if (stopChecking) {
            //check if the given vehicle failed to stop at a stop regulated intersection
            vehicleViolations.ruleI = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                          runtimeCoefficients.stop.distanceThreshold,
                                                                          runtimeCoefficients.stop.velocityThreshold,
                                                                          lanelet::traffic_rules::America);
                                                                          //END checkStopAtSign
            vehicleViolations.ruleIStrictVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictVelocity.distanceThreshold,
                                                                                        stopStrictVelocity.velocityThreshold,
                                                                                        lanelet::traffic_rules::America);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardVelocity.distanceThreshold,
                                                                                      stopHardVelocity.velocityThreshold,
                                                                                      lanelet::traffic_rules::America);
                                                                                      //END checkStopAtSign
            vehicleViolations.ruleIStrictDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictDistance.distanceThreshold,
                                                                                        stopStrictDistance.velocityThreshold,
                                                                                        lanelet::traffic_rules::America);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardDistance.distanceThreshold,
                                                                                      stopHardDistance.velocityThreshold,
                                                                                      lanelet::traffic_rules::America);

            //records the stop results to the given string
            stopResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + ",";

            //check if the vehicle needs to stop
            stops += int(vehicleViolations.ruleI.neededToStop.get_value_or(0));

            //record the results of the stop sign checking
            if (vehicleViolations.ruleI.ruleStatus && vehicleViolations.ruleI.endTimestamp && vehicleViolations.ruleI.timestamp) {
              //adds the stop results to the current output string
              stopResults += (std::to_string((vehicleViolations.ruleI.endTimestamp.get_value_or(0) - vehicleViolations.ruleI.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0))) + "," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictVelocity.ruleStatus && vehicleViolations.ruleIStrictVelocity.endTimestamp && vehicleViolations.ruleIStrictVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardVelocity.ruleStatus && vehicleViolations.ruleIHardVelocity.endTimestamp && vehicleViolations.ruleIHardVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictDistance.ruleStatus && vehicleViolations.ruleIStrictDistance.endTimestamp && vehicleViolations.ruleIStrictDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictDistance.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardDistance.ruleStatus && vehicleViolations.ruleIHardDistance.endTimestamp && vehicleViolations.ruleIHardDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardDistance.magnitude.get_value_or(-1)) + ",";
            } //END IF

            //output the results of the stop sign detection to the output file
            americanVehiclePtr->outputDataToExcel("./Output/StoppingResultsStandard-RoundaboutSixWay.csv", stopResults);
          } //END IF

          if (tailgatingChecking) {
            //check if the given vehicle is tailgating other vehicles along the intersection
            vehicleViolations.tailgating = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                                    runtimeCoefficients.tailgating.velocityThreshold,
                                                                                    runtimeCoefficients.tailgating.distanceThreshold,
                                                                                    runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                                    //END isTailgatingAtMoment

            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
          } //END IF
          
          if (speedingChecking) {
            //check if the vehicle is violating the lanelets speed limit
            vehicleViolations.speedLimitViolationsStrict = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonStrict, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsSoft = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonSoft, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsHard = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonHard, lanelet::traffic_rules::America, 2.2352);
          } //END IF

          if (offroadChecking) {
            //check if the vehicle is violating the lanelets off-road boundaries
            vehicleViolations.offroadViolations = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveHalf);
            vehicleViolations.offroadViolationsFull = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Full);
            vehicleViolations.offroadViolationsFrame = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveFrame);
            vehicleViolations.offroadViolationsChassis = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveChassis);
            vehicleViolations.offroadViolationsCenter = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Centre);

            //contains the results of vehicle speeding detection            
            noOffroadResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                              std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check if the results from the above tests are not empty
            if (!vehicleViolations.offroadViolationsFull.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFull).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFull).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsFrame.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFrame).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFrame).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsChassis.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsChassis).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsChassis).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolations.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolations).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolations).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsCenter.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsCenter).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsCenter).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results to the csv file
            americanVehiclePtr->outputDataToExcel(filePathFull, noOffroadResults);
          } //END IF

          /*
            If the Given Vehicle Stopped at the Intersections Stop-Line Given an AllWayStop or a Stop Sign Regulated Intersection,
            We can Check if the Vehicle Performed the Correct Intersectional Right-Of-Way Transfers Given Multiple Vehicles Stopped
            at the Same Time or Another Vehicle Stopped First and has the Intersectional Right-Of-Way.
            Essentially, Checks if the Reference Vehicle Passes the Intersection with the Right-Of-Way After Stopping:
              -> If multiple Vehicles Stop at Once, the Reference Vehicle Starts Once Given the Right-Of-Way
              -> If Another Vehicle Stops just Before the Reference Vehicle, the Reference Vehicle Waits to be Given the Right-Of-Way
          */
          if (stopChecking && turnChecking && vehicleViolations.ruleI.ruleStatus) {
            //check if Rules III, IV, or V were violated by the Currently Iterated Vehicle
            vehicleViolations.ruleIII = americanVehiclePtr->checkLeftBeforeRight(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                 runtimeCoefficients.leftBeforeRight.distanceThreshold,
                                                                                 runtimeCoefficients.leftBeforeRight.velocityThreshold,
                                                                                 lanelet::traffic_rules::America);
                                                                                 //END checkLeftBeforeRight

            vehicleViolations.ruleV = americanVehiclePtr->checkShouldBeTurningLeft(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                   runtimeCoefficients.leftTurnPriority.distanceThreshold,
                                                                                   runtimeCoefficients.leftTurnPriority.velocityThreshold,
                                                                                   lanelet::traffic_rules::America);
                                                                                   //END checkShouldBeTurningLeft

            yieldsOne += int(vehicleViolations.ruleIII.neededToStop.get_value_or(0));
            yieldsTwo += int(vehicleViolations.ruleV.neededToStop.get_value_or(0));

            //check for Violations Involving the Right-before-Left Rule (A Vehicle went LeftBeforeRight)
            if (!vehicleViolations.ruleIII.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleIII.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleIII.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)),
                        "LeftBeforeRight");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleIII.referenceID) + "," + std::to_string(vehicleViolations.ruleIII.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleIII.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.turnDirection.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";
              turnViolationResults += std::to_string(((double) vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left-right turn priority intersection stop violation
              leftBeforeRight += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF

            //check for Violations Involving the Turn Left Rule
            if (!vehicleViolations.ruleV.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleV.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleV.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)),
                        "ShouldBeTurningLeft");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleV.referenceID) + "," + std::to_string(vehicleViolations.ruleV.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleV.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.turnDirection.get_value_or(0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left turn intersection stop violation
              turningLeft += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          }
          //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
          else if (stopChecking && !vehicleViolations.ruleI.ruleStatus) {
            //record that a vehicle was detected engaging in a stopline intersection stop violation
            shouldStop += 1;
            //record that the vehicle has violated at least one rule
            nonViolator = false;
          } //END IF

          if (speedingChecking) {
            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsStrict).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsStrict))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
              
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              speedLimit += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsSoft).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsSoft))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsHard).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsHard))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } //END IF
          } //END IF

          if (offroadChecking) {
            //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
            if (vehicleViolations.offroadViolations.size() != 0) {
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              noOffroad += 1;
              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          } //END IF

          if (tailgatingChecking) {
            //contains the results of vehicle speeding detection            
            tailgatingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check for Violations Involving Tailgating between Vehicles
            if (vehicleViolations.tailgating.tailgatedVehicle.size() != 0) {
              //record that a vehicle was detected tailgating
              isTailgating += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;

              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgating.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgating.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results of the vehicle speeding
            americanVehiclePtr->outputDataToExcel(tailgatingViolations, tailgatingResults);
          } //END IF

          //record all rule violations for the given vehicle by pushing it into the vector of all rule violations
          allVehicleViolations.push_back(vehicleViolations);

          //record that a non violating vehicle was recorded if no rules were broken
          if (nonViolator) {
            //no rules were violated
            nonViolators += 1;
          } //END IF
        } //END FOR

        //counts the total number of vehicles ran through by the loop
        totalRuns += runs;

        //counts the total number of non-rule violating vehicles encountered
        totalNonViolators += nonViolators;

        //counts the number of rule violating vehicle encountered in the given interaction dataset
        totalShouldStop += shouldStop;
        totalLeftBeforeRight += leftBeforeRight;
        totalHasPriority += hasPriority;
        totalTurningLeft += turningLeft;
        totalTailgating += isTailgating;
        totalSpeedLimit += speedLimit;
        totalNoOffroad += noOffroad;
        totalStops += stops;
        totalYieldsOne += yieldsOne;
        totalYieldsTwo += yieldsTwo;

        //store the results in a results statistics struct
        struct ruleViolationStatistics results = {runs, nonViolators, shouldStop, leftBeforeRight, hasPriority, turningLeft, isTailgating,
                                                  speedLimit, noOffroad, PATH, std::string(ent->d_name), stops, yieldsOne, yieldsTwo};

        //print the results struct to a CSV file
        americanVehiclePtr->outputToResultsLog(results);

        //append the vector and the interaction dataset path to the list of all rule violations
        allViolationsAllDatasets.push_back(std::make_pair(allVehicleViolations, path));

        //clear the given vector to get ready for the next interaction dataset
        allVehicleViolations.clear();
      } //END WHILE
    } //END IF

    //store the results in a results statistics struct
    struct ruleViolationStatistics results = {totalRuns, totalNonViolators, totalShouldStop, totalLeftBeforeRight, totalHasPriority, totalTurningLeft,
                                              totalTailgating, totalSpeedLimit, totalNoOffroad, PATH, std::string("All Files"), totalStops, totalYieldsOne, totalYieldsTwo};

    //print the results struct to a CSV file
    if (roundaboutSixWayActive) americanVehiclePtr->outputToResultsLog(results);
  } //END TEST_F(AmericanTrafficRulesVehicleTest, roundaboutSixWay)
} //END namespace vehicleData

/*
The following test script runs through the interaction datasets in the specified folder and checks for any rule violations,
as they were specified in the paper Formalization of Intersection Traffic Rules in Temporal Logic by Sebastian Maierhofer, Paul Moosbrugger, and Matthias Althoff.
  A link to the paper can be found here: https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
Rule violations can be visually inspected by checking the terminal output and looking for any test failures:
  The terminal will reference a failure message for the current CSV file, and continue to show the failure for the current test.
  As an example, we see the following (Note that "3" can represent any vehicle ID and "LeftBeforeRight" can represent any rule violation)
    The terminal will say it expected the quality of these values:
      std::to_string(vehicleData[vehicleID][0])
        Which is: "3"
        "LeftBeforeRight"]]></failure>
    Where "3" is the vehicle ID violating the rule, and "LeftBeforeRight" is the rule being violated
We can visualize the interaction dataset in question and manually inspect offending vehicle by using the following tool from GitHub: https://github.com/interaction-dataset/interaction-dataset
  Firstly, opening the terminal and moving the working directory to the interaction-dataset/python folder is required
  Secondly, run: ./main_visualize_data.py .TestScenarioForScripts 
    Where .TestScenarioForScripts (under interaction-dataset/recorded_trackfiles/.TestScenarioForScripts/) contains the correct interaction-CSV file, and is renamed to vehicle_tracks_000.csv
    And (interaction-dataset/maps/) contains the correct OSM map (the one corresponding to the interaction-CSV being analyzed) renamed to .TestScenarioForScripts.osm 
*/
namespace vehicleData {
  TEST_F(AmericanTrafficRulesVehicleTest, roundaboutFourWay) {
    //container for the directories being checked
    DIR * dir;
    struct dirent * ent;

    //container to determine if a vehicle is tailgating another
    struct tailgatingInformation tailgating;

    //create a struct object to contain all violations for a given vehicle
    struct violations vehicleViolations;

    //create a vector to contain all violations for all vehicles in an interaction dataset
    std::vector<struct violations> allVehicleViolations;

    //create a vector to contain all violations for all vehicles across interaction datasets
    std::vector<std::pair<std::vector<struct violations>, std::string>> allViolationsAllDatasets;

    //container for the number of people who did not violate any rule
    bool nonViolator;

    //contains the number of people not to violate any traffic rules
    int nonViolators = 0;
    int totalNonViolators = 0;

    //contains the statistics for all interaction datasets within a given test
    int totalRuns = 0;
    int totalShouldStop = 0;
    int totalLeftBeforeRight = 0;
    int totalHasPriority = 0;
    int totalTurningLeft = 0;
    int totalTailgating = 0;
    int totalSpeedLimit = 0;
    int totalNoOffroad = 0;
    int totalStops = 0;
    int totalYieldsOne = 0;
    int totalYieldsTwo = 0;

    //contains statistics for specific interaction datasets within a given test
    int runs = 0;
    int shouldStop = 0;
    int leftBeforeRight = 0;
    int hasPriority = 0;
    int turningLeft = 0;
    int isTailgating = 0;
    int speedLimit = 0;
    int noOffroad = 0;
    int stops = 0;
    int yieldsOne = 0;
    int yieldsTwo = 0;

    //used for exiting early
    int iteration = 0;
    int COUNTER = 0;

    //define the working map path
    std::string workingMapPath = mapPathUSARoundaboutSR;

    //define the working data path
    std::string workingDataPath = dataPathUSARoundaboutSRDir;

    //stores the path to the output file
    std::string PATH = "./Output/resultsLogRoundaboutFourWay.csv";

    //stores the path to the output file and the new header for the new file
    const char HEADER[] = "File,Total Vehicles,Non-Rule-Violating Vehicles,Stop-Violators,Stops Needed,Left Before Right Violators,Lanelet Priority Violators,Should Turn Left Violators,Yields One Needed,Yields Two Needed,Vehicle Tailgating Violations,Speed Limit,No Offroad\n";

    //clear the output file
    if (roundaboutFourWayActive) americanVehiclePtr->clearOutput(PATH, HEADER);

    //contains the speed limit epsilon value in terms of meters per second
    double speedingEpsilonStrict = 0;
    double speedingEpsilonSoft = 1.38889;
    double speedingEpsilonHard = 2.77778;
    
    //contains the results of the vehicle speed limit violation
    std::string vehicleSpeedingResults;

    //format header for the speeding results
    const char SPEEDING_HEADER[] = "FILE,ID,TRIP DURATION,SPEEDING DURATION,MOTION DURATION,TIME BASED PROPORTION OF SPEEDING,SPEED LIMIT,EPSILON";;

    //containers for the file paths to the speeding results spreadsheets
    std::string filePathSpeedingStrict = "./Output/SpeedingResultsStrict-RoundaboutFourWay.csv";
    std::string filePathSpeedingSoft = "./Output/SpeedingResultsSoft-RoundaboutFourWay.csv";
    std::string filePathSpeedingHard = "./Output/SpeedingResultsHard-RoundaboutFourWay.csv";

    //reset the output file for the speeding results
    if (roundaboutFourWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingStrict, SPEEDING_HEADER);
    if (roundaboutFourWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingSoft, SPEEDING_HEADER);
    if (roundaboutFourWayActive && speedingChecking) americanVehiclePtr->resetOutputDataToExcel(filePathSpeedingHard, SPEEDING_HEADER);

    //container for the output line for the vehicle stop analysis
    std::string stopResults;
    //format header for the stop violation results
    const char STOP_HEADER[] = "FILE,ID,Standard,Standard-Velocity,High Velocity,High-Velocity,Low Velocity,Low-Velocity,High Distance,Long-Velocity,Low Distance,Short-Velocity";
    //reset the output file for the stopping results
    if (roundaboutFourWayActive && stopChecking) americanVehiclePtr->resetOutputDataToExcel("./Output/StoppingResultsStandard-RoundaboutFourWay.csv", STOP_HEADER);

    //contains the file path for the output log
    std::string filePathFull = "./Output/noOffroadFull-RoundaboutFourWay.csv";

    //contains the header for the output file
    const char OFFROAD_HEADER[] = "FILE,ID,TRIP DURATION,"
                                  "OFFROAD DURATION (FULL),TIME BASED PROPORTION OF OFFROAD (FULL),"
                                  "OFFROAD DURATION (FRAME),TIME BASED PROPORTION OF OFFROAD (FRAME),"
                                  "OFFROAD DURATION (CHASSIS),TIME BASED PROPORTION OF OFFROAD (CHASSIS),"
                                  "OFFROAD DURATION (HALF),TIME BASED PROPORTION OF OFFROAD(HALF),"
                                  "OFFROAD DURATION (CENTER),TIME BASED PROPORTION OF OFFROAD (CENTER)";

    //clears the output file
    if (roundaboutFourWayActive && offroadChecking) americanVehiclePtr->resetOutputDataToExcel(filePathFull, OFFROAD_HEADER);

    //contains the collision results for the given vehicle
    std::vector<struct violation> resultsFull, resultsFrame, resultsChassis, resultsHalf, resultsCenter;

    //container for the results string for off-road violation detection
    std::string noOffroadResults;

    //contains the file path for the turning violation results
    std::string turnViolations = "./Output/turnViolationResults-RoundaboutFourWay.csv";

    //contains the header for the output file
    const char TURN_VIOLATION_HEADER[] = "FILE,Offender ID,X-Position,Y-Position,Timestamp,Stop Duration,Length,Width,Yaw,Direction,Trip Duration,Proportion";

    //clears the output file
    if (roundaboutFourWayActive && turnChecking) americanVehiclePtr->resetOutputDataToExcel(turnViolations, TURN_VIOLATION_HEADER);

    //container for the results string for turn violation detection
    std::string turnViolationResults = "";

    //container for the tailgating results
    std::string tailgatingResults;

    //container for the spreadsheet for tailgating violations
    std::string tailgatingViolations = "./Output/tailgatingViolations-RoundaboutFourWay.csv";

    //contains the header for the output file
    const char TAILGATING_VIOLATION_HEADER[] = "FILE,ID,TRIP DURATION,"
                                               "TAILGATING DURATION (Default),TIME BASED PROPORTION OF TAILGATING (Default),"
                                               "TAILGATING DURATION (HighResponse),TIME BASED PROPORTION OF TAILGATING (HighResponse),"
                                               "TAILGATING DURATION (HighBreak),TIME BASED PROPORTION OF TAILGATING (HighBreak),"
                                               "TAILGATING DURATION (HighAccel),TIME BASED PROPORTION OF TAILGATING (HighAccel),"
                                               "TAILGATING DURATION (HighMinAccel),TIME BASED PROPORTION OF TAILGATING (HighMinAccel),"
                                               "TAILGATING DURATION (LowResponse),TIME BASED PROPORTION OF TAILGATING (LowResponse),"
                                               "TAILGATING DURATION (LowBreak),TIME BASED PROPORTION OF TAILGATING (LowBreak),"
                                               "TAILGATING DURATION (LowAccel),TIME BASED PROPORTION OF TAILGATING (LowAccel),"
                                               "TAILGATING DURATION (LowMinAccel),TIME BASED PROPORTION OF TAILGATING (LowMinAccel)";

    //reset the output file for tailgating violations
    if (roundaboutFourWayActive && tailgatingChecking) americanVehiclePtr->resetOutputDataToExcel(tailgatingViolations, TAILGATING_VIOLATION_HEADER);

    //if the Given Directory Path can be Opened by the File System
    if ((dir = opendir(workingDataPath.c_str())) != NULL) {
      //for Every File in the Given Directory Path
      while ((ent = readdir (dir)) != NULL) {
        if (!roundaboutFourWayActive) break;
        //formats the directory path into a string
        std::string path = workingDataPath + "/" + std::string(ent->d_name);

        //filters out file names too short to be CSV's like "." and ".."
        if (strlen(ent->d_name) < 4) {
          //move to the next iteration of the loop
          continue;
        }
        else if (iteration != COUNTER) {
          iteration += 1;
          continue;
        } //END IF

        //get and store the formatted vehicle data from the interaction dataset
        std::vector<struct vehicles> vehicleData = americanVehiclePtr->getVehicleData(path);

        //contains the origin point for the open street map file
        lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

        //output the file name
        EXPECT_EQ("file: ", path);

        //contains statistics for the vehicles being scanned
        runs = 0;
        nonViolators = 0;
        shouldStop = 0;
        leftBeforeRight = 0;
        hasPriority = 0;
        turningLeft = 0;
        isTailgating = 0;
        speedLimit = 0;
        noOffroad = 0;
        stops = 0;
        yieldsOne = 0;
        yieldsTwo = 0;

        //tests every vehicle, and reports those that fail with their ID
        for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
          //re-build a violations struct for the next vehicle encountered
          vehicleViolations = violations();

          //count the iteration through the FOR loop as a run
          runs += 1;

          //signals if the given vehicle NOT has violated any rules, and is true by default
          nonViolator = true;

          //determines if stop checking is active or not
          if (stopChecking) {
            //check if the given vehicle failed to stop at a stop regulated intersection
            vehicleViolations.ruleI = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                          runtimeCoefficients.stop.distanceThreshold,
                                                                          runtimeCoefficients.stop.velocityThreshold,
                                                                          lanelet::traffic_rules::America);
                                                                          //END checkStopAtSign
            vehicleViolations.ruleIStrictVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictVelocity.distanceThreshold,
                                                                                        stopStrictVelocity.velocityThreshold,
                                                                                        lanelet::traffic_rules::America);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardVelocity = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardVelocity.distanceThreshold,
                                                                                      stopHardVelocity.velocityThreshold,
                                                                                      lanelet::traffic_rules::America);
                                                                                      //END checkStopAtSign
            vehicleViolations.ruleIStrictDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictDistance.distanceThreshold,
                                                                                        stopStrictDistance.velocityThreshold,
                                                                                        lanelet::traffic_rules::America);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardDistance = americanVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardDistance.distanceThreshold,
                                                                                      stopHardDistance.velocityThreshold,
                                                                                      lanelet::traffic_rules::America);

            //records the stop results to the given string
            stopResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + ",";

            //check if the vehicle needs to stop
            stops += int(vehicleViolations.ruleI.neededToStop.get_value_or(0));

            //record the results of the stop sign checking
            if (vehicleViolations.ruleI.ruleStatus && vehicleViolations.ruleI.endTimestamp && vehicleViolations.ruleI.timestamp) {
              //adds the stop results to the current output string
              stopResults += (std::to_string((vehicleViolations.ruleI.endTimestamp.get_value_or(0) - vehicleViolations.ruleI.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0))) + "," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictVelocity.ruleStatus && vehicleViolations.ruleIStrictVelocity.endTimestamp && vehicleViolations.ruleIStrictVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardVelocity.ruleStatus && vehicleViolations.ruleIHardVelocity.endTimestamp && vehicleViolations.ruleIHardVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictDistance.ruleStatus && vehicleViolations.ruleIStrictDistance.endTimestamp && vehicleViolations.ruleIStrictDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictDistance.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardDistance.ruleStatus && vehicleViolations.ruleIHardDistance.endTimestamp && vehicleViolations.ruleIHardDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardDistance.magnitude.get_value_or(-1)) + ",";
            } //END IF

            //output the results of the stop sign detection to the output file
            americanVehiclePtr->outputDataToExcel("./Output/StoppingResultsStandard-RoundaboutFourWay.csv", stopResults);
          } //END IF

          if (tailgatingChecking) {
            //check if the given vehicle is tailgating other vehicles along the intersection
            vehicleViolations.tailgating = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                                    runtimeCoefficients.tailgating.velocityThreshold,
                                                                                    runtimeCoefficients.tailgating.distanceThreshold,
                                                                                    runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                                    //END isTailgatingAtMoment

            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowResponse = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowBreak = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowMinAccel = americanVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0), DrivingOnly, lanelet::traffic_rules::America);
                                                                  //END isTailgatingAtMoment
          } //END IF
          
          if (speedingChecking) {
            //check if the vehicle is violating the lanelets speed limit
            vehicleViolations.speedLimitViolationsStrict = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonStrict, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsSoft = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonSoft, lanelet::traffic_rules::America, 2.2352);
            vehicleViolations.speedLimitViolationsHard = americanVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonHard, lanelet::traffic_rules::America, 2.2352);
          } //END IF

          if (offroadChecking) {
            //check if the vehicle is violating the lanelets off-road boundaries
            vehicleViolations.offroadViolations = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveHalf);
            vehicleViolations.offroadViolationsFull = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Full);
            vehicleViolations.offroadViolationsFrame = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveFrame);
            vehicleViolations.offroadViolationsChassis = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveChassis);
            vehicleViolations.offroadViolationsCenter = americanVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Centre);

            //contains the results of vehicle speeding detection            
            noOffroadResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                              std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check if the results from the above tests are not empty
            if (!vehicleViolations.offroadViolationsFull.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFull).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFull).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsFrame.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFrame).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFrame).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsChassis.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsChassis).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsChassis).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolations.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolations).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolations).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsCenter.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsCenter).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsCenter).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results to the csv file
            americanVehiclePtr->outputDataToExcel(filePathFull, noOffroadResults);
          } //END IF

          /*
            If the Given Vehicle Stopped at the Intersections Stop-Line Given an AllWayStop or a Stop Sign Regulated Intersection,
            We can Check if the Vehicle Performed the Correct Intersectional Right-Of-Way Transfers Given Multiple Vehicles Stopped
            at the Same Time or Another Vehicle Stopped First and has the Intersectional Right-Of-Way.
            Essentially, Checks if the Reference Vehicle Passes the Intersection with the Right-Of-Way After Stopping:
              -> If multiple Vehicles Stop at Once, the Reference Vehicle Starts Once Given the Right-Of-Way
              -> If Another Vehicle Stops just Before the Reference Vehicle, the Reference Vehicle Waits to be Given the Right-Of-Way
          */
          if (stopChecking && turnChecking && vehicleViolations.ruleI.ruleStatus) {
            //check if Rules III, IV, or V were violated by the Currently Iterated Vehicle
            vehicleViolations.ruleIII = americanVehiclePtr->checkLeftBeforeRight(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                 runtimeCoefficients.leftBeforeRight.distanceThreshold,
                                                                                 runtimeCoefficients.leftBeforeRight.velocityThreshold,
                                                                                 lanelet::traffic_rules::America);
                                                                                 //END checkLeftBeforeRight

            vehicleViolations.ruleV = americanVehiclePtr->checkShouldBeTurningLeft(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                   runtimeCoefficients.leftTurnPriority.distanceThreshold,
                                                                                   runtimeCoefficients.leftTurnPriority.velocityThreshold,
                                                                                   lanelet::traffic_rules::America);
                                                                                   //END checkShouldBeTurningLeft

            yieldsOne += int(vehicleViolations.ruleIII.neededToStop.get_value_or(0));
            yieldsTwo += int(vehicleViolations.ruleV.neededToStop.get_value_or(0));

            //check for Violations Involving the Right-before-Left Rule (A Vehicle went LeftBeforeRight)
            if (!vehicleViolations.ruleIII.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleIII.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleIII.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)),
                        "LeftBeforeRight");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleIII.referenceID) + "," + std::to_string(vehicleViolations.ruleIII.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleIII.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.turnDirection.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";
              turnViolationResults += std::to_string(((double) vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left-right turn priority intersection stop violation
              leftBeforeRight += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF

            //check for Violations Involving the Turn Left Rule
            if (!vehicleViolations.ruleV.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleV.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleV.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)),
                        "ShouldBeTurningLeft");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleV.referenceID) + "," + std::to_string(vehicleViolations.ruleV.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleV.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.turnDirection.get_value_or(0));

              //add the turn violation to the output spreadsheet
              americanVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left turn intersection stop violation
              turningLeft += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          }
          //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
          else if (stopChecking && !vehicleViolations.ruleI.ruleStatus) {
            //record that a vehicle was detected engaging in a stopline intersection stop violation
            shouldStop += 1;
            //record that the vehicle has violated at least one rule
            nonViolator = false;
          } //END IF

          if (speedingChecking) {
            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsStrict).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsStrict))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
              
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              speedLimit += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsSoft).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsSoft))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsHard).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsHard))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              americanVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } //END IF
          } //END IF

          if (offroadChecking) {
            //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
            if (vehicleViolations.offroadViolations.size() != 0) {
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              noOffroad += 1;
              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          } //END IF

          if (tailgatingChecking) {
            //contains the results of vehicle speeding detection            
            tailgatingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check for Violations Involving Tailgating between Vehicles
            if (vehicleViolations.tailgating.tailgatedVehicle.size() != 0) {
              //record that a vehicle was detected tailgating
              isTailgating += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;

              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgating.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgating.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results of the vehicle speeding
            americanVehiclePtr->outputDataToExcel(tailgatingViolations, tailgatingResults);
          } //END IF

          //record all rule violations for the given vehicle by pushing it into the vector of all rule violations
          allVehicleViolations.push_back(vehicleViolations);

          //record that a non violating vehicle was recorded if no rules were broken
          if (nonViolator) {
            //no rules were violated
            nonViolators += 1;
          } //END IF
        } //END FOR

        //counts the total number of vehicles ran through by the loop
        totalRuns += runs;

        //counts the total number of non-rule violating vehicles encountered
        totalNonViolators += nonViolators;

        //counts the number of rule violating vehicle encountered in the given interaction dataset
        totalShouldStop += shouldStop;
        totalLeftBeforeRight += leftBeforeRight;
        totalHasPriority += hasPriority;
        totalTurningLeft += turningLeft;
        totalTailgating += isTailgating;
        totalSpeedLimit += speedLimit;
        totalNoOffroad += noOffroad;
        totalStops += stops;
        totalYieldsOne += yieldsOne;
        totalYieldsTwo += yieldsTwo;

        //store the results in a results statistics struct
        struct ruleViolationStatistics results = {runs, nonViolators, shouldStop, leftBeforeRight, hasPriority, turningLeft, isTailgating,
                                                  speedLimit, noOffroad, PATH, std::string(ent->d_name), stops, yieldsOne, yieldsTwo};

        //print the results struct to a CSV file
        americanVehiclePtr->outputToResultsLog(results);

        //append the vector and the interaction dataset path to the list of all rule violations
        allViolationsAllDatasets.push_back(std::make_pair(allVehicleViolations, path));

        //clear the given vector to get ready for the next interaction dataset
        allVehicleViolations.clear();
      } //END WHILE
    } //END IF

    //store the results in a results statistics struct
    struct ruleViolationStatistics results = {totalRuns, totalNonViolators, totalShouldStop, totalLeftBeforeRight, totalHasPriority, totalTurningLeft,
                                              totalTailgating, totalSpeedLimit, totalNoOffroad, PATH, std::string("All Files"), totalStops, totalYieldsOne, totalYieldsTwo};

    //print the results struct to a CSV file
    if (roundaboutFourWayActive) americanVehiclePtr->outputToResultsLog(results);
  } //END TEST_F(AmericanTrafficRulesVehicleTest, roundaboutFourWay)
} //END namespace vehicleData

/*
The following test script runs through the interaction datasets in the specified folder and checks for any rule violations,
as they were specified in the paper Formalization of Intersection Traffic Rules in Temporal Logic by Sebastian Maierhofer, Paul Moosbrugger, and Matthias Althoff.
  A link to the paper can be found here: https://mediatum.ub.tum.de/doc/1664592/uw2i3i5kwjh3w4ezek0qov5og.Maierhofer-2022-IV.pdf
Rule violations can be visually inspected by checking the terminal output and looking for any test failures:
  The terminal will reference a failure message for the current CSV file, and continue to show the failure for the current test.
  As an example, we see the following (Note that "3" can represent any vehicle ID and "LeftBeforeRight" can represent any rule violation)
    The terminal will say it expected the quality of these values:
      std::to_string(vehicleData[vehicleID][0])
        Which is: "3"
        "LeftBeforeRight"]]></failure>
    Where "3" is the vehicle ID violating the rule, and "LeftBeforeRight" is the rule being violated
We can visualize the interaction dataset in question and manually inspect offending vehicle by using the following tool from GitHub: https://github.com/interaction-dataset/interaction-dataset
  Firstly, opening the terminal and moving the working directory to the interaction-dataset/python folder is required
  Secondly, run: ./main_visualize_data.py .TestScenarioForScripts 
    Where .TestScenarioForScripts (under interaction-dataset/recorded_trackfiles/.TestScenarioForScripts/) contains the correct interaction-CSV file, and is renamed to vehicle_tracks_000.csv
    And (interaction-dataset/maps/) contains the correct OSM map (the one corresponding to the interaction-CSV being analyzed) renamed to .TestScenarioForScripts.osm 
*/
namespace vehicleData {
  TEST_F(ChineseTrafficRulesVehicleTest, roundaboutFiveWay) {
    //container for the directories being checked
    DIR * dir;
    struct dirent * ent;

    //container to determine if a vehicle is tailgating another
    struct tailgatingInformation tailgating;

    //create a struct object to contain all violations for a given vehicle
    struct violations vehicleViolations;

    //create a vector to contain all violations for all vehicles in an interaction dataset
    std::vector<struct violations> allVehicleViolations;

    //create a vector to contain all violations for all vehicles across interaction datasets
    std::vector<std::pair<std::vector<struct violations>, std::string>> allViolationsAllDatasets;

    //container for the number of people who did not violate any rule
    bool nonViolator;

    //contains the number of people not to violate any traffic rules
    int nonViolators = 0;
    int totalNonViolators = 0;

    //contains the statistics for all interaction datasets within a given test
    int totalRuns = 0;
    int totalShouldStop = 0;
    int totalLeftBeforeRight = 0;
    int totalHasPriority = 0;
    int totalTurningLeft = 0;
    int totalTailgating = 0;
    int totalSpeedLimit = 0;
    int totalNoOffroad = 0;
    int totalStops = 0;
    int totalYieldsOne = 0;
    int totalYieldsTwo = 0;

    //contains statistics for specific interaction datasets within a given test
    int runs = 0;
    int shouldStop = 0;
    int leftBeforeRight = 0;
    int hasPriority = 0;
    int turningLeft = 0;
    int isTailgating = 0;
    int speedLimit = 0;
    int noOffroad = 0;
    int stops = 0;
    int yieldsOne = 0;
    int yieldsTwo = 0;

    //used for exiting early
    int iteration = 0;
    int COUNTER = 0;

    //define the working map path
    std::string workingMapPath = mapPathCHNRoundabout;

    //define the working data path
    std::string workingDataPath = dataPathCHNRoundaboutLNDir;

    //stores the path to the output file
    std::string PATH = "./Output/resultsLogRoundaboutFiveWay.csv";

    //stores the path to the output file and the new header for the new file
    const char HEADER[] = "File,Total Vehicles,Non-Rule-Violating Vehicles,Stop-Violators,Stops Needed,Left Before Right Violators,Lanelet Priority Violators,Should Turn Left Violators,Yields One Needed,Yields Two Needed,Vehicle Tailgating Violations,Speed Limit,No Offroad\n";

    //clear the output file
    if (roundaboutFiveWayActive) chineseVehiclePtr->clearOutput(PATH, HEADER);

    //contains the speed limit epsilon value in terms of meters per second
    double speedingEpsilonStrict = 0;
    double speedingEpsilonSoft = 1.38889;
    double speedingEpsilonHard = 2.77778;
    
    //contains the results of the vehicle speed limit violation
    std::string vehicleSpeedingResults;

    //format header for the speeding results
    const char SPEEDING_HEADER[] = "FILE,ID,TRIP DURATION,SPEEDING DURATION,MOTION DURATION,TIME BASED PROPORTION OF SPEEDING,SPEED LIMIT,EPSILON";

    //containers for the file paths to the speeding results spreadsheets
    std::string filePathSpeedingStrict = "./Output/SpeedingResultsStrict-RoundaboutFiveWay.csv";
    std::string filePathSpeedingSoft = "./Output/SpeedingResultsSoft-RoundaboutFiveWay.csv";
    std::string filePathSpeedingHard = "./Output/SpeedingResultsHard-RoundaboutFiveWay.csv";

    //reset the output file for the speeding results
    if (roundaboutFiveWayActive && speedingChecking) chineseVehiclePtr->resetOutputDataToExcel(filePathSpeedingStrict, SPEEDING_HEADER);
    if (roundaboutFiveWayActive && speedingChecking) chineseVehiclePtr->resetOutputDataToExcel(filePathSpeedingSoft, SPEEDING_HEADER);
    if (roundaboutFiveWayActive && speedingChecking) chineseVehiclePtr->resetOutputDataToExcel(filePathSpeedingHard, SPEEDING_HEADER);

    //container for the output line for the vehicle stop analysis
    std::string stopResults;
    //format header for the stop violation results
    const char STOP_HEADER[] = "FILE,ID,Standard,Standard-Velocity,High Velocity,High-Velocity,Low Velocity,Low-Velocity,High Distance,Long-Velocity,Low Distance,Short-Velocity";
    //reset the output file for the stopping results
    if (roundaboutFiveWayActive && stopChecking) chineseVehiclePtr->resetOutputDataToExcel("./Output/StoppingResultsStandard-RoundaboutFiveWay.csv", STOP_HEADER);

    //contains the file path for the output log
    std::string filePathFull = "./Output/noOffroadFull-RoundaboutFiveWay.csv";

    //contains the header for the output file
    const char OFFROAD_HEADER[] = "FILE,ID,TRIP DURATION,"
                                  "OFFROAD DURATION (FULL),TIME BASED PROPORTION OF OFFROAD (FULL),"
                                  "OFFROAD DURATION (FRAME),TIME BASED PROPORTION OF OFFROAD (FRAME),"
                                  "OFFROAD DURATION (CHASSIS),TIME BASED PROPORTION OF OFFROAD (CHASSIS),"
                                  "OFFROAD DURATION (HALF),TIME BASED PROPORTION OF OFFROAD(HALF),"
                                  "OFFROAD DURATION (CENTER),TIME BASED PROPORTION OF OFFROAD (CENTER)";

    //clears the output file
    if (roundaboutFiveWayActive && offroadChecking) chineseVehiclePtr->resetOutputDataToExcel(filePathFull, OFFROAD_HEADER);

    //contains the collision results for the given vehicle
    std::vector<struct violation> resultsFull, resultsFrame, resultsChassis, resultsHalf, resultsCenter;

    //container for the results string for off-road violation detection
    std::string noOffroadResults;

    //contains the file path for the turning violation results
    std::string turnViolations = "./Output/turnViolationResults-RoundaboutFiveWay.csv";

    //contains the header for the output file
    const char TURN_VIOLATION_HEADER[] = "FILE,Offender ID,X-Position,Y-Position,Timestamp,Stop Duration,Length,Width,Yaw,Direction,Trip Duration,Proportion";

    //clears the output file
    if (roundaboutFiveWayActive && turnChecking) chineseVehiclePtr->resetOutputDataToExcel(turnViolations, TURN_VIOLATION_HEADER);

    //container for the results string for turn violation detection
    std::string turnViolationResults = "";

    //container for the tailgating results
    std::string tailgatingResults;

    //container for the spreadsheet for tailgating violations
    std::string tailgatingViolations = "./Output/tailgatingViolations-RoundaboutFiveWay.csv";

    //contains the header for the output file
    const char TAILGATING_VIOLATION_HEADER[] = "FILE,ID,TRIP DURATION,"
                                               "TAILGATING DURATION (Default),TIME BASED PROPORTION OF TAILGATING (Default),"
                                               "TAILGATING DURATION (HighResponse),TIME BASED PROPORTION OF TAILGATING (HighResponse),"
                                               "TAILGATING DURATION (HighBreak),TIME BASED PROPORTION OF TAILGATING (HighBreak),"
                                               "TAILGATING DURATION (HighAccel),TIME BASED PROPORTION OF TAILGATING (HighAccel),"
                                               "TAILGATING DURATION (HighMinAccel),TIME BASED PROPORTION OF TAILGATING (HighMinAccel),"
                                               "TAILGATING DURATION (LowResponse),TIME BASED PROPORTION OF TAILGATING (LowResponse),"
                                               "TAILGATING DURATION (LowBreak),TIME BASED PROPORTION OF TAILGATING (LowBreak),"
                                               "TAILGATING DURATION (LowAccel),TIME BASED PROPORTION OF TAILGATING (LowAccel),"
                                               "TAILGATING DURATION (LowMinAccel),TIME BASED PROPORTION OF TAILGATING (LowMinAccel)";

    //reset the output file for tailgating violations
    if (roundaboutFiveWayActive && tailgatingChecking) chineseVehiclePtr->resetOutputDataToExcel(tailgatingViolations, TAILGATING_VIOLATION_HEADER);

    //if the Given Directory Path can be Opened by the File System
    if ((dir = opendir(workingDataPath.c_str())) != NULL) {
      //for Every File in the Given Directory Path
      while ((ent = readdir (dir)) != NULL) {
        if (!roundaboutFiveWayActive) break;
        //formats the directory path into a string
        std::string path = workingDataPath + "/" + std::string(ent->d_name);

        //filters out file names too short to be CSV's like "." and ".."
        if (strlen(ent->d_name) < 4) {
          //move to the next iteration of the loop
          continue;
        }
        else if (iteration != COUNTER) {
          iteration += 1;
          continue;
        } //END IF

        //get and store the formatted vehicle data from the interaction dataset
        std::vector<struct vehicles> vehicleData = chineseVehiclePtr->getVehicleData(path);

        //contains the origin point for the open street map file
        lanelet::BasicPoint2d originPoint = lanelet::BasicPoint2d(0, 0);

        //output the file name
        EXPECT_EQ("file: ", path);

        //contains statistics for the vehicles being scanned
        runs = 0;
        nonViolators = 0;
        shouldStop = 0;
        leftBeforeRight = 0;
        hasPriority = 0;
        turningLeft = 0;
        isTailgating = 0;
        speedLimit = 0;
        noOffroad = 0;
        stops = 0;
        yieldsOne = 0;
        yieldsTwo = 0;

        //tests every vehicle, and reports those that fail with their ID
        for (size_t vehicleID = 0; vehicleID < vehicleData.size(); vehicleID++) {
          //re-build a violations struct for the next vehicle encountered
          vehicleViolations = violations();

          //count the iteration through the FOR loop as a run
          runs += 1;

          //signals if the given vehicle NOT has violated any rules, and is true by default
          nonViolator = true;

          //determines if stop checking is active or not
          if (stopChecking) {
            //check if the given vehicle failed to stop at a stop regulated intersection
            vehicleViolations.ruleI = chineseVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                          runtimeCoefficients.stop.distanceThreshold,
                                                                          runtimeCoefficients.stop.velocityThreshold,
                                                                          lanelet::traffic_rules::China);
                                                                          //END checkStopAtSign
            vehicleViolations.ruleIStrictVelocity = chineseVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictVelocity.distanceThreshold,
                                                                                        stopStrictVelocity.velocityThreshold,
                                                                                        lanelet::traffic_rules::China);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardVelocity = chineseVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardVelocity.distanceThreshold,
                                                                                      stopHardVelocity.velocityThreshold,
                                                                                      lanelet::traffic_rules::China);
                                                                                      //END checkStopAtSign
            vehicleViolations.ruleIStrictDistance = chineseVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                        stopStrictDistance.distanceThreshold,
                                                                                        stopStrictDistance.velocityThreshold,
                                                                                        lanelet::traffic_rules::China);
                                                                                        //END checkStopAtSign
            vehicleViolations.ruleIHardDistance = chineseVehiclePtr->checkStopAtSign(vehicleData[vehicleID].intervals, workingMapPath, originPoint,
                                                                                      stopHardDistance.distanceThreshold,
                                                                                      stopHardDistance.velocityThreshold,
                                                                                      lanelet::traffic_rules::China);

            //records the stop results to the given string
            stopResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + ",";

            //check if the vehicle needs to stop
            stops += int(vehicleViolations.ruleI.neededToStop.get_value_or(0));

            //record the results of the stop sign checking
            if (vehicleViolations.ruleI.ruleStatus && vehicleViolations.ruleI.endTimestamp && vehicleViolations.ruleI.timestamp) {
              //adds the stop results to the current output string
              stopResults += (std::to_string((vehicleViolations.ruleI.endTimestamp.get_value_or(0) - vehicleViolations.ruleI.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0))) + "," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictVelocity.ruleStatus && vehicleViolations.ruleIStrictVelocity.endTimestamp && vehicleViolations.ruleIStrictVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardVelocity.ruleStatus && vehicleViolations.ruleIHardVelocity.endTimestamp && vehicleViolations.ruleIHardVelocity.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardVelocity.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardVelocity.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardVelocity.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIStrictDistance.ruleStatus && vehicleViolations.ruleIStrictDistance.endTimestamp && vehicleViolations.ruleIStrictDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIStrictDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIStrictDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIStrictDistance.magnitude.get_value_or(-1)) + ",";
            }
            else {
              //adds a blank column to the current output string if no stop was detected given the threshold values
              stopResults += "False," + std::to_string(vehicleViolations.ruleI.magnitude.get_value_or(-1)) + ",";
            } //END IF

            if (vehicleViolations.ruleIHardDistance.ruleStatus && vehicleViolations.ruleIHardDistance.endTimestamp && vehicleViolations.ruleIHardDistance.timestamp) {
              //adds the stop results to the current output string
              stopResults += std::to_string((vehicleViolations.ruleIHardDistance.endTimestamp.get_value_or(0) - vehicleViolations.ruleIHardDistance.timestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0)) + "," + std::to_string(vehicleViolations.ruleIHardDistance.magnitude.get_value_or(-1)) + ",";
            } //END IF

            //output the results of the stop sign detection to the output file
            chineseVehiclePtr->outputDataToExcel("./Output/StoppingResultsStandard-RoundaboutFiveWay.csv", stopResults);
          } //END IF

          if (tailgatingChecking) {
            //check if the given vehicle is tailgating other vehicles along the intersection
            vehicleViolations.tailgating = chineseVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                                    runtimeCoefficients.tailgating.velocityThreshold,
                                                                                    runtimeCoefficients.tailgating.distanceThreshold,
                                                                                    runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                                    runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::China);
                                                                                    //END isTailgatingAtMoment

            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighResponse = chineseVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::China);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighBreak = chineseVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::China);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighAccel = chineseVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMax.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::China);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingHighMinAccel = chineseVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMax.get_value_or(0), DrivingOnly, lanelet::traffic_rules::China);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowResponse = chineseVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTimeMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::China);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowBreak = chineseVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFrontMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::China);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowAccel = chineseVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRearMin.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRear.get_value_or(0), DrivingOnly, lanelet::traffic_rules::China);
                                                                  //END isTailgatingAtMoment
            //obtain tailgating records and place in the container
            vehicleViolations.tailgatingLowMinAccel = chineseVehiclePtr->isTailgatingAtMoment(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData, workingMapPath, originPoint,
                                                                  runtimeCoefficients.tailgating.velocityThreshold,
                                                                  runtimeCoefficients.tailgating.distanceThreshold,
                                                                  runtimeCoefficients.tailgating.responseTime.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxBreakAccFront.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.maxAccelAccRear.get_value_or(0),
                                                                  runtimeCoefficients.tailgating.minBreakAccRearMin.get_value_or(0), DrivingOnly, lanelet::traffic_rules::China);
                                                                  //END isTailgatingAtMoment
          } //END IF
          
          if (speedingChecking) {
            //check if the vehicle is violating the lanelets speed limit
            vehicleViolations.speedLimitViolationsStrict = chineseVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonStrict, lanelet::traffic_rules::China, 2.2352);
            vehicleViolations.speedLimitViolationsSoft = chineseVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonSoft, lanelet::traffic_rules::China, 2.2352);
            vehicleViolations.speedLimitViolationsHard = chineseVehiclePtr->checkSpeedLimit(vehicleData[vehicleID].intervals, workingMapPath, originPoint, speedingEpsilonHard, lanelet::traffic_rules::China, 2.2352);
          } //END IF

          if (offroadChecking) {
            //check if the vehicle is violating the lanelets off-road boundaries
            vehicleViolations.offroadViolations = chineseVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveHalf);
            vehicleViolations.offroadViolationsFull = chineseVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Full);
            vehicleViolations.offroadViolationsFrame = chineseVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveFrame);
            vehicleViolations.offroadViolationsChassis = chineseVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::RemoveChassis);
            vehicleViolations.offroadViolationsCenter = chineseVehiclePtr->noOffroad(vehicleData[vehicleID].intervals, vehicleData[vehicleID].length, vehicleData[vehicleID].width, workingMapPath, originPoint, lanelet::traffic_rules::America, BoundaryValues::Centre);

            //contains the results of vehicle speeding detection            
            noOffroadResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                              std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check if the results from the above tests are not empty
            if (!vehicleViolations.offroadViolationsFull.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFull).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFull).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsFrame.empty()) {
              //contains the results of vehicle speeding detection            
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsFrame).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsFrame).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsChassis.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsChassis).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsChassis).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolations.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolations).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolations).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (!vehicleViolations.offroadViolationsCenter.empty()) {
              noOffroadResults += std::to_string((vehicleViolations.offroadViolationsCenter).size() * 100) + "," +
                                  std::to_string(((vehicleViolations.offroadViolationsCenter).size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            }
            else {
              noOffroadResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results to the csv file
            chineseVehiclePtr->outputDataToExcel(filePathFull, noOffroadResults);
          } //END IF

          /*
            If the Given Vehicle Stopped at the Intersections Stop-Line Given an AllWayStop or a Stop Sign Regulated Intersection,
            We can Check if the Vehicle Performed the Correct Intersectional Right-Of-Way Transfers Given Multiple Vehicles Stopped
            at the Same Time or Another Vehicle Stopped First and has the Intersectional Right-Of-Way.
            Essentially, Checks if the Reference Vehicle Passes the Intersection with the Right-Of-Way After Stopping:
              -> If multiple Vehicles Stop at Once, the Reference Vehicle Starts Once Given the Right-Of-Way
              -> If Another Vehicle Stops just Before the Reference Vehicle, the Reference Vehicle Waits to be Given the Right-Of-Way
          */
          if (stopChecking && turnChecking && vehicleViolations.ruleI.ruleStatus) {
            //check if Rules III, IV, or V were violated by the Currently Iterated Vehicle
            vehicleViolations.ruleIII = chineseVehiclePtr->checkLeftBeforeRight(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                 runtimeCoefficients.leftBeforeRight.distanceThreshold,
                                                                                 runtimeCoefficients.leftBeforeRight.velocityThreshold,
                                                                                 lanelet::traffic_rules::China);
                                                                                 //END checkLeftBeforeRight

            vehicleViolations.ruleV = chineseVehiclePtr->checkShouldBeTurningLeft(vehicleData, vehicleData[vehicleID], workingMapPath, originPoint,
                                                                                   runtimeCoefficients.leftTurnPriority.distanceThreshold,
                                                                                   runtimeCoefficients.leftTurnPriority.velocityThreshold,
                                                                                   lanelet::traffic_rules::China);
                                                                                   //END checkShouldBeTurningLeft

            yieldsOne += int(vehicleViolations.ruleIII.neededToStop.get_value_or(0));
            yieldsTwo += int(vehicleViolations.ruleV.neededToStop.get_value_or(0));

            //check for Violations Involving the Right-before-Left Rule (A Vehicle went LeftBeforeRight)
            if (!vehicleViolations.ruleIII.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleIII.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleIII.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)),
                        "LeftBeforeRight");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleIII.referenceID) + "," + std::to_string(vehicleViolations.ruleIII.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleIII.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleIII.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleIII.turnDirection.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";
              turnViolationResults += std::to_string(((double) vehicleViolations.ruleIII.endTimestamp.get_value_or(0)) / (vehicleData[vehicleID].intervals.size() * 100.0));

              //add the turn violation to the output spreadsheet
              chineseVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left-right turn priority intersection stop violation
              leftBeforeRight += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF

            //check for Violations Involving the Turn Left Rule
            if (!vehicleViolations.ruleV.ruleStatus) {
              EXPECT_EQ(std::to_string(vehicleViolations.ruleV.referenceID) + "|" +
                        std::to_string(vehicleViolations.ruleV.otherID.get_value_or(0)) + "|" +
                        std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)),
                        "ShouldBeTurningLeft");
                        //END EXPECT_EQ

              //gets a string representation of the turn violation
              turnViolationResults = std::string(ent->d_name) + "," + std::to_string(vehicleViolations.ruleV.referenceID) + "," + std::to_string(vehicleViolations.ruleV.xPos.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.yPos.get_value_or(0)) + "," + std::to_string(vehicleViolations.ruleV.timestamp.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.endTimestamp.get_value_or(0)) + "," + std::to_string(vehicleData[vehicleID].length) + ",";
              turnViolationResults += std::to_string(vehicleData[vehicleID].width) + "," + std::to_string(vehicleViolations.ruleV.yaw.get_value_or(0)) + ",";
              turnViolationResults += std::to_string(vehicleViolations.ruleV.turnDirection.get_value_or(0));

              //add the turn violation to the output spreadsheet
              chineseVehiclePtr->outputDataToExcel(turnViolations, turnViolationResults);

              //record that a vehicle was detected engaging in a left turn intersection stop violation
              turningLeft += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          }
          //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
          else if (stopChecking && !vehicleViolations.ruleI.ruleStatus) {
            //record that a vehicle was detected engaging in a stopline intersection stop violation
            shouldStop += 1;
            //record that the vehicle has violated at least one rule
            nonViolator = false;
          } //END IF

          if (speedingChecking) {
            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsStrict).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsStrict).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsStrict))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              chineseVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
              
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              speedLimit += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsStrict)) + "," +
                                      std::to_string(speedingEpsilonStrict*3.6);

              //output the results of the vehicle speeding
              chineseVehiclePtr->outputDataToExcel(filePathSpeedingStrict, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsSoft).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsSoft).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsSoft))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              chineseVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsSoft)) + "," +
                                      std::to_string(speedingEpsilonSoft*3.6);

              //output the results of the vehicle speeding
              chineseVehiclePtr->outputDataToExcel(filePathSpeedingSoft, vehicleSpeedingResults);
            } //END IF

            //check for Violations Involving Speeding at a Stop Line at a Non-Signalized Intersection
            if (!std::get<0>(vehicleViolations.speedLimitViolationsHard).empty()) {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      (std::to_string((std::get<0>(vehicleViolations.speedLimitViolationsHard).size() * 100.0) / 
                                        std::get<1>(vehicleViolations.speedLimitViolationsHard))) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              chineseVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } 
            else {
              //contains the results of vehicle speeding detection            
              vehicleSpeedingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                      std::to_string(vehicleData[vehicleID].intervals.size() * 100) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<1>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(0) + "," +
                                      std::to_string(std::get<2>(vehicleViolations.speedLimitViolationsHard)) + "," +
                                      std::to_string(speedingEpsilonHard*3.6);

              //output the results of the vehicle speeding
              chineseVehiclePtr->outputDataToExcel(filePathSpeedingHard, vehicleSpeedingResults);
            } //END IF
          } //END IF

          if (offroadChecking) {
            //check for Violations Involving Stopping at a Stop Line at a Non-Signalized Intersection
            if (vehicleViolations.offroadViolations.size() != 0) {
              //record that a vehicle was detected engaging in a stopline intersection stop violation
              noOffroad += 1;
              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } //END IF
          } //END IF

          if (tailgatingChecking) {
            //contains the results of vehicle speeding detection            
            tailgatingResults = std::string(ent->d_name) + "," + std::to_string(vehicleData[vehicleID].intervals[0].id) + "," +
                                std::to_string(vehicleData[vehicleID].intervals.size() * 100) + ",";

            //check for Violations Involving Tailgating between Vehicles
            if (vehicleViolations.tailgating.tailgatedVehicle.size() != 0) {
              //record that a vehicle was detected tailgating
              isTailgating += 1;

              //record that the vehicle has violated at least one rule
              nonViolator = false;

              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgating.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgating.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";

              //record that the vehicle has violated at least one rule
              nonViolator = false;
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingHighMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowResponse.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowBreak.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            if (vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() != 0) {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 100) + "," +
                                  std::to_string((vehicleViolations.tailgatingLowMinAccel.tailgatedVehicle.size() * 1.0) / (vehicleData[vehicleID].intervals.size() * 1.0)) + ",";
            } 
            else {
              //contains the results of vehicle speeding detection            
              tailgatingResults += std::to_string(0) + "," + std::to_string(0) + ",";
            } //END IF

            //output the results of the vehicle speeding
            chineseVehiclePtr->outputDataToExcel(tailgatingViolations, tailgatingResults);
          } //END IF

          //record all rule violations for the given vehicle by pushing it into the vector of all rule violations
          allVehicleViolations.push_back(vehicleViolations);

          //record that a non violating vehicle was recorded if no rules were broken
          if (nonViolator) {
            //no rules were violated
            nonViolators += 1;
          } //END IF
        } //END FOR

        //counts the total number of vehicles ran through by the loop
        totalRuns += runs;

        //counts the total number of non-rule violating vehicles encountered
        totalNonViolators += nonViolators;

        //counts the number of rule violating vehicle encountered in the given interaction dataset
        totalShouldStop += shouldStop;
        totalLeftBeforeRight += leftBeforeRight;
        totalHasPriority += hasPriority;
        totalTurningLeft += turningLeft;
        totalTailgating += isTailgating;
        totalSpeedLimit += speedLimit;
        totalNoOffroad += noOffroad;
        totalStops += stops;
	totalYieldsOne += yieldsOne;
	totalYieldsTwo += yieldsTwo;

        //store the results in a results statistics struct
        struct ruleViolationStatistics results = {runs, nonViolators, shouldStop, leftBeforeRight, hasPriority, turningLeft, isTailgating,
                                                  speedLimit, noOffroad, PATH, std::string(ent->d_name), stops, yieldsOne, yieldsTwo};

        //print the results struct to a CSV file
        chineseVehiclePtr->outputToResultsLog(results);

        //append the vector and the interaction dataset path to the list of all rule violations
        allViolationsAllDatasets.push_back(std::make_pair(allVehicleViolations, path));

        //clear the given vector to get ready for the next interaction dataset
        allVehicleViolations.clear();
      } //END WHILE
    } //END IF

    //store the results in a results statistics struct
    struct ruleViolationStatistics results = {totalRuns, totalNonViolators, totalShouldStop, totalLeftBeforeRight, totalHasPriority, totalTurningLeft,
                                              totalTailgating, totalSpeedLimit, totalNoOffroad, PATH, std::string("All Files"), totalStops, totalYieldsOne, totalYieldsTwo};

    //print the results struct to a CSV file
    if (roundaboutFiveWayActive) chineseVehiclePtr->outputToResultsLog(results);
  } //END TEST_F(ChineseTrafficRulesVehicleTest, roundaboutFiveWay)
} //END namespace vehicleData
