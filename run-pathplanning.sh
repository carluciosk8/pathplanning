##***************************************************************************
##    Original code written in: Oct 2001    
##    copyright: (C) Carlucio Santos Cordeiro
##***************************************************************************
./genmaze $1 $2 $3 $4 $5 > maptmp.map

./pathplanning -m maptmp.map -e diagonal
./pathplanning-mpi.sh -m maptmp.map -e diagonal

rm maptmp.map
