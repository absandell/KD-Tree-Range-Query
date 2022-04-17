rangeQ: rangeQ.o
        g++ rangeQ.o -o rangeQ

rangeQ.o: rangeQ.cpp rangeQ.h
        g++ -c rangeQ.cpp