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
print user_input.name user_input.starting_address
*
else
*
interpret a bad entry to mean user wants to quit
*
user_wants_to_continue = false
*
*
*
*                 Estimates        Actuals
* Design:         2 hours          1
* Implementation: 2 hours          2
* Testing:        .5 hours         2
**********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct InterruptSimulation {
  int id;
  int startingAddress;
  char name[13];
};

int main() {
  // Array of names for use later
  const char *interruptNames[7];
  interruptNames[0] = "Keyboard";
  interruptNames[1] = "Mouse";
  interruptNames[2] = "Touch Screen";
  interruptNames[3] = "Printer";
  interruptNames[4] = "Scanner";
  interruptNames[5] = "Camera";
  interruptNames[6] = "USB Drive";

  struct InterruptSimulation simulations[8];

  // populate array of structs
  for (int i = 0; i <= 6; i++) {
    int startingAddress = 64 + (i * 4);
    struct InterruptSimulation simulation;
    simulation.id = i + 1;
    simulation.startingAddress = startingAddress;
    strncpy(simulation.name, interruptNames[i], sizeof(interruptNames[i]));
    simulations[i] = simulation;
  }

  int userInput;
  while (userInput != 0) {
    printf("Please select an interrupt from 1 to 7 (enter 0 to exit)\n");
    scanf("%d", &userInput);
    if (userInput >= 8 || userInput < 0) {
      printf("Please enter a valid number\n");
    } else if (userInput == 0) {
      printf("Goodbye!\n");
    } else {
      int index = userInput - 1; // For array indexing
      printf("Interrupt ID: %d\n", simulations[index].id);
      printf("Interrupt Starting Address: %d\n", simulations[index].startingAddress);
      printf("Interrupt Name: %s\n", simulations[index].name);
    }
  }
  return 0;
}