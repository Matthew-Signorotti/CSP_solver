#include "csp_solver.h"

#include "csp_definitions.h"

#include <stddef.h>

Assignment*
backtrack(
  Assignment* assignment,
  AssignmentToBool isConsistent,
  AssignmentToInt selectUnassignedVariable,
  AssignmentToValue getDomainValue)
{
  if (!isConsistent(assignment)) return failureAssignment;

  if (Assignment_isComplete(assignment)) return assignment;

  unsigned int varI = selectUnassignedVariable(assignment);
  Value val;
  while ((val = getDomainValue(assignment)) != NULL) {
    assignment = Assignment_assignValue(assignment, varI, val);
    Assignment* result = backtrack(
      assignment, isConsistent, selectUnassignedVariable, getDomainValue);
    if (result != failureAssignment) return result;
    assignment = Assignment_removeValue(assignment, varI);
  }
  return failureAssignment;
}
