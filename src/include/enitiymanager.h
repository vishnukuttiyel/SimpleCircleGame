#pragma once

#include <map>
#include <vector>

#include "include/entity.h"

typedef std::vector<std::shared_ptr<Entity>> EntityVec;
typedef std::map<std::string, EntityVec> EntityMap;

class EnitityManager {
  EntityVec m_entities;
  EntityVec m_entitiesToAdd;
  EntityMap m_enitityMap;

  size_t m_totalEntities = 0;

  void removeDeadEntities(EntityVec &vec);

 public:
  EnitityManager();
  void update();

  std::shared_ptr<Entity> addEntity(const std::string &tag);

  const EntityVec &getEntities();
  const EntityVec &getEntities(const std::string &tag);
};