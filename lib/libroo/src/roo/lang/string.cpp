
#include <algorithm>
#include <cctype>
#include <climits>
#include <string>
#include <utility>
#include <vector>

#include <roo/exception.h>
#include <roo/impl.h>
#include <roo/lang/string.h>
#include <roo/runtime/dict.h>
#include <roo/runtime/value.h>
#include <roo/type.h>

namespace Roo
{
  namespace
  {
    std::string trim_copy(const std::string& str)
    {
      size_t start = 0;
      while (start < str.size() && std::isspace(static_cast<unsigned char>(str[start])))
      {
        start++;
      }

      size_t end = str.size();
      while (end > start && std::isspace(static_cast<unsigned char>(str[end - 1])))
      {
        end--;
      }

      return str.substr(start, end - start);
    }

    int option_indent_width(Value& options, const std::string& function_name)
    {
      sptr_val value = Dict::get_property(options, "indent");
      if (value->type == Value::Type::NIL)
      {
        return 2;
      }
      if (value->type != Value::Type::NUMBER)
      {
        throw TypeError(function_name +
                        " option :indent must be a number, got: " + value->to_string());
      }

      const int indent = value->num().get_int();
      if (indent < 1)
      {
        throw TypeError(function_name + " option :indent must be greater than zero.");
      }
      return indent;
    }

    char lower_ascii(char c)
    {
      return static_cast<char>(std::tolower(static_cast<unsigned char>(c)));
    }

    char upper_ascii(char c)
    {
      return static_cast<char>(std::toupper(static_cast<unsigned char>(c)));
    }

    bool is_word_char(char c)
    {
      return std::isalnum(static_cast<unsigned char>(c));
    }

    bool starts_new_word(const std::string& str, size_t index)
    {
      if (index == 0)
      {
        return false;
      }

      const char prev = str[index - 1];
      const char current = str[index];
      if (!is_word_char(prev) || !is_word_char(current))
      {
        return false;
      }

      const bool prev_lower_or_digit = std::islower(static_cast<unsigned char>(prev)) ||
                                       std::isdigit(static_cast<unsigned char>(prev));
      const bool current_upper = std::isupper(static_cast<unsigned char>(current));
      if (prev_lower_or_digit && current_upper)
      {
        return true;
      }

      if (current_upper && index + 1 < str.size())
      {
        const char next = str[index + 1];
        return std::isupper(static_cast<unsigned char>(prev)) &&
               std::islower(static_cast<unsigned char>(next));
      }

      return false;
    }

    std::vector<std::string> case_words(const std::string& str)
    {
      std::vector<std::string> words;
      std::string current;

      for (size_t i = 0; i < str.size(); i++)
      {
        const char c = str[i];
        if (!is_word_char(c))
        {
          if (!current.empty())
          {
            words.push_back(current);
            current.clear();
          }
          continue;
        }

        if (!current.empty() && starts_new_word(str, i))
        {
          words.push_back(current);
          current.clear();
        }

        current += lower_ascii(c);
      }

      if (!current.empty())
      {
        words.push_back(current);
      }

      return words;
    }

    std::string join_words(const std::vector<std::string>& words, const std::string& sep)
    {
      std::string result;
      for (size_t i = 0; i < words.size(); i++)
      {
        if (i > 0)
        {
          result += sep;
        }
        result += words[i];
      }
      return result;
    }

    std::string title_word(std::string word)
    {
      if (!word.empty())
      {
        word[0] = upper_ascii(word[0]);
      }
      return word;
    }

    std::string pascal_case(const std::vector<std::string>& words)
    {
      std::string result;
      for (auto word : words)
      {
        result += title_word(std::move(word));
      }
      return result;
    }

    long integer_number(const sptr_val& value, const std::string& name)
    {
      const Value::Number& num = value->num();
      if (num.num_type == Value::NumberType::FLOAT)
      {
        throw TypeError(name + " expects integer arguments.");
      }
      return num.get_long();
    }

    std::string capitalize_words(const std::string& str, long start_word, long end_word)
    {
      if (end_word < start_word)
      {
        return str;
      }

      std::string result = str;
      long word_index = -1;
      bool in_word = false;
      bool first_word_char = false;

      for (char& c : result)
      {
        if (!is_word_char(c))
        {
          in_word = false;
          first_word_char = false;
          continue;
        }

        if (!in_word)
        {
          in_word = true;
          first_word_char = true;
          word_index++;
        }

        if (word_index >= start_word && word_index <= end_word)
        {
          c = first_word_char ? upper_ascii(c) : lower_ascii(c);
        }

        first_word_char = false;
      }

      return result;
    }

    std::pair<long, long> word_range(const sptr_val& value)
    {
      const sptr_val_v& range = value->elements();
      if (range.size() != 2)
      {
        throw RooException("capitalize range must contain exactly two numbers.");
      }

      if (range[0]->type != Value::Type::NUMBER || range[1]->type != Value::Type::NUMBER)
      {
        throw TypeError("capitalize range values must be numbers.");
      }

      long start = integer_number(range[0], "capitalize");
      long end = integer_number(range[1], "capitalize");
      if (start < 0 || end < 0)
      {
        throw RooException("capitalize range indexes must be non-negative.");
      }
      if (end < start)
      {
        throw RooException("capitalize range end must be greater than or equal to start.");
      }

      return {start, end};
    }
  } // namespace

  /**
   * StrFunction - str
   */
  FUNC_IMPL(StrFunction,
            SIG((FN_ARGS((VARARG, &Type::ANY)), EXEC_DISPATCH(&StrFunction::exec_str))))

  /**
   * Uses regular string concatenation with += which forces re-allocation as the string
   * content grows.
   *
   * Attempts to pre-reserve by computing the total length up front were
   * benchmarked and found to be consistently slower, as the cost of the measurement
   * pass outweighs the savings from avoiding reallocations - the allocator's
   * geometric growth strategy wins at every arg count tested (5–1000 args).
   */
  EXEC_BODY(StrFunction, exec_str)
  {
    std::string result;

    for (auto& arg : args)
    {
      switch (arg->type)
      {
      case Value::Type::STRING:
        result += std::get<std::string>(arg->value);
        break;
      case Value::Type::CHAR:
        result += std::get<char>(arg->value);
        break;
      default:
        result += arg->to_string();
        break;
      }
    }

    return Value::string(result);
  }

  /** PrettyStrFunction - pretty-str */
  FUNC_IMPL(PrettyStrFunction,
            MULTI_SIG((FN_ARGS((&Type::ANY)),
                       EXEC_DISPATCH(&PrettyStrFunction::exec_pretty_str)),
                      (FN_ARGS((&Type::ANY), (&Type::MAP)),
                       EXEC_DISPATCH(&PrettyStrFunction::exec_pretty_str))))

  EXEC_BODY(PrettyStrFunction, exec_pretty_str)
  {
    const int indent_width =
      args.size() > 1 ? option_indent_width(*args[1], "pretty-str") : 2;
    return Value::string(args[0]->to_pretty_string(indent_width));
  }

  /** JoinFunction - join */
  FUNC_IMPL(JoinFunction,
            SIG((FN_ARGS((VARARG, &Type::STRING)), EXEC_DISPATCH(&JoinFunction::exec_join))))

  EXEC_BODY(JoinFunction, exec_join)
  {
    if (args.size() < 2) return Value::string("");

    const std::string& joiner = args[0]->str();
    std::string result = args[1]->str();

    for (size_t i = 2; i < args.size(); i++)
    {
      result += joiner;
      result += args[i]->str();
    }

    return Value::string(result);
  }

  /** SubstrFunction - substr */
  FUNC_IMPL(SubstrFunction,
            MULTI_SIG((FN_ARGS((&Type::STRING), (&Type::NUMBER)),
                       EXEC_DISPATCH(&SubstrFunction::exec_substr)),
                      (FN_ARGS((&Type::STRING), (&Type::NUMBER), (&Type::NUMBER)),
                       EXEC_DISPATCH(&SubstrFunction::exec_substr))))

  EXEC_BODY(SubstrFunction, exec_substr)
  {
    const std::string& str = args[0]->str();
    int start = args[1]->i32();

    if (start < 0 || start >= static_cast<int>(str.size()))
    {
      return Value::string("");
    }

    if (args.size() == 2)
    {
      return Value::string(str.substr(static_cast<size_t>(start)));
    }

    int length = args[2]->i32();
    if (length <= 0)
    {
      return Value::string("");
    }

    return Value::string(
      str.substr(static_cast<size_t>(start), static_cast<size_t>(length)));
  }

  /** TrimFunction - trim */
  FUNC_IMPL(TrimFunction,
            SIG((FN_ARGS((&Type::STRING)), EXEC_DISPATCH(&TrimFunction::exec_trim))))

  EXEC_BODY(TrimFunction, exec_trim)
  {
    return Value::string(trim_copy(args[0]->str()));
  }

  /** BlankPFunction - blank? */
  FUNC_IMPL(BlankPFunction,
            MULTI_SIG((FN_ARGS((&Type::STRING)),
                       EXEC_DISPATCH(&BlankPFunction::exec_blank_p)),
                      (FN_ARGS((&Type::NIL)), EXEC_DISPATCH(&BlankPFunction::exec_blank_p))))

  EXEC_BODY(BlankPFunction, exec_blank_p)
  {
    if (args[0]->type == Value::Type::NIL)
    {
      return Constant::BOOL_TRUE;
    }

    return trim_copy(args[0]->str()).empty() ? Constant::BOOL_TRUE : Constant::BOOL_FALSE;
  }

  /** UpperCaseFunction - upper-case */
  FUNC_IMPL(UpperCaseFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&UpperCaseFunction::exec_upper_case))))

  EXEC_BODY(UpperCaseFunction, exec_upper_case)
  {
    std::string str =
      args[0]->type == Value::Type::STRING ? args[0]->str() : args[0]->to_string();
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return Value::string(str);
  }

  /** LowerCaseFunction - lower-case */
  FUNC_IMPL(LowerCaseFunction,
            SIG((FN_ARGS((&Type::ANY)), EXEC_DISPATCH(&LowerCaseFunction::exec_lower_case))))

  EXEC_BODY(LowerCaseFunction, exec_lower_case)
  {
    std::string str =
      args[0]->type == Value::Type::STRING ? args[0]->str() : args[0]->to_string();
    std::transform(str.begin(), str.end(), str.begin(), ::tolower);
    return Value::string(str);
  }

  /** CapitalizeFunction - capitalize */
  FUNC_IMPL(CapitalizeFunction,
            MULTI_SIG((FN_ARGS((&Type::STRING)),
                       EXEC_DISPATCH(&CapitalizeFunction::exec_capitalize_all)),
                      (FN_ARGS((&Type::STRING), (&Type::NUMBER)),
                       EXEC_DISPATCH(&CapitalizeFunction::exec_capitalize_count)),
                      (FN_ARGS((&Type::STRING), (&Type::VECTOR)),
                       EXEC_DISPATCH(&CapitalizeFunction::exec_capitalize_range))))

  EXEC_BODY(CapitalizeFunction, exec_capitalize_all)
  {
    return Value::string(capitalize_words(args[0]->str(), 0, LONG_MAX));
  }

  EXEC_BODY(CapitalizeFunction, exec_capitalize_count)
  {
    const long count = integer_number(args[1], "capitalize");
    if (count < 0)
    {
      throw RooException("capitalize word count must be non-negative.");
    }
    return Value::string(capitalize_words(args[0]->str(), 0, count - 1));
  }

  EXEC_BODY(CapitalizeFunction, exec_capitalize_range)
  {
    auto [start, end] = word_range(args[1]);
    return Value::string(capitalize_words(args[0]->str(), start, end));
  }

  /** CapitalizeFirstFunction - capitalize-first */
  FUNC_IMPL(CapitalizeFirstFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&CapitalizeFirstFunction::exec_capitalize_first))))

  EXEC_BODY(CapitalizeFirstFunction, exec_capitalize_first)
  {
    return Value::string(capitalize_words(args[0]->str(), 0, 0));
  }

  /** KebabCaseFunction - kebab-case */
  FUNC_IMPL(KebabCaseFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&KebabCaseFunction::exec_kebab_case))))

  EXEC_BODY(KebabCaseFunction, exec_kebab_case)
  {
    return Value::string(join_words(case_words(args[0]->str()), "-"));
  }

  /** PascalCaseFunction - pascal-case */
  FUNC_IMPL(PascalCaseFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&PascalCaseFunction::exec_pascal_case))))

  EXEC_BODY(PascalCaseFunction, exec_pascal_case)
  {
    return Value::string(pascal_case(case_words(args[0]->str())));
  }

  /** CamelCaseFunction - camel-case */
  FUNC_IMPL(CamelCaseFunction,
            SIG((FN_ARGS((&Type::STRING)),
                 EXEC_DISPATCH(&CamelCaseFunction::exec_camel_case))))

  EXEC_BODY(CamelCaseFunction, exec_camel_case)
  {
    std::vector<std::string> words = case_words(args[0]->str());
    if (words.empty())
    {
      return Value::string("");
    }

    std::string result = words[0];
    for (size_t i = 1; i < words.size(); i++)
    {
      result += title_word(std::move(words[i]));
    }
    return Value::string(result);
  }

} // namespace Roo
