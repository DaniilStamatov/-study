#include <iostream>
#include <map>
#include <queue>
#include <set>
using namespace std;
struct Booking {
    const int64_t time;
    const int client_id;
    const int room_count;
};

class HotelManager {
private:
    std::map<string, std::queue<Booking>> bookings_by_hotel;
    bool IsOutDate(int64_t current_time, int64_t time) {
        return !(current_time >= 86400 ? (current_time - 86400) < time : true && time <= current_time);
    }
public:
    void Book(const std::string& hotel_name, const int64_t time, const int client_id, const int room_count) {
        bookings_by_hotel[hotel_name].push({ time, client_id, room_count });
        for (auto& [name, structure] : bookings_by_hotel)
        {
            while (IsOutDate(time, structure.front().time)) {
                structure.pop();
            }
        }
    }

    int ComputeClientCount(const std::string& hotel_name) {
        set<int> clients;

        if (bookings_by_hotel.count(hotel_name)) {
            queue<Booking> copy = bookings_by_hotel.at(hotel_name);
            for (size_t i = 0; i < copy.size(); i++) {
                clients.insert(copy.front().client_id);
                copy.pop();
            }
        }

        return static_cast<int>(clients.size());
    }
    int ComputeRoomCount(const std::string& hotel_name) {
        int count = 0;
        if (bookings_by_hotel.count(hotel_name)) {
            queue<Booking> copy = bookings_by_hotel.at(hotel_name);
            for (size_t i = 0; i < bookings_by_hotel.at(hotel_name).size(); ++i) {
                count += copy.front().room_count;
                copy.pop();
            }
        }
        return count;
    }

    

};

//int main() {
//    HotelManager manager;
//
//    int query_count;
//    cin >> query_count;
//
//    for (int query_id = 0; query_id < query_count; ++query_id) {
//        string query_type;
//        cin >> query_type;
//
//        if (query_type == "BOOK") {
//            int64_t time;
//            cin >> time;
//            string hotel_name;
//            cin >> hotel_name;
//            int client_id, room_count;
//            cin >> client_id >> room_count;
//            manager.Book(hotel_name, time, client_id, room_count);
//        }
//        else {
//            string hotel_name;
//            cin >> hotel_name;
//            if (query_type == "CLIENTS") {
//                cout << manager.ComputeClientCount(hotel_name) << "\n";
//            }
//            else if (query_type == "ROOMS") {
//                cout << manager.ComputeRoomCount(hotel_name) << "\n";
//            }
//        }
//    }
//
//    return 0;
//}