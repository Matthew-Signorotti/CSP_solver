#ifndef CSP_SOLVER_H
#define CSP_SOLVER_H

#include "csp_definitions.h"

#include <stdbool.h>

typedef bool (*AssignmentToBool)(Assignment*);
typedef unsigned int (*AssignmentToInt)(Assignment*);
typedef Value (*AssignmentToValue)(Assignment*);

Assignment*
backtrack(
  Assignment* assignment,
  AssignmentToBool isConsistent,
  AssignmentToInt selectUnassignedVariable,
  AssignmentToValue getDomainValue);

#endif
