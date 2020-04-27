#include "post-processor.hpp"

PostProcessor::PostProcessor(const std::shared_ptr<Renderer> renderer,
                             const std::shared_ptr<Shader>   shader,
                             unsigned                        width,
                             unsigned                        height)
    : renderer(renderer),
      post_processing_shader(shader),
      width(width),
      height(height)
{
  auto renderbuffer = std::make_unique<Renderbuffer>(width, height);
  ms_framebuffer    = std::make_unique<Framebuffer>(std::move(renderbuffer));

  texture     = std::make_shared<Texture2D>(width, height, nullptr);
  framebuffer = std::make_unique<Framebuffer>(texture);

  init_render_data();

  post_processing_shader->bind();
  post_processing_shader->set_uniform("scene", 0);

  const auto offset = 1.0f / 300.0f;

  std::array<glm::vec2, 9> offsets = {
      glm::vec2(-offset, offset),  // top-left
      glm::vec2(0.0f, offset),     // top-center
      glm::vec2(offset, offset),   // top-right
      glm::vec2(-offset, 0.0f),    // center-left
      glm::vec2(0.0f, 0.0f),       // center-center
      glm::vec2(offset, 0.0f),     // center - right
      glm::vec2(-offset, -offset), // bottom-left
      glm::vec2(0.0f, -offset),    // bottom-center
      glm::vec2(offset, -offset)   // bottom-right
  };
  post_processing_shader->set_uniform("offsets", offsets);

  std::array<int, 9> edge_kernel = {-1, -1, -1, -1, 8, -1, -1, -1, -1};
  post_processing_shader->set_uniform("edge_kernel", edge_kernel);

  std::array<float, 9> blur_kernel = {1.0f / 16.0f,
                                      2.0f / 16.0f,
                                      1.0f / 16.0f,
                                      2.0f / 16.0f,
                                      4.0f / 16.0f,
                                      2.0f / 16.0f,
                                      1.0f / 16.0f,
                                      2.0f / 16.0f,
                                      1.0f / 16.0f};
  post_processing_shader->set_uniform("blur_kernel", blur_kernel);
}

void PostProcessor::begin_render()
{
  ms_framebuffer->bind();
  renderer->clear_color(0.0f, 0.0f, 0.0f, 1.0f);
  renderer->clear(GL_COLOR_BUFFER_BIT);
}

void PostProcessor::end_render()
{
  // Now resolve multisampled color-buffer into intermediate FBO to
  // store to texture
  ms_framebuffer->bind(Framebuffer::Target::READ_FRAMEBUFFER);
  framebuffer->bind(Framebuffer::Target::DRAW_FRAMEBUFFER);
  renderer->blit_framebuffer(0,
                             0,
                             width,
                             height,
                             0,
                             0,
                             width,
                             height,
                             GL_COLOR_BUFFER_BIT,
                             GL_NEAREST);

  ms_framebuffer->unbind(); // Unbinds both
}

void PostProcessor::render(float time)
{
  post_processing_shader->bind();
  post_processing_shader->set_uniform("time", time);
  post_processing_shader->set_uniform("confuse", confuse);
  post_processing_shader->set_uniform("chaos", chaos);
  post_processing_shader->set_uniform("shake", shake);

  texture->bind(0);

  renderer->draw(vertex_array);
}

void PostProcessor::init_render_data()
{
  VertexBuffer vertex_buffer({
      {glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
      {glm::vec2(-1.0f, 1.0f), glm::vec2(0.0f, 1.0f)},
      {glm::vec2(-1.0f, -1.0f), glm::vec2(0.0f, 0.0f)},
      {glm::vec2(1.0f, -1.0f), glm::vec2(1.0f, 0.0f)},
      {glm::vec2(1.0f, 1.0f), glm::vec2(1.0f, 1.0f)},
  });

  VertexBufferLayout layout;
  layout.pushFloat(2);
  layout.pushFloat(2);

  vertex_array.add_buffer(std::move(vertex_buffer), layout);
}
