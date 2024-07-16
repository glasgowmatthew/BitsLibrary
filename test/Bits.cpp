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

static std::bitset<sc_Size> GetBitset(uint8_t index)
{
	std::bitset<sc_Size> bitset;
	bitset[index] = true;
	return bitset;
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(Bits, Mask)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bitset = GetBitset(i);
		EXPECT_EQ(Bits::Mask<Value>(i), bitset.to_ullong());
	}
}

TEST(Bits, IsClear)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bitset = GetBitset(i);
		EXPECT_TRUE(Bits::IsClear<Value>(~bitset.to_ullong(), i));
		EXPECT_FALSE(Bits::IsClear<Value>(~bitset.to_ullong(), i));
	}
}

TEST(Bits, IsSet)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bitset = GetBitset(i);
		EXPECT_TRUE(Bits::IsSet<Value>(bitset.to_ullong(), i));
		EXPECT_FALSE(Bits::IsSet<Value>(~bitset.to_ullong(), i));
	}
}

TEST(Bits, Set)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bitset = GetBitset(i);
		EXPECT_EQ(Bits::Set<Value>(0u, i), bitset.to_ullong());
		EXPECT_EQ(Bits::Set<Value>(bitset.to_ullong(), i), bitset.to_ullong());
	}
}

TEST(Bits, Unset)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bitset = GetBitset(i);
		EXPECT_EQ(Bits::Clear<Value>(bitset.to_ullong(), i), 0u);
		bitset.flip();
		EXPECT_EQ(Bits::Clear<Value>(bitset.to_ullong(), i), bitset.to_ullong());
	}
}

TEST(Bits, Flip)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bitset = GetBitset(i);
		EXPECT_EQ(Bits::Flip<Value>(0u, i), bitset.to_ullong());
		EXPECT_EQ(Bits::Flip<Value>(bitset.to_ullong(), i), 0u);
	}
}
