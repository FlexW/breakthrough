#pragma once

#include <glm/glm.hpp>

#include "sprite-renderer.hpp"
#include "texture.hpp"

/**
 * @brief Container object for game entity.
 *
 * Container object for holding all state relevant for a single
 * game object entity. Each object in the game likely needs the
 * minimal of state as described within GameObject.
 */
class GameObject
{
public:
  GameObject();

  GameObject(glm::vec2                  pos,
             glm::vec2                  size,
             std::shared_ptr<Texture2D> sprite,
             bool                       solid    = false,
             glm::vec3                  color    = glm::vec3(1.0f),
             glm::vec2                  velocity = glm::vec2(0.0f, 0.0f));

  virtual void draw(std::shared_ptr<SpriteRenderer> renderer) const;

  bool is_destroyed() const { return destroyed; }

  bool is_solid() const { return solid; }

  glm::vec2 get_position() { return position; }

  void set_position(const glm::vec2 &position) { this->position = position; }

  glm::vec2 get_size() { return size; }

protected:
  glm::vec2 position, size, velocity;

private:
  glm::vec3 color;

  float rotation;

  bool solid;

  bool destroyed;

  std::shared_ptr<Texture2D> sprite;
};
