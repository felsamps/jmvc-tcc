#ifndef _MEMACCESSHANDLER_H
#define	_MEMACCESSHANDLER_H

#include <set>
#include <stdio.h>

#define W 640
#define H 480

class MemAccessHandler {
private:
    static std::set<int> acc;
    static std::set<int> block_acc;
    static int counter;
    static bool bipred;
    static int refView, currView;
    
    static int currMbX, currMbY;

    static void insert(int x, int y);

    static long long int bw;
    
public:
    static FILE* fp;
    
    MemAccessHandler();
    virtual ~MemAccessHandler();
    
    static void init();
    static void initBW();
    static void incBW(int value);
    static void setMb(int x, int y);
    static void setBiPred(bool flag);
    static void setCurrView(int view);
    static void setRefView(int view);
    static void insertBlock(int x, int y, int size);

    static void report();
    static void reportBW();
};

#endif