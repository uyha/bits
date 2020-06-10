//
// Created by uy on 10.06.20.
//

#pragma once
#include <cstdint>
#include <type_traits>

namespace river::bits {
template <typename T>
concept integral = std::is_integral_v<T>;
template <typename T>
concept unsigned_integral = integral<T> &&std::is_unsigned_v<T>;
namespace details {
template <unsigned_integral T, std::size_t byte>
constexpr auto expand() noexcept {
  static_assert(sizeof(T) + byte <= sizeof(std::uint64_t));

  constexpr auto target_size = sizeof(T) + byte;
  if constexpr (target_size <= 1) {
    return std::uint8_t{};
  } else if constexpr (target_size <= 2) {
    return std::uint16_t{};
  } else if constexpr (target_size <= 4) {
    return std::uint32_t{};
  } else {
    return std::uint64_t{};
  }
}

template <unsigned_integral T, std::size_t byte>
struct expanded {
  using type = decltype(details::expand<T, byte>());
};
template <unsigned_integral T, std::size_t byte>
using expanded_t = typename expanded<T, byte>::type;

template <unsigned_integral T1, unsigned_integral T2>
using larger = std::conditional<sizeof(T1) < sizeof(T2), T2, T1>;
template <unsigned_integral T1, unsigned_integral T2>
using larger_t = typename larger<T1, T2>::type;
} // namespace details

template <unsigned_integral T>
struct Bits {
  T value;

  constexpr Bits() noexcept = default;

  constexpr Bits(T source) noexcept
      : value{source} {}

  template <unsigned_integral T1>
  constexpr Bits(Bits<T1> source) noexcept
      : value{source.value} {}

  template <unsigned_integral T1>
  constexpr auto to() const noexcept -> Bits<T> {
    return value;
  }

  template <std::size_t bit>
  constexpr auto lsh() const noexcept {
    using result_type = details::expanded_t<T, (bit + 7) / 8>;
    return Bits<result_type>{result_type(result_type(value) << bit)};
  }
  template <std::size_t bit>
  constexpr auto rsh() const noexcept -> Bits<T> {
    return T{value >> bit};
  }
};

template <unsigned_integral T>
constexpr auto bits(T raw) noexcept -> Bits<T> {
  return raw;
}

template <unsigned_integral T, T target_mask>
struct MaskedBits : Bits<T> {
  static constexpr Bits<T> mask = target_mask;

  using Bits<T>::Bits;

  constexpr auto set(T source) const noexcept -> Bits<T> {
    auto const self = *this;

    return bits(source) & ~mask | self;
  }

  static constexpr auto masked_value(T source) noexcept -> Bits<T> {
    return bits(source) & mask;
  }
};

template <unsigned_integral T>
constexpr auto operator~(Bits<T> bits) noexcept -> Bits<T> {
  return ~bits.value;
}

template <unsigned_integral T>
constexpr auto operator<<(Bits<T> bits, std::size_t shift) noexcept -> Bits<T> {
  return T(bits.value << shift);
}
template <unsigned_integral T>
constexpr auto operator>>(Bits<T> bits, std::size_t shift) noexcept -> Bits<T> {
  return T(bits.value >> shift);
}

template <unsigned_integral T1, unsigned_integral T2>
constexpr auto operator&(Bits<T1> lhs, Bits<T2> rhs) noexcept -> Bits<details::larger_t<T1, T2>> {
  using result_type = details::larger_t<T1, T2>;
  return static_cast<result_type>(lhs.value & rhs.value);
}
template <unsigned_integral T1, unsigned_integral T2>
constexpr auto operator|(Bits<T1> lhs, Bits<T2> rhs) noexcept -> Bits<details::larger_t<T1, T2>> {
  using result_type = details::larger_t<T1, T2>;
  return static_cast<result_type>(lhs.value | rhs.value);
}
template <unsigned_integral T1, unsigned_integral T2>
constexpr auto operator^(Bits<T1> lhs, Bits<T2> rhs) noexcept -> Bits<details::larger_t<T1, T2>> {
  using result_type = details::larger_t<T1, T2>;
  return static_cast<result_type>(lhs.value ^ rhs.value);
}
template <unsigned_integral T1, unsigned_integral T2>
constexpr bool operator==(Bits<T1> lhs, Bits<T2> rhs) noexcept {
  return lhs.value == rhs.value;
}
template <unsigned_integral T1, unsigned_integral T2>
constexpr bool operator!=(Bits<T1> lhs, Bits<T2> rhs) noexcept {
  return !(lhs == rhs);
}

} // namespace river::bits
