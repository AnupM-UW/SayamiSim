#ifndef _ELMA_H
#define _ELMA_H

#include <set>
#include <algorithm>
#include <tuple>

// Utilities

#include "literals.h"
#include "exceptions.h"

// Communications
#include "channel.h"
#include "event.h"

// Processes
#include "process.h"
#include "manager.h"

// State Machines
#include "state.h"
#include "transition.h"
#include "state_machine.h"

#ifdef _RPI_
#include <json/nlohmann_json.h>
#else
#include <json/json.h>
#endif

#endif
