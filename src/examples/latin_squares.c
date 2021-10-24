#include "csp_definitions.h"
#include "csp_solver.h"

#include <math.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

bool
isConsistent(Assignment* assignment)
{
  return true;
}

unsigned int width;
unsigned int nVariables;

// Properties hold before, after selectUnassignedVariable, getDomainValue:
unsigned int currIdx = 0; // Indexes variable for next Value
Value currVal = NULL; // The last-set Value in this backtrack call; NULL if N/A
// Track whether each Value has appeared in each column/row, respectively:
bool* colCounts = NULL;
bool* rowCounts = NULL;

unsigned int
selectUnassignedVariable(Assignment* assignment)
{
  // Global initialization
  if (colCounts == NULL) colCounts = calloc(nVariables, sizeof(bool));
  if (rowCounts == NULL) rowCounts = calloc(nVariables, sizeof(bool));

  return currIdx;
}

void
_toggle(Value val)
{
  unsigned int col = currIdx % width;
  unsigned int row = currIdx / width;
  unsigned int valIdx = (unsigned int)val - 1;
  colCounts[col * width + valIdx] ^= true;
  rowCounts[row * width + valIdx] ^= true;
}

bool
_valid(Value val)
{
  unsigned int col = currIdx % width;
  unsigned int row = currIdx / width;
  unsigned int valIdx = (unsigned int)val - 1;
  return !colCounts[col * width + valIdx] && !rowCounts[row * width + valIdx];
}

static Value* permutations = NULL;
// "Call stack" of current index in each permutation to consider
static unsigned int* permutationIdxs = NULL;

void
_initializePermutation()
{
  Value* permutation = &permutations[currIdx * width];
  permutationIdxs[currIdx] = 0;

  unsigned int i = 0;
  for (; i < width; i++) permutation[i] = (Value)(i + 1);
  while (i > 1) {
    unsigned int swapIdx = rand() % i;
    Value temp = permutation[--i];
    permutation[i] = permutation[swapIdx];
    permutation[swapIdx] = temp;
  }
}

Value
getDomainValue(Assignment* assignment)
{
  // Static initialization
  if (permutations == NULL) {
    permutations = malloc(nVariables * width * sizeof(Value));
    permutationIdxs = calloc(nVariables, sizeof(unsigned int));
    _initializePermutation();
  }

  Value toReturn;
  while (true) {
    toReturn = NULL;
    if (permutationIdxs[currIdx] == width) break;
    toReturn = permutations[currIdx * width + permutationIdxs[currIdx]++];
    if (_valid(toReturn)) break;
  }

  if (toReturn == NULL) {
    currIdx--;
    _toggle(currVal);
    currVal = (currIdx > 0) ? assignment->values[currIdx - 1] : NULL;
  } else {
    currVal = toReturn;
    _toggle(toReturn);
    currIdx++;
    _initializePermutation();
  }

  return toReturn;
}

int
main(void)
{
  unsigned int seed;
  printf("Enter unsigned int seed: ");
  scanf("%u", &seed);
  srand(seed);

  printf("Enter unsigned int width: ");
  scanf("%u", &width);
  if (width == 0) return 1;
  double BNeeded = log(width) / log(2.0) * 2 / 8; // Most bytes Value/uint store
  if (BNeeded >= sizeof(Value) || BNeeded >= sizeof(unsigned int)) return 1;

  printf("Generating Latin square of width %u\n", width);
  nVariables = width * width;
  Value* values = calloc(nVariables, sizeof(Value));
  Assignment assignment = {.nVariables = nVariables, .values = values};
  Assignment* assignmentPtr = &assignment;
  assignmentPtr = backtrack(
    assignmentPtr, isConsistent, selectUnassignedVariable, getDomainValue);

  if (assignmentPtr == failureAssignment) {
    printf("Failed to find Latin square\n");
  } else {
    printf("Found Latin square:\n");
    for (unsigned int i = 0; i < nVariables; i++) {
      printf("%u\t", (unsigned int)(assignmentPtr->values[i]));
      if ((i + 1) % width == 0) printf("\n");
    }
  }

  return 0;
}
