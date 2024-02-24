#pragma once
#include <lanelet2_core/utility/Units.h>

#include <lanelet2_traffic_rules_canada/ExtendedTrafficRules.h>

namespace lanelet {
namespace traffic_rules {

CountrySpeedLimits canadianSpeedLimits();

class ExtendedTrafficRules;

class CanadianVehicle : public ExtendedTrafficRules {
 public:
  using ExtendedTrafficRules::ExtendedTrafficRules;

  bool canPass(const lanelet::ConstLanelet& lanelet) const override {
    return ExtendedTrafficRules::canPass(lanelet);
  }
  // using areas is disallowed for vehicles in normal driving mode
  bool canPass(const ConstArea& /*area*/) const override { return false; }

 protected:
  Optional<bool> canPass(const RegulatoryElementConstPtrs& /*regElems*/) const override { return {}; }
  const CountrySpeedLimits& countrySpeedLimits() const override { return speedLimits_; }
  Optional<SpeedLimitInformation> speedLimit(const RegulatoryElementConstPtrs& regelems) const override;

 private:
  CountrySpeedLimits speedLimits_{canadianSpeedLimits()};
};

class CanadianPedestrian : public ExtendedTrafficRules {
 public:
  using ExtendedTrafficRules::ExtendedTrafficRules;

 protected:
  Optional<bool> canPass(const RegulatoryElementConstPtrs& /*regElems*/) const override { return {}; }
  const CountrySpeedLimits& countrySpeedLimits() const override { return speedLimits_; }
  Optional<SpeedLimitInformation> speedLimit(const RegulatoryElementConstPtrs& /*regelems*/) const override {
    return {};
  }

 private:
  CountrySpeedLimits speedLimits_{canadianSpeedLimits()};
};

class CanadianBicycle : public ExtendedTrafficRules {
 public:
  using ExtendedTrafficRules::ExtendedTrafficRules;

 protected:
  Optional<bool> canPass(const RegulatoryElementConstPtrs& /*regElems*/) const override { return {}; }
  const CountrySpeedLimits& countrySpeedLimits() const override { return speedLimits_; }
  Optional<SpeedLimitInformation> speedLimit(const RegulatoryElementConstPtrs& /*regelems*/) const override {
    return {};
  }

 private:
  CountrySpeedLimits speedLimits_{canadianSpeedLimits()};
};
}  // namespace traffic_rules
}  // namespace lanelet
