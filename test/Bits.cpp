//
// Bits.cpp
//
// Copyright Matthew Caryl 2024
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Bits.hpp"

#include <bitset>
#include <random>

#include <gtest/gtest.h>

using namespace Tier0;

////////////////////////////////////////////////////////////////////////////////////////////////////

using Value = uint32_t;
static const uint8_t sc_Size = 32;
using Bitset = std::bitset<sc_Size>;

static const uint8_t sc_TestCount = 100;

////////////////////////////////////////////////////////////////////////////////////////////////////

static Value GetBits(uint8_t index, uint8_t size)
{
	Bitset bitset;
	for (uint8_t i = 0; i < size; i++)
	{
		bitset[index + i] = true;
	}
	return bitset.to_ullong();
}

static Value GetBit(uint8_t index)
{
	return GetBits(index, 1);
}

static Value BitsetGetAt(Value value, uint8_t index, uint8_t size)
{
	Bitset bits(value);
	bits >>= index;
	for (uint8_t i = size; i < bits.size(); i++)
	{
		bits[i] = false;
	}
	return static_cast<Value>(bits.to_ullong());
}

static Value SetBitsetAt(Value value, uint8_t index, uint8_t size, Value subValue)
{
	Bitset bits(value);
	Bitset subBits(subValue);
	for (uint8_t i = 0; i < size; i++)
	{
		bits[index + i] = subBits[i];
	}
	return static_cast<Value>(bits.to_ullong());
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(Bits, GetBitset)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_EQ(bit, 1u << i);
	}
	for (uint8_t i = 0; i < sc_Size - 1; i++)
	{
		auto bits = GetBits(i, 2);
		EXPECT_EQ(bits, 3u << i);
	}
}

TEST(Bits, Mask)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_EQ(Bits::Mask<Value>(i), bit) << std::to_string(i);

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBits(i, j);
			EXPECT_EQ(Bits::Mask<Value>(i, j), bits) << std::to_string(i);
		}
	}
}

TEST(Bits, MaskUpTo)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bits = GetBits(0, i + 1);
		EXPECT_EQ(Bits::MaskUpTo<Value>(i), bits);
	}
}

TEST(Bits, IsAllSet)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_TRUE(Bits::IsAllSet<Value>(bit, i));
		EXPECT_FALSE(Bits::IsAllSet<Value>(~bit, i));

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBits(i, j);
			EXPECT_TRUE(Bits::IsAllSet<Value>(bits, i, j));
			EXPECT_TRUE(Bits::IsAllSet<Value>(~bits, 0, i));
			EXPECT_TRUE(Bits::IsAllSet<Value>(~bits, i + j, sc_Size - i - j));
		}
	}
}

TEST(Bits, IsAnySet)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		// We must check at least one bit for IsAnySet to be true.
		for (uint8_t j = 1; i + j < sc_Size; j++)
		{
			EXPECT_TRUE(Bits::IsAnySet<Value>(bit, i, j));
		}
	}
}

TEST(Bits, IsNoneSet)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_TRUE(Bits::IsNoneSet<Value>(~bit, i));
		EXPECT_FALSE(Bits::IsNoneSet<Value>(bit, i));

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBits(i, j);
			EXPECT_TRUE(Bits::IsNoneSet<Value>(~bits, i, j));
			EXPECT_TRUE(Bits::IsNoneSet<Value>(bits, 0, i));
			EXPECT_TRUE(Bits::IsNoneSet<Value>(bits, i + j, sc_Size - i - j));
		}
	}
}

TEST(Bits, Set)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_EQ(Bits::Set<Value>(0u, i), bit);
		EXPECT_EQ(Bits::Set<Value>(bit, i), bit);

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBits(i, j);
			EXPECT_EQ(Bits::Set<Value>(0u, i, j), bits);
			EXPECT_EQ(Bits::Set<Value>(bits, i, j), bits);
		}
	}
}

TEST(Bits, Clear)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_EQ(Bits::Clear<Value>(bit, i), 0u);
		EXPECT_EQ(Bits::Clear<Value>(~bit, i), static_cast<Value>(~bit));

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBits(i, j);
			EXPECT_EQ(Bits::Clear<Value>(bits, i, j), 0u);
			EXPECT_EQ(Bits::Clear<Value>(~bits, i, j), static_cast<Value>(~bits));
		}
	}
}

TEST(Bits, Flip)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_EQ(Bits::Flip<Value>(0u, i), bit);
		EXPECT_EQ(Bits::Flip<Value>(bit, i), 0u);

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBits(i, j);
			EXPECT_EQ(Bits::Flip<Value>(0u, i, j), bits);
			EXPECT_EQ(Bits::Flip<Value>(bits, i, j), 0u);
		}
	}
}

TEST(Bits, GetAt)
{
	std::mt19937 gen32;

	for (uint8_t i = 0; i < sc_TestCount; i++)
	{
		Value value = gen32();
		uint8_t index = std::uniform_int_distribution<>(0, 31)(gen32);
		uint8_t size = std::uniform_int_distribution<>(1, sc_Size - index)(gen32);
		EXPECT_EQ(Bits::GetAt(value, index, size), BitsetGetAt(value, index, size));
	}
}

TEST(Bits, SetAt)
{
	std::mt19937 gen32;

	for (uint8_t i = 0; i < sc_TestCount; i++)
	{
		Value value = gen32();
		uint8_t index = std::uniform_int_distribution<>(0, 31)(gen32);
		uint8_t size = std::uniform_int_distribution<>(1, sc_Size - index)(gen32);
		Value subValue = gen32();
		EXPECT_EQ(Bits::SetAt(value, index, size, subValue), SetBitsetAt(value, index, size, subValue));
	}
}
