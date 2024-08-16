#pragma once
#include <string>
#include <chrono>
#include <iostream>

#define PROFILE_CONCAT_INTERNAL(X, Y) X ## Y
#define PROFILE_CONCAT(X, Y) PROFILE_CONCAT_INTERNAL(X, Y)
#define UNIQUE_VAR_NAME_PROFILE PROFILE_CONCAT(profile_guard_, __LINE__)
#define LOG_DURATION(x) LogDuration UNIQUE_VAR_NAME_PROFILE(x)
#define LOG_DURATION_STREAM(x, os) LogDuration UNIQUE_VAR_NAME_PROFILE(x, os)

using CHRONO = std::chrono::steady_clock;
class LogDuration {
public:
    LogDuration(std::string name, std::ostream& os = std::cerr) : name_(name), os_(os){

    }

    ~LogDuration() {
        const auto end_time = CHRONO::now();
        const auto duration = end_time - start_time;
        os_ << name_ << std::chrono::duration_cast<std::chrono::milliseconds>(duration).count() << " ms" << std::endl;
    }
private:
    std::string name_;
    std::ostream& os_;
    const CHRONO::time_point start_time = CHRONO::now();
};
