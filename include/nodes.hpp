#ifndef SYMULACJA_SIECI_NODES_HPP
#define SYMULACJA_SIECI_NODES_HPP

#include "package.hpp"
#include "storage_types.hpp"
#include <list>
#include <memory>
#include <map>

class IPackageReceiver {
public:
    using const_iterator = std::list<Package>::const_iterator;
    virtual void receive_package(Package&& package) = 0;
    virtual ElementID get_id() = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;
};


class Storehouse: public IPackageReceiver {
public:
    Storehouse(ElementID  id, std::unique_ptr<IPackageStockpile> ptr_);
    void receive_package(Package&& package) override;
    ElementID get_id() override {return id_;}
    const_iterator begin() const override {return package_stockpile_->begin();}
    const_iterator cbegin() const override {return package_stockpile_->cbegin();}
    const_iterator end() const override {return package_stockpile_->end();}
    const_iterator cend() const override {return package_stockpile_->cend();}
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> package_stockpile_;
};


class ReceiverPreferences {
public:
    using preference_t = std::map<IPackageReceiver*, double>;
    void add_receiver(IPackageReceiver* receiver);
    void remove_receiver(IPackageReceiver* receiver);
    IPackageReceiver* choose_receiver();
    preference_t& get_preferences ();
private:
    preference_t preferences_;
};

#endif //SYMULACJA_SIECI_NODES_HPP
