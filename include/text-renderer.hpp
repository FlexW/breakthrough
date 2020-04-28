#pragma once

#include <unordered_map>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "renderer.hpp"
#include "shader.hpp"
#include "texture.hpp"
#include "vertex-array.hpp"

/**
 * Holds all state information relevant to a character as loaded using
 * FreeType
 */
struct Character
{
  /** Handle of the glyph texture */
  std::shared_ptr<Texture2D> texture;

  /** Size of glyph */
  glm::ivec2 size;

  /** Offset from baseline to left/top of glyph */
  glm::ivec2 bearing;

  /** Horizontal offset to advance to next glyph */
  long advance;

  Character(std::shared_ptr<Texture2D> texture,
            glm::ivec2                 size,
            glm::ivec2                 bearing,
            long                       advance)
      : texture(texture),
        size(size),
        bearing(bearing),
        advance(advance)
  {
  }

  // Character(Character &&c)
  // {
  //   texture = c.texture;
  //   size    = c.size;
  //   bearing = c.bearing;
  //   advance = c.advance;
  // }

  // void operator=(Character &&c)
  // {
  //   texture = c.texture;
  //   size    = c.size;
  //   bearing = c.bearing;
  //   advance = c.advance;
  // }
};

/**
 * A renderer class for rendering text displayed by a font loaded
 * using the FreeType library. A single font is loaded, processed into
 * a list of Character items for later rendering.
 */
class TextRenderer
{
public:
  TextRenderer(const std::shared_ptr<Renderer> renderer,
               std::shared_ptr<Shader>         text_shader,
               unsigned                        width,
               unsigned                        height);

  /**
   * @brief Pre-compiles a list of characters from the given font.
   */
  void load(const std::string font, unsigned font_size);

  /**
   * @brief Renders a string of text using the precompiled list of characters.
   */
  void render_text(const std::string &text,
                   float              x,
                   float              y,
                   float              scale,
                   const glm::vec3    color = glm::vec3(1.0f));

private:
  VertexArray vertex_array;

  std::unique_ptr<VertexBuffer>   vertex_buffer = nullptr;
  const std::shared_ptr<Renderer> renderer      = nullptr;
  std::shared_ptr<Shader>         text_shader   = nullptr;

  // Holds a list of pre-compiled characters
  std::unordered_map<char, std::shared_ptr<Character>> characters;
};
