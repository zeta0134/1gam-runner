#include "camera.h"

#include <cstdio>

#include <nds.h>

#include "drawable_entity.h"

using namespace std;
using numeric_types::literals::operator"" _f;
using numeric_types::fixed;

using numeric_types::literals::operator"" _brad;
using numeric_types::Brads;

Camera::Camera() {
  position_destination_ = Vec3{0_f, 6_f, 4_f};
  target_destination_  = Vec3{0_f, 3_f, 0_f};

  position_current_ = position_destination_;
  target_current_ = target_destination_;

  SetCache();
}

void Camera::Update() {
  fixed const kFollowDistance = 10_f;
  if (entity_to_follow_) {
    Vec3 offset = {0_f, 2_f, 0_f};
    position_destination_ = entity_to_follow_->position() + offset;
    position_destination_.z = 0_f;
    target_destination_ = entity_to_follow_->position();
  } else {
    printf("No entity?\n");
  }

  // Take the weighted average for position and target to take a smooth
  // transition between the old value and the new one.
  const fixed chase_weight = 0.25_f;
  position_current_.z = 0_f;
  position_current_ = position_destination_ * chase_weight + position_current_ *
      (1_f - chase_weight);
  position_current_.z = entity_to_follow_->position().z - kFollowDistance;
  target_current_ = target_destination_ * chase_weight + target_current_ * 
      (1_f - chase_weight);
}

void Camera::LookAt(Vec3 position, Vec3 target, bool instant) {
  position_destination_ = position;
  target_destination_ = target;
  if (instant) {
    position_current_ = position;
    target_current_ = target;
  }
}

Brads Camera::GetAngle() {
  Vec3 facing;
  facing = entity_to_follow_->position() - position_current_;
  facing.y = 0_f;  // Work on the XZ plane.
  if (facing.Length() <= 0_f) {
    return 0_brad;
  }
  facing = facing.Normalize();

  // return 0;
  if (facing.z <= 0_f) {
    return Brads::Raw(acosLerp(facing.x.data_));
  } else {
    return Brads::Raw(-acosLerp(facing.x.data_));
  }
}

void Camera::FollowEntity(DrawableEntity* entity) {
  entity_to_follow_ = entity;
}

// TODO(Nick) Convert this to non-float implementation.
void Camera::ApplyTransform() {
  gluLookAt(
      (float)position_cached_.x, (float)position_cached_.y,
      (float)position_cached_.z, (float)target_cached_.x,
      (float)target_cached_.y, (float)target_cached_.z, 0.0f, 1.0f, 0.0f);
}

void Camera::SetCache() {
  position_cached_ = position_current_;
  target_cached_ = target_current_;
}
