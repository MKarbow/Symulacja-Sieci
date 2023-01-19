#include "nodes.hpp"


Storehouse::Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> ptr_): id_(id), package_stockpile_(std::move(ptr_)) {}


void Storehouse::receive_package(Package&& package) {
    package_stockpile_->push(std::move(package)); //???
}


void ReceiverPreferences::add_receiver(IPackageReceiver* receiver) {
    preferences_[receiver] = 0.0;
    for (auto n : preferences_)
        n.second = 1 / preferences_.size();
}


void ReceiverPreferences::remove_receiver(IPackageReceiver* receiver) {
    preferences_.erase(receiver);
    for (auto n: preferences_)
        n.second = 1 / preferences_.size();
}