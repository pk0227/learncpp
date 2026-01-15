/**
 * @file state.cpp
 * @brief State Pattern
 *
 * SCENARIO:
 * Audio Player States: Stopped, Playing, Paused.
 * Action: "Press Play/Pause Button".
 *
 * Logic:
 * - If Stopped -> Playing.
 * - If Playing -> Paused.
 * - If Paused -> Playing.
 */

#include <iostream>
#include <memory>
#include <string>

class AudioPlayerContext; // Forward declaration

// ===================================
// 1. State Interface
// ===================================
class State {
public:
  virtual ~State() = default;
  virtual void clickPlay(AudioPlayerContext *player) = 0;
};

// ===================================
// 2. Context
// ===================================
class AudioPlayerContext {
  std::unique_ptr<State> state_;

public:
  AudioPlayerContext(); // Defined below

  void changeState(std::unique_ptr<State> newState) {
    state_ = std::move(newState);
  }

  void clickPlay() {
    state_->clickPlay(this); // Delegate to current state
  }
};

// ===================================
// 3. Concrete States
// ===================================

class PlayingState;
class PausedState;
class StoppedState;

// Implementation of StoppedState
class StoppedState : public State {
public:
  void clickPlay(AudioPlayerContext *player) override; // Defined below
};

class PlayingState : public State {
public:
  void clickPlay(AudioPlayerContext *player) override;
};

class PausedState : public State {
public:
  void clickPlay(AudioPlayerContext *player) override;
};

// ===================================
// Wiring Transitions
// ===================================

void StoppedState::clickPlay(AudioPlayerContext *player) {
  std::cout << "   [Stopped -> Playing] Starting music.\n";
  // Using simple new for demo, in production prefer factories or making states
  // shared/static if stateless
  player->changeState(std::make_unique<PlayingState>());
}

void PlayingState::clickPlay(AudioPlayerContext *player) {
  std::cout << "   [Playing -> Paused] Pausing music.\n";
  player->changeState(std::make_unique<PausedState>());
}

void PausedState::clickPlay(AudioPlayerContext *player) {
  std::cout << "   [Paused -> Playing] Resuming music.\n";
  player->changeState(std::make_unique<PlayingState>());
}

// Context Constructor starts in Stopped
AudioPlayerContext::AudioPlayerContext()
    : state_(std::make_unique<StoppedState>()) {}

// ===================================
// DEMONSTRATION
// ===================================
int main() {
  std::cout << "--- State Pattern Demo ---\n";

  AudioPlayerContext player;

  // Simulate user clicks
  std::cout << "User clicks Play:\n";
  player.clickPlay(); // Stopped -> Playing

  std::cout << "User clicks Play:\n";
  player.clickPlay(); // Playing -> Paused

  std::cout << "User clicks Play:\n";
  player.clickPlay(); // Paused -> Playing

  return 0;
}
