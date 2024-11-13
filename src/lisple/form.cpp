
#include "form.h"

#include <algorithm>
#include <compare>
#include <iterator>
#include <cmath>
#include <stddef.h>
#include <stdexcept>
#include <sstream>

#include "type.h"
#include "exception.h"

namespace Lisple
{
  const int INT_CONSTANTS_SIZE = 1001;
  std::vector<std::shared_ptr<Number>> INT_CONSTANTS = []() {
    std::vector<std::shared_ptr<Number>> ints(INT_CONSTANTS_SIZE);

    for (int i=0; i<INT_CONSTANTS_SIZE; i++)
    {
      ints[i] = std::make_shared<Number>(i);
    }

    return ints;
  }();

  Object::Object(Form form)
    : type(form)
  {

  }

  Form Object::get_type() const
  {
    return type;
  }

  void Object::append(const sptr_sobject&)
  {
    throw std::runtime_error("S-expression: Cannot append to value type");
  }

  unsigned int Object::size() const
  {
    return 1;
  }

  bool Object::is_truthy() const
  {
    return true;
  }

  bool Object::has_key(const Object&) const
  {
    return false;
  }

  Object& Object::get_property(const Object& key) const
  {
    return *this->get_sptr_property(key);
  }

  sptr_sobject Object::get_sptr_property(const Object&) const
  {
    return NIL;
  }

  void Object::set_property(const Object &key, sptr_sobject&)
  {
    throw InvocationException("Cannot set property '" + key.to_string() + "' of " + this->to_string(2));
  }

  void Object::set_property(Context*, const Object& key, sptr_sobject& value)
  {
    this->set_property(key, value);
  }

  std::shared_ptr<Object> Object::execute(Context&, sptr_sobject_v&)
  {
    throw InvocationException(this->to_string(2) + " cannot be executed");
  }

  sptr_sobject_v& Object::get_children()
  {
    throw InvocationException(this->to_string(2) + " is not a sequence");
  }

  bool Object::has_value(const std::string&) const
  {
    return false;
  }

  bool Object::has_value(const float) const
  {
    return false;
  }

  bool Object::has_value(const int) const
  {
    return false;
  }

  bool Object::operator!=(const Object& other) const
  {
    return !(other == *this);
  }

  /*
   * Nil - no values here
   */
  Nil::Nil() : Object(Form::NIL)
  {
  }

  std::string Nil::to_string(int) const
  {
    return "nil";
  }

  bool Nil::is_truthy() const
  {
    return false;
  }

  std::shared_ptr<Object> Nil::execute(Context&, sptr_sobject_v&)
  {
    throw InvocationException("Execution of nil");
  }

  bool Nil::operator==(const Object& other) const
  {
    return type == other.get_type();
  }

  /*
   * Discard - Ignored form
   */
  Discard::Discard() : Object(Form::DISCARD)
  {
  }

  void Discard::append(const sptr_sobject& value)
  {
    this->value = value;
  }

  std::string Discard::to_string(int) const
  {
    return "#_" + value->to_string();
  }

  std::shared_ptr<Object> Discard::execute(Context&, sptr_sobject_v&)
  {
    throw InvocationException("Execution of form comment");
  }

  bool Discard::operator==(const Object& other) const
  {
    return type == other.get_type();
  }

  /* QualifiableStringValue */
  QualifiableStringValue::QualifiableStringValue(Form type, const std::string& value)
    : Value(type, value)
  {
    std::stringstream ss(value);
    std::vector<std::string> tokens;
    std::string token;
    while(std::getline(ss, token, '/'))
    {
      tokens.push_back(token);
    }

    switch (tokens.size())
    {
     case 0:
      throw IdentifierException("Invalid symbol. Cannot be empty.");
     case 1:
      this->ns_qualifier = "";
      this->identifier = tokens.front();
      break;
     case 2:
      this->ns_qualifier = tokens.front();
      this->identifier = tokens.back();
      break;
     default:
      throw IdentifierException("Invalid symbol: " + value);
    }
  }

  bool QualifiableStringValue::is_qualified() const
  {
    return !ns_qualifier.empty();
  }

  const std::string& QualifiableStringValue::get_qualifier() const
  {
    return ns_qualifier;
  }

  const std::string& QualifiableStringValue::get_identifier() const
  {
    return identifier;
  }


  /**
   * String form
   */
  String::String(const std::string& value)
    : Value(Form::STRING, value)
  {
  }

  bool String::has_value(const std::string& value) const
  {
    return this->value == value;
  }

  std::string String::to_string(int) const
  {
    return "\"" + value + "\"";
  }

  unsigned int String::size() const
  {
    return this->value.size();
  }

  std::shared_ptr<String> String::make(const std::string& value)
  {
    return std::make_shared<String>(value);
  }

  /**
   * Boolean form
   */
  Boolean::Boolean(const bool value)
    : Value(Form::BOOLEAN, value)
  {

  }

  std::string Boolean::to_string(int) const
  {
    if (value)
    {
      return "true";
    }

    return "false";
  }

  bool Boolean::is_truthy() const
  {
    return value;
  }

  std::shared_ptr<Boolean> Boolean::wrap(bool value)
  {
    return value ? B_TRUE : B_FALSE;
  }

  /**
   * Char form
   */
  Char::Char(const char value)
    : Value(Form::CHAR, value)
  {

  }

  std::string Char::to_string(int) const
  {
    return "'" + std::string {value} + "'";
  }

  std::shared_ptr<Char> Char::make(char value)
  {
    return std::make_shared<Char>(value);
  }


  /**
   * Key form
   */
  Key::Key(const std::string& value)
    : QualifiableStringValue(Form::KEY, value)
  {
  }

  bool Key::has_value(const std::string& value) const
  {
    return this->value == value;
  }

  std::string Key::to_string(int) const
  {
    return ":" + value;
  }

  bool Key::operator<(const Key& other) const
  {
    return this->value < other.value;
  }

  std::shared_ptr<Object> Key::execute(Context&, sptr_sobject_v& args)
  {
    if (args.size() == 1)
      {
        return args.front()->get_sptr_property(*this);
      }
    throw InvocationException("Get value by key requires a single argument.");
  }

  std::shared_ptr<Key> Key::make(const std::string& value)
  {
    return std::make_shared<Key>(value);
  }

  /**
   * QSymbol - a quoted word
   */
  QSymbol::QSymbol(const std::string& value)
    : QualifiableStringValue(Form::SYMBOL, value)
  {
  }

  bool QSymbol::has_value(const std::string& value) const
  {
    return this->value == value;
  }

  std::string QSymbol::to_string(int) const
  {
    return "'" + value;
  }

  /**
   * Word - an actual word
   */
  Word::Word(const std::string& value)
    : QualifiableStringValue(Form::WORD, value)
  {
  }

  bool Word::has_value(const std::string& value) const
  {
    return this->value == value;
  }

  std::string Word::to_string(int) const
  {
    return this->value;
  }

  std::shared_ptr<Word> Word::make(const std::string& value)
  {
    return std::make_shared<Word>(value);
  }

  /**
   * Number
   */
  Number::Number(int value)
    : Value(Form::NUMBER, static_cast<float>(value))
    , num_type(NumberType::INT)
  {
  }

  Number::Number(unsigned int value)
    : Number(static_cast<int>(value))
  {
  }

  Number::Number(float value)
    : Value(Form::NUMBER, value)
    , num_type(NumberType::FLOAT)
  {
  }

  Number::Number(long value)
    : Value(Form::NUMBER, value)
    , num_type(NumberType::LONG)
  {
  }

  bool Number::is_num_type(NumberType type) const
  {
    return num_type == type;
  }

  int Number::int_value() const
  {
    return round(value);
  }

  float Number::float_value() const
  {
    return value;
  }

  long Number::long_value() const
  {
    return value;
  }

  std::shared_ptr<Number> Number::operator+(const Number& other)
  {
    if (num_type == NumberType::INT &&
        other.num_type == NumberType::INT)
    {
      return make(static_cast<int>(value + other.value));
    }

    return make(value + other.value);
  }

  std::shared_ptr<Number> Number::operator-(const Number& other)
  {
    if (num_type == NumberType::INT &&
        other.num_type == NumberType::INT)
    {
      return make(static_cast<int>(value - other.value));
    }

    return make(value - other.value);
  }

  std::shared_ptr<Number> Number::operator*(const Number& other)
  {
    if (num_type == NumberType::INT &&
        other.num_type == NumberType::INT)
    {
      return make(static_cast<int>(value * other.value));
    }

    return make(value * other.value);
  }

  std::shared_ptr<Number> Number::operator/(const Number& other)
  {
    int int_result = value / other.value;
    float float_result = value / other.value;
    if (int_result == float_result)
    {
      return make(int_result);
    }
    return make(float_result);
  }

  std::shared_ptr<Number> Number::flip_sign()
  {
    float flipped = -value;

    switch (num_type)
    {
    case NumberType::INT:
      return Number::make(static_cast<int>(flipped));
    case NumberType::FLOAT:
      return Number::make(flipped);
    case NumberType::LONG:
      return Number::make(static_cast<long>(flipped));
    }

    return Number::make(flipped);
  }

  bool Number::has_value(const int value) const
  {
    return this->value == value;
  }

  bool Number::has_value(const float value) const
  {
    return this->value == value;
  }

  std::string Number::to_string(int) const
  {
    switch (num_type)
    {
    case NumberType::INT:
      return std::to_string(static_cast<int>(value));
    case NumberType::LONG:
      return std::to_string(static_cast<long>(value));
    case NumberType::FLOAT:
      return std::to_string(value);
    }

    return std::to_string(value);
  }

  std::shared_ptr<Number> Number::make(int value)
  {
    if (value >= 0 && value < INT_CONSTANTS_SIZE)
    {
      return INT_CONSTANTS[value];
    }
    return std::make_shared<Number>(value);
  }

  std::shared_ptr<Number> Number::make(unsigned int value)
  {
    return std::make_shared<Number>(value);
  }

  std::shared_ptr<Number> Number::make(float value)
  {
    return std::make_shared<Number>(value);
  }

  std::shared_ptr<Number> Number::make(long value)
  {
    return std::make_shared<Number>(value);
  }

  std::shared_ptr<Number> Number::make(const std::string& str_value)
  {
    try
    {
      if (str_value.find(".") == std::string::npos)
      {
        return Number::make(stoi(str_value));
      }
      else
      {
        return Number::make(stof(str_value));
      }
    }
    catch (std::runtime_error& error)
    {
      throw TypeError("Not a valid number: \"" + str_value + "\"");
    }
  }


  /**
   * Sexpression - Abstract base for lists, arrays and maps
   */
  Sexpression::Sexpression(Form form)
    : Object(form)
  {

  }

  std::shared_ptr<Sexpression> Sexpression::new_sequence(Form type)
  {
    switch(type)
      {
      case Form::LIST:
        return std::make_shared<List>();
      case Form::ARRAY:
        return std::make_shared<Array>();
      case Form::MAP:
        return std::make_shared<Map>();
      default:
        throw LispleException("Type is not a sequence");
      }
  }

  Sexpression::Sexpression(Form form, const sptr_sobject_v& children)
    : Object(form)
    , children(children)
  {
  }

  sptr_sobject Sexpression::get_sptr_property(const Object& form) const
  {
    for (auto i = this->children.begin(); i < this->children.end(); i+= 2)
    {
      if (**i == form)
      {
        if (i == this->children.end())
        {
          return NIL;
        }
        return *std::next(i);
      }
    }

    return NIL;
  }

  void Sexpression::set_property(const Object&, sptr_sobject&)
  {
    throw InvocationException("Set on sequences not implemented.");
  }

  void Sexpression::append(const sptr_sobject& child)
  {
    this->children.push_back(child);
  }

  std::shared_ptr<Object>& Sexpression::head()
  {
    return children.empty() ? NIL : children.front();
  }

  sptr_sobject_v Sexpression::tail()
  {
    sptr_sobject_v tail;
    if (children.size() < 2) return tail;

    tail.reserve(children.size()-1);
    for (size_t i=1; i<children.size(); i++)
    {
      tail.push_back(children[i]);
    }

    return tail;
  }

  std::string Sexpression::to_string(int depth) const
  {
    std::string str;
    if (depth == 0)
    {
      str = "...";
    }
    else
    {
      for (auto& element : children)
      {
        if (!str.empty())
        {
          str += " ";
        }
        if(element)
        {
          str += element->to_string(depth == -1 ? -1 : depth-1);
        }
      }
    }
    return this->lpar() + str + this->rpar();
  }

  bool Sexpression::operator==(const Object& other) const
  {
    if (type != other.get_type())
    {
      return false;
    }
    auto& other_sexp = dynamic_cast<const Sexpression&>(other);
    if (children.size() != other_sexp.children.size())
    {
      return false;
    }
    for (size_t i=0; i<children.size(); i++)
    {
      if (*children[i] != *other_sexp.children[i])
      {
        return false;
      }
    }
    return true;
  }

  sptr_sobject_v& Sexpression::get_children()
  {
    return children;
  }

  unsigned int Sexpression::size() const
  {
    return this->children.size();
  }

  /**
   * List
   */
  List::List(bool q)
    : Sexpression(Form::LIST)
    , q(q)
  {
  }

  List::List(const sptr_sobject_v& children, bool q)
    : Sexpression(Form::LIST, children)
    , q(q)
  {

  }

  std::shared_ptr<List> List::from(sptr_sobject head, sptr_sobject_v tail)
  {
    tail.insert(tail.begin(), head);
    return std::make_shared<List>(tail);
  }

  std::shared_ptr<List> List::insert(unsigned int index, sptr_sobject& element)
  {
    sptr_sobject_v new_elements(children.size()+1);
    std::copy(children.begin(), children.begin()+index, new_elements.begin());
    new_elements[index] = element;
    std::copy(children.begin()+index, children.end(), new_elements.begin()+index+1 );
    return std::make_shared<List>(new_elements, q);
  }

  bool List::is_quoted() const
  {
    return q;
  }

  std::shared_ptr<List> List::quot_toggle() const
  {
    return std::make_shared<List>(children, !q);
  }

  std::shared_ptr<Object> List::execute(Context& ctx)
  {
    auto head = this->head();
    auto tail = this->tail();
    return head->execute(ctx, tail);
  }

  std::shared_ptr<Object> List::execute(Context&, sptr_sobject_v&)
  {
    throw InvocationException("Illegal invocation of list: " + this->to_string(2));
  }

  const std::string List::lpar() const
  {
    return q ? "'(" : "(";
  }

  const std::string List::rpar() const
  {
    return ")";
  }

  /**
   * Array
   */
  Array::Array()
    : Sexpression(Form::ARRAY)
  {
  }

  Array::Array(const sptr_sobject_v& children)
    : Sexpression(Form::ARRAY, children)
  {
  }

  const std::string Array::lpar() const
  {
    return "[";
  }

  const std::string Array::rpar() const
  {
    return "]";
  }

  std::shared_ptr<Array> Array::make(const sptr_sobject_v& children)
  {
    return std::make_shared<Array>(children);
  }

  /**
   * Map
   */
  Map::Map()
    : Sexpression(Form::MAP)
  {
  }

  Map::Map(const sptr_sobject_v& children)
    : Sexpression(Form::MAP, children)
  {
    this->validate_keys();
  }

  void Map::validate_keys() const
  {
    if (this->children.size() % 2 != 0)
    {
      throw TypeError("Odd number of symbols in Map form: " + this->to_string(2));
    }

    std::vector<Object*> keys = this->keys();
    for (auto key : keys)
    {
      int count = std::count_if(keys.begin(),
                                keys.end(),
                                [key](const Object* other)
                                {
                                  return *key == *other;
                                });
      if (count != 1)
      {
        throw TypeError("Duplicate key " + key->to_string() + " in Map: " +this->to_string(2));
      }
    }
  }

  const std::vector<Object*> Map::keys() const
  {
    std::vector<Object*> keys;
    keys.reserve(size());
    for (size_t i = 0; i<children.size(); i+=2)
    {
      keys.push_back(children[i].get());
    }
    return keys;
  }

  sptr_sobject_v Map::key_ptrs() const
  {
    sptr_sobject_v keys;
    keys.reserve(size());
    for (size_t i=0; i<children.size(); i+=2)
    {
      keys.push_back(children[i]);
    }
    return keys;
  }

  bool Map::has_key(const Object& key) const
  {
    for (size_t i = 0; i < children.size(); i += 2)
    {
      if (*children[i] == key)
      {
        return true;
      }
    }
    return false;
  }

  unsigned int Map::size() const
  {
    return Sexpression::size() / 2;
  }

  void Map::set_property(const Object& key, sptr_sobject& value)
  {
    for (size_t i=0; i<children.size(); i+=2)
    {
      if (*children[i] == key)
      {
        children[i+1] = value;
        return;
      }
    }
    throw InvocationException("No key " + key.to_string() + " in " + this->to_string());
  }

  void Map::set_property(const sptr_sobject& key, const sptr_sobject& value)
  {
    if (this->has_key(*key))
    {
      sptr_sobject val = value;
      this->set_property(*key, val);
    }
    else
    {
      this->append(key);
      this->append(value);
    }
  }

  sptr_sobject Map::remove_key(const Object& key)
  {
    sptr_sobject removed_val = NIL;

    int index = -1;

    for (size_t i=0; i<children.size(); i+=2)
    {
      if (*children[i] == key)
      {
        index = i;
        break;
      }
    }

    if (index != -1)
    {
      removed_val = children[index+1];
      children.erase(children.begin() + index, children.begin() + index+2);
    }

    return removed_val;
  }

  const std::string Map::lpar() const
  {
    return "{";
  }

  const std::string Map::rpar() const
  {
    return "}";
  }

  std::shared_ptr<Map> Map::make(const sptr_sobject_v& children)
  {
    return std::make_shared<Map>(children);
  }

  template class Value<std::string>;
  template class Value<bool>;
  template class Value<char>;
  template class Value<float>;

}
