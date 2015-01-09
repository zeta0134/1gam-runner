#ifndef AI_OBSTACLE_H
#define AI_OBSTACLE_H

#include "state_machine.h"
#include "drawable_entity.h"

namespace obstacle_ai {

struct ObstacleState : ObjectState {
};

extern StateMachine<ObstacleState> machine;

}  // namespace captain_ai

#endif