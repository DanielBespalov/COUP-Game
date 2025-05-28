#ifndef COUP_ROLETYPE_HPP
#define COUP_ROLETYPE_HPP

#include <string>

namespace coup {

enum class RoleType {
    Governor,
    Spy,
    Baron,
    General,
    Judge,
    Merchant
};

// C++ string conversion
inline std::string to_string(RoleType r) {
    switch (r) {
        case RoleType::Governor: return "Governor";
        case RoleType::Spy: return "Spy";
        case RoleType::Baron: return "Baron";
        case RoleType::General: return "General";
        case RoleType::Judge: return "Judge";
        case RoleType::Merchant: return "Merchant";
    }
    return "Unknown";
}

} // namespace coup

#endif // COUP_ROLETYPE_HPP
