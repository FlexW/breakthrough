#include "ball-object.hpp"

BallObject::BallObject() : GameObject(), radius(12.5f), stuck(true) {}

BallObject::BallObject(const glm::vec2 &                pos,
                       float                            radius,
                       const glm::vec2 &                velocity,
                       const std::shared_ptr<Texture2D> sprite)
    : GameObject(pos,
                 glm::vec2(radius * 2.0f, radius * 2.0f),
                 sprite,
                 false,
                 glm::vec3(1.0f),
                 velocity),
      radius(radius),
      stuck(true)
{
}

glm::vec2 BallObject::move(float delta_time, unsigned window_width)
{
  if (!stuck)
  {
    // Move
    position += velocity * delta_time;

    // Check if outside window bounds and if so, reverse velocity and
    // restore at correct position
    if (position.x <= 0.0f)
    {
      velocity.x = -velocity.x;
      position.x = 0.0f;
    }
    else if (position.x + size.x >= window_width)
    {
      velocity.x = -velocity.x;
      position.x = window_width - size.x;
    }
    if (position.y <= 0.0f)
    {
      velocity.y = -velocity.y;
      position.y = 0.0f;
    }
  }
  return position;
}

void BallObject::reset(glm::vec2 position, glm::vec2 velocity)
{
  this->position = position;
  this->velocity = velocity;
  stuck          = true;
}
