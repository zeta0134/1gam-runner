#include "ship.h"

#include <nds/arm9/input.h>
#include "dsgx.h"
#include "multipass_engine.h"

// Model data
#include "really_bad_ship_dsgx.h"

using numeric_types::literals::operator"" _f;
using numeric_types::fixed;

using numeric_types::literals::operator"" _brad;
using numeric_types::Brads;

namespace ship_ai {

Dsgx ship_actor((u32*)really_bad_ship_dsgx, really_bad_ship_dsgx_size);

void InitAlways(ShipState& ship) {
  //set the actor for animation
  ship.entity->set_actor(&ship_actor);

  //setup physics parameters for collision
  auto body = ship.entity->body();
  body->height = 6_f;
  body->radius = 1.5_f;

  body->collides_with_bodies = 1;

  //initialize our walking angle?
  ship.current_angle = 270_brad;
}

bool DpadActive(const ShipState& ship) {
  return (keysHeld() & KEY_RIGHT) or 
         (keysHeld() & KEY_LEFT) or 
         (keysHeld() & KEY_UP) or 
         (keysHeld() & KEY_DOWN);
}

bool DpadInactive(const ShipState& ship) {
  return !DpadActive(ship);
}

const fixed kShipForwardSpeed = 1_f;
const fixed kShipStrafeSpeed = 0.2_f;

void ReturnToIdle(ShipState& ship) {
  //reset velocity to 0, so we stop moving
  ship.entity->body()->velocity = {0_f,0_f,kShipForwardSpeed};
}

void MoveShip(ShipState& ship) {
  auto engine = ship.entity->engine();
  Brads dpad_angle = engine->DPadDirection();

  ship.entity->set_rotation(0_brad, ship.current_angle + 90_brad, 0_brad);

  // Apply velocity in the direction of the current angle.
  ship.entity->body()->velocity.x.data_ = -cosLerp(dpad_angle.data_);
  ship.entity->body()->velocity.y.data_ = sinLerp(dpad_angle.data_);
  ship.entity->body()->velocity *= kShipStrafeSpeed;
  ship.entity->body()->velocity.z = kShipForwardSpeed;
}

Edge<ShipState> edge_list[] {
  Edge<ShipState>{kAlways, nullptr, InitAlways, 1}, //Init -> Idle
  {kAlways,DpadActive,MoveShip,2},  // Idle -> Run
  {kAlways,DpadInactive,ReturnToIdle,1},  // Run -> Idle
  {kAlways,nullptr,MoveShip,2},  // Run -> Run (loopback, handles movement)
};

Node node_list[] {
  {"Init", true, 0, 0},
  {"Idle", true, 1, 1, "Armature|Idle1", 30},
  {"Run", true, 2, 3, "Armature|Run", 60},
};

StateMachine<ShipState> machine(node_list, edge_list);

}