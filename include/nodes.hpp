#ifndef SIECI_NODES_HPP
#define SIECI_NODES_HPP

#include <map>
#include <optional>
#include <memory>
#include "types.hpp"
#include "storage_types.hpp"
#include "helpers.hpp"
#include "config.hpp"

enum class ReceiverType {
    WORKER, STOREHOUSE
};



class IPackageReceiver {
public:
    using const_iterator = IPackageStockpile::const_iterator;
    virtual void receive_package(Package&& package) = 0;
    virtual ElementID get_id() const = 0;



    virtual ReceiverType get_receiver_type()  = 0;


    virtual const_iterator begin() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cend() const = 0;
};


class Storehouse : public IPackageReceiver {
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueueType::FIFO)):
    id_(id), package_stockpile_(std::move(d)){}
    void receive_package(Package&& package) override {package_stockpile_->push(std::move(package));};
    ElementID get_id() const override {return id_;};



    ReceiverType get_receiver_type() override { return ReceiverType::STOREHOUSE; }


    const_iterator cbegin() const override {return package_stockpile_->cbegin();}
    const_iterator begin() const override {return cbegin();}
    const_iterator cend() const override {return package_stockpile_->cend();}
    const_iterator end() const override {return cend();}

private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> package_stockpile_;
};


class ReceiverPreferences
{
public:
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;

    ReceiverPreferences(ProbabilityGenerator pg = probability_generator) : pg_(pg) {}
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    const preferences_t & get_preferences() const { return preferences_; };

    const_iterator begin() const { return preferences_.cbegin(); }
    const_iterator end() const { return preferences_.cend(); }
    const_iterator cbegin() const { return preferences_.cbegin(); };
    const_iterator cend() const { return preferences_.cend(); };

private:
    ProbabilityGenerator pg_;
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


class Worker : public PackageSender, public IPackageReceiver
{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> q) : id_(id), pd_(pd), q_(std::move(q)), package_processing_start_time(0) {}

    virtual void receive_package(Package&& p) override;
    virtual ElementID get_id() const override { return id_; }

    void do_work(Time t);
    TimeOffset get_processing_duration() const { return pd_; }
    Time get_package_processing_start_time() const { return package_processing_start_time; }

    virtual IPackageStockpile::const_iterator begin() const override { return q_->cbegin(); }
    virtual IPackageStockpile::const_iterator end() const override { return q_->cend(); }
    virtual IPackageStockpile::const_iterator cbegin() const override { return q_->cbegin(); }
    virtual IPackageStockpile::const_iterator cend() const override { return q_->cend(); }

    ReceiverType get_receiver_type() override { return ReceiverType::WORKER; }
    IPackageQueue * get_queue() const { return q_.get(); }

    Worker() = default;
    Worker(Worker&&) = default;
    Worker(Worker&) = delete;
private:
    ElementID id_;
    TimeOffset pd_;
    std::unique_ptr<IPackageQueue> q_;

    Time package_processing_start_time;
};
#endif //SIECI_NODES_HPP
