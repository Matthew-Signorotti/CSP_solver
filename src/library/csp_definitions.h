#ifndef CSP_DEFINITIONS_H
#define CSP_DEFINITIONS_H

#include <stdbool.h>

typedef void* Value;
typedef struct {
  unsigned int nVariables;
  Value* values; // Of length `nVariables`
} Assignment;

Assignment* failureAssignment;

bool
Assignment_isComplete(Assignment* assignment);

Assignment*
Assignment_assignValue(Assignment* assignment, unsigned int varI, Value val);

Assignment*
Assignment_removeValue(Assignment* assignment, unsigned int varI);

#endif
