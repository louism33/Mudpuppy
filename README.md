# Mudpuppy

Mudpuppy is a c++ othello / reversi program. 

Version 0.2 can be played in two ways. Call the executalb ewith no arguments, and you get a command line player that probably only works on linux. Call `mudpuppy-v0.2 nboard` and you will get an nboard compliant mudpuppy, which can nicely connect to NBoard: https://github.com/weltyc/nboard http://www.orbanova.com/nboard/ 

On a perft command, we reach approximately 32 mnps.

The Engine is a standard minimax searcher with most of the trimmings.
