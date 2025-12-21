#include <iostream>
/* RACE CONDITION DEMO - Unsynchronized shared access
 * Multiple threads increment shared counter without protection
 * Results in lost updates - final count less than expected
 * Classic bug: demonstrates WHY we need synchronization
 * Interview: Identify race, explain fix (mutex or atomic)
 */

#include <thread>
#include <vector>

int COUNTER = 1;

void foo(int multiplier) {

  if (COUNTER < 10) {
    COUNTER *= multiplier;
    std::cout << "The COUNTER get's multiplied by: " << multiplier << std::endl;
  }
}

auto main() -> int {

  std::vector<std::thread> threads;

  threads.reserve(10);
  for (int i = 0; i < 10; i++) {
    threads.push_back(std::thread(foo, i + 1));
  }

  for (auto &t : threads) {
    t.join();
  }

  std::cout << "The final value of COUNTER is: " << COUNTER << std::endl;

  return 0;
}
