#pragma once
#include <game.h>
#include <string>
#include <thread>

struct SaveGameJob {
  const std::string filename;
  const Game game;

  void operator()() const { saveGame(); }

private:
  void saveGame() const;
};

struct LoadGameJob {
  const std::string filename;
  Game *const game;

  void operator()() const { return loadGame(); }

private:
  void loadGame() const;
};

class GameSaver {
  // Singleton pattern: https://stackoverflow.com/questions/1008019/how-do-you-implement-the-singleton-design-pattern
public:
  GameSaver();
  ~GameSaver();
  GameSaver(const GameSaver &s) = delete;
  GameSaver &operator=(const GameSaver &s) = delete;

  static GameSaver &getInstance() {
    static GameSaver instance;
    return instance;
  }

  static bool hasSaveGame(const std::string &filename);
  void saveGame(const std::string &filename, const Game &game);
  void loadGame(const std::string &filename, Game *game);

private:
  std::thread *save_thread;
  std::thread *load_thread;
};
