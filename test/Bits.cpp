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

static Value GetBit(uint8_t index)
{
	Bitset bitset;
	bitset[index] = true;
	return bitset.to_ullong();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

TEST(Bits, GetBit)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_EQ(bit, 1u << i);
	}
}

TEST(Bits, Mask)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_EQ(Bits::Mask<Value>(i), bit);
	}
}

TEST(Bits, IsSet)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_TRUE(Bits::IsAllSet<Value>(bit, i));
		EXPECT_FALSE(Bits::IsAllSet<Value>(~bit, i));
	}
}

TEST(Bits, IsClear)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_TRUE(Bits::IsClear<Value>(~bit, i));
		EXPECT_FALSE(Bits::IsClear<Value>(bit, i));
	}
}

TEST(Bits, Set)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_EQ(Bits::Set<Value>(0u, i), bit);
		EXPECT_EQ(Bits::Set<Value>(bit, i), bit);
	}
}

TEST(Bits, Clear)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_EQ(Bits::Clear<Value>(bit, i), 0u);
		EXPECT_EQ(Bits::Clear<Value>(~bit, i), static_cast<Value>(~bit));
	}
}

TEST(Bits, Flip)
{
	for (uint8_t i = 0; i < sc_Size; i++)
	{
		auto bit = GetBit(i);
		EXPECT_EQ(Bits::Flip<Value>(0u, i), bit);
		EXPECT_EQ(Bits::Flip<Value>(bit, i), 0u);
	}
}
