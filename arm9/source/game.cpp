#include "game.h"

Game::Game(MultipassEngine& engine) : engine{engine} {
}

Game::~Game() {
}

 DrawableEntity* Game::allocate_entity() {
  entities_.push_back(new DrawableEntity());
  engine.AddEntity(entities_.back());
  return entities_.back();
}

void Game::Step() {
  for (auto ship_state : ships_) {
    ship_ai::machine.RunLogic(*ship_state);
  }
}

template <>
ShipState* Game::SpawnObject<ShipState>() {
  ShipState* state = new ShipState();
  state->entity = allocate_entity();
  const bool too_many_objects = state->entity == nullptr;
  if (too_many_objects) {
    return nullptr;
  }
  ships_.push_back(state);
  return ships_.back();
}

template<>
void Game::RemoveObject<ShipState>(ShipState* object) {
  ships_.remove(object);
}