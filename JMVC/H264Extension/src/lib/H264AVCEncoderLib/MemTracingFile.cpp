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
FILE* MemTracingFile::west;
FILE* MemTracingFile::east;
FILE* MemTracingFile::north;
FILE* MemTracingFile::south;
FILE* MemTracingFile::sad_west;
FILE* MemTracingFile::sad_east;
FILE* MemTracingFile::sad_north;
FILE* MemTracingFile::sad_south;
FILE* MemTracingFile::sm_west;
FILE* MemTracingFile::sm_east;
FILE* MemTracingFile::sm_north;
FILE* MemTracingFile::sm_south;
FILE* MemTracingFile::var_west;
FILE* MemTracingFile::var_east;
FILE* MemTracingFile::var_north;
FILE* MemTracingFile::var_south;
FILE* MemTracingFile::sadFile;
FILE* MemTracingFile::varFile;
int MemTracingFile::currVar;
FILE* MemTracingFile::vetor;
FILE* MemTracingFile::mb_type;


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
    fprintf(startPointsFile,"Mb(%d,%d) (%d,%d) -- %d,%d\n",currMbX, currMbY, refView, refFrame, x, y);

}

void MemTracingFile::initCacheFiles(int view) {
    if( view == 0 ) {
        north = fopen("mem_tracing/cache/trace/north_0.dat","w");
        east = fopen("mem_tracing/cache/trace/east_0.dat","w");
        west = fopen("mem_tracing/cache/trace/west_0.dat","w");
        south = fopen("mem_tracing/cache/trace/south_0.dat","w");
        sm_north = fopen("mem_tracing/cache/search_map/north_0.dat","w");
        sm_east = fopen("mem_tracing/cache/search_map/east_0.dat","w");
        sm_west = fopen("mem_tracing/cache/search_map/west_0.dat","w");
        sm_south = fopen("mem_tracing/cache/search_map/south_0.dat","w");
        sad_north = fopen("mem_tracing/cache/sad/north_0.dat","w");
        sad_east = fopen("mem_tracing/cache/sad/east_0.dat","w");
        sad_west = fopen("mem_tracing/cache/sad/west_0.dat","w");
        sad_south = fopen("mem_tracing/cache/sad/south_0.dat","w");
        var_north = fopen("mem_tracing/cache/var/north_0.dat","w");
        var_east = fopen("mem_tracing/cache/var/east_0.dat","w");
        var_west = fopen("mem_tracing/cache/var/west_0.dat","w");
        var_south = fopen("mem_tracing/cache/var/south_0.dat","w");
       
    }
    if( view == 1 ) {
        north = fopen("mem_tracing/cache/trace/north_1.dat","w");
        east = fopen("mem_tracing/cache/trace/east_1.dat","w");
        west = fopen("mem_tracing/cache/trace/west_1.dat","w");
        south = fopen("mem_tracing/cache/trace/south_1.dat","w");
        sm_north = fopen("mem_tracing/cache/search_map/north_1.dat","w");
        sm_east = fopen("mem_tracing/cache/search_map/east_1.dat","w");
        sm_west = fopen("mem_tracing/cache/search_map/west_1.dat","w");
        sm_south = fopen("mem_tracing/cache/search_map/south_1.dat","w");
        sad_north = fopen("mem_tracing/cache/sad/north_1.dat","w");
        sad_east = fopen("mem_tracing/cache/sad/east_1.dat","w");
        sad_west = fopen("mem_tracing/cache/sad/west_1.dat","w");
        sad_south = fopen("mem_tracing/cache/sad/south_1.dat","w");
        var_north = fopen("mem_tracing/cache/var/north_1.dat","w");
        var_east = fopen("mem_tracing/cache/var/east_1.dat","w");
        var_west = fopen("mem_tracing/cache/var/west_1.dat","w");
        var_south = fopen("mem_tracing/cache/var/south_1.dat","w");
    }
    if( view == 2 ) {
        north = fopen("mem_tracing/cache/trace/north_2.dat","w");
        east = fopen("mem_tracing/cache/trace/east_2.dat","w");
        west = fopen("mem_tracing/cache/trace/west_2.dat","w");
        south = fopen("mem_tracing/cache/trace/south_2.dat","w");
        sm_north = fopen("mem_tracing/cache/search_map/north_2.dat","w");
        sm_east = fopen("mem_tracing/cache/search_map/east_2.dat","w");
        sm_west = fopen("mem_tracing/cache/search_map/west_2.dat","w");
        sm_south = fopen("mem_tracing/cache/search_map/south_2.dat","w");
        sad_north = fopen("mem_tracing/cache/sad/north_2.dat","w");
        sad_east = fopen("mem_tracing/cache/sad/east_2.dat","w");
        sad_west = fopen("mem_tracing/cache/sad/west_2.dat","w");
        sad_south = fopen("mem_tracing/cache/sad/south_2.dat","w");
        var_north = fopen("mem_tracing/cache/var/north_2.dat","w");
        var_east = fopen("mem_tracing/cache/var/east_2.dat","w");
        var_west = fopen("mem_tracing/cache/var/west_2.dat","w");
        var_south = fopen("mem_tracing/cache/var/south_2.dat","w");
    }
    
}

void MemTracingFile::insertCacheAccess(int x, int y) {
    FILE *temp;
    if( refView < currView ) { //NORTH
        temp = north;
    }
    if( refView > currView ) { //SOUTH
        temp = south;
    }
    if( refView == currView ) {
        if( refFrame < currFrame ) { //WEST
            temp = west;
        }
        else { //EAST
            temp = east;
        }
    }
    fprintf(temp, "%d %d\n",x, y);
}

void MemTracingFile::insertCacheCurrMB() {
    insertCacheAccess(currMbX, currMbY);
}

void MemTracingFile::closeCacheFiles() {
    fclose(north);
    fclose(south);
    fclose(east);
    fclose(west);
    fclose(sadFile);
    fclose(varFile);
    fclose(sm_north);
    fclose(sm_south);
    fclose(sm_east);
    fclose(sm_west);
}

void MemTracingFile::insertSearchMapMB(int num) {
    FILE *temp;
    if( refView < currView ) { //NORTH
        temp = sm_north;
    }
    if( refView > currView ) { //SOUTH
        temp = sm_south;
    }
    if( refView == currView ) {
        if( refFrame < currFrame ) { //WEST
            temp = sm_west;
        }
        else { //EAST
            temp = sm_east;
        }
    }
    
    if( num == -1) { //FIM DO MB!
        fprintf(temp, "666\n");
    }
    else {
        fprintf(temp, "%d %d\n", currMbX, currMbY);
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

    FILE *temp;
    if( refView < currView ) { //NORTH
        temp = sm_north;
    }
    if( refView > currView ) { //SOUTH
        temp = sm_south;
    }
    if( refView == currView ) {
        if( refFrame < currFrame ) { //WEST
            temp = sm_west;
        }
        else { //EAST
            temp = sm_east;
        }
    }

    fprintf(temp, "%d\n", it);
    
}

void MemTracingFile::insertZeroSad(unsigned int sad) {
    FILE *temp;
    if( refView < currView ) { //NORTH
        temp = sad_north;
    }
    if( refView > currView ) { //SOUTH
        temp = sad_south;
    }
    if( refView == currView ) {
        if( refFrame < currFrame ) { //WEST
            temp = sad_west;
        }
        else { //EAST
            temp = sad_east;
        }
    }

    fprintf(temp, "%d\n", sad);
}

void MemTracingFile::insertVar(int var) {
    currVar = var;

}

void MemTracingFile::printVar() {
    FILE *temp;
    if( refView < currView ) { //NORTH
        temp = var_north;
    }
    if( refView > currView ) { //SOUTH
        temp = var_south;
    }
    if( refView == currView ) {
        if( refFrame < currFrame ) { //WEST
            temp = var_west;
        }
        else { //EAST
            temp = var_east;
        }
    }
    fprintf(temp,"%d\n", currVar);
}

