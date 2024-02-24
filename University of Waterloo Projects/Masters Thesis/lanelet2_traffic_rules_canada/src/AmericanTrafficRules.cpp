#include "lanelet2_traffic_rules_canada/AmericanTrafficRules.h"

#include <lanelet2_core/Forward.h>
#include <lanelet2_core/primitives/BasicRegulatoryElements.h>
#include <lanelet2_core/utility/Units.h>

#include "lanelet2_traffic_rules/Exceptions.h"
#include "lanelet2_traffic_rules/TrafficRulesFactory.h"

using namespace std::string_literals;

namespace lanelet {
namespace traffic_rules {

namespace {
RegisterTrafficRules<AmericanVehicle> cvRules(lanelet::traffic_rules::America, Participants::Vehicle);
RegisterTrafficRules<AmericanPedestrian> cpRules(lanelet::traffic_rules::America, Participants::Pedestrian);
RegisterTrafficRules<AmericanBicycle> cbRules(lanelet::traffic_rules::America, Participants::Bicycle);

Velocity trafficSignToVelocity(const std::string& typeString) {
  using namespace lanelet::units::literals;
  const static std::map<std::string, Velocity> StrToVelocity{
      //values for school zones (School Zone).
      {"usS4-I100", 15_mph}, {"usS4-I100-15", 15_mph}, {"usS4-I100-20", 20_mph}, {"usS4-I100-25", 25_mph},

      //construction zone speed limits (Maximum Speed)
      {"usW13-1-5", 5_mph}, {"usW13-1-10", 10_mph}, {"usW13-1-15", 15_mph},
      {"usW13-1-20", 20_mph}, {"usW13-1-25", 25_mph}, {"usW13-1-30", 30_mph},
      {"usW13-1-35", 35_mph}, {"usW13-1-40", 40_mph}, {"usW13-1-45", 45_mph},
      {"usW13-1-50", 50_mph}, {"usW13-1-55", 55_mph}, {"usW13-1-60", 60_mph},
      {"usW13-1-65", 65_mph}, {"usW13-1-70", 70_mph},

      //default values for urban and rural roads (Maximum Speed).
      {"usR2-1", 30_mph}, {"usR2-1-Rural", 45_mph},
      //values for speed limits in Ontario (Maximum Speed).
      {"usR2-1-5", 5_mph}, {"usR2-1-10", 10_mph}, {"usR2-1-15", 15_mph},
      {"usR2-1-20", 20_mph}, {"usR2-1-25", 25_mph}, {"usR2-1-30", 30_mph},
      {"usR2-1-35", 35_mph}, {"usR2-1-40", 40_mph}, {"usR2-1-45", 45_mph},
      {"usR2-1-50", 50_mph}, {"usR2-1-55", 55_mph}, {"usR2-1-60", 60_mph},
      {"usR2-1-65", 65_mph}, {"usR2-1-70", 70_mph}};
  try {
    return StrToVelocity.at(typeString);
  } catch (std::out_of_range&) {
    // try to interpret typeString directly as velocity
    Attribute asAttribute(typeString);
    auto velocity = asAttribute.asVelocity();
    if (!!velocity) {
      return *velocity;
    }
    throw lanelet::InterpretationError("Unable to interpret the velocity information from " + typeString);
  }
}
}  // namespace

Optional<SpeedLimitInformation> AmericanVehicle::speedLimit(const RegulatoryElementConstPtrs& regelems) const {
  for (const auto& regelem : regelems) {
    auto speedLimit = std::dynamic_pointer_cast<const SpeedLimit>(regelem);
    if (!!speedLimit) {
      return SpeedLimitInformation{trafficSignToVelocity(speedLimit->type()), true};
    }
  }
  return {};
}

/*
CountrySpeedLimits is from Struct in GenericTrafficRules.h:
  SpeedLimitInformation vehicleUrbanRoad;
  SpeedLimitInformation vehicleNonurbanRoad;
  SpeedLimitInformation vehicleUrbanHighway;
  SpeedLimitInformation vehicleNonurbanHighway;
  SpeedLimitInformation playStreet;
  SpeedLimitInformation pedestrian;
  SpeedLimitInformation bicycle;
*/
CountrySpeedLimits americanSpeedLimits() {
  using namespace units::literals;
  //Note: I am unsure about the pedestrian limit value
  return {{30_mph}, {45_mph}, {55_mph}, {75_mph}, {25_mph}, {5_mph, false}, {20_mph}};
}

}  // namespace traffic_rules
}  // namespace lanelet
