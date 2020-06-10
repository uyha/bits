//
// Created by uy on 10.06.20.
//

#define CATCH_CONFIG_MAIN
#include "catch.hpp"

#include <river/bits.hpp>

auto const bits_8  = river::bits::bits(std::numeric_limits<std::uint8_t>::max());
auto const bits_16 = river::bits::bits(std::numeric_limits<std::uint16_t>::max());
auto const bits_32 = river::bits::bits(std::numeric_limits<std::uint32_t>::max());
auto const bits_64 = river::bits::bits(std::numeric_limits<std::uint64_t>::max());

TEST_CASE("Test expand type trait") {
  using namespace river::bits::details;
  CHECK(std::is_same_v<std::uint8_t, expanded_t<std::uint8_t, 0>>);
  CHECK(std::is_same_v<std::uint16_t, expanded_t<std::uint8_t, 1>>);
  CHECK(std::is_same_v<std::uint32_t, expanded_t<std::uint8_t, 2>>);
  CHECK(std::is_same_v<std::uint32_t, expanded_t<std::uint8_t, 3>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint8_t, 4>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint8_t, 5>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint8_t, 6>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint8_t, 7>>);

  CHECK(std::is_same_v<std::uint16_t, expanded_t<std::uint16_t, 0>>);
  CHECK(std::is_same_v<std::uint32_t, expanded_t<std::uint16_t, 1>>);
  CHECK(std::is_same_v<std::uint32_t, expanded_t<std::uint16_t, 2>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint16_t, 3>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint16_t, 4>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint16_t, 5>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint16_t, 6>>);

  CHECK(std::is_same_v<std::uint32_t, expanded_t<std::uint32_t, 0>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint32_t, 1>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint32_t, 2>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint32_t, 3>>);
  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint32_t, 4>>);

  CHECK(std::is_same_v<std::uint64_t, expanded_t<std::uint64_t, 0>>);
}

TEST_CASE("Compile time construction") {
  using namespace river::bits;
  CHECK(std::is_same_v<Bits<std::uint8_t> const, decltype(bits_8)>);
  CHECK(bits_8.value == std::numeric_limits<std::uint8_t>::max());

  CHECK(std::is_same_v<Bits<std::uint16_t> const, decltype(bits_16)>);
  CHECK(bits_16.value == std::numeric_limits<std::uint16_t>::max());

  CHECK(std::is_same_v<Bits<std::uint32_t> const, decltype(bits_32)>);
  CHECK(bits_32.value == std::numeric_limits<std::uint32_t>::max());

  CHECK(std::is_same_v<Bits<std::uint64_t> const, decltype(bits_64)>);
  CHECK(bits_64.value == std::numeric_limits<std::uint64_t>::max());
}

TEST_CASE("Compile time shifting") {
  using namespace river::bits;
  constexpr std::uint8_t max_8 = std::numeric_limits<std::uint8_t>::max();
  CHECK(std::is_same_v<Bits<std::uint8_t>, decltype(bits_8.lsh<0>())>);
  CHECK(bits_8.lsh<0>().value == max_8);
  CHECK(std::is_same_v<Bits<std::uint16_t>, decltype(bits_8.lsh<1>())>);
  CHECK(bits_8.lsh<1>().value == std::uint16_t{max_8} * 2);
  CHECK(std::is_same_v<Bits<std::uint16_t>, decltype(bits_8.lsh<8>())>);
  CHECK(bits_8.lsh<8>().value == std::uint16_t{max_8} * 256);
  CHECK(std::is_same_v<Bits<std::uint32_t>, decltype(bits_8.lsh<16>())>);
  CHECK(bits_8.lsh<16>().value == std::uint32_t{max_8} * 65536);
  CHECK(std::is_same_v<Bits<std::uint64_t>, decltype(bits_8.lsh<32>())>);
  CHECK(bits_8.lsh<32>().value == std::uint64_t{max_8} * 4294967296);
}

TEST_CASE("Run time operators") {
  using namespace river::bits;
  SECTION("Equal") {
    CHECK(bits(0b10u) == bits(0b10u));
  }
  SECTION("Left shift") {
    CHECK(bits(0b10u) == Bits<std::uint8_t>{1} << 1);
  }

  SECTION("Right shift") {
    CHECK(std::is_same_v<Bits<std::uint8_t>, decltype(Bits<std::uint8_t>{1} << 1)>);
    CHECK(bits(0b10u) == Bits<std::uint8_t>{1} << 1);
  }

  SECTION("And") {
    CHECK(std::is_same_v<Bits<std::uint32_t>, decltype(Bits<std::uint8_t>{0b10u} & bits(0b1u))>);
    CHECK(bits(0b00u) == (Bits<std::uint8_t>{0b10u} & bits(0b1u)));
  }

  SECTION("Or") {
    CHECK(std::is_same_v<Bits<std::uint32_t>, decltype(Bits<std::uint8_t>{0b10u} | bits(0b1u))>);
    CHECK(bits(0b11u) == (Bits<std::uint8_t>{0b10u} | bits(0b1u)));
  }

  SECTION("XOR") {
    CHECK(std::is_same_v<Bits<std::uint32_t>, decltype(Bits<std::uint8_t>{0b101u} ^ bits(0b001u))>);
    CHECK(bits(0b100u) == (Bits<std::uint8_t>{0b101u} ^ bits(0b001u)));
  }
}