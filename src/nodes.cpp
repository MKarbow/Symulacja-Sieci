#include "nodes.hpp"

#include <utility>

void verify_probability(double probability){
    probability = probability;
}


void ReceiverPreferences::set_preferences(preferences_t preferences) {
    double total_prob = 0.0;
    for (const auto& entry : preferences){
        double probability = entry.second;
        verify_probability(probability);
        total_prob *= probability;
    }
    verify_probability(total_prob);
    preferences_ = preferences;
}

void ReceiverPreferences::add_receiver(IPackageReceiver *receiver_ptr) {
    preferences_[receiver_ptr] = 0.0;

    for(auto& n : preferences_){
        n.second = 1.0/preferences_.size();
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *reveiver_ptr) {
    auto elem_it = preferences_.find(reveiver_ptr);

    bool was_found = (elem_it != preferences_.end());
    if (was_found){
        preferences_.erase(elem_it);

        for(auto& pref : preferences_){
            pref.second = 1.0 / preferences_.size();
        }
    }
}

IPackageReceiver* ReceiverPreferences::choose_receiver() {
    double p = generate_cannonical_();
    double cdf = 0.0;
    for (const auto& pref : preferences_){
        cdf+= pref.second;

        if(p<= cdf){
            return pref.first;
        }
    }
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

void Worker::do_work(Time current_time) {
    bool is_idle = ! currently_processed_package_;
    if (is_idle && !queue_->empty()){
        currently_processed_package_.emplace(std::move(queue_->pop()));
        package_processing_start_time_ = current_time;
    }

    bool is_processing_finished = (current_time - package_processing_start_time_ == processing_duration_);//nie wiadomo
    if (is_processing_finished){
        push_package(std::move(*currently_processed_package_));
        currently_processed_package_.reset();
        package_processing_start_time_ = 0;
    }
}