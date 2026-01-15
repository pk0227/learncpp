/**
 * @file adapter.cpp
 * @brief Adapter Pattern (Object Adapter)
 *
 * SCENARIO:
 * We have a modern "MediaPlayer" interface that plays MP3s.
 * We want to play "VLC" and "MP4" formats, but we only have a
 * legacy "AdvancedMediaPlayer" class with a weird interface.
 */

#include <iostream>
#include <memory>
#include <string>

// ===================================
// 1. The Target Interface (Modern)
// ===================================
class MediaPlayer {
public:
  virtual ~MediaPlayer() = default;
  virtual void play(const std::string &audioType,
                    const std::string &fileName) = 0;
};

// ===================================
// 2. The Adaptee (Legacy/Incompatible)
// ===================================
class AdvancedMediaPlayer {
public:
  void playVlc(const std::string &fileName) {
    std::cout << "[Legacy Lib] Playing vlc file: " << fileName << "\n";
  }

  void playMp4(const std::string &fileName) {
    std::cout << "[Legacy Lib] Playing mp4 file: " << fileName << "\n";
  }
};

// ===================================
// 3. The Adapter
// ===================================
class MediaAdapter : public MediaPlayer {
  std::unique_ptr<AdvancedMediaPlayer> advancedMusicPlayer_;

public:
  MediaAdapter() {
    advancedMusicPlayer_ = std::make_unique<AdvancedMediaPlayer>();
  }

  void play(const std::string &audioType,
            const std::string &fileName) override {
    // Translation Logic
    if (audioType == "vlc") {
      advancedMusicPlayer_->playVlc(fileName);
    } else if (audioType == "mp4") {
      advancedMusicPlayer_->playMp4(fileName);
    } else {
      std::cout << "Error: Format not supported by Adapter\n";
    }
  }
};

// ===================================
// 4. The Client (AudioPlayer)
// ===================================
class AudioPlayer : public MediaPlayer {
  std::unique_ptr<MediaAdapter> mediaAdapter_;

public:
  void play(const std::string &audioType,
            const std::string &fileName) override {
    // Built-in support
    if (audioType == "mp3") {
      std::cout << "Playing mp3 file: " << fileName << "\n";
    }
    // Adapter support
    else if (audioType == "vlc" || audioType == "mp4") {
      mediaAdapter_ = std::make_unique<MediaAdapter>();
      mediaAdapter_->play(audioType, fileName);
    } else {
      std::cout << "Invalid media. " << audioType << " format not supported\n";
    }
  }
};

int main() {
  std::cout << "--- Adapter Pattern Demo ---\n";
  AudioPlayer audioPlayer;

  audioPlayer.play("mp3", "beyond_the_horizon.mp3");
  audioPlayer.play("mp4", "alone.mp4");
  audioPlayer.play("vlc", "far_far_away.vlc");
  audioPlayer.play("avi", "mind_me.avi");

  return 0;
}
