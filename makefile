rangequery: rangequery.o
        g++ rangequery.o -o rangequery

rangequery.o: rangequery.cpp rangequery.h
        g++ -c rangequery.cpp