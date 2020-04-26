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

  std::vector<GameObject> &get_bricks() { return bricks; }

  void reset();

private:
  std::vector<GameObject> bricks;

  std::vector<std::vector<unsigned>> tile_data;

  unsigned level_width  = 0;
  unsigned level_height = 0;

  void load_from_file(const std::string &file,
                      unsigned           level_width,
                      unsigned           level_height);

  void init(std::vector<std::vector<unsigned int>> tile_data,
            unsigned                               level_width,
            unsigned                               level_height);
};
