#include <iostream>

#include "obstacle.h"

#include "dsgx.h"
#include "game.h"

// Model data
#include "cube_dsgx.h"

#include "debug.h"

using numeric_types::literals::operator"" _f;
using numeric_types::fixed;

using numeric_types::literals::operator"" _brad;
using numeric_types::Brads;

namespace obstacle_ai {

Dsgx obstacle_actor((u32*)cube_dsgx, cube_dsgx_size);

void InitAlways(ObstacleState& obstacle) {
  //set the actor for animation
  obstacle.entity->set_actor(&obstacle_actor);

  //setup physics parameters for collision
  auto body = obstacle.entity->body();
  body->height = 3_f;
  body->radius = 3_f;

  body->collides_with_bodies = 1;
  body->is_sensor = 1;
  body->is_pikmin = 1;
}

void DeleteSelf(ObstacleState& obstacle) {
  obstacle.dead = true;
}

bool AliveFor150Frames(const ObstacleState& obstacle) {
  return obstacle.frames_alive > 220;
}

Edge<ObstacleState> edge_list[] {
  Edge<ObstacleState>{kAlways, nullptr, InitAlways, 1}, //Init -> Idle
  {kAlways,AliveFor150Frames,DeleteSelf,1},  // Idle -> Deleted
  {kAlways,nullptr,nullptr,1}  // Idle -> Idle
};

Node node_list[] {
  {"Init", true, 0, 0},
  {"Idle", true, 1, 2},
};

StateMachine<ObstacleState> machine(node_list, edge_list);

}  // namespace obstacle_ai