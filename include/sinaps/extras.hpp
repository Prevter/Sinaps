#pragma once

#include <string>
#include <stdexcept>
#include <utility>

namespace sinaps {

    /// @brief Result structure for error handling
    template<typename T>
    struct Result {
        T value;
        bool success;
        std::string error;

        Result(T value, bool success, std::string error)
                : value(value), success(success), error(std::move(error)) {}

        bool isOk() {
            return success;
        }

        bool isErr() {
            return !success;
        }

        T val() {
            return value;
        }

        std::string err() {
            return error;
        }

        static Result<T> ok(T value) {
            return Result(value, true, "");
        }

        static Result<T> fail(std::string error) {
            return Result(T(), false, error);
        }
    };

}