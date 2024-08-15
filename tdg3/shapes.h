#pragma once
#include "glm.hpp"
namespace tdg
{
  class shapes {
  public:

    static void cube_shape();
    static void line(float sx, float sy, float ex, float ey);

    static void draw_cube(glm::vec3 pos, glm::vec3 euler = glm::vec3{0}, glm::vec3 scale = glm::vec3{1});
  };
}