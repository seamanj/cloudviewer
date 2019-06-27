#pragma once

#include <vector>

#include "entities/Entity.hpp"
#include "entities/Camera.hpp"


class Scene {
public:
  Scene();

  void add_entity(Entity::Ptr entity);
  void remove_entity(Entity::Ptr entity);

  void render(const Camera& _camera);
  void render_entity(const Entity& entity, const Camera& _camera);

  void update();

  const std::vector<Entity::Ptr>& entities() const;

private:
  std::vector<Entity::Ptr> _entities;
};
