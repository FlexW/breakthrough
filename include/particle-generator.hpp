#pragma once

#include <memory>

#include "game-object.hpp"
#include "shader.hpp"
#include "texture.hpp"

/**
 * @brief Represents a single particle and it's state.
 */
struct Particle
{
  glm::vec2 position = glm::vec2(0.0f);
  glm::vec2 velocity = glm::vec2(0.0f);
  glm::vec4 color    = glm::vec4(1.0f);
  float     life     = 0.0f;
};

/**
 * @brief Generates Particle
 *
 * ParticleGenerator acts as a container for rendering a large number
 * of particles by repeatedly spawning and updating particles and
 * killing them after a given amount of time.
 */
class ParticleGenerator
{
public:
  ParticleGenerator(const std::shared_ptr<Shader>    shader,
                    const std::shared_ptr<Texture2D> texture,
                    unsigned                         amount,
                    const std::shared_ptr<Renderer>  renderer);

  /**
   * @brief Update all particles
   *
   * @param delta_time Delta time
   */
  void update(const float       delta_time,
              const GameObject &object,
              const unsigned    new_particles,
              const glm::vec2 & offset = glm::vec2(0.0f, 0.0f));

  /**
   * @brief Render all particles
   */
  void draw();

private:
  // State

  std::vector<Particle> particles;
  const unsigned        amount;

  // Render state

  const std::shared_ptr<Renderer>  renderer;
  const std::shared_ptr<Shader>    shader;
  const std::shared_ptr<Texture2D> texture;
  VertexArray                      vertex_array;
  unsigned                         last_used_particle = 0;

  void init();

  /**
   * @brief Returns first unused particle index.
   *
   * @return Index of first unused particle or 0 if no unused particle
   */
  unsigned first_unused_particle();

  void respawn_particle(Particle &        particle,
                        const GameObject &object,
                        const glm::vec2 & offset = glm::vec2(0.0f, 0.0f));
};
