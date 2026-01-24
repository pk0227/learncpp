/*

Interview Question :

“Given an integer array, rearrange it so that all zero values appear
contiguously in the middle of the array, while preserving the relative order of
all non-zero elements on both sides. The operation should be performed in O(n)
time.”

*/

#include <algorithm>
#include <array>
#include <iostream>

void reverse(int *begin, int *last) {
  last--;

  for (; begin < last; begin++, last--)
    std::swap(*begin, *last);
}

void rotate(int *begin, int *middle, int *last) {
  reverse(begin, middle);
  reverse(middle, last);
  reverse(begin, last);
}

template <std::size_t N> void zeros_to_middle(std::array<int, N> &arr) {
  /*
   * Algorithm: Compact + Rotate (O(N) Time, O(1) Space)
   *
   * Goal: Move all zeros to the middle while preserving the relative order
   * of non-zero elements.
   *
   * Strategy:
   * 1. Compact: Move all non-zero elements to the front using a persistent
   *    write index. This is a "stable partition" operation. Because we move
   *    non-zeros to the front, all zeros naturally accumulate at the end.
   *    Cost: O(N) linear scan.
   *
   * 2. Rotate: Now the array looks like:
   *    [ NZ_1, NZ_2, ... NZ_k, 0, 0, ... 0 ]
   *    We want the block of zeros to be in the middle (roughly N/2).
   *    To achieve this, we take the second half of the non-zero elements
   *    and rotate them to the end of the array (after the zeros).
   *    Alternatively, we can think of it as rotating the zero-block
   *    leftwards into its correct center position.
   *    Cost: O(N) rotation.
   */

  // Step 1: Stable Partition (Move all non-zeros to front)
  auto size{std::ssize(arr)};
  auto write_idx{0};

  for (auto i{0}; i < size; ++i) {
    if (arr[i] != 0) {
      if (i != write_idx) {
        std::swap(arr[i], arr[write_idx]);
      }
      write_idx++;
    }
  }

  // State after Step 1:
  // [ Non-Zeros (0 to write_idx-1) | Zeros (write_idx to end) ]
  auto num_nz = write_idx;

  // Step 2: Calculate Rotation Point
  // We want the zeros to start after the first half of the non-zeros.
  // Example: If 4 non-zeros, we keep 2 on left.
  // Example: If 5 non-zeros, we keep 2 on left (integer division).
  auto nz_on_left = num_nz / 2;

  // Step 3: Rotate Zeros into position
  // We rotate the range starting from `nz_on_left` to the end.
  // The pivots ensure the block of zeros (currently at `num_nz`) moves
  // to `nz_on_left`.
  //
  // std::rotate parameters:
  // - first:  Start of the range to be modified (nz_on_left)
  // - middle: The element that should move to 'first' (start of zeros ->
  // num_nz)
  // - last:   End of the range (arr.end())
  std::rotate(arr.begin() + nz_on_left, arr.begin() + num_nz, arr.end());
  // rotate(arr.begin()+nz_on_left, arr.begin()+num_nz, arr.end());
}

template <std::size_t N> void print_array(const std::array<int, N> &arr) {
  for (const auto &el : arr)
    std::cout << el << " ";
}

template <std::size_t N> void test_case(std::array<int, N> &&arr) {
  print_array(arr);
  std::cout << " <-- Before\n";

  zeros_to_middle(arr);

  print_array(arr);
  std::cout << " <-- After\n";

  std::cout << "=============================================\n";
}

int main() {
  // Minimal Boundary / Corner Cases
  // test_case(std::array{});                 // empty array
  test_case(std::array{0}); // single zero
  test_case(std::array{5}); // single non-zero

  // All Zeros / No Zeros
  test_case(std::array{0, 0, 0});
  test_case(std::array{0, 0, 0, 0});

  test_case(std::array{1, 2, 3});
  test_case(std::array{4, 5, 6, 7});

  // Small Mixed Edge Cases (VERY IMPORTANT)
  test_case(std::array{1, 0});
  test_case(std::array{0, 1});

  test_case(std::array{1, 0, 2});
  test_case(std::array{0, 1, 0});
  test_case(std::array{0, 0, 1});
  test_case(std::array{1, 0, 0});

  // Zeros Already in the Middle (Should Be No-Op)
  test_case(std::array{1, 0, 2});
  test_case(std::array{1, 2, 0, 0, 3});
  test_case(std::array{1, 2, 0, 0, 3, 4});

  // Zeros All at One Side (COMMON FAILURES)
  test_case(std::array{0, 0, 1, 2, 3});
  test_case(std::array{1, 0, 2, 3, 4, 5, 0, 0}); // Critical stability case
  test_case(std::array{0, 0, 0, 1, 2, 3});

  test_case(std::array{1, 2, 3, 0, 0});
  test_case(std::array{1, 2, 3, 4, 0, 0, 0});

  // Alternating / Interleaved Zeros (KILLER CASES)
  test_case(std::array{1, 0, 2, 0, 3});
  test_case(std::array{1, 0, 2, 0, 3, 0, 4});
  test_case(std::array{0, 1, 0, 2, 0, 3});
  test_case(std::array{0, 1, 0, 2, 3, 0, 4});

  // Odd vs Even Length Arrays
  test_case(std::array{1, 0, 2, 3, 0});    // odd length
  test_case(std::array{1, 0, 2, 0, 3, 4}); // even length

  // Stability Tests (ORDER MUST BE PRESERVED)
  test_case(std::array{3, 1, 0, 2, 0});    // left order must stay (3,1)
  test_case(std::array{1, 0, 3, 4, 0, 2}); // right order must stay (4,2)

  // Negative Numbers / Mixed Values
  test_case(std::array{-1, 0, -2, 0, 3});
  test_case(std::array{0, -1, 0, -2, 3, 4});

  // Original Interview Example (REFERENCE)
  test_case(std::array{1, 0, 2, 0, 3, 4, 0, 5});

  // test_case(std::array{});
  test_case(std::array{0});
  test_case(std::array{1, 2, 3});
  test_case(std::array{0, 1, 0});
  test_case(std::array{1, 0, 2, 0, 3, 4, 0, 5});
  test_case(std::array{0, 0, 1, 2, 3});

  return 0;
}