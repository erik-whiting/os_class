/***************************
ivt_sim.c
*****************************
* Programmer: Erik Whiting
*
* Course: CSCI 4354.01
*
* Date: September 13, 2021
*
* Assignment: Program #1: Interrupt Vector Simulation
*
* Environment: Ubuntu Linux using the gcc compiler, Ruby Rocks
*
* Files Included: ivt_sim.c
*
* Purpose: Create a program simulating the use of an interrupt vector.
*
* Input: User provided integer between 0 and 7
*
* Preconditions: None that are apparent
*
* Output: The name and starting address of the simulated interrupt present at the
*
user selection.
*
* Postconditions: The user will be asked for input at least five times or until they decide to stop
*
* Algorithm:
*
Define struct with name and starting address to simulate IVT entry
*
Populate an 8-item array with simulation struct
*
while(user_wants_to_continue)
*
get user_input for IVT item or if they want to quit
*
if (user_input >= 1 and <= 8)
*
user_input -= 1 for array indexing
*
ivt_selection = ivt_table[user_input]
*
print user_input->name user_input->starting_address
*
else
*
interpret a bad entry to mean user wants to quit
*
user_wants_to_continue = false
*
*
*
* Estimates
Actuals
* Design: 2 hours
TBA
* Implementation: 2 hours TBA
* Testing: .5 hours
TBA
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct InterruptSimulation {
  int id;
  int startingAddress;
  char name[13];
};

int generateNextId(int lastId);
struct InterruptSimulation *createInterrupts();

int main() {
  int interruptId = 0;
  struct InterruptSimulation *simulations = createInterrupts();
  for (int i = 0; i <= 7; i++) {
    printf("Id: %d\n", simulations[i].id);
    printf("Address: %d\n", simulations[i].startingAddress);
    printf("Name: %s\n", simulations[i].name);
  }

  return 0;
}

struct InterruptSimulation *createInterrupts() {
  int interruptAddress = 0;
  char interruptNames[8][13] = {
    "Keyboard",
    "Mouse",
    "Touch Screen",
    "Printer",
    "Scanner",
    "Camera",
    "USB Drive",
  };

  struct InterruptSimulation *simulations = malloc(sizeof(struct InterruptSimulation) * 8);

  for (int i = 0; i <= 7; i++) {
    interruptAddress = generateNextId(interruptAddress);
    char name = &interruptNames[i];
    struct InterruptSimulation simulation = {.id = i, .startingAddress = interruptAddress, .name = &name};
    simulations[i] = simulation;
  }
}

int generateNextId(int lastId) {
  if (lastId == 0) {
    return 64;
  } else {
    return lastId + 4;
  }
}