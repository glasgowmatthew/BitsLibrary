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
	uint8_t BitCount()
	{
		return sizeof(Value) * 8;
	}

	template <typename Value>
	Value MaskUpTo(uint8_t index)
	{
		return std::numeric_limits<Value>::max() >> (BitCount<Value>() - 1 - index);
	}

	template <typename Value>
	Value Mask(uint8_t index, uint8_t size = 1)
	{
		if (size == 0)
		{
			return 0;
		}
		Value value = MaskUpTo<Value>(index + size - 1);
		if (index == 0)
		{
			return value;
		}
		return value & ~MaskUpTo<Value>(index - 1);
	}

	template <typename Value>
	bool IsAllSet(Value value, uint8_t index, uint8_t size = 1)
	{
		const auto mask = Mask<Value>(index, size);
		return (value & mask) == mask;
	}

	template <typename Value>
	bool IsAnySet(Value value, uint8_t index, uint8_t size = 1)
	{
		const auto mask = Mask<Value>(index, size);
		return (value & mask) != 0;
	}

	template <typename Value>
	bool IsNoneSet(Value value, uint8_t index, uint8_t size = 1)
	{
		const auto mask = Mask<Value>(index, size);
		return (value & mask) == 0;
	}

	template <typename Value>
	Value Clear(Value value, uint8_t index, uint8_t size = 1)
	{
		return value & ~Mask<Value>(index, size);
	}

	template <typename Value>
	Value Set(Value value, uint8_t index, uint8_t size = 1)
	{
		return value | Mask<Value>(index, size);
	}

	template <typename Value>
	Value Flip(Value value, uint8_t index, uint8_t size = 1)
	{
		return value ^ Mask<Value>(index, size);
	}
}
