#!/bin/bash


#[ $# -ne 1 ] && echo arguments error && exit 0
# g++ -o $#.exe 


#g++ -o solver solver.cc ;

Read -p 'Saisissez le nom de l’exécutable' nomfichier
g++ solver.cc -o $nomfichier
