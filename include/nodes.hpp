#ifndef SIECI_NODES_HPP
#define SIECI_NODES_HPP

#include <map>
#include <optional>
#include <memory>
#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"



class IPackageReceiver {
public:
    using const_iterator = IPackageStockpile::const_iterator;
    virtual void receive_package(Package&& package) = 0;
    virtual ElementID get_id() const = 0;

    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;
};


class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO))://nie widamomo
    id_(id), package_stockpile_(std::move(d)){}
    void receive_package(Package&& package) override {package_stockpile_->push(std::move(package));};
    ElementID get_id() const override {return id_;};

    const_iterator cbegin() const override {return package_stockpile_->cbegin();}
    const_iterator begin() const override {return cbegin();}
    const_iterator cend() const override {return package_stockpile_->cend();}
    const_iterator end() const override {return cend();}

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> package_stockpile_;
};


class ReceiverPreferences {
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    explicit ReceiverPreferences(ProbabilityGenerator probability_function = probability_generator) : generate_cannonical_(probability_function) {}
    const preferences_t& get_preferences() const { return preferences_; }
    void set_preferences(preferences_t preferences);
    void add_receiver(IPackageReceiver* receiver_ptr);
    void remove_receiver(IPackageReceiver* reveiver_ptr);
    IPackageReceiver * choose_receiver();

    const_iterator cbegin() const { return preferences_.cbegin();}
    const_iterator cend() const { return preferences_.cend();}
    const_iterator begin() const { return cbegin();}
    const_iterator end() const { return cend();}


private:
    ProbabilityGenerator generate_cannonical_ = probability_generator;
    preferences_t preferences_;
};


class PackageSender {
public:
    ReceiverPreferences receiver_preferences_;
    PackageSender() = default;
    PackageSender(PackageSender&&) = default;
    void send_package();
    virtual ~PackageSender() = default;
    const std::optional<Package>& get_sending_buffer() const { return sending_buffer_; }
protected:
    void push_package(Package&& package) {sending_buffer_.emplace(std::move(package));}
private:
    std::optional<Package> sending_buffer_ = std::nullopt;
};


class Ramp : public PackageSender {
public:
    Ramp(ElementID id, TimeOffset delivery_interval)
            : id_(id), delivery_interval_(delivery_interval) {}

    void deliver_goods(Time current_time);
    TimeOffset get_delivery_interval() const { return delivery_interval_; }
    ElementID get_id() const { return id_; }
private:
    ElementID id_;
    TimeOffset delivery_interval_;
};


class Worker : public PackageSender, public IPackageReceiver {
public:
    Worker(ElementID id, TimeOffset processing_duration, std::unique_ptr<IPackageQueue> queue_ptr)
            : id_(id), processing_duration_(processing_duration), queue_(std::move(queue_ptr)) {}

    void receive_package(Package&& package) override {queue_->push(std::move(package));}

    const_iterator cbegin() const override { return queue_->cbegin();}
    const_iterator cend() const override { return queue_->cend();}
    const_iterator begin() const override { return cbegin();}
    const_iterator end() const override { return cend();}


    void do_work(Time current_time);
    TimeOffset get_processing_duration() const { return processing_duration_; }
    const IPackageQueue* get_queue() const {return queue_.get();}
    Time get_package_processing_time() const {return package_processing_start_time_;}
    ElementID get_id() const override {return id_; }
    const std::optional<Package>& get_processing_buffer() const {return currently_processed_package_;}

private:
    ElementID id_;
    TimeOffset processing_duration_;
    Time package_processing_start_time_ = 0;
    std::optional<Package> currently_processed_package_;
    std::unique_ptr<IPackageQueue> queue_;
};

#endif //SIECI_NODES_HPP
