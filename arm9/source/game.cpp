#include "game.h"

using ship_ai::ShipState;
using obstacle_ai::ObstacleState;

Game::Game(MultipassEngine& engine) : engine{engine} {
}

Game::~Game() {
}

 DrawableEntity* Game::allocate_entity() {
  if (entities_.size() >= kMaxEntities) {
    return nullptr;
  }
  entities_.push_back(new DrawableEntity());
  engine.AddEntity(entities_.back());
  return entities_.back();
}

template <>
ShipState* Game::SpawnObject<ShipState>() {
  ShipState* state = InitObject<ShipState>();
  ships_.push_back(state);
  return state;
}

template<>
void Game::RemoveObject<ShipState>(ShipState* object) {
  ships_.remove(object);
  CleanupObject(object);
}

template <>
ObstacleState* Game::SpawnObject<ObstacleState>() {
  ObstacleState* state = InitObject<ObstacleState>();
  obstacles_.push_back(state);
  return state;
}

template<>
void Game::RemoveObject<ObstacleState>(ObstacleState* object) {
  obstacles_.remove(object);
  CleanupObject(object);
}

void Game::Step() {
  for (auto ship_state : ships_) {
    ship_ai::machine.RunLogic(*ship_state);
  }

  auto i = obstacles_.begin();
  while (i != obstacles_.end()) {
    auto obstacle_state = *i;
    obstacle_ai::machine.RunLogic(*obstacle_state);
    if (obstacle_state->dead) {
      CleanupObject(obstacle_state);
      obstacles_.erase(i++);
    } else {
      i++;
    }
  }
}