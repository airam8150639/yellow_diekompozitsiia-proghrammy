#include <string>
#include <iostream>
#include <cassert>
#include <vector>
#include <map>

using namespace std;

enum class QueryType {
    NewBus,
    BusesForStop,
    StopsForBus,
    AllBuses
};

struct Query {
    QueryType type;
    string bus;
    string stop;
    vector<string> stops;
};

istream& operator >> (istream& is, Query& q) {
    string operation;
    is >> operation;
    if (operation == "NEW_BUS")        q.type = QueryType :: NewBus;
    if (operation == "BUSES_FOR_STOP") q.type = QueryType :: BusesForStop;
    if (operation == "STOPS_FOR_BUS")  q.type = QueryType :: StopsForBus;
    if (operation == "ALL_BUSES")     q.type = QueryType :: AllBuses;

    q.stops.resize(0);

    switch (q.type) {
    case QueryType::NewBus:
        int stops_number;
        is >> q.bus;
        is >> stops_number;
        for (int i = 0; i < stops_number; ++i) {
            string stop;
            is >> stop;
            q.stops.push_back(stop);
        }
        break;
    case QueryType::BusesForStop :
        is >> q.stop;
        break;
    case QueryType::StopsForBus:
        is >> q.bus;
        break;
    case QueryType::AllBuses: 
        break;
    }
    return is;
}

struct BusesForStopResponse {
    vector<string> buses;
};

ostream& operator << (ostream& os, const BusesForStopResponse& r) {
    if (r.buses.size()) for (auto bus : r.buses) os << bus << " ";
    else os << "No stop";
    return os;
}

struct StopsForBusResponse {
    map<string, vector<string>> stops_to_buses;
    vector<string> stops;
};

ostream& operator << (ostream& os, const StopsForBusResponse& r) {
    bool flag_new_line = false;
    if (!r.stops_to_buses.size()) os << "No bus";
    else {
        for (auto stop : r.stops) {
            if (flag_new_line) os << endl;
            else flag_new_line = true;
            os << "Stop " << stop << ": ";
            if (r.stops_to_buses.at(stop).empty()) os << "no interchange";
            for (auto bus : r.stops_to_buses.at(stop)) os << bus << ' ';
        }
    }
    
    return os;
}

struct AllBusesResponse {
    map<string, vector<string>> buses_to_stops;
};

ostream& operator << (ostream& os, const AllBusesResponse& r) {
    bool flag_next_line = false;
    if (r.buses_to_stops.empty()) {
        os << "No buses";
    }
    else {
        for (const auto& bus_item : r.buses_to_stops) {
            if (!flag_next_line) flag_next_line = true;
            else os << endl;
            os << "Bus " << bus_item.first << ": ";
            for (const string& stop : bus_item.second) os << stop << " ";
        }
    }
    return os;
}

class BusManager {
public:
    void AddBus(const string& bus, const vector<string>& stops) {
        buses_to_stops[bus] = stops;
        for (auto s : stops) {
            stops_to_buses[s].push_back(bus);
        }
    }

    BusesForStopResponse GetBusesForStop(const string& stop) const {
        BusesForStopResponse ans;
        try {
            ans.buses = stops_to_buses.at(stop);
        }
        catch (out_of_range e) {};
        return ans;
    }

    StopsForBusResponse GetStopsForBus(const string& bus) const {
        StopsForBusResponse ans;
        if (buses_to_stops.count(bus) == 0) return ans;
        else {
            for (const string& stop : buses_to_stops.at(bus)) {
                ans.stops.push_back(stop);
                //ans.stops_to_buses[stop] = stops_to_buses.at(stop);
                ans.stops_to_buses[stop];
                for (const string& bus_ : stops_to_buses.at(stop)) if (bus_ != bus) ans.stops_to_buses[stop].push_back(bus_);
            }
        }
        return ans;
    }

    AllBusesResponse GetAllBuses() const {
        AllBusesResponse ans;
        ans.buses_to_stops = buses_to_stops;
        return ans;
    }
private:
    map<string, vector<string>> buses_to_stops, stops_to_buses;
};

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

    return 0;
}