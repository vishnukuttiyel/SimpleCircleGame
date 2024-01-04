#include "include/enitiymanager.h"

EnitityManager::EnitityManager() {}

void EnitityManager::update() { removeDeadEntities(m_entities); }

void EnitityManager::removeDeadEntities(EntityVec& vec) {}

std::shared_ptr<Entity> EnitityManager::addEntity(const std::string& tag) {
  auto entiy = std::shared_ptr<Entity>(new Entity(m_totalEntities++, tag));

  n_entitiesToAdd.push_back(entiy);

  return entiy;
}

const EntityVec& EnitityManager::getEntities() { return m_entities; }

const EntityVec& EnitityManager::getEntities(const std::string& tag) {
  return m_entities;
}