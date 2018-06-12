/*
 * LogReader.h
 *
 *  Created on: 19 Nov 2012
 *      Author: thomas
 */

#pragma once

#include <string>

#include <cassert>
#include <zlib.h>
#include <iostream>

#ifdef OPENCV3
	#include <opencv2/core.hpp>
#else
	#include <opencv2/opencv.hpp>
#endif

#include <stdio.h>

#include "LogFields.h"

namespace logger {

	using std::string;

class LogReader
{
    public:
        LogReader(void);
        virtual ~LogReader();

        bool open( const string &file );
        bool close( void );

        FieldHandle_t findField( const string &field );

        bool grab();

        cv::Mat retrieve( FieldHandle_t handle );

        int getNumFrames() const { return numFrames; }

        //
        // bool hasMore();
        //
        // const std::string getFile();
        //
        // unsigned short minVal, maxVal;
        // int64_t timestamp;
        //
        // unsigned short * depth;
        // unsigned char * rgb;

    private:

        // Bytef *& decompressionBuffer;
        // IplImage *& deCompImage;
        // unsigned char * depthReadBuffer;
        // unsigned char * imageReadBuffer;
        // int32_t depthSize;
        // int32_t imageSize;

        // const std::string file;
        FILE * fp;
        int32_t numFrames;
        int currentFrame;

        uint16_t _featureFlags;

        Fields _fields;
        std::deque< Chunk > _data;
        std::deque< Chunk > _compressed;

        // int width;
        // int height;
        // int numPixels;
};

}
