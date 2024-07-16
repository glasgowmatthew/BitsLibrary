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
	Value Mask(uint8_t index)
	{
		return 1u << index;
	}

	template <typename Value>
	bool IsClear(Value value, uint8_t index)
	{
		return !IsSet<Value>(value, index);
	}

	template <typename Value>
	bool IsSet(Value value, uint8_t index)
	{
		return value & Mask<Value>(index);
	}

	template <typename Value>
	Value Clear(Value value, uint8_t index)
	{
		return value & ~Mask<Value>(index);
	}

	template <typename Value>
	Value Set(Value value, uint8_t index)
	{
		return value | Mask<Value>(index);
	}

	template <typename Value>
	Value Set(Value value, uint8_t index, bool set)
	{
		return set ? Set(value, index) : Clear(value, index);
	}

	template <typename Value>
	Value Flip(Value value, uint8_t index)
	{
		return value ^ Mask<Value>(index);
	}
}
