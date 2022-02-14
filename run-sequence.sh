##***************************************************************************
##    Original code written in: Oct 2001    
##    copyright: (C) Carlucio Santos Cordeiro
##***************************************************************************
./pathplanning -m maps/map1.map -e diagonal -i -s -1 -f 2000
./pathplanning-mpi.sh -m maps/map1.map -e diagonal -i -s 2000 -f 2000

./pathplanning -m maps/map2.map -e diagonal -i -s 2000 -f 2000
./pathplanning-mpi.sh -m maps/map2.map -e diagonal -i -s 2000 -f 2000

./pathplanning -m maps/map3.map -e diagonal -i -s 2000 -f 2000
./pathplanning-mpi.sh -m maps/map3.map -e diagonal -i -s 2000 -f 2000

./pathplanning -m maps/map4.map -e diagonal -i -s 2000 -f 2000
./pathplanning-mpi.sh -m maps/map4.map -e diagonal -i -s 2000 -f 2000

./pathplanning -m maps/map5.map -e diagonal -i -s 2000 -f 2000
./pathplanning-mpi.sh -m maps/map5.map -e diagonal -i -s 2000 -f 2000

./pathplanning -m maps/map6.map -e diagonal -i -s 2000 -f 2000
./pathplanning-mpi.sh -m maps/map6.map -e diagonal -i -s 2000 -f 2000

./pathplanning -m maps/map7.map -e diagonal -i -s 2000 -f 2000
./pathplanning-mpi.sh -m maps/map7.map -e diagonal -i -s 2000 -f 2000

./genmaze 71 39 2 2 4 > maptmp.map
./pathplanning -m maptmp.map -e diagonal -i -s 2000 -f 2000
./pathplanning-mpi.sh -m maptmp.map -e diagonal -i -s 2000 -f 2000

./genmaze 71 39 2 2 4 > maptmp.map
./pathplanning -m maptmp.map -e diagonal -i -s 2000 -f 2000
./pathplanning-mpi.sh -m maptmp.map -e diagonal -i -s 2000 -f 2000

./genmaze 71 39 2 2 4 > maptmp.map
./pathplanning -m maptmp.map -e diagonal -i -s 2000 -f 2000
./pathplanning-mpi.sh -m maptmp.map -e diagonal -i -s 2000 -f 2000

./genmaze 71 39 2 2 4 > maptmp.map
./pathplanning -m maptmp.map -e diagonal -i -s 2000 -f 2000
./pathplanning-mpi.sh -m maptmp.map -e diagonal -i -s 2000 -f -1

rm maptmp.map
