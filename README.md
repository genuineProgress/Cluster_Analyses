# Cluster_Analyses
@genuineProgress (artsemzdanovich@mail.ru zdanovichartsem@gmail.com)
This file is subject to change. Any feedback is appreciated!
This is the college project (at Mech-Math MSU, Moscow, group 211) which uses cluster analyses to predict value of given function at random not used point.
The program by default runs the command file run.txt. You can switch that option by directly renaming command file in main.cpp.
Commands which program understands lie in help.txt (which also could be called by including command HELP in command file).
Each command in the command file begins in a new string and consists of the name of the command (the resgister is not important).
The usual sequence of command is as follows:

  GC<your parameters> //generate clouds

  BUFFER SET <parameters> //load specific cloud in the buffer, where it can be multiplied and distributed at different places of the field.

  BUFFER UNLOAD //unload operated cloud from the buffer

  MATRIX //The phase of changing field state from being able to accept new clouds to operating with any cluster analysis's algorithm.
  <algorithm name> <parameters> //select algorithm which you want to operate, multiple choices are acceptable.

  PLOT //generate the data for gnuplot, which in his order plots the resulting work of the algorithm(s), optional

  HELP //described above, optional

  EXIT //finish the program, optional
