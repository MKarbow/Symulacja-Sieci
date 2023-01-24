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
    auto it = preferences_.find(receiver);
    bool was_found = (it != preferences_.end());
    if (was_found == true) {
        preferences_.erase(it);
        for (auto n: preferences_)
            n.second = 1 / preferences_.size();
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double p = default_probability_generator();
    double p1 = 0.0;
    for (auto n : preferences_) {
        p1 += n.second;
        if (p <= p1)
            return n.first;
    }
}
