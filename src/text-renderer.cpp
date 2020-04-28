#include "text-renderer.hpp"
#include "freetype.hpp"

const static std::string LOG_TAG = "TextRenderer";

TextRenderer::TextRenderer(const std::shared_ptr<Renderer> renderer,
                           const std::shared_ptr<Shader>   text_shader,
                           unsigned                        width,
                           unsigned                        height)
    : renderer(renderer),
      text_shader(text_shader)
{

  this->text_shader->bind();
  this->text_shader->set_uniform("projection_matrix",
                                 glm::ortho(0.0f,
                                            static_cast<float>(width),
                                            static_cast<float>(height),
                                            0.0f));
  this->text_shader->set_uniform("text", 0);

  // Configure vertex array for texture quads
  vertex_buffer = std::make_unique<VertexBuffer>(sizeof(float) * 6 * 4);
  VertexBufferLayout layout;
  layout.pushFloat(2);
  layout.pushFloat(2);

  vertex_array.add_buffer(*vertex_buffer, layout);
  vertex_array.unbind();
}

void TextRenderer::load(const std::string font, unsigned int fontSize)
{
  // First clear the previously loaded Characters
  characters.clear();

  // Then initialize and load the FreeType library
  FT_Library ft;
  if (FT_Init_FreeType(&ft))
  {
    throw std::runtime_error("Could not init FreeType Library");
  }

  // Load font as face
  FT_Face face;
  if (FT_New_Face(ft, ("resources/" + font).c_str(), 0, &face))
  {
    throw std::runtime_error("Failed to load font");
  }

  // Set size to load glyphs as
  FT_Set_Pixel_Sizes(face, 0, fontSize);

  // Disable byte-alignment restriction
  // TODO: Move this to renderer
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

  // Then for the first 128 ASCII characters, pre-load/compile their
  // characters and store them
  for (GLubyte c = 0; c < 128; ++c)
  {
    // Load character glyph
    if (FT_Load_Char(face, c, FT_LOAD_RENDER))
    {
      Log().w(LOG_TAG) << "Failed to load Glyph";
      continue;
    }

    // Generate texture
    auto texture = std::make_shared<Texture2D>(face->glyph->bitmap.width,
                                               face->glyph->bitmap.rows,
                                               face->glyph->bitmap.buffer,
                                               GL_RED,
                                               GL_RED,
                                               GL_CLAMP_TO_EDGE,
                                               GL_CLAMP_TO_EDGE);

    // Now store character for later use
    auto character = std::make_shared<Character>(
        texture,
        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
        face->glyph->advance.x);

    characters[c] = character;
  }

  // Destroy FreeType once we're finished
  FT_Done_Face(face);
  FT_Done_FreeType(ft);
}

void TextRenderer::render_text(const std::string &text,
                               float              x,
                               float              y,
                               float              scale,
                               glm::vec3          color)
{
  // Activate corresponding render state
  text_shader->bind();

  text_shader->set_uniform("text_color", color);

  // glBindVertexArray(this->VAO);

  // Iterate through all characters
  std::string::const_iterator c;
  for (c = text.begin(); c != text.end(); c++)
  {
    const auto &ch = characters[*c];

    float xpos = x + ch->bearing.x * scale;
    float ypos = y + (characters['H']->bearing.y - ch->bearing.y) * scale;

    float w = ch->size.x * scale;
    float h = ch->size.y * scale;

    // Update vertex buffer for each character
    Vertex2D vertex1 = {glm::vec2(xpos, ypos + h), glm::vec2(0.0f, 1.0f)};
    Vertex2D vertex2 = {glm::vec2(xpos + w, ypos), glm::vec2(1.0f, 0.0f)};
    Vertex2D vertex3 = {glm::vec2(xpos, ypos), glm::vec2(0.0f, 0.0f)};
    Vertex2D vertex4 = {glm::vec2(xpos, ypos + h), glm::vec2(0.0f, 1.0f)};
    Vertex2D vertex5 = {glm::vec2(xpos + w, ypos + h), glm::vec2(1.0f, 1.0f)};
    Vertex2D vertex6 = {glm::vec2(xpos + w, ypos), glm::vec2(1.0f, 0.0f)};

    // Vertex2D vertex1 = {glm::vec2(0, 1), glm::vec2(0.0f, 1.0f)};
    // Vertex2D vertex2 = {glm::vec2(1, 1), glm::vec2(1.0f, 0.0f)};
    // Vertex2D vertex3 = {glm::vec2(1, 1), glm::vec2(0.0f, 0.0f)};
    // Vertex2D vertex4 = {glm::vec2(1, 0), glm::vec2(0.0f, 1.0f)};
    // Vertex2D vertex5 = {glm::vec2(0, 0), glm::vec2(1.0f, 1.0f)};
    // Vertex2D vertex6 = {glm::vec2(0, 1), glm::vec2(1.0f, 0.0f)};

    std::vector<Vertex2D> vertices{vertex1,
                                   vertex2,
                                   vertex3,
                                   vertex4,
                                   vertex5,
                                   vertex6};

    // Render glyph texture over quad
    ch->texture->bind(0);

    // Update content of vertex buffer memory
    vertex_buffer->set_sub_data(0, sizeof(vertices), vertices);

    // Render quad
    renderer->draw(vertex_array);

    // Now advance cursors for next glyph
    // bitshift by 6 to get value in pixels (1/64th times 2^6 = 64)
    x += (ch->advance >> 6) * scale;

    ch->texture->unbind();
  }
}
