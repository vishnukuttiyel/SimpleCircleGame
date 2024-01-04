// entity.h
#pragma once

#include <memory>
#include <string>

#include "include/component.h"

class Entity {
  // Enbable only entity manager to call entity constructor
  friend class EnitityManager;

  bool m_active = true;
  size_t m_id = 0;
  std::string m_tag = "default";

  // Constructor
  Entity(const size_t id, const std::string& tag);

 public:
  // component pointers
  std::shared_ptr<CTransform> cTransform;
  std::shared_ptr<CShape> cShape;
  std::shared_ptr<CCollision> cCollision;
  std::shared_ptr<CInput> cInput;
  std::shared_ptr<CScore> cScore;
  std::shared_ptr<CLifespan> cLifespan;

  // private member access functions
  bool isActive() const;
  const std::string& tag() const;
  const size_t id() const;
  void destroy();
};
