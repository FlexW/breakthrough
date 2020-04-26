#include <glm/gtx/string_cast.hpp>

#include "particle-generator.hpp"

ParticleGenerator::ParticleGenerator(const std::shared_ptr<Shader>    shader,
                                     const std::shared_ptr<Texture2D> texture,
                                     unsigned                         amount,
                                     const std::shared_ptr<Renderer>  renderer)
    : amount(amount),
      renderer(renderer),
      shader(shader),
      texture(texture)
{
  init();
}

void ParticleGenerator::update(const float       delta_time,
                               const GameObject &object,
                               const unsigned    new_particles,
                               const glm::vec2 & offset)
{
  // Add new particles
  for (unsigned i = 0; i < new_particles; ++i)
  {
    const auto unused_particle = first_unused_particle();
    respawn_particle(particles[unused_particle], object, offset);
  }
  // update all particles
  for (unsigned int i = 0; i < amount; ++i)
  {
    auto &p = this->particles[i];
    p.life -= delta_time; // reduce life
    if (p.life > 0.0f)
    {
      // particle is alive, thus update
      p.position -= p.velocity * delta_time;
      p.color.a -= delta_time * 2.5f;
    }
  }
}

void ParticleGenerator::draw()
{
  // Use additive blending to give it a 'glow' effect
  renderer->blend_func(GL_SRC_ALPHA, GL_ONE);

  shader->bind();

  for (auto &particle : particles)
  {
    if (particle.life > 0.0f)
    {
      shader->setVec2("offset", particle.position);
      shader->setVec4("color", particle.color);
      texture->bind();

      vertex_array.bind();

      renderer->draw(vertex_array);

      vertex_array.unbind();
    }
  }
  // Reset to default blending mode
  renderer->blend_func(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
  VertexBuffer vertex_buffer({
      {glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
      {glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
      {glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec2(0.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
      {glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
      {glm::vec2(1.0f, 0.0f), glm::vec2(1.0f, 0.0f)},
  });

  VertexBufferLayout layout;
  layout.pushFloat(2);
  layout.pushFloat(2);

  vertex_array.add_buffer(std::move(vertex_buffer), layout);

  // for (unsigned i = 0; i < amount; ++i)
  // {
  //   particles.push_back(Particle());
  // }
  particles.resize(amount);
}

unsigned ParticleGenerator::first_unused_particle()
{
  // First search from last used particle, this will usually return
  // almost instantly
  for (unsigned i = last_used_particle; i < amount; ++i)
  {
    if (particles[i].life <= 0.0f)
    {
      last_used_particle = i;
      return i;
    }
  }
  // Otherwise, do a linear search
  for (unsigned i = 0; i < last_used_particle; ++i)
  {
    if (particles[i].life <= 0.0f)
    {
      last_used_particle = i;
      return i;
    }
  }
  // All particles are taken, override the first one (note that if it
  // repeatedly hits this case, more particles should be reserved)
  last_used_particle = 0;
  return 0;
}

void ParticleGenerator::respawn_particle(Particle &        particle,
                                         const GameObject &object,
                                         const glm::vec2 & offset)
{
  const auto random  = ((rand() % 100) - 50) / 10.0f;
  const auto r_color = 0.5f + ((rand() % 100) / 100.0f);

  particle.position = object.get_position() + random + offset;
  particle.color    = glm::vec4(r_color, r_color, r_color, 1.0f);
  particle.life     = 1.0f;
  particle.velocity = object.get_velocity() * 0.1f;
}
