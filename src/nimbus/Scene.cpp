#include "Scene.hpp"

#include "shaders/CameraShaderProgram.hpp"


Scene::Scene()
  : _entities({})
{
}


void Scene::add_entity(Entity::Ptr entity) {
  _entities.emplace_back(std::move(entity));
}


void Scene::remove_entity(Entity::Ptr entity) {
  _entities.erase(std::remove(_entities.begin(),_entities.end(), entity ), _entities.end());
}


void Scene::render(const Camera& _camera) {
  for (const auto& entity : _entities) {
    render_entity(*entity, _camera);
  }
}


void Scene::render_entity(const Entity& entity, const Camera& _camera) {
  if (const auto& shader_entity = dynamic_cast<const ShaderEntity*>(&entity)) {
    auto program = shader_entity->program();

    glUseProgram(*program);

    if (auto camera_program = std::dynamic_pointer_cast<CameraShaderProgram>(program)) {
      camera_program->set_camera(_camera);
    }

    entity.draw();
    glUseProgram(0);
  } else {
    entity.draw();
  }
}


void Scene::update() {
  for (const auto& entity : _entities) {
    entity->update();
  }
}


const std::vector<Entity::Ptr>& Scene::entities() const {
  return _entities;
}
