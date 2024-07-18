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

static Bitset GetBitset(uint8_t index, uint8_t size = 1)
{
	Bitset bitset;
	for (uint8_t i = 0; i < size; i++)
	{
		bitset[index + i] = true;
	}
	return bitset;
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
		auto bit = GetBitset(i);
		EXPECT_EQ(bit.to_ullong(), 1u << i);
	}
	for (uint8_t i = 0; i < sc_Size - 1; i++)
	{
		auto bit = GetBitset(i, 2);
		EXPECT_EQ(bit.to_ullong(), 3u << i);
	}
}

TEST(Bits, Mask)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBitset(i);
		EXPECT_EQ(Bits::Mask<Value>(i), bit.to_ullong()) << std::to_string(i);

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBitset(i, j);
			EXPECT_EQ(Bits::Mask<Value>(i, j), bits.to_ullong()) << std::to_string(i);
		}
	}
}

TEST(Bits, MaskUpTo)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bits = GetBitset(0, i + 1);
		EXPECT_EQ(Bits::MaskUpTo<Value>(i), bits.to_ullong());
	}
}

TEST(Bits, IsAllSet)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBitset(i);
		EXPECT_TRUE(Bits::IsAllSet<Value>(bit.to_ullong(), i));
		EXPECT_FALSE(Bits::IsAllSet<Value>(~bit.to_ullong(), i));

		Bits::Index<Value> bitIndex(i);
		EXPECT_TRUE(bitIndex.IsSet(bit.to_ullong()));
		EXPECT_FALSE(bitIndex.IsSet(~bit.to_ullong()));

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBitset(i, j);
			EXPECT_TRUE(Bits::IsAllSet<Value>(bits.to_ullong(), i, j));
			EXPECT_TRUE(Bits::IsAllSet<Value>(~bits.to_ullong(), 0, i));
			EXPECT_TRUE(Bits::IsAllSet<Value>(~bits.to_ullong(), i + j, sc_Size - i - j));
		}
	}
}

TEST(Bits, IsAnySet)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bits = GetBitset(i);
		// We must check at least one bit for IsAnySet to be true.
		for (uint8_t j = 1; i + j < sc_Size; j++)
		{
			EXPECT_TRUE(Bits::IsAnySet<Value>(bits.to_ullong(), i, j));
		}
	}
}

TEST(Bits, IsNoneSet)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBitset(i);
		EXPECT_TRUE(Bits::IsNoneSet<Value>(~bit.to_ullong(), i));
		EXPECT_FALSE(Bits::IsNoneSet<Value>(bit.to_ullong(), i));

		Bits::Index<Value> bitIndex(i);
		EXPECT_TRUE(bitIndex.IsClear(~bit.to_ullong()));
		EXPECT_FALSE(bitIndex.IsClear(bit.to_ullong()));

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBitset(i, j);
			EXPECT_TRUE(Bits::IsNoneSet<Value>(~bits.to_ullong(), i, j));
			EXPECT_TRUE(Bits::IsNoneSet<Value>(bits.to_ullong(), 0, i));
			EXPECT_TRUE(Bits::IsNoneSet<Value>(bits.to_ullong(), i + j, sc_Size - i - j));
		}
	}
}

TEST(Bits, Set)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBitset(i);
		EXPECT_EQ(Bits::Set<Value>(0u, i), bit.to_ullong());
		EXPECT_EQ(Bits::Set<Value>(bit.to_ullong(), i), bit.to_ullong());

		Bits::Index<Value> bitIndex(i);
		EXPECT_EQ(bitIndex.Set(0u), bit.to_ullong());
		EXPECT_EQ(bitIndex.Set(bit.to_ullong()), bit.to_ullong());

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBitset(i, j);
			EXPECT_EQ(Bits::Set<Value>(0u, i, j), bits.to_ullong());
			EXPECT_EQ(Bits::Set<Value>(bits.to_ullong(), i, j), bits.to_ullong());
		}
	}
}

TEST(Bits, Clear)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBitset(i);
		EXPECT_EQ(Bits::Clear<Value>(bit.to_ullong(), i), 0u);
		bit.flip();
		EXPECT_EQ(Bits::Clear<Value>(bit.to_ullong(), i), bit.to_ullong());
		bit.flip();

		Bits::Index<Value> bitIndex(i);
		EXPECT_EQ(bitIndex.Clear(bit.to_ullong()), 0u);
		bit.flip();
		EXPECT_EQ(bitIndex.Clear(bit.to_ullong()), bit.to_ullong());
		bit.flip();

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBitset(i, j);
			EXPECT_EQ(Bits::Clear<Value>(bits.to_ullong(), i, j), 0u);
			bits.flip();
			EXPECT_EQ(Bits::Clear<Value>(bits.to_ullong(), i, j), bits.to_ullong());
		}
	}
}

TEST(Bits, Flip)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBitset(i);
		EXPECT_EQ(Bits::Flip<Value>(0u, i), bit.to_ullong());
		EXPECT_EQ(Bits::Flip<Value>(bit.to_ullong(), i), 0u);

		Bits::Index<Value> bitIndex(i);
		EXPECT_EQ(bitIndex.Flip(0u), bit.to_ullong());
		EXPECT_EQ(bitIndex.Flip(bit.to_ullong()), 0u);

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBitset(i, j);
			EXPECT_EQ(Bits::Flip<Value>(0u, i, j), bits.to_ullong());
			EXPECT_EQ(Bits::Flip<Value>(bits.to_ullong(), i, j), 0u);
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
