//
// Bits.cpp
//
// Copyright Matthew Caryl 2024
//
////////////////////////////////////////////////////////////////////////////////////////////////////

#include "Bits.hpp"

#include <bitset>

#include <gtest/gtest.h>

using namespace Tier0;

////////////////////////////////////////////////////////////////////////////////////////////////////

using Value = uint32_t;
static const uint8_t sc_Size = 32;

////////////////////////////////////////////////////////////////////////////////////////////////////

static std::bitset<sc_Size> GetBitset(uint8_t index, uint8_t size = 1)
{
	std::bitset<sc_Size> bitset;
	for (uint8_t i = 0; i < size; i++)
	{
		bitset[index + i] = true;
	}
	return bitset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(Bits, Mask)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBitset(i);
		EXPECT_EQ(Bits::Mask<Value>(i), bit.to_ullong());

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBitset(i, j);
			EXPECT_EQ(Bits::Mask<Value>(i, j), bits.to_ullong());
		}
	}
}

TEST(Bits, MaskUpTo)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bits = GetBitset(0, i);
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

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBitset(i, j);
			EXPECT_TRUE(Bits::IsAllSet<Value>(bits.to_ullong(), i, j));
			EXPECT_TRUE(Bits::IsAllSet<Value>(~bits.to_ullong(), 0, i));
			EXPECT_TRUE(Bits::IsAllSet<Value>(~bits.to_ullong(), j, sc_Size - j));
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

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBitset(i, j);
			EXPECT_TRUE(Bits::IsNoneSet<Value>(~bits.to_ullong(), i, j));
			EXPECT_TRUE(Bits::IsNoneSet<Value>(bits.to_ullong(), 0, i));
			EXPECT_TRUE(Bits::IsNoneSet<Value>(bits.to_ullong(), j, sc_Size - j));
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

		for (uint8_t j = 0; i + j < sc_Size; j++)
		{
			auto bits = GetBitset(i, j);
			EXPECT_EQ(Bits::Flip<Value>(0u, i, j), bits.to_ullong());
			EXPECT_EQ(Bits::Flip<Value>(bits.to_ullong(), i, j), 0u);
		}
	}
}
