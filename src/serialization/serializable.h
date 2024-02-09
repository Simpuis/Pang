#pragma once

#include "refl.hpp"

struct serializable : refl::attr::usage::field {};

struct singleton_component : refl::attr::usage::type {};