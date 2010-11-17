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
		
		static unsigned int blkSize;
		static unsigned int targetBlk;
		static unsigned char biPrediction;
                static unsigned char refinement;
                static int currVar;

	public:
                static unsigned int currFrame,currView;
		static FILE* otherFile;
		static FILE* traceFile;
                static FILE* startPointsFile;
                static FILE* west;
                static FILE* east;
                static FILE* north;
                static FILE* south;
                static FILE* sadFile;
                static FILE* varFile;
                static FILE* sm_west;
                static FILE* sm_east;
                static FILE* sm_north;
                static FILE* sm_south;
                static FILE* sad_west;
                static FILE* sad_east;
                static FILE* sad_north;
                static FILE* sad_south;
                static FILE* var_west;
                static FILE* var_east;
                static FILE* var_north;
                static FILE* var_south;
                static FILE* vetor;
                static FILE* mb_type;


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
		static void setBiPrediction(char c);
                static void setRefinement(char c);  //'N' - not refinement 'R' - refinement

		static std::string toString();
		static void saveInTraceFile();
		static void saveInTraceFileBin();

                static void initStartPointsFile(std::string nome);
                static void printMbStartPoint(int x, int y);
                static void closeStartPointsFile();

                static void initCacheFiles(int view);
                static void insertCacheAccess(int x, int y);
                static void insertCacheCurrMB();
                static void closeCacheFiles();
                static void insertSearchMap(bool position[4]);
                static void insertSearchMapMB(int num);

                static void insertZeroSad(unsigned int sad);
                static void insertVar(int var);
                static void printVar();

                static void writeVetor(int x, int y);
                static void writeMbType(int type);

};


#endif	/* _MEMTRACINGFILE_H */

