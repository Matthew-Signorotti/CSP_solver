#include "csp_definitions.h"

#include "csp_solver.h"

#include <stdbool.h>
#include <stddef.h>

Assignment* failureAssignment = NULL;

bool
Assignment_isComplete(Assignment* assignment)
{
  for (unsigned int i = 0; i < assignment->nVariables; i++) {
    if (assignment->values[i] == NULL) return false;
  }
  return true;
}

Assignment*
Assignment_assignValue(Assignment* assignment, unsigned int varI, Value val)
{
  assignment->values[varI] = val;
  return assignment;
}

Assignment*
Assignment_removeValue(Assignment* assignment, unsigned int varI)
{
  assignment->values[varI] = NULL;
  return assignment;
}
