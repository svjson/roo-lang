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
    const std::string letters;
    const std::string numbers;

   public:
    RegNumber(const std::string& letters, const std::string& numbers);

    const std::string& get_letters() const;
    const std::string& get_numbers() const;

    bool operator<(const RegNumber& other) const;
  };

  class Vehicle
  {
    const std::string model_name;
    int seats;

   public:
    Vehicle(const std::string& model_name, int seats);

    const std::string& get_model_name() const;
    int get_seats() const;

    void set_seats(int seats);

    bool operator==(const Vehicle& other) const;
  };

} // namespace LispleTest

#endif /* LISPLETEST__HOST__VEHICLE_IMPL_H */
