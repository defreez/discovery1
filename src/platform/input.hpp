#pragma once
#include "core/player.hpp"

namespace platform {

// Read current input state from raylib, return core input struct
core::PlayerInput read_input();

} // namespace platform
