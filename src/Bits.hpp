//
// Bits.hpp
//
// Copyright Matthew Caryl 2024
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Tier0::Bits
{
    template <typename Value>
    constexpr Value Mask(uint8_t index)
    {
        return 1u << index;
    }

    template <typename Value>
    constexpr bool IsClear(Value value, uint8_t index)
    {
        return !IsSet<Value>(value, index);
    }

    template <typename Value>
    constexpr bool IsSet(Value value, uint8_t index)
    {
		const auto mask = Mask<Value>(index);
        return value & mask;
    }

    template <typename Value>
    constexpr Value Clear(Value value, uint8_t index)
    {
		const auto mask = Mask<Value>(index);
        return value & ~mask;
    }

    template <typename Value>
    constexpr Value Set(Value value, uint8_t index)
    {
		const auto mask = Mask<Value>(index);
        return value | mask;
    }

    template <typename Value>
    constexpr Value Flip(Value value, uint8_t index)
    {
		const auto mask = Mask<Value>(index);
        return value ^ mask;
    }
}
