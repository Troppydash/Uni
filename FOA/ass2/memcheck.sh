valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         --log-file=valgrind-out.txt \
         ./cmake-build-debug/ass2 <test0.txt


echo "written to valgrind-out.txt"