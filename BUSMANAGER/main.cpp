#include <cassert>
#include <iostream>
#include <map>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses,
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator>>(istream& is, Query& q) {
    string _type;
    is >> _type;
    if (_type == "NEW_BUS"s) {
        q.type = QueryType::NewBus;
        string _stop_count_str;
        is >> q.bus >> _stop_count_str;
        int _stop_count = stoi(_stop_count_str);
        q.stops.clear();
        for (int i = 0; i < _stop_count; ++i) {
            string _stop;
            is >> _stop;
            q.stops.push_back(_stop);
        }
    } else if (_type == "BUSES_FOR_STOP"s) {
        q.type = QueryType::BusesForStop;
        is >> q.stop;
    } else if (_type == "STOPS_FOR_BUS"s) {
        q.type = QueryType::StopsForBus;
        is >> q.bus;
    } else if (_type == "ALL_BUSES"s) {
        q.type = QueryType::AllBuses;
    }
    return is;
}

struct BusesForStopResponse {
    vector<string> buses;
};

ostream& operator<<(ostream& os, const BusesForStopResponse& r) {
    if (r.buses.empty()) {
        os << "No stop"s;
        return os;
    }
    bool first = true;
    for (const auto& bus : r.buses) {
        if (first) {
            os << bus;
            first = false;
            continue;
        }
        os << " "s;
        os << bus;
    }
    return os;
}

struct StopsForBusResponse {
    vector<pair<string, vector<string>>> bus_stops; //остановка, которая лежит на маршруте bus и вектор других автобусов, проходящих через неё
};

ostream& operator<<(ostream& os, const StopsForBusResponse& r) {
    int size = static_cast<int>(r.bus_stops.size());
    if (size == 0) {
        os << "No bus"s;
        return os;
    }
    int i = 0;
    for (const auto& [stop, buses]:r.bus_stops) {
        os << "Stop "s << stop << ": "s;
        if (buses.empty()) {
            os << "no interchange"s;
        }
        bool first = true;
        for (const auto& bus : buses) {
            if (first) {
                os << bus;
                first = false;
                continue;
            }
            os << " "s;
            os << bus;
        }
        if (i < size - 1) {
            os << endl;
        }
        ++i;
    }
    return os;
}

struct AllBusesResponse {
    map<string, vector<string>> buses;
};

ostream& operator<<(ostream& os, const AllBusesResponse& r) {
    int size_dict = static_cast<int>(r.buses.size());
    if (size_dict == 0) {
        os << "No buses"s;
        return os;
    }

    int i = 0;
    for (const auto& [bus, stops] : r.buses) {
        os << "Bus "s << bus << ": "s;
        bool first = true;
        for (const auto& stop : stops) {
            if (first) {
                os << stop;
                first = false;
                continue;
            }
            os << " "s;
            os << stop;
        }
        if (i < size_dict-1) {
            os << endl;
        }
        ++i;
    }

    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        stops_to_buses_[bus] = stops;
        for (const auto& stop : stops) {
            buses_to_stops_[stop].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse result;
        if (buses_to_stops_.count(stop) > 0) {
            result.buses = buses_to_stops_.at(stop);
        }
        return result;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse result;
        if (stops_to_buses_.count(bus) > 0) {
            for (const auto& stop : stops_to_buses_.at(bus)) {
                vector<string> temp_buses;
                for (const auto& _bus : buses_to_stops_.at(stop)) {
                    if (_bus != bus) {
                        temp_buses.push_back(_bus);
                    }
                }
                result.bus_stops.push_back(make_pair(stop, temp_buses));
            }
        }
        return result;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse result;
        result.buses = stops_to_buses_;
        return result;
    }
private:
    map<string, vector<string>> stops_to_buses_; // автобус и его вектор остановок;
    map<string, vector<string>> buses_to_stops_; //остановка и её вектор автобусов;
};

void TestAddBus() {
    BusManager bm;
    bm.AddBus("32", {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s});
    bm.AddBus("32K", {"Tolstopaltsevo"s, "Marushkino"s, "Vnukovo"s, "Peredelkino"s, "Solntsevo"s, "Skolkovo"s});
}

int main() {
    int query_count;
    Query q;

    cin >> query_count;

    BusManager bm;
    for (int i = 0; i < query_count; ++i) {
        cin >> q;
        switch (q.type) {
        case QueryType::NewBus:
            bm.AddBus(q.bus, q.stops);
            break;
        case QueryType::BusesForStop:
            cout << bm.GetBusesForStop(q.stop) << endl;
            break;
        case QueryType::StopsForBus:
            cout << bm.GetStopsForBus(q.bus) << endl;
            break;
        case QueryType::AllBuses:
            cout << bm.GetAllBuses() << endl;
            break;
        }
    }
}
