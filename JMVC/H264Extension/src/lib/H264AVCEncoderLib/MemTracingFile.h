/* 
 * File:   MemTracingFile.h
 * Author: felsamps
 *
 * Created on 16 de Maio de 2010, 01:19
 */

#ifndef _MEMTRACINGFILE_H
#define	_MEMTRACINGFILE_H

#include <string.h>
#include <stdio.h>
#include <string>
#include <stdarg.h>


class MemTracingFile {
private:
    static int refMinX,refMinY,refMaxX,refMaxY;
    static unsigned int refFrame, refView;
    static unsigned int currMbX,currMbY;
    static unsigned int currFrame,currView;
    static unsigned int blkSize;
    static unsigned int targetBlk;



public:
    static FILE* otherFile;
    static FILE* traceFile;
    
    MemTracingFile();
    virtual ~MemTracingFile();

    static void initOtherFile(std::string nome);
    static int printOtherFile(char *s, ...);
    static void closeOtherFile();

    static void initTraceFile(std::string nome);
    static void closeTraceFile();
    static void setTraceFile(FILE *file);

    static unsigned int getRefFrame();
    static unsigned int getRefView();
		static std::string getAreaRef();
    
    static void setMinRefArea(int x, int y);
    static void setMaxRefArea(int x, int y);
    static void setRefFrame(unsigned int frame);
    static void setRefView(unsigned int view);
    static void setCurrMb(unsigned int x,unsigned int y);
    static void setCurrFrame(unsigned int frame);
    static void setCurrView(unsigned int view);
    static void setBlkSize(unsigned int blkSize);
    static void setTargetBlk(unsigned int targetBlk);
    static void setRefRectangle(int x, int y);

    static std::string toString();
    static void saveInTraceFile();
    static void saveInTraceFileBin();


};



#endif	/* _MEMTRACINGFILE_H */

