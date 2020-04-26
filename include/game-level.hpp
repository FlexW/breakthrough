#pragma once

#include "game-object.hpp"

/**
 * @brief A level of the game.
 *
 * GameLevel holds all tiles as part of a Breakout level and
 * hosts functionality to load/render levels from the harddisk.
 */
class GameLevel
{
public:
  GameLevel(const std::string &file,
            unsigned           level_width,
            unsigned           level_height);

  GameLevel(GameLevel &&game_level);

  void operator=(GameLevel &&game_level);

  void draw(std::shared_ptr<SpriteRenderer> renderer) const;

  /**
   * @brief Check if the level is completed.
   *
   * A level is completed if all non soild bricks are destroyed.
   *
   * @return true on completed, false otherwise
   */
  bool is_completed();

private:
  std::vector<GameObject> bricks;

  void load_from_file(const std::string &file,
                      unsigned           level_width,
                      unsigned           level_height);

  void init(std::vector<std::vector<unsigned int>> tile_data,
            unsigned                               level_width,
            unsigned                               level_height);
};
