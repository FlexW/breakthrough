#include <fstream>

#include "game-level.hpp"
#include "log.hpp"
#include "resource-manager.hpp"

static const std::string LOG_TAG = "GameLevel";

GameLevel::GameLevel(const std::string &file,
                     unsigned           level_width,
                     unsigned           level_height)
{
  load_from_file(file, level_width, level_height);
}

GameLevel::GameLevel(GameLevel &&game_level)
{
  bricks = std::move(game_level.bricks);
}

void GameLevel::operator=(GameLevel &&game_level)
{
  bricks = std::move(game_level.bricks);
}

void GameLevel::load_from_file(const std::string &file,
                               unsigned           level_width,
                               unsigned           level_height)
{
  const auto real_file_path = "resources/" + file;

  Log().i(LOG_TAG) << "Load level from file: " << real_file_path;

  unsigned                           tile_code;
  std::string                        line;
  std::ifstream                      fstream(real_file_path);
  std::vector<std::vector<unsigned>> tile_data;

  if (fstream)
  {
    while (std::getline(fstream, line))
    {
      std::istringstream    sstream(line);
      std::vector<unsigned> row;

      // Read each word seperated by spaces
      while (sstream >> tile_code)
        row.push_back(tile_code);

      tile_data.push_back(row);
    }

    if (tile_data.size() > 0)
    {
      this->tile_data    = tile_data;
      this->level_width  = level_width;
      this->level_height = level_height;

      init(tile_data, level_width, level_height);
    }
  }
}

void GameLevel::draw(std::shared_ptr<SpriteRenderer> renderer) const
{
  for (const auto &tile : bricks)
    if (!tile.is_destroyed())
      tile.draw(renderer);
}

bool GameLevel::is_completed()
{
  for (const auto &tile : bricks)
    if (!tile.is_solid() && !tile.is_destroyed())
      return false;
  return true;
}

void GameLevel::init(std::vector<std::vector<unsigned int>> tile_data,
                     unsigned                               level_width,
                     unsigned                               level_height)
{
  // Calculate dimensions
  // Note we can index vector at [0] since this function is only called if
  // height > 0
  const auto height = tile_data.size();
  const auto width  = tile_data[0].size();

  const auto unit_width  = level_width / static_cast<float>(width);
  const auto unit_height = level_height / static_cast<float>(height);

  // Initialize level tiles based on tile_data
  for (unsigned y = 0; y < height; ++y)
  {
    for (unsigned x = 0; x < width; ++x)
    {
      // Check block type from level data (2D level array)
      if (tile_data[y][x] == 1) // Solid
      {
        glm::vec2 pos(unit_width * x, unit_height * y);
        glm::vec2 size(unit_width, unit_height);

        GameObject obj(pos,
                       size,
                       ResourceManager::get_texture("block_solid"),
                       true,
                       glm::vec3(0.8f, 0.8f, 0.7f));

        bricks.push_back(obj);
      }
      else if (tile_data[y][x] > 1) // Non-solid
      {
        // Now determine its color based on level data
        glm::vec3 color = glm::vec3(1.0f); // Original: white
        if (tile_data[y][x] == 2)
          color = glm::vec3(0.2f, 0.6f, 1.0f);
        else if (tile_data[y][x] == 3)
          color = glm::vec3(0.0f, 0.7f, 0.0f);
        else if (tile_data[y][x] == 4)
          color = glm::vec3(0.8f, 0.8f, 0.4f);
        else if (tile_data[y][x] == 5)
          color = glm::vec3(1.0f, 0.5f, 0.0f);

        glm::vec2 pos(unit_width * x, unit_height * y);
        glm::vec2 size(unit_width, unit_height);

        bricks.push_back(GameObject(pos,
                                    size,
                                    ResourceManager::get_texture("block"),
                                    false,
                                    color));
      }
    }
  }
}

void GameLevel::reset()
{
  for (auto &brick : bricks)
  {
    brick.set_destroyed(false);
  }

  if (tile_data.size() > 0)
    init(tile_data, level_width, level_height);
}
