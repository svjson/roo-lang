
#include "vehicle_impl.h"

namespace LispleTest
{
  // ===============================================================
  // Vehicle example - with nested type RegNumber
  // ===============================================================

  /* RegNumber */
  RegNumber::RegNumber(const std::string& letters, const std::string& numbers)
    : letters(letters)
    , numbers(numbers)
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

  bool RegNumber::operator<(const RegNumber& other) const
  {
    return this->letters < other.letters ||
           (this->letters == other.letters && this->numbers < other.numbers);
  }

  /* Vehicle */
  Vehicle::Vehicle(const std::string& model_name, int seats)
    : model_name(model_name)
    , seats(seats)
  {
  }

  const std::string& Vehicle::get_model_name() const
  {
    return model_name;
  }

  int Vehicle::get_seats() const
  {
    return seats;
  }

  void Vehicle::set_seats(int seats)
  {
    this->seats = seats;
  }

  bool Vehicle::operator==(const Vehicle& other) const
  {
    return this->model_name == other.model_name && this->seats == other.seats;
  }

} // namespace LispleTest
