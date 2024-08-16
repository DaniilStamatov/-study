#pragma once

#include <cmath>

namespace geo {

	struct Coordinates {
		double latitude;
		double longitude;

		bool operator==(const Coordinates& other) const {
			return (other.latitude == latitude) && (other.longitude == longitude);
		}

		bool operator!=(const Coordinates& other) const {
			return !(*this == other);
		}
	};

	double ComputeDistance(const Coordinates from, const Coordinates to) {
		if (from == to) return 0;


		constexpr double R = 6371.01; // Earth's radius in kilometers
		constexpr double pi = 3.141592653589793238463;
		constexpr double deg_to_rad = pi / 180;

		double dLat = (to.latitude - from.latitude) * deg_to_rad;
		double dLon = (to.longitude - from.longitude) * deg_to_rad; // calculates distance between latitude and longitude and transforms to radians

		double from_latitude = from.latitude * deg_to_rad;
		double to_latitude = to.latitude * deg_to_rad;

		double a = sin(dLat / 2) * sin(dLat / 2) + sin(dLon / 2) * sin(dLon / 2) * cos(from_latitude) * cos(to_latitude);

		double c = 2 * atan2(sqrt(a), sqrt(1 - a));
		return R * c;
	}
}