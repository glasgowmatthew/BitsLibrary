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

	template <typename Value>
	Value GetAt(Value value, uint8_t index, uint8_t size)
	{
		const auto mask = Mask<Value>(index, size);
		return (value & mask) >> index;
	}

	template <typename Value>
	Value SetAt(Value value, uint8_t index, uint8_t size, Value subValue)
	{
		const auto mask = Mask<Value>(index, size);
		subValue = subValue << index;
		return (value & ~mask) | (subValue & mask);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename Value>
	class Index
	{
	public:
		Index(uint8_t index) :
			m_Index(index) {}

		bool IsSet(Value value) const
		{
			return Bits::IsAllSet<Value>(value, m_Index);
		}

		bool IsClear(Value value) const
		{
			return Bits::IsNoneSet<Value>(value, m_Index);
		}

		Value Set(Value value) const
		{
			return Bits::Set(value, m_Index);
		}

		Value Clear(Value value) const
		{
			return Bits::Clear(value, m_Index);
		}

		Value Flip(Value value) const
		{
			return Bits::Flip(value, m_Index);
		}

	private:
		uint8_t m_Index;
	};
}
