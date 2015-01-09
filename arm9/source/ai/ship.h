#ifndef AI_SHIP_H
#define AI_SHIP_H

#include "state_machine.h"
#include "drawable_entity.h"

namespace ship_ai {

struct ShipState : ObjectState {
  numeric_types::Brads current_angle = numeric_types::Brads::Raw(0);
};

extern StateMachine<ShipState> machine;

}  // namespace captain_ai

#endif