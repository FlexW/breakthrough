#include "game-object.hpp"

const glm::vec2 SIZE(60.0f, 20.0f);
const glm::vec2 VELOCITY(0.0f, 150.0f);

class PowerUp : public GameObject
{
public:
  enum class Type
  {
    SPEED,
    STICKY,
    PASS_THROUGH,
    PAD_SIZE_INCREASE,
    CONFUSE,
    CHAOS
  };

  PowerUp(Type                       type,
          glm::vec3                  color,
          float                      duration,
          glm::vec2                  position,
          std::shared_ptr<Texture2D> texture)
      : GameObject(position, SIZE, texture, false, color, VELOCITY),
        type(type),
        duration(duration),
        activated(true)
  {
  }

  void set_activated(bool value) { activated = value; }

  void set_duration(float value) { duration = value; }

  bool is_activated() const { return activated; }

  float get_duration() const { return duration; }

  Type get_type() const { return type; }

private:
  Type  type;
  float duration  = 0.0f;
  bool  activated = true;
};
