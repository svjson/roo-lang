
#include "vehicle_impl.h"

namespace RooTest
{
  // ===============================================================
  // Vehicle example - with nested type RegNumber
  // ===============================================================

  /* Vehicle */
  Vehicle::Vehicle(const VehicleModel& model, const RegNumber& reg_number)
    : model(model)
    , reg_number(reg_number)
  {
  }

  const VehicleModel& Vehicle::get_model() const
  {
    return model;
  }

  const RegNumber& Vehicle::get_reg_number() const
  {
    return reg_number;
  }

  void Vehicle::set_reg_number(const RegNumber& reg_number)
  {
    this->reg_number = reg_number;
  }

  /* RegNumber */
  RegNumber::RegNumber(const std::string& letters, const std::string& numbers)
    : letters(letters)
    , numbers(numbers)
  {
  }

  RegNumber::RegNumber(const RegNumber& other)
    : letters(other.letters)
    , numbers(other.numbers)
  {
  }

  const std::string& RegNumber::get_letters() const
  {
    return letters;
  }

  const std::string& RegNumber::get_numbers() const
  {
    return numbers;
  }

  void RegNumber::operator=(const RegNumber& other)
  {
    this->letters = other.letters;
    this->numbers = other.numbers;
  }

  bool RegNumber::operator<(const RegNumber& other) const
  {
    return this->letters < other.letters ||
           (this->letters == other.letters && this->numbers < other.numbers);
  }

  /* VehicleModel */
  VehicleModel::VehicleModel(const std::string& model_name, int seats)
    : model_name(model_name)
    , seats(seats)
  {
  }

  const std::string& VehicleModel::get_model_name() const
  {
    return model_name;
  }

  int VehicleModel::get_seats() const
  {
    return seats;
  }

  void VehicleModel::set_seats(int seats)
  {
    this->seats = seats;
  }

  bool VehicleModel::operator==(const VehicleModel& other) const
  {
    return this->model_name == other.model_name && this->seats == other.seats;
  }

} // namespace RooTest
