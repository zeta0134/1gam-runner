#ifndef GAME_H
#define GAME_H

#include "multipass_engine.h"
#include "drawable_entity.h"
#include "ai/ship.h"
#include <list>

using ship_ai::ShipState;

class Game {
  public:
    Game(MultipassEngine& engine);
    ~Game();

    template <typename StateType> 
    StateType* SpawnObject();

    template <typename StateType>
    void RemoveObject(StateType* object);

    void Step();

  private:
    const u32 kMaxEntities = 100;
    std::list<DrawableEntity*> entities_;
    std::list<ShipState*> ships_;

    DrawableEntity* allocate_entity();
    MultipassEngine& engine;
};

#endif  // GAME_H