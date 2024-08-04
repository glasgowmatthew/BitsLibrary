//
// Bits.hpp
//
// Copyright Matthew Caryl 2024
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <limits>

////////////////////////////////////////////////////////////////////////////////////////////////////

namespace Tier0::Bits
{
	template <typename Value>
	constexpr uint8_t BitCount()
	{
		return sizeof(Value) * 8;
	}
	
    template <typename Value>
    constexpr Value Mask(uint8_t index)
    {
        return 1u << index;
    }

	template <typename Value>
	constexpr Value MaskUpTo(uint8_t index)
	{
		return std::numeric_limits<Value>::max() >> (BitCount<Value>() - 1 - index);
	}

	template <typename Value>
	constexpr bool IsAllSet(Value value, uint8_t index, uint8_t size = 1)
	{
		const auto mask = Mask<Value>(index);
        return (value & mask) == mask;
	}

	template <typename Value>
	constexpr bool IsAnySet(Value value, uint8_t index, uint8_t size = 1)
	{
		const auto mask = Mask<Value>(index);
        return (value & mask) != 0;
	}

	template <typename Value>
	constexpr bool IsNoneSet(Value value, uint8_t index, uint8_t size = 1)
	{
		const auto mask = Mask<Value>(index);
        return (value & mask) == 0;
	}

    template <typename Value>
	constexpr Value Clear(Value value, uint8_t index, uint8_t size = 1)
    {
		const auto mask = Mask<Value>(index);
        return value & ~mask;
    }

    template <typename Value>
    constexpr Value Set(Value value, uint8_t index, uint8_t size = 1)
    {
		const auto mask = Mask<Value>(index);
        return value | mask;
    }

    template <typename Value>
    constexpr Value Flip(Value value, uint8_t index, uint8_t size = 1)
    {
		const auto mask = Mask<Value>(index);
        return value ^ mask;
    }
	
    template <typename Value>
    constexpr Value GetAt(Value value, uint8_t index, uint8_t size)
    {
        const auto mask = Mask<Value>(index, size);
        return (value & mask) >> index;
    }

    template <typename Value>
    constexpr Value SetAt(Value value, uint8_t index, uint8_t size, Value subValue)
    {
        const auto mask = Mask<Value>(index, size);
        subValue = subValue << index;
        return (value & ~mask) | (subValue & mask);
    }
}
