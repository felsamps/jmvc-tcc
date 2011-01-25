#include <stdlib.h>

#include "MemTracingFile.h"
#include "MemAccessHandler.h"

MemTracingFile::MemTracingFile() {
}

MemTracingFile::~MemTracingFile() {
}

int MemTracingFile::refMinX;
int MemTracingFile::refMinY;
int MemTracingFile::refMaxX;
int MemTracingFile::refMaxY;
unsigned int MemTracingFile::refFrame;
unsigned int MemTracingFile::refView;
unsigned int MemTracingFile::currMbX;
unsigned int MemTracingFile::currMbY;
unsigned int MemTracingFile::currFrame;
unsigned int MemTracingFile::currView;
unsigned int MemTracingFile::blkSize;
unsigned int MemTracingFile::targetBlk;
FILE* MemTracingFile::traceFile;
FILE* MemTracingFile::otherFile;
FILE* MemTracingFile::startPointsFile;
unsigned char MemTracingFile::biPrediction;
unsigned char MemTracingFile::refinement;
FILE* MemTracingFile::tracingFile;
FILE* MemTracingFile::varFile;
FILE* MemTracingFile::sadFile;
FILE* MemTracingFile::smFile;
int MemTracingFile::currVar;
FILE* MemTracingFile::vetor;
FILE* MemTracingFile::mb_type;
bool MemTracingFile::west;
bool MemTracingFile::east;
bool MemTracingFile::north;
bool MemTracingFile::south;
bool MemTracingFile::enable;

void MemTracingFile::setTraceFile(FILE* newFile) {
	traceFile = newFile;
}

void MemTracingFile::initTraceFile(std::string nome) {
	traceFile = fopen(nome.c_str(),"w");
}

void MemTracingFile::closeTraceFile() {
	fclose(traceFile);
}

void MemTracingFile::initOtherFile(std::string nome) {
	otherFile = fopen(nome.c_str(),"w");
}

void MemTracingFile::initStartPointsFile(std::string nome) {
    startPointsFile = fopen(nome.c_str(),"w");
}

void MemTracingFile::closeStartPointsFile() {
    fclose(startPointsFile);
}

void MemTracingFile::closeOtherFile() {
	fclose(otherFile);
}

int MemTracingFile::printOtherFile(char* s, ...) {
	va_list args;
	va_start(args, s);
	return fprintf(otherFile,s,args);
}

void MemTracingFile::setMinRefArea(int x, int y) {
	//VERIFICAR SE AJUSTE ESTÁ CORRETO!!!!!
	refMinX = x + currMbX*16;
	refMinY = y + currMbY*16;
}

void MemTracingFile::setMaxRefArea(int x, int y) {
	refMaxX = x + currMbX*16;
	refMaxY = y + currMbY*16;
}

void MemTracingFile::setRefFrame(unsigned int frame) {
	refFrame = frame;
}
void MemTracingFile::setRefView(unsigned int view) {
	refView = view;
}
void MemTracingFile::setCurrMb(unsigned int x, unsigned int y) {
	currMbX = x;
	currMbY = y;
}
void MemTracingFile::setCurrFrame(unsigned int frame) {
	currFrame = frame;
}
void MemTracingFile::setCurrView(unsigned int view) {
	currView = view;
}
void MemTracingFile::setBlkSize(unsigned int size) {
	blkSize = size;
}
void MemTracingFile::setTargetBlk(unsigned int blk) {
	targetBlk = blk;
}
void MemTracingFile::setRefRectangle(int x, int y) {
	if(blkSize == 1) { //MB_16x16
		setMinRefArea(x,y);
		setMaxRefArea(x+16,y+16);
	}
}

void MemTracingFile::setBiPrediction(char c) {
	biPrediction = c;
}

std::string MemTracingFile::toString() {
	char traceObj[50];
	sprintf(traceObj,"%d %d\n%d %d\n%u %u\n%d %d\n%u %u\n%u\n\n",
			refMinX, refMinY,
			refMaxX, refMaxY,
			refFrame, refView,
			currMbX, currMbY,
			currFrame, currView,
			blkSize
			);
	std::string str(traceObj);
	return str;
}
void MemTracingFile::saveInTraceFile() {
	std::string traceObj = toString();
	fprintf(traceFile,"%s",traceObj.c_str());
}

void MemTracingFile::saveInTraceFileBin() {
	fwrite(&refMinX, sizeof(int), 1, traceFile);
	fwrite(&refMinY, sizeof(int), 1, traceFile);
	fwrite(&refMaxX, sizeof(int), 1, traceFile);
	fwrite(&refMaxY, sizeof(int), 1, traceFile);
	fwrite(&refFrame, sizeof(int), 1, traceFile);
	fwrite(&refView, sizeof(int), 1, traceFile);
	fwrite(&currMbX, sizeof(int), 1, traceFile);
	fwrite(&currMbY, sizeof(int), 1, traceFile);
	fwrite(&currFrame, sizeof(int), 1, traceFile);
	fwrite(&currView, sizeof(int), 1, traceFile);
	fwrite(&blkSize, sizeof(int), 1, traceFile);
	fwrite(&biPrediction, sizeof(char), 1, traceFile);
        fwrite(&refinement, sizeof(char), 1, traceFile);
}

unsigned int MemTracingFile::getRefFrame() {
	return refFrame;
}

unsigned int MemTracingFile::getRefView() {
	return refView;
}

std::string MemTracingFile::getAreaRef() {
	char str[50];
	sprintf(str,"(%d,%d)(%d,%d) %c\n", refMinX, refMinY, refMaxX, refMaxY, refinement);
	std::string returnable(str);
	return returnable;
}

void MemTracingFile::setRefinement(char c) {
    refinement = c;
}

void MemTracingFile::printMbStartPoint(int x, int y) {
    fprintf(startPointsFile,"%d %d %c\n",x, y, (refView == currView) ? '0' : '1');

}

void MemTracingFile::initCacheFiles(int view) {
    if( view == 0 ) {
        tracingFile = fopen("mem_tracing/cache/trace/trace_0.dat","w");
        smFile = fopen("mem_tracing/cache/search_map/search_map_0.dat","w");
        sadFile = fopen("mem_tracing/cache/sad/sad_0.dat","w");
        varFile = fopen("mem_tracing/cache/var/var_0.dat","w");
    }
    if( view == 1 ) {
        tracingFile = fopen("mem_tracing/cache/trace/trace_1.dat","w");
        smFile = fopen("mem_tracing/cache/search_map/search_map_1.dat","w");
        sadFile = fopen("mem_tracing/cache/sad/sad_1.dat","w");
        varFile = fopen("mem_tracing/cache/var/var_1.dat","w");
    }
    if( view == 2 ) {
        tracingFile = fopen("mem_tracing/cache/trace/trace_2.dat","w");
        smFile = fopen("mem_tracing/cache/search_map/search_map_2.dat","w");
        sadFile = fopen("mem_tracing/cache/sad/sad_2.dat","w");
        varFile = fopen("mem_tracing/cache/var/var_2.dat","w");
    }
    
}

void MemTracingFile::insertCacheAccess(int x, int y) {
    if( biPrediction == 'P' && enable ) {
        fprintf(tracingFile, "%d %d\n",x, y);
    }
}

void MemTracingFile::insertCacheCurrMB() {
    int direction;
    if( refView < currView ) { //NORTH
        direction = 0;
    }
    if( refView > currView ) { //SOUTH
        direction = 1;
    }
    if( refView == currView ) {
        if( refFrame < currFrame ) { //WEST
            direction = 2;
        }
        else { //EAST
            direction = 3;
        }
    }
    if( biPrediction == 'P'&& enable ) {

        fprintf(tracingFile, "%d %d %d\n",currMbX, currMbY, direction);
    }
}

void MemTracingFile::closeCacheFiles() {
    fclose(sadFile);
    fclose(varFile);
    fclose(smFile);
    fclose(smFile);
}

void MemTracingFile::insertSearchMapMB(int num) {
    int direction;
    if( refView < currView ) { //NORTH
        direction = 0;
    }
    if( refView > currView ) { //SOUTH
        direction = 1;
    }
    if( refView == currView ) {
        if( refFrame < currFrame ) { //WEST
            direction = 2;
        }
        else { //EAST
            direction = 3;
        }
    }
    if( biPrediction == 'P'&& enable ) {
        if( num == -1) { //FIM DO MB!
            fprintf(smFile, "666\n");
        }
        else {
            fprintf(smFile, "%d %d %d\n", currMbX, currMbY, direction);
        }
    }
}



void MemTracingFile::insertSearchMap(bool position[4]) {
    int it;
    if(position[0]) {
        it = 7;
    }
    if(position[1]) {
        it = 3;
    }
    if(position[2]) {
        it = 1;
    }
    if(position[3]) {
        it = 5;
    }

    if( biPrediction == 'P' && enable) {
        fprintf(smFile, "%d\n", it);
    }
    
}

void MemTracingFile::insertZeroSad(unsigned int sad) {
    fprintf(sadFile, "%d\n", sad);
}

void MemTracingFile::insertVar(int var) {
    currVar = var;

}

void MemTracingFile::printVar() {
    fprintf(varFile,"%d\n", currVar);
}

void MemTracingFile::initStatus() {
    south = false;
    north = false;
    west = false;
    east = false;
    enable = true;
}

void MemTracingFile::checkStatus() {
    if( refView < currView) { //NORTH
        if( !north ) {
            north = true;
        }
        else {
            enable = false;
        }
    }
    if( refView > currView ) { //SOUTH
        if( !south ) {
            south = true;
        }
        else {
            enable = false;
        }
    }
    if( refView == currView ) {
        if( refFrame < currFrame ) { //WEST
            if( !west ) {
                west = true;
            }
            else {
                enable = false;
            }
        }
        if( refFrame > currFrame ) {
            if( !east ) {
                east = true;
            }
            else {
                enable = false;
            }
            
        }
    }
}
