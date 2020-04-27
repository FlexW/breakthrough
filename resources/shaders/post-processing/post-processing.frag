#version 330 core

in vec2 frag_texture_coords;
out vec4 color;

uniform sampler2D scene;
uniform vec2 offsets[9];
uniform int edge_kernel[9];
uniform float blur_kernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

void main()
{
  // Color will be set to random values, reset it
  color = vec4(0.0, 0.0, 0.0, 1.0);

  float offset = 1.0f/300.0f;

  vec3 samplee[9];
  // sample from texture offsets if using convolution matrix
  if(chaos || shake)
  {
    for(int i = 0; i < 9; i++)
    {
      samplee[i] = vec3(texture(scene, frag_texture_coords.st + offsets[i]));
    }
  }

  // process effects
  if(chaos)
  {
      for(int i = 0; i < 9; i++)
          color += vec4(samplee[i] * edge_kernel[i], 0.0f);
      color.a = 1.0f;
  }
  else if(confuse)
  {
      color = vec4(1.0 - texture(scene, frag_texture_coords).rgb, 1.0);
  }
  if(shake)
  {
    for(int i = 0; i < 9; i++)
      color += vec4(samplee[i] * blur_kernel[i], 0.0f);
    color.a = 1.0f;
  }
  else
  {
    color = texture(scene, frag_texture_coords);
  }
}
