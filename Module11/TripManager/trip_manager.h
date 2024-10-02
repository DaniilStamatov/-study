//#pragma once
//#include "flight_provider.h"
//#include "hotel_provider.h"
//
//#include <string>
//#include <vector>
//
//class Trip {
//private:
//    HotelProvider& hp_;
//    FlightProvider& fp_;
//public:
//    vector<FlightProvider::BookingId> flights;
//    vector<HotelProvider::BookingId> hotels;
//
//    Trip(HotelProvider& hp, FlightProvider& fp) : hp_(hp), fp_(fp){
//        
//    }
//
//    Trip(const Trip& other) : hp_(other.hp_), fp_(other.fp_) {
//    }
//
//    Trip(Trip&& other) : hp_(other.hp_), fp_(other.fp_) {
//
//    }
//    Trip& operator=(const Trip& other) {
//        hp_ = other.hp_;
//        fp_ = other.fp_;
//        return *this;
//    }
//    Trip& operator=(Trip&& other) {
//        hp_ = other.hp_;
//        fp_ = other.fp_;
//        return *this;
//    }
//    void Cancel() {
//        for (auto& hotel : hotels) {
//            hp_.Cancel(hotel);
//        }
//        hotels.clear();
//        for (auto& flight : flights) {
//            fp_.Cancel(flight);
//        }
//        flights.clear();
//
//    }
//    ~Trip() {
//        Cancel();
//    }
//
//
//};
//
//class TripManager {
//public:
//    using BookingId = std::string;
//    struct BookingData {
//        std::string city_from;
//        std::string city_to;
//        std::string date_from;
//        std::string date_to;
//    };
//
//    Trip Book(const BookingData& data) {
//        Trip trip(hotel_provider_, flight_provider_);
//        {
//            FlightProvider::BookingData flight_booking_data;
//            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
//        }
//        {
//            HotelProvider::BookingData hotel_booking_data;
//            trip.hotels.push_back(hotel_provider_.Book(hotel_booking_data));
//        }
//        {
//            FlightProvider::BookingData flight_booking_data;
//            trip.flights.push_back(flight_provider_.Book(flight_booking_data));
//        }
//        return trip;
//    }
//
//    void Cancel(Trip& trip) {
//        trip.Cancel();
//    }
//
//private:
//    HotelProvider hotel_provider_;
//    FlightProvider flight_provider_;
//};