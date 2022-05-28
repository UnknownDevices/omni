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
        return error_;
    }

    constexpr bool is_null()
    {
        return this->error() == 0u;
    }

    constexpr void clear()
    {
        this->set_error(0u);
    }

    void post_error(Error err)
    {
        omni_soft_assert(this->error() == 0u,
            "Posting new error: [{}] without first clearing previous error: [{}]",
            err, this->error());

        this->set_error(err);
    }

    ErrorState& operator=(const ErrorState& other) = delete;

    constexpr bool operator==(const ErrorState& other) const = default;
    constexpr bool operator!=(const ErrorState& other) const = default;
    constexpr auto operator<=>(const ErrorState&) const = default;

private:
    constexpr void set_error(Error error) noexcept
    {
        error_ = error;
    }

    Error error_;
};

inline thread_local ErrorState error_state;
}
