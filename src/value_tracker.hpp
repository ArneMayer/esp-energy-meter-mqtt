#pragma once

#include <vector>
#include <map>

template<typename Identificator, typename ValueType>
class ValueTracker {
public:
    using time_ms = unsigned long;

    ValueTracker(time_ms notification_time) : _notification_time{notification_time} {

    }

    void submit_values(std::vector<Identificator, ValueType> values) {

    }

    std::vector<Identificator, ValueType> get_notifications() {
        return {};
    }

    private:
    time_ms _notification_time;
    std::map<Identificator, ValueType> last_seen_value;
    std::map<Identificator, time_ms> last_notification_time;
};