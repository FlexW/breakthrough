#pragma once

class Renderbuffer
{
public:
  Renderbuffer(unsigned width, unsigned height);

  ~Renderbuffer();

  void bind();

  void unbind();

  unsigned get_id() const { return id; }

private:
  unsigned id = 0;
};
