// Copyright (c) Advanced Micro Devices, Inc.
// SPDX-License-Identifier: MIT

#pragma once

#include <spdlog/fmt/fmt.h>

#include "common/units/data_size.hpp"
#include "common/units/frequency.hpp"
#include "common/units/power.hpp"

// ---------------------------------------------------------------------------
// fmt::formatter<rocprofsys::common::units::frequency<Rep, Period>>
//
// Default: fmt::format("{}", 2_mhz) -> "2 MHz"
// ---------------------------------------------------------------------------
template <typename Rep, typename Period>
struct fmt::formatter<rocprofsys::common::units::frequency<Rep, Period>>
: fmt::formatter<Rep>
{
    template <typename FormatContext>
    auto format(const rocprofsys::common::units::frequency<Rep, Period>& value,
                FormatContext&                                           ctx) const
    {
        using rocprofsys::common::units::frequency_suffix;
        auto out = fmt::formatter<Rep>::format(value.count(), ctx);
        return fmt::format_to(out, " {}", frequency_suffix<Period>::k_value);
    }
};

// ---------------------------------------------------------------------------
// fmt::formatter<rocprofsys::common::units::data_size<Rep, Scale>>
//
// Default: fmt::format("{}", 2_mb)  -> "2 MB"
//          fmt::format("{}", 2_mib) -> "2 MiB"
// ---------------------------------------------------------------------------
template <typename Rep, typename Scale>
struct fmt::formatter<rocprofsys::common::units::data_size<Rep, Scale>>
: fmt::formatter<Rep>
{
    template <typename FormatContext>
    auto format(const rocprofsys::common::units::data_size<Rep, Scale>& value,
                FormatContext&                                          ctx) const
    {
        using rocprofsys::common::units::data_size_suffix;
        auto out = fmt::formatter<Rep>::format(value.count(), ctx);
        return fmt::format_to(out, " {}", data_size_suffix<Scale>::k_value);
    }
};

// ---------------------------------------------------------------------------
// fmt::formatter<rocprofsys::common::units::power<Rep, Period>>
//
// Default: fmt::format("{}", 250_mw) -> "250 mW"
// ---------------------------------------------------------------------------
template <typename Rep, typename Period>
struct fmt::formatter<rocprofsys::common::units::power<Rep, Period>> : fmt::formatter<Rep>
{
    template <typename FormatContext>
    auto format(const rocprofsys::common::units::power<Rep, Period>& value,
                FormatContext&                                       ctx) const
    {
        using rocprofsys::common::units::power_suffix;
        auto out = fmt::formatter<Rep>::format(value.count(), ctx);
        return fmt::format_to(out, " {}", power_suffix<Period>::k_value);
    }
};
