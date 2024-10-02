#include "saving.h"

#include <fstream>
#include <game.h>
#include <iostream>
#include <thread>

void SaveGameJob::saveGame() const {
  std::ofstream file(filename);

  if (!file.is_open()) {
    std::cout << "Could not open file for writing" << std::endl;
  }

  file << game;
  file.close();

  std::cout << "Game saved to " << filename << " from thread ID: " << std::this_thread::get_id() << std::endl;
}

void LoadGameJob::loadGame() const {
  std::ifstream file(filename);

  if (!file.is_open()) {
    throw std::runtime_error("Could not open file for reading");
  }

  file >> *game;
  file.close();

  std::cout << "Game loaded from " << filename << " from thread ID: " << std::this_thread::get_id() << std::endl;
}

GameSaver::GameSaver() : save_thread(nullptr), load_thread(nullptr) {}

GameSaver::~GameSaver() {
  if (save_thread != nullptr) {
    if (save_thread->joinable()) {
      save_thread->join();
    }

    delete save_thread;
  }

  if (load_thread != nullptr) {
    if (load_thread->joinable()) {
      load_thread->join();
    }

    delete load_thread;
  }
}

bool GameSaver::hasSaveGame(const std::string &filename) {
  // checks if the file exists
  const std::ifstream file(filename);

  return file.good();
}

void GameSaver::saveGame(const std::string &filename, const Game &game) {
  // saves the game to a file, using the filename provided in a different thread
  if (save_thread != nullptr && save_thread->joinable()) {
    save_thread->join();
    delete save_thread;
  }

  const auto save_game_job = SaveGameJob{filename, game};
  save_thread = new std::thread(save_game_job);
}

void GameSaver::loadGame(const std::string &filename, Game *const game) {
  // loads the game from a file, using the filename provided in a different thread
  if (load_thread != nullptr && load_thread->joinable()) {
    load_thread->join();
    delete load_thread;
  }

  const auto load_game_job = LoadGameJob{filename, game};

  load_thread = new std::thread(load_game_job);
  load_thread->join();
}
