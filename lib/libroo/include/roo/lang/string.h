
#ifndef ROO__LANG__STRING_H
#define ROO__LANG__STRING_H

#include <roo/exec.h>

namespace Roo
{
  /*!
   * @brief Converts any object to their string representation and concatenates
   * multiple forms or strings to a single string
   *
   * Usage:
   * @code
   * (str 1) => "1"
   * (str "There is no " (rand-nth ["spoon" "hope"])) => "There is no spoon"
   * (str :regular-key) => ":regular-key"
   * (str "The number " x " is the value of x")
   * => "The number 10 is the value of x"
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | values... | Values to concatenate as strings.                                  |
   */
  FUNC(StrFunction, str)

  /*!
   * @brief Formats a Roo value as a pretty-printed EDN-style string.
   *
   * Usage:
   * @code
   * (pretty-str {:name "Roo" :ok true})
   * => "{\n  :name \"Roo\"\n  :ok true\n}"
   * @endcode
   *
   * | Arg     | Description                                                        |
   * | ------- | ------------------------------------------------------------------ |
   * | value   | The value to format.                                               |
   * | options | Optional map with :indent number.                                  |
   */
  FUNC(PrettyStrFunction, pretty_str)

  /*!
   * @brief Joins multiple strings with a separator.
   *
   * Usage:
   * @code
   * (join ", " "a" "b" "c") => "a, b, c"
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | separator | String inserted between values.                                    |
   * | values... | Values to join as strings.                                         |
   */
  FUNC(JoinFunction, join)

  /*!
   * @brief Extracts a substring from a string.
   *
   * Usage:
   * @code
   * (substr "abcdef" 2) => "cdef"
   * (substr "abcdef" 2 3) => "cde"
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | string | String to read from.                                               |
   * | start  | Zero-based start index.                                            |
   * | end    | Optional zero-based end index, exclusive.                          |
   */
  FUNC(SubstrFunction, substr)

  /*!
   * @brief Removes leading and trailing whitespace from a string.
   *
   * Usage:
   * @code
   * (trim "  hello  ") => "hello"
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | string | String to trim.                                                    |
   */
  FUNC(TrimFunction, trim)

  /*!
   * @brief Tests whether a string is empty or contains only whitespace.
   *
   * Usage:
   * @code
   * (blank? "") => true
   * (blank? " \n\t ") => true
   * (blank? nil) => true
   * @endcode
   *
   * | Arg    | Description                                                        |
   * | ------ | ------------------------------------------------------------------ |
   * | string | String to test. Nil is considered blank.                           |
   */
  FUNC(BlankPFunction, blank_p)

  /*!
   * @brief Converts string or string representation to all upper-case.
   *
   * Usage:
   * @code
   * (upper-case "mIxEd-CaSe!") => "MIXED-CASE!"
   * (upper-case :regular-key) => ":REGULAR-KEY"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | Value to convert through its string representation.                 |
   */
  FUNC(UpperCaseFunction, upper_case)

  /*!
   * @brief Converts string or string representation to all lower-case.
   *
   * Usage:
   * @code
   * (lower-case "mIxEd-CaSe!") => "mixed-case!"
   * (lower-case :UPCASE-KEY) => ":upcase-key"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | value | Value to convert through its string representation.                 |
   */
  FUNC(LowerCaseFunction, lower_case)

  /*!
   * @brief Capitalizes words in a string while preserving existing separators.
   *
   * With one argument, capitalizes every ASCII word. With a numeric second
   * argument, capitalizes only the first N words. With a vector second argument,
   * capitalizes the inclusive zero-based word-index range `[start end]`. Words are
   * alphanumeric runs; selected words have their first character uppercased and
   * remaining characters lowercased.
   *
   * Usage:
   * @code
   * (capitalize "string") => "String"
   * (capitalize "string of words") => "String Of Words"
   * (capitalize "string of words" 2) => "String Of words"
   * (capitalize "a brown fox ate a bug today" [2 4])
   * => "a brown Fox Ate A bug today"
   * @endcode
   *
   * | Arg      | Description                                                        |
   * | -------- | ------------------------------------------------------------------ |
   * | text     | String to capitalize.                                              |
   * | selector | Optional word count, or `[start end]` inclusive word range.        |
   */
  FUNC(CapitalizeFunction, capitalize_all, capitalize_count, capitalize_range)

  /*!
   * @brief Capitalizes only the first word in a string.
   *
   * Convenience form equivalent to `(capitalize value 1)`. Preserves existing
   * separators and only changes the first alphanumeric word.
   *
   * Usage:
   * @code
   * (capitalize-first "string of words") => "String of words"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | text  | String to capitalize.                                              |
   */
  FUNC(CapitalizeFirstFunction, capitalize_first)

  /*!
   * @brief Converts a string to kebab-case.
   *
   * Splits the input into ASCII word segments using whitespace, punctuation,
   * underscore, hyphen, and common camel/Pascal-case boundaries, lowercases every
   * segment, and joins the result with `-`.
   *
   * Usage:
   * @code
   * (kebab-case "Player Ship State") => "player-ship-state"
   * (kebab-case "playerShipState") => "player-ship-state"
   * (kebab-case "player_ship_state") => "player-ship-state"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | text  | String to convert.                                                 |
   */
  FUNC(KebabCaseFunction, kebab_case)

  /*!
   * @brief Converts a string to PascalCase.
   *
   * Splits the input into ASCII word segments using whitespace, punctuation,
   * underscore, hyphen, and common camel/Pascal-case boundaries, lowercases every
   * segment, capitalizes the first character of each segment, and joins the result
   * without separators.
   *
   * Usage:
   * @code
   * (pascal-case "player ship state") => "PlayerShipState"
   * (pascal-case "player-ship-state") => "PlayerShipState"
   * (pascal-case "player_ship_state") => "PlayerShipState"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | text  | String to convert.                                                 |
   */
  FUNC(PascalCaseFunction, pascal_case)

  /*!
   * @brief Converts a string to camelCase.
   *
   * Splits the input into ASCII word segments using whitespace, punctuation,
   * underscore, hyphen, and common camel/Pascal-case boundaries, lowercases every
   * segment, keeps the first segment lowercase, capitalizes the first character of
   * each following segment, and joins the result without separators.
   *
   * Usage:
   * @code
   * (camel-case "player ship state") => "playerShipState"
   * (camel-case "PlayerShipState") => "playerShipState"
   * (camel-case "player_ship_state") => "playerShipState"
   * @endcode
   *
   * | Arg   | Description                                                        |
   * | ----- | ------------------------------------------------------------------ |
   * | text  | String to convert.                                                 |
   */
  FUNC(CamelCaseFunction, camel_case)

  /*!
   * @brief Splits a string on a delimiter and returns a vector of strings.
   *
   * Each occurrence of the delimiter produces a split point. Empty strings
   * between adjacent delimiters, at the start, or at the end of the input are
   * preserved as empty-string elements in the result.
   *
   * When the delimiter is an empty string the input is split into individual
   * single-character strings.
   *
   * Usage:
   * @code
   * (split "a,b,c" ",")     => ["a" "b" "c"]
   * (split "a,,b" ",")      => ["a" "" "b"]
   * (split ",a" ",")        => ["" "a"]
   * (split "abc" "")        => ["a" "b" "c"]
   * (split "no-match" ",")  => ["no-match"]
   * @endcode
   *
   * | Arg       | Description                                                        |
   * | --------- | ------------------------------------------------------------------ |
   * | string    | String to split.                                                   |
   * | delimiter | String to split on.                                                |
   */
  FUNC(SplitFunction, split)

} // namespace Roo

#endif /* ROO__LANG__STRING_H */
