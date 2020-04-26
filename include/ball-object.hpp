#pragma once

#include "game-object.hpp"

class BallObject : public GameObject
{
public:
  BallObject();

  BallObject(const glm::vec2 &                pos,
             float                            radius,
             const glm::vec2 &                velocity,
             const std::shared_ptr<Texture2D> sprite);

  glm::vec2 move(float delta_time, unsigned window_width);

  void reset(glm::vec2 position, glm::vec2 velocity);

  void set_stuck(bool stuck) { this->stuck = stuck; }

  bool is_stuck() const { return stuck; }

  float get_radius() const { return radius; }

  void set_velocity(const glm::vec2 &velocity) { this->velocity = velocity; }

  glm::vec2 get_velocity() const { return velocity; }

private:
  float     radius;
  bool      stuck;
  glm::vec2 velocity;
};
