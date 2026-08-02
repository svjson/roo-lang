#ifndef ROO_SUPPORT_VERSION_H
#define ROO_SUPPORT_VERSION_H

#include <cstdint>
#include <string>
#include <vector>

namespace Roo::Support
{
  struct VersionIdentifier
  {
    bool numeric = false;
    std::uint64_t number = 0;
    std::string text;
  };

  struct Version
  {
    std::uint64_t major = 0;
    std::uint64_t minor = 0;
    std::uint64_t patch = 0;
    std::vector<VersionIdentifier> prerelease;
    std::string build_metadata;
  };

  enum class VersionOperator
  {
    Eq,
    Lt,
    Lte,
    Gt,
    Gte,
  };

  struct VersionComparator
  {
    VersionOperator op = VersionOperator::Eq;
    Version version;
  };

  struct VersionConstraint
  {
    std::vector<VersionComparator> comparators;
  };

  Version parse_version(const std::string& text);
  std::string version_to_string(const Version& version);
  int compare_versions(const Version& left, const Version& right);

  VersionConstraint parse_version_constraint(const std::string& text);
  bool satisfies_version_constraint(const Version& version,
                                    const VersionConstraint& constraint);

  bool operator==(const VersionIdentifier& left, const VersionIdentifier& right);
  bool operator!=(const VersionIdentifier& left, const VersionIdentifier& right);

  bool operator==(const Version& left, const Version& right);
  bool operator!=(const Version& left, const Version& right);
  bool operator<(const Version& left, const Version& right);
  bool operator<=(const Version& left, const Version& right);
  bool operator>(const Version& left, const Version& right);
  bool operator>=(const Version& left, const Version& right);

  inline bool operator==(const VersionIdentifier& left,
                         const VersionIdentifier& right)
  {
    return left.numeric == right.numeric && left.number == right.number &&
           left.text == right.text;
  }

  inline bool operator!=(const VersionIdentifier& left,
                         const VersionIdentifier& right)
  {
    return !(left == right);
  }
} // namespace Roo::Support

#endif
