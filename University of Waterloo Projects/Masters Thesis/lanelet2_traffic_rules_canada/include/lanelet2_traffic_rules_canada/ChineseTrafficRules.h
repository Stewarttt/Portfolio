#pragma once
#include <lanelet2_core/utility/Units.h>

#include <lanelet2_traffic_rules_canada/ExtendedTrafficRules.h>

namespace lanelet {
namespace traffic_rules {

CountrySpeedLimits chineseSpeedLimits();

class ExtendedTrafficRules;

class ChineseVehicle : public ExtendedTrafficRules {
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
  CountrySpeedLimits speedLimits_{chineseSpeedLimits()};
};

class ChinesePedestrian : public ExtendedTrafficRules {
 public:
  using ExtendedTrafficRules::ExtendedTrafficRules;

 protected:
  Optional<bool> canPass(const RegulatoryElementConstPtrs& /*regElems*/) const override { return {}; }
  const CountrySpeedLimits& countrySpeedLimits() const override { return speedLimits_; }
  Optional<SpeedLimitInformation> speedLimit(const RegulatoryElementConstPtrs& /*regelems*/) const override {
    return {};
  }

 private:
  CountrySpeedLimits speedLimits_{chineseSpeedLimits()};
};

class ChineseBicycle : public ExtendedTrafficRules {
 public:
  using ExtendedTrafficRules::ExtendedTrafficRules;

 protected:
  Optional<bool> canPass(const RegulatoryElementConstPtrs& /*regElems*/) const override { return {}; }
  const CountrySpeedLimits& countrySpeedLimits() const override { return speedLimits_; }
  Optional<SpeedLimitInformation> speedLimit(const RegulatoryElementConstPtrs& /*regelems*/) const override {
    return {};
  }

 private:
  CountrySpeedLimits speedLimits_{chineseSpeedLimits()};
};
}  // namespace traffic_rules
}  // namespace lanelet
