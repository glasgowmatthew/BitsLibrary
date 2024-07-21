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
	constexpr Value MaskUpTo(uint8_t index)
	{
		return std::numeric_limits<Value>::max() >> (BitCount<Value>() - 1 - index);
	}

	template <typename Value>
	constexpr Value Mask(uint8_t index, uint8_t size = 1)
	{
		return Range<Value>(index, size).GetMask();
	}

	template <typename Value>
	constexpr bool IsAllSet(Value value, uint8_t index, uint8_t size = 1)
	{
		return Range<Value>(index, size).IsAllSet(value);
	}

	template <typename Value>
	constexpr bool IsAnySet(Value value, uint8_t index, uint8_t size = 1)
	{
		return Range<Value>(index, size).IsAnySet(value);
	}

	template <typename Value>
	constexpr bool IsNoneSet(Value value, uint8_t index, uint8_t size = 1)
	{
		return Range<Value>(index, size).IsNoneSet(value);
	}

	template <typename Value>
	constexpr Value Clear(Value value, uint8_t index, uint8_t size = 1)
	{
		return Range<Value>(index, size).Clear(value);
	}

	template <typename Value>
	constexpr Value Set(Value value, uint8_t index, uint8_t size = 1)
	{
		return Range<Value>(index, size).Set(value);
	}

	template <typename Value>
	constexpr Value Flip(Value value, uint8_t index, uint8_t size = 1)
	{
		return Range<Value>(index, size).Flip(value);
	}

	template <typename Value>
	constexpr Value GetAt(Value value, uint8_t index, uint8_t size)
	{
		return Range<Value>(index, size).GetAt(value);
	}

	template <typename Value>
	constexpr Value SetAt(Value value, uint8_t index, uint8_t size, Value subValue)
	{
		return Range<Value>(index, size).SetAt(value, subValue);
	}

	////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename Value>
	class Index
	{
	public:
		Index(uint8_t index) :
			m_Range(index, 1) {}

		Value GetMask() const
		{
			return m_Range.GetMask();
		}

		bool IsSet(Value value) const
		{
			return m_Range.IsAllSet(value);
		}

		bool IsClear(Value value) const
		{
			return m_Range.IsNoneSet(value);
		}

		Value Set(Value value) const
		{
			return m_Range.Set(value);
		}

		Value Clear(Value value) const
		{
			return m_Range.Clear(value);
		}

		Value Flip(Value value) const
		{
			return m_Range.Flip(value);
		}

	private:
		Range<Value> m_Range;
	};

	////////////////////////////////////////////////////////////////////////////////////////////////

	template <typename Value>
	class Range
	{
	public:
		Range(uint8_t index, uint8_t size) :
			m_Index(index),
			m_Size(size),
			m_Mask(CalculateMask(m_Index, m_Size)) {}

		Value GetMask() const
		{
			return m_Mask;
		}

		bool IsAllSet(Value value) const
		{
			return (value & m_Mask) == m_Mask;
		}

		bool IsAnySet(Value value) const
		{
			return (value & m_Mask) != 0;
		}

		bool IsNoneSet(Value value) const
		{
			return (value & m_Mask) == 0;
		}

		Value Set(Value value) const
		{
			return value | m_Mask;
		}

		Value Clear(Value value) const
		{
			return value & ~m_Mask;
		}

		Value Flip(Value value) const
		{
			return value ^ m_Mask;
		}

		Value GetAt(Value value) const
		{
			return (value & m_Mask) >> m_Index;
		}

		Value SetAt(Value value, Value subValue) const
		{
			subValue = subValue << m_Index;
			return (value & ~m_Mask) | (subValue & m_Mask);

		}

	private:
		uint8_t m_Index;
		uint8_t m_Size;
		Value m_Mask;

		constexpr Value CalculateMask(uint8_t index, uint8_t size = 1)
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
	};
}
