
#include "form.h"

#include <algorithm>
#include <iterator>
#include <cmath>
#include <stddef.h>
#include <stdexcept>
#include <sstream>

#include "type.h"
#include "lisple_exception.h"

namespace Lisple
{
  /*
   * Object - abstract base class for all forms
   **/
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
    return 0;
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
    throw Lisple::InvocationException("Cannot set property '" + key.to_string() + "' of " + this->to_string());
  }

  void Object::set_property(Context*, const Object& key, sptr_sobject& value)
  {
    this->set_property(key, value);
  }

  std::shared_ptr<Object> Object::execute(Context&, sptr_sobject_v&)
  {
    throw Lisple::InvocationException(this->to_string() + " cannot be executed");
  }

  sptr_sobject_v& Object::get_children()
  {
    throw Lisple::InvocationException(this->to_string() + " is not a sequence");
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

  std::string Nil::to_string() const
  {
    return "nil";
  }

  std::shared_ptr<Object> Nil::execute(Context&, sptr_sobject_v&)
  {
    throw Lisple::InvocationException("Execution of nil");
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

  std::string Discard::to_string() const
  {
    return "#_" + value->to_string();
  }

  std::shared_ptr<Object> Discard::execute(Context&, sptr_sobject_v&)
  {
    throw Lisple::InvocationException("Execution of form comment");
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
      throw IdentifierException("Invalid synbol. Cannot be empty.");
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
    : Value(Lisple::Form::STRING, value)
  {
  }

  bool String::has_value(const std::string& value) const
  {
    return this->value == value;
  }

  std::string String::to_string() const
  {
    return "\"" + value + "\"";
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

  std::string Boolean::to_string() const
  {
    if (value)
    {
      return "true";
    }

    return "false";
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

  std::string Char::to_string() const
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

  std::string Key::to_string() const
  {
    return ":" + value;
  }

  bool Key::operator<(const Key& other) const
  {
    return this->value < other.value;
  }

  std::shared_ptr<Lisple::Object> Key::execute(Lisple::Context&, sptr_sobject_v& args)
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
    : Value(Form::SYMBOL, value)
  {
  }

  bool QSymbol::has_value(const std::string& value) const
  {
    return this->value == value;
  }

  std::string QSymbol::to_string() const
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

  std::string Word::to_string() const
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

  Number::Number(float value)
    : Value(Form::NUMBER, value)
    , num_type(NumberType::FLOAT)
  {
  }

  /* Use of this one should be restricted to internal use */
  Number::Number(const std::string& value)
    : Value(Form::NUMBER, std::stof(value))
    , num_type(value.find(".") == std::string::npos ? NumberType::INT : NumberType::FLOAT)
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

  bool Number::has_value(const int value) const
  {
    return this->value == value;
  }

  bool Number::has_value(const float value) const
  {
    return this->value == value;
  }

  std::string Number::to_string() const
  {
    if (num_type == NumberType::INT)
    {
      return std::to_string((int) value);
    }
    return std::to_string(value);
  }

  std::shared_ptr<Number> Number::make(int value)
  {
    return std::make_shared<Number>(value);
  }

  std::shared_ptr<Number> Number::make(float value)
  {
    return std::make_shared<Number>(value);
  }

  /**
   * Sexpression - Abstract base for lists, arrays and maps
   */
  Sexpression::Sexpression(Form form)
    : Object(form)
  {

  }

  std::shared_ptr<Lisple::Sexpression> Sexpression::new_sequence(Form type)
  {
    switch(type)
      {
      case Form::LIST:
        return std::make_shared<Lisple::List>();
      case Form::ARRAY:
        return std::make_shared<Lisple::Array>();
      case Form::MAP:
        return std::make_shared<Lisple::Map>();
      default:
        throw LispleException("Type is not a sequence");
      }
  }

  Sexpression::Sexpression(Form form, const sptr_sobject_v& children)
    : Object(form)
    , children(children)
  {
  }

  sptr_sobject Sexpression::get_sptr_property(const Lisple::Object& form) const
  {
    auto it = std::find_if(this->children.begin(),
                           this->children.end(), [&]
                           (const Lisple::sptr_sobject& item) { return *item == form; });

    if (it == this->children.end())
    {
      return NIL;
    }

    it = std::next(it);

    if (it == this->children.end())
    {
      return NIL;
    }

    return const_cast<sptr_sobject&>(*it);
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
    return children.front();
  }

  sptr_sobject_v Sexpression::tail()
  {
    sptr_sobject_v tail;
    if (children.size() < 2) return tail;

    for (size_t i=1; i<children.size(); i++)
    {
      tail.push_back(children.at(i));
    }

    return tail;
  }

  std::string Sexpression::to_string() const
  {
    std::string str;
    for (auto& element : children)
    {
      if (!str.empty())
      {
        str += " ";
      }
      if(element)
      {
        str += element->to_string();
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
      if (*children.at(0) != *other_sexp.children.at(0))
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
    sptr_sobject_v new_elements;
    for (size_t i=0; i<children.size(); i++)
    {
      if (i == index)
      {
        new_elements.push_back(element);
      }
      new_elements.push_back(children.at(i));
    }
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

  Map::Map()
    : Sexpression(Form::MAP)
  {
  }

  const std::string Map::lpar() const
  {
    return "{";
  }

  const std::string Map::rpar() const
  {
    return "}";
  }

  std::shared_ptr<Object> List::execute(Context& ctx)
  {
    auto head = this->head();
    auto tail = this->tail();
    return head->execute(ctx, tail);
  }

  std::shared_ptr<Object> List::execute(Context&, sptr_sobject_v&)
  {
    throw InvocationException("Illegal invocation of list: " + this->to_string());
  }

  /**
   * Map
   */
  Map::Map(const sptr_sobject_v& children)
    : Sexpression(Form::MAP, children)
  {
    this->validate_keys();
  }

  void Map::validate_keys() const
  {
    if (this->children.size() % 2 != 0)
    {
      throw TypeError("Odd number of symbols in Map form: " + this->to_string());
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
        throw TypeError("Duplicate key " + key->to_string() + " in Map: " +this->to_string());
      }
    }
  }

  const std::vector<Object*> Map::keys() const
  {
    std::vector<Object*> keys;
    for (size_t i = 0; i<children.size(); i+=2)
    {
      keys.push_back(children.at(i).get());
    }
    return keys;
  }

  Lisple::sptr_sobject_v Map::key_ptrs() const
  {
    Lisple::sptr_sobject_v keys;
    for (size_t i=0; i<children.size(); i+=2)
    {
      keys.push_back(children.at(i));
    }
    return keys;
  }

  bool Map::has_key(const Object& key) const
  {
    for (size_t i = 0; i < children.size(); i += 2)
    {
      if (*children.at(i) == key)
      {
        return true;
      }
    }
    return false;
  }

  void Map::set_property(const Object& key, sptr_sobject& value)
  {
    for (size_t i=0; i<size(); i+=2)
    {
      if (*children.at(i) == key)
      {
        children.at(i+1) = value;
        return;
      }
    }
    throw Lisple::InvocationException("No key " + key.to_string() + " in " + this->to_string());
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

  template class Value<std::string>;
  template class Value<bool>;
  template class Value<char>;
  template class Value<float>;
}
