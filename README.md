# TuringMachine
A program in cpp that simulates any turing machine.

## How to run
Use the makefile to make the executable. When you execute the program it will ask you for a filename. This file should exist in the machines directory. If it doesn't the program will tell you and just wait for more input. If the file exists it will simulate the Turing machine then output the tape at the halting state. To exit just CTRL-C or enter 'q'.

### For example running BusyBeaver for 4 states
```
What file do you want to load?
BB4.utm
10111111111111
Number of steps = 107
Number of ones = 13
```

# File Format
The files are in two sections.
## Section 1 - The transition function
The transitions have 5 parameters each semicolon separated. 
1. state name
2. read symbol
3. write symbol
4. move direction
5. next state

### For example
State A reads a 0 writes a 1 moves to the right and transitions to State B
```A;0;1;R;B```

## Section 2 - Other stuff
This section begins after a single line starting with '
Here you can define the blank symbol to use when resetting the tape. The blank symbol should follow a !.
The starting state should be after a ^.
The input to the tape should be after a >.

The input can be empty and a blank tape will be initialized this is the case for the BusyBeaver problem.

### For example the sorting Turing machine
- '           // This ends the transition function
- !0          // This is the blank symbol
- ^A          // This is the starting state
- \>1221221211 // This is the input
