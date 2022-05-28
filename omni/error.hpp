#pragma once

#include <omni/core.hpp>
#include <omni/logger.hpp>

namespace Omni
{
using Error = uint32;

class ErrorState
{
public:
    constexpr ErrorState() noexcept = default;
    constexpr ErrorState(Error error) noexcept : error_(error)
    {}

    constexpr ~ErrorState() noexcept = default;

    constexpr Error error() const noexcept
    {
        return this->error_;
    }

    constexpr bool is_null()
    {
        return this->error_ == 0u;
    }

    constexpr void clear()
    {
        this->error_ = 0u;
    }

    void post_error(Error err)
    {
        omni_soft_assert(this->error_ == 0u,
            "Posting new error: [{}] without first clearing previous error: [{}]",
            err, this->error_);

        this->error_ = err;
    }

    ErrorState& operator=(const ErrorState& other) = delete;

    constexpr bool operator==(const ErrorState& other) const = default;
    constexpr bool operator!=(const ErrorState& other) const = default;
    constexpr auto operator<=>(const ErrorState&) const = default;

private:
    Error error_;
};

inline thread_local ErrorState error_state;
}
