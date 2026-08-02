#include <roo-support/version.h>

#include <algorithm>
#include <charconv>
#include <cctype>
#include <stdexcept>
#include <string_view>

namespace Roo::Support
{
  namespace
  {
    bool is_digit(char c)
    {
      return c >= '0' && c <= '9';
    }

    bool is_identifier_char(char c)
    {
      return (c >= '0' && c <= '9') || (c >= 'A' && c <= 'Z') ||
             (c >= 'a' && c <= 'z') || c == '-';
    }

    bool is_numeric(std::string_view text)
    {
      return !text.empty() && std::all_of(text.begin(), text.end(), is_digit);
    }

    bool has_leading_zero(std::string_view text)
    {
      return text.size() > 1 && text.front() == '0';
    }

    std::uint64_t parse_uint64(std::string_view text, const std::string& context)
    {
      if (!is_numeric(text))
      {
        throw std::invalid_argument("Invalid semantic version: " + context +
                                    " must be numeric.");
      }
      if (has_leading_zero(text))
      {
        throw std::invalid_argument("Invalid semantic version: " + context +
                                    " must not contain leading zeroes.");
      }

      std::uint64_t value = 0;
      auto* begin = text.data();
      auto* end = text.data() + text.size();
      auto [ptr, ec] = std::from_chars(begin, end, value);
      if (ec != std::errc() || ptr != end)
      {
        throw std::invalid_argument("Invalid semantic version: " + context +
                                    " is out of range.");
      }
      return value;
    }

    std::vector<std::string_view> split_dot(std::string_view text,
                                            const std::string& context)
    {
      std::vector<std::string_view> parts;
      size_t start = 0;

      while (start <= text.size())
      {
        const size_t end = text.find('.', start);
        const size_t length =
          end == std::string_view::npos ? text.size() - start : end - start;
        auto part = text.substr(start, length);
        if (part.empty())
        {
          throw std::invalid_argument("Invalid semantic version: empty " + context +
                                      " identifier.");
        }
        parts.push_back(part);
        if (end == std::string_view::npos)
        {
          break;
        }
        start = end + 1;
      }

      return parts;
    }

    void validate_identifier_chars(std::string_view identifier,
                                   const std::string& context)
    {
      if (!std::all_of(identifier.begin(), identifier.end(), is_identifier_char))
      {
        throw std::invalid_argument("Invalid semantic version: " + context +
                                    " identifier contains invalid characters.");
      }
    }

    std::vector<VersionIdentifier> parse_prerelease_identifiers(
      std::string_view text)
    {
      std::vector<VersionIdentifier> identifiers;
      for (auto part : split_dot(text, "prerelease"))
      {
        validate_identifier_chars(part, "prerelease");

        VersionIdentifier identifier;
        identifier.text = std::string(part);
        if (is_numeric(part))
        {
          if (has_leading_zero(part))
          {
            throw std::invalid_argument(
              "Invalid semantic version: numeric prerelease identifiers must not "
              "contain leading zeroes.");
          }
          identifier.numeric = true;
          identifier.number = parse_uint64(part, "prerelease identifier");
        }
        identifiers.push_back(std::move(identifier));
      }
      return identifiers;
    }

    std::string parse_build_metadata(std::string_view text)
    {
      for (auto part : split_dot(text, "build metadata"))
      {
        validate_identifier_chars(part, "build metadata");
      }
      return std::string(text);
    }

    Version parse_version_view(std::string_view text)
    {
      if (text.empty())
      {
        throw std::invalid_argument("Invalid semantic version: expected version.");
      }

      Version version;
      auto version_text = text;

      const size_t plus = version_text.find('+');
      if (plus != std::string_view::npos)
      {
        if (version_text.find('+', plus + 1) != std::string_view::npos)
        {
          throw std::invalid_argument(
            "Invalid semantic version: build metadata marker appears more than once.");
        }

        auto build = version_text.substr(plus + 1);
        if (build.empty())
        {
          throw std::invalid_argument(
            "Invalid semantic version: build metadata must not be empty.");
        }
        version.build_metadata = parse_build_metadata(build);
        version_text = version_text.substr(0, plus);
      }

      const size_t dash = version_text.find('-');
      if (dash != std::string_view::npos)
      {
        auto prerelease = version_text.substr(dash + 1);
        if (prerelease.empty())
        {
          throw std::invalid_argument(
            "Invalid semantic version: prerelease must not be empty.");
        }
        version.prerelease = parse_prerelease_identifiers(prerelease);
        version_text = version_text.substr(0, dash);
      }

      auto core_parts = split_dot(version_text, "core version");
      if (core_parts.size() != 3)
      {
        throw std::invalid_argument(
          "Invalid semantic version: expected major.minor.patch.");
      }

      version.major = parse_uint64(core_parts[0], "major version");
      version.minor = parse_uint64(core_parts[1], "minor version");
      version.patch = parse_uint64(core_parts[2], "patch version");
      return version;
    }

    int compare_uint64(std::uint64_t left, std::uint64_t right)
    {
      if (left < right)
      {
        return -1;
      }
      if (left > right)
      {
        return 1;
      }
      return 0;
    }

    int compare_identifier(const VersionIdentifier& left,
                           const VersionIdentifier& right)
    {
      if (left.numeric && right.numeric)
      {
        return compare_uint64(left.number, right.number);
      }

      if (left.numeric != right.numeric)
      {
        return left.numeric ? -1 : 1;
      }

      if (left.text < right.text)
      {
        return -1;
      }
      if (left.text > right.text)
      {
        return 1;
      }
      return 0;
    }

    std::string identifier_to_string(const VersionIdentifier& identifier)
    {
      if (identifier.numeric)
      {
        return std::to_string(identifier.number);
      }
      return identifier.text;
    }

    std::string join_identifiers(const std::vector<VersionIdentifier>& identifiers)
    {
      std::string result;
      for (const auto& identifier : identifiers)
      {
        if (!result.empty())
        {
          result += ".";
        }
        result += identifier_to_string(identifier);
      }
      return result;
    }

    std::string_view trim_left(std::string_view text)
    {
      while (!text.empty() &&
             std::isspace(static_cast<unsigned char>(text.front())) != 0)
      {
        text.remove_prefix(1);
      }
      return text;
    }

    std::string_view trim_right(std::string_view text)
    {
      while (!text.empty() &&
             std::isspace(static_cast<unsigned char>(text.back())) != 0)
      {
        text.remove_suffix(1);
      }
      return text;
    }

    std::string_view trim(std::string_view text)
    {
      return trim_right(trim_left(text));
    }

    VersionComparator parse_comparator(std::string_view text)
    {
      VersionComparator comparator;
      text = trim(text);
      if (text.empty())
      {
        throw std::invalid_argument(
          "Invalid semantic version constraint: empty comparator.");
      }

      if (text.rfind(">=", 0) == 0)
      {
        comparator.op = VersionOperator::Gte;
        text.remove_prefix(2);
      }
      else if (text.rfind("<=", 0) == 0)
      {
        comparator.op = VersionOperator::Lte;
        text.remove_prefix(2);
      }
      else if (text.rfind(">", 0) == 0)
      {
        comparator.op = VersionOperator::Gt;
        text.remove_prefix(1);
      }
      else if (text.rfind("<", 0) == 0)
      {
        comparator.op = VersionOperator::Lt;
        text.remove_prefix(1);
      }
      else if (text.rfind("=", 0) == 0)
      {
        comparator.op = VersionOperator::Eq;
        text.remove_prefix(1);
      }
      else
      {
        comparator.op = VersionOperator::Eq;
      }

      text = trim(text);
      if (text.empty())
      {
        throw std::invalid_argument(
          "Invalid semantic version constraint: comparator is missing a version.");
      }
      comparator.version = parse_version_view(text);
      return comparator;
    }

    bool satisfies_comparator(const Version& version,
                              const VersionComparator& comparator)
    {
      const int comparison = compare_versions(version, comparator.version);
      switch (comparator.op)
      {
      case VersionOperator::Eq:
        return comparison == 0;
      case VersionOperator::Lt:
        return comparison < 0;
      case VersionOperator::Lte:
        return comparison <= 0;
      case VersionOperator::Gt:
        return comparison > 0;
      case VersionOperator::Gte:
        return comparison >= 0;
      }
      return false;
    }
  } // namespace

  Version parse_version(const std::string& text)
  {
    return parse_version_view(text);
  }

  std::string version_to_string(const Version& version)
  {
    std::string result = std::to_string(version.major) + "." +
                         std::to_string(version.minor) + "." +
                         std::to_string(version.patch);
    if (!version.prerelease.empty())
    {
      result += "-";
      result += join_identifiers(version.prerelease);
    }
    if (!version.build_metadata.empty())
    {
      result += "+";
      result += version.build_metadata;
    }
    return result;
  }

  int compare_versions(const Version& left, const Version& right)
  {
    if (auto major = compare_uint64(left.major, right.major); major != 0)
    {
      return major;
    }
    if (auto minor = compare_uint64(left.minor, right.minor); minor != 0)
    {
      return minor;
    }
    if (auto patch = compare_uint64(left.patch, right.patch); patch != 0)
    {
      return patch;
    }

    if (left.prerelease.empty() && right.prerelease.empty())
    {
      return 0;
    }
    if (left.prerelease.empty())
    {
      return 1;
    }
    if (right.prerelease.empty())
    {
      return -1;
    }

    const size_t count = std::min(left.prerelease.size(), right.prerelease.size());
    for (size_t i = 0; i < count; ++i)
    {
      if (auto identifier = compare_identifier(left.prerelease[i], right.prerelease[i]);
          identifier != 0)
      {
        return identifier;
      }
    }

    if (left.prerelease.size() < right.prerelease.size())
    {
      return -1;
    }
    if (left.prerelease.size() > right.prerelease.size())
    {
      return 1;
    }
    return 0;
  }

  VersionConstraint parse_version_constraint(const std::string& text)
  {
    VersionConstraint constraint;
    std::string_view remaining = trim(text);
    if (remaining.empty())
    {
      throw std::invalid_argument(
        "Invalid semantic version constraint: expected comparator.");
    }

    while (!remaining.empty())
    {
      std::string comparator_text;
      if (remaining.rfind(">=", 0) == 0 || remaining.rfind("<=", 0) == 0)
      {
        comparator_text = std::string(remaining.substr(0, 2));
        remaining.remove_prefix(2);
      }
      else if (remaining.front() == '>' || remaining.front() == '<' ||
               remaining.front() == '=')
      {
        comparator_text = std::string(remaining.substr(0, 1));
        remaining.remove_prefix(1);
      }

      remaining = trim_left(remaining);
      if (remaining.empty())
      {
        throw std::invalid_argument(
          "Invalid semantic version constraint: comparator is missing a version.");
      }

      size_t end = 0;
      while (end < remaining.size() &&
             std::isspace(static_cast<unsigned char>(remaining[end])) == 0)
      {
        ++end;
      }

      comparator_text += std::string(remaining.substr(0, end));
      constraint.comparators.push_back(parse_comparator(comparator_text));
      remaining.remove_prefix(end);
      remaining = trim_left(remaining);
    }

    return constraint;
  }

  bool satisfies_version_constraint(const Version& version,
                                    const VersionConstraint& constraint)
  {
    return std::all_of(constraint.comparators.begin(),
                       constraint.comparators.end(),
                       [&](const VersionComparator& comparator) {
                         return satisfies_comparator(version, comparator);
                       });
  }

  bool operator==(const Version& left, const Version& right)
  {
    return compare_versions(left, right) == 0;
  }

  bool operator!=(const Version& left, const Version& right)
  {
    return !(left == right);
  }

  bool operator<(const Version& left, const Version& right)
  {
    return compare_versions(left, right) < 0;
  }

  bool operator<=(const Version& left, const Version& right)
  {
    return compare_versions(left, right) <= 0;
  }

  bool operator>(const Version& left, const Version& right)
  {
    return compare_versions(left, right) > 0;
  }

  bool operator>=(const Version& left, const Version& right)
  {
    return compare_versions(left, right) >= 0;
  }
} // namespace Roo::Support
