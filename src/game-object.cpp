#include "game-object.hpp"

GameObject::GameObject()
    : position(0.0f, 0.0f),
      size(1.0f, 1.0f),
      velocity(0.0f),
      color(1.0f),
      rotation(0.0f),
      solid(false),
      destroyed(false),
      sprite(nullptr)
{
}

GameObject::GameObject(glm::vec2                  pos,
                       glm::vec2                  size,
                       std::shared_ptr<Texture2D> sprite,
                       bool                       solid,
                       glm::vec3                  color,
                       glm::vec2                  velocity)
    : position(pos),
      size(size),
      velocity(velocity),
      color(color),
      rotation(0.0f),
      solid(solid),
      destroyed(false),
      sprite(sprite)
{
}

void GameObject::draw(std::shared_ptr<SpriteRenderer> renderer) const
{
  renderer->draw_sprite(sprite, position, size, rotation, color);
}
