#include "lanelet2_traffic_rules_canada/CanadianTrafficRules.h"

#include <lanelet2_core/Forward.h>
#include <lanelet2_core/primitives/BasicRegulatoryElements.h>
#include <lanelet2_core/utility/Units.h>

#include "lanelet2_traffic_rules/Exceptions.h"
#include "lanelet2_traffic_rules/TrafficRulesFactory.h"

using namespace std::string_literals;

namespace lanelet {
namespace traffic_rules {

namespace {
RegisterTrafficRules<CanadianVehicle> cvRules(lanelet::traffic_rules::Canada, Participants::Vehicle);
RegisterTrafficRules<CanadianPedestrian> cpRules(lanelet::traffic_rules::Canada, Participants::Pedestrian);
RegisterTrafficRules<CanadianBicycle> cbRules(lanelet::traffic_rules::Canada, Participants::Bicycle);

Velocity trafficSignToVelocity(const std::string& typeString) {
  using namespace lanelet::units::literals;

  //speed limits are based on Ontario Canada
  const static std::map<std::string, Velocity> StrToVelocity{
      //values for school zones (School Zone).
      {"caRB-6", 30_kmh}, {"caRB-6-30", 30_kmh}, {"caRB-6-40", 40_kmh}, {"caRB-6-50", 50_kmh},

      //default values for urban and rural roads (Maximum Speed).
      {"caRB-1", 50_kmh}, {"caRB-1-Rural", 80_kmh},
      //values for speed limits in Ontario (Maximum Speed).
      {"caRB-1-10", 10_kmh}, {"caRB-1-20", 20_kmh}, {"caRB-1-30", 30_kmh},
      {"caRB-1-40", 40_kmh}, {"caRB-1-50", 50_kmh}, {"caRB-1-60", 60_kmh},
      {"caRB-1-70", 70_kmh}, {"caRB-1-80", 80_kmh}, {"caRB-1-90", 90_kmh},
      {"caRB-1-100", 100_kmh}, {"caRB-1-110", 110_kmh},

      //default values for urban and rural roads (Maximum Speed with KM/H).
      {"caRB-1A", 50_kmh}, {"caRB-1A-Rural", 80_kmh},
      //values for speed limits in Ontario (Maximum Speed with KM/H).
      {"caRB-1A-10", 10_kmh}, {"caRB-1A-20", 20_kmh}, {"caRB-1A-30", 30_kmh},
      {"caRB-1A-40", 40_kmh}, {"caRB-1A-50", 50_kmh}, {"caRB-1A-60", 60_kmh},
      {"caRB-1A-70", 70_kmh}, {"caRB-1A-80", 80_kmh}, {"caRB-1A-90", 90_kmh},
      {"caRB-1A-100", 100_kmh}, {"caRB-1A-110", 110_kmh},

      //default values for urban and rural roads (Maximum Speed and Begins).
      {"caRB-2", 50_kmh}, {"caRB-2-Rural", 80_kmh},
      //values for speed limits in Ontario (Maximum Speed and Begins).
      {"caRB-2-10", 10_kmh}, {"caRB-2-20", 20_kmh}, {"caRB-2-30", 30_kmh},
      {"caRB-2-40", 40_kmh}, {"caRB-2-50", 50_kmh}, {"caRB-2-60", 60_kmh},
      {"caRB-2-70", 70_kmh}, {"caRB-2-80", 80_kmh}, {"caRB-2-90", 90_kmh},
      {"caRB-2-100", 100_kmh}, {"caRB-2-110", 110_kmh},

      //default values for urban and rural roads (Maximum Speed KM/H and Begins).
      {"caRB-3", 50_kmh}, {"caRB-3-Rural", 80_kmh},
      //values for speed limits in Ontario (Maximum Speed KM/H and Begins).
      {"caRB-3-10", 10_kmh}, {"caRB-3-20", 20_kmh}, {"caRB-3-30", 30_kmh},
      {"caRB-3-40", 40_kmh}, {"caRB-3-50", 50_kmh}, {"caRB-3-60", 60_kmh},
      {"caRB-3-70", 70_kmh}, {"caRB-3-80", 80_kmh}, {"caRB-3-90", 90_kmh},
      {"caRB-3-100", 100_kmh}, {"caRB-3-110", 110_kmh},

      //default values for construction zones
      {"caTC-36-10", 10_kmh}, {"caTC-36-20", 20_kmh}, {"caTC-36-30", 30_kmh},
      {"caTC-36-40", 40_kmh}, {"caTC-36-50", 50_kmh}, {"caTC-36-60", 60_kmh},
      {"caTC-36-70", 70_kmh}, {"caTC-36-80", 80_kmh}, {"caTC-36-90", 90_kmh}};
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

Optional<SpeedLimitInformation> CanadianVehicle::speedLimit(const RegulatoryElementConstPtrs& regelems) const {
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
CountrySpeedLimits canadianSpeedLimits() {
  using namespace units::literals;
  //Note: I am unsure about the pedestrian limit value
  return {{50_kmh}, {80_kmh}, {100_kmh}, {100_kmh}, {30_kmh}, {5_kmh, false}, {32_kmh}};
}

}  // namespace traffic_rules
}  // namespace lanelet
