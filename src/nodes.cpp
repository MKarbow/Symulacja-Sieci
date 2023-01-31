#include "nodes.hpp"

#include <utility>



void ReceiverPreferences::add_receiver(IPackageReceiver* r)
{
    preferences_.emplace(std::make_pair(r, 0));
    const double probability = 1.0 / preferences_.size();
    for(auto& x : preferences_)
        x.second = probability;
}

void ReceiverPreferences::remove_receiver(IPackageReceiver* r)
{
    if(preferences_.find(r) != preferences_.end())
    {
        preferences_.erase(r);
        const double probability = 1.0 / preferences_.size();
        for(auto& x : preferences_)
            x.second = probability;
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver()
{
    const double random_val = pg_();
    double min_val = 0.0;

    for(const auto& x : preferences_)
    {
        min_val += x.second;
        if(random_val <= min_val)
            return x.first;
    }

    return nullptr;
}


void PackageSender::send_package() {
    if (sending_buffer_){
        auto receiver_ptr = receiver_preferences_.choose_receiver();
        receiver_ptr->receive_package(std::move(*sending_buffer_));
        sending_buffer_.reset();
    }
}

void Ramp::deliver_goods(Time current_time) {
    bool is_delivery_time_now = ((current_time - 1) % delivery_interval_ == 0);
    if (is_delivery_time_now){
        push_package(Package());
    }
}

void Worker::do_work(Time t)
{
    if(t >= package_processing_start_time + pd_)
        send_package();

    if(get_sending_buffer().has_value() == false && q_->size() != 0)
    {
        push_package(q_->pop());
        package_processing_start_time = t;
    }
}

void Worker::receive_package(Package&& p)
{
    q_->push(std::move(p));
}