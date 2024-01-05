#include "include/enitiymanager.h"

EnitityManager::EnitityManager() {}

void EnitityManager::update() {
  for (auto e : m_entitiesToAdd) {
    m_entities.push_back(e);
    m_enitityMap[e->tag()].push_back(e);
  }

  removeDeadEntities(m_entities);
  for (auto& [tag, enityVec] : m_enitityMap) {
    removeDeadEntities(enityVec);
  }

  m_entitiesToAdd.clear();
}

void EnitityManager::removeDeadEntities(EntityVec& enityVec) {
  enityVec.erase(
      std::remove_if(enityVec.begin(), enityVec.end(),
                     [](const auto& entity) { return !entity->isActive(); }),
      enityVec.end());
}

std::shared_ptr<Entity> EnitityManager::addEntity(const std::string& tag) {
  auto entity = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

  m_entitiesToAdd.push_back(entity);

  return entity;
}

const EntityVec& EnitityManager::getEntities() { return m_entities; }

const EntityVec& EnitityManager::getEntities(const std::string& tag) {
  return m_enitityMap[tag];
}