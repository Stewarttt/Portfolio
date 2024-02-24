#include "lanelet2_traffic_rules_canada/ChineseTrafficRules.h"

#include <lanelet2_core/Forward.h>
#include <lanelet2_core/primitives/BasicRegulatoryElements.h>
#include <lanelet2_core/utility/Units.h>

#include "lanelet2_traffic_rules/Exceptions.h"
#include "lanelet2_traffic_rules/TrafficRulesFactory.h"

using namespace std::string_literals;

namespace lanelet {
namespace traffic_rules {

namespace {
RegisterTrafficRules<ChineseVehicle> cvRules(lanelet::traffic_rules::China, Participants::Vehicle);
RegisterTrafficRules<ChinesePedestrian> cpRules(lanelet::traffic_rules::China, Participants::Pedestrian);
RegisterTrafficRules<ChineseBicycle> cbRules(lanelet::traffic_rules::China, Participants::Bicycle);

Velocity trafficSignToVelocity(const std::string& typeString) {
  using namespace lanelet::units::literals;
  const static std::map<std::string, Velocity> StrToVelocity{
      //values for speed limits in China (Maximum Speed).
      {"cn10kmh", 10_kmh}, {"cn20kmh", 20_kmh}, {"cn30kmh", 30_kmh},
      {"cn40kmh", 40_kmh}, {"cn50kmh", 50_kmh}, {"cn60kmh", 60_kmh},
      {"cn70kmh", 70_kmh}, {"cn80kmh", 80_kmh}, {"cn90kmh", 90_kmh},
      {"cn100kmh", 100_kmh}, {"cn110kmh", 110_kmh}, {"cn120kmh", 120_kmh}};
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

Optional<SpeedLimitInformation> ChineseVehicle::speedLimit(const RegulatoryElementConstPtrs& regelems) const {
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
CountrySpeedLimits chineseSpeedLimits() {
  using namespace units::literals;
  //Note: I am unsure about the pedestrian limit value
  return {{30_kmh}, {40_kmh}, {80_kmh}, {120_kmh}, {30_kmh}, {5_kmh, false}, {25_kmh}};
}

}  // namespace traffic_rules
}  // namespace lanelet
