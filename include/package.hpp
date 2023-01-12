#ifndef SIECI_PACKAGE_HPP
#define SIECI_PACKAGE_HPP

#include "types.hpp"
#include <utility>


class Package {
public:
    Package();
    Package(ElementID);
    Package(Package&&) noexcept;
    Package(Package&) = delete;
    Package& operator=(Package&&) noexcept;
    Package& operator=(Package&) = delete;
    ElementID get_id() {return id_;}
    ~Package();
private:
    ElementID id_;
    inline static std::set<ElementID> assigned_IDs{0};
    inline static std::set<ElementID> freed_IDs;
    static const ElementID BLANK_ID = -1;
};

#endif //SIECI_PACKAGE_HPP
