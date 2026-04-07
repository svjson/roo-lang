#ifndef LISPLETEST__HOST__VEHICLE_IMPL_H
#define LISPLETEST__HOST__VEHICLE_IMPL_H

#include <string>

#include <lisple/form.h>
#include <lisple/host/type.h>

namespace LispleTest
{
  // ===============================================================
  // Vehicle example classes
  // ===============================================================

  class RegNumber
  {
    std::string letters;
    std::string numbers;

   public:
    RegNumber(const std::string& letters, const std::string& numbers);
    RegNumber(const RegNumber& other);

    const std::string& get_letters() const;
    const std::string& get_numbers() const;

    void operator=(const RegNumber& other);

    bool operator<(const RegNumber& other) const;
  };

  class VehicleModel
  {
    const std::string model_name;
    int seats;

   public:
    VehicleModel(const std::string& model_name, int seats);

    const std::string& get_model_name() const;
    int get_seats() const;

    void set_seats(int seats);

    bool operator==(const VehicleModel& other) const;
  };

  class Vehicle
  {
    VehicleModel model;
    RegNumber reg_number;

   public:
    Vehicle(const VehicleModel& model, const RegNumber& reg_number);

    const VehicleModel& get_model() const;

    const RegNumber& get_reg_number() const;
    void set_reg_number(const RegNumber& reg_number);
  };

} // namespace LispleTest

#endif /* LISPLETEST__HOST__VEHICLE_IMPL_H */
