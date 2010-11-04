/* 
 * File:   MyDefs.h
 * Author: felsamps
 *
 * Created on 14 de Maio de 2010, 15:06
 */

#ifndef _MYDEFS_H
#define	_MYDEFS_H

#include <string>
#include <map>

//#define DBG_MODE
//#define TRACING_MODE
#define TRACE_SW_USAGE

#ifdef TRACING_MODE
//    #define T_FS
//    #define T_TZ
//    #define T_SPIRAL
    #define T_LOG
#endif

//Defines that control the debug prints
#ifdef DBG_MODE
    #define SLICE_ENCODER_DBG
//    #define MB_ENCODER_DBG
    #define ME_DBG
//    #define TZ_SEARCH_DBG
//    #define FULL_SEARCH_DBG
//    #define SPIRAL_SEARCH_DBG
//    #define LOG_SEARCH_DBG
#endif


//Class for the mbType conversion: int -> string
class MbTypeToString {
private:
    std::map<int,std::string> mbType;

public:
    MbTypeToString() {
        mbType[0] = "MbBlock_Skip";
        mbType[1] = "Mb_16x16";
        mbType[2] = "Mb_16x8";
        mbType[3] = "Mb_8x16";
        mbType[4] = "Mb_8x8";
        mbType[5] = "Mb_8x8ref0";
        //...
        mbType[8] = "Blk_8x8";
        mbType[9] = "Blk_8x4";
        mbType[10] = "Blk_4x8";
        mbType[11] = "Blk_4x4";
    }

    std::string getMbType(int mbType) {
        return this->mbType[mbType];
    }
};

#endif	/* _MYDEFS_H */

