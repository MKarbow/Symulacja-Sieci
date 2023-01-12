#include "package.hpp"



Package::Package() {
    ElementID newId = BLANK_ID;
    if (!freed_IDs.empty()) {
        newId = *(freed_IDs.begin());
        freed_IDs.erase(newId);
    }
    else {
        newId = *assigned_IDs.rbegin() + 1;
    }
    assigned_IDs.insert(newId);
    id_ = newId;
}

Package::Package(ElementID id) {
    assigned_IDs.insert(id);
    id_ = id;
}


Package::Package(Package && package) noexcept {
    id_ = package.id_;
    package.id_ = BLANK_ID;
}

Package &Package::operator=(Package && package) noexcept {
    if (id_ != BLANK_ID) {
        assigned_IDs.erase(id_);
        freed_IDs.insert(id_);
    }
    id_ = package.id_;
    package.id_ = BLANK_ID;

    return *this;
}

Package::~Package() {
    if (id_ != BLANK_ID) {
        freed_IDs.insert(id_);
        assigned_IDs.erase(id_);
    }
}