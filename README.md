# Cluster_Analyses
@Artsem Zdanovich (artsemzdanovich@mail.ru zdanovichartsem@gmail.com, genuineProgress)
This is the college project (at Mech-Math MSU, Moscow, group 211) which uses cluster analyses to predict value of given function at random not used point.
The program by default runs the command file run.txt. You can switch that option by directly renaming command file in main.cpp.
Commands which program understands lie in help.txt (which also could be called by including command HELP in command file).
Each command in the command file begins in a new string and consists of the name of the command (the resgister is not important).
The usual sequence of command is as follows:
GC<your parameters>//generate clouds
BUFFER SET <parameters>//load specific cloud in the buffer, where it can be multiplied and distributed at different places of the field.
BUFFER UNLOAD //unloads operated cloud from the buffer
