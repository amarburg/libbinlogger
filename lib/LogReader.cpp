/*
 * LogReader.cpp
 *
 *  Created on: 19 Nov 2012
 *      Author: thomas
 */


// TODO:   Currently single threaded.  Uncompression could be parallelized
// in manner similar to LogWriter...

#include "logger/LogReader.h"

#ifdef USE_SNAPPY
#include <snappy.h>
#endif
#include <zlib.h>

#include <boost/filesystem.hpp>
namespace fs = boost::filesystem;

#include <g3log/g3log.hpp>

namespace logger {

LogReader::LogReader( void )
  : numFrames( 0 ),
    currentFrame( 0 )
{
}

LogReader::~LogReader()
{
  close();
}

FieldHandle_t LogReader::findField( const std::string &field )
{
  for( Fields::size_type i = 0; i < _fields.size(); ++i ) {
    if( field == _fields[i].name ) return i;
  }

  return -1;
}

bool LogReader::open( const std::string &filename )
{
  CHECK(fs::exists(fs::path(filename)));

  fp = fopen(filename.c_str(), "rb");
  if( !fp ) {
    LOG(WARNING) << "Couldn't open log file " << filename;
    return false;
  }

  currentFrame = 0;
  uint16_t version = 0;

  CHECK( fread( &version, sizeof( int16_t), 1, fp ) == 1 );
  CHECK( version == LogFormatVersion );
  LOG(DEBUG) << "Format version: " << version;

  CHECK( fread( &_featureFlags, sizeof( int16_t), 1, fp ) == 1 );
  LOG(DEBUG) << "Feature flags: " << _featureFlags;

#if not defined(USE_SNAPPY)
  CHECK( (_featureFlags & SNAPPY_COMPRESSION) == 0 ) << "Log written with Snappy compressor, which isn't compiled in...";
#endif

  CHECK( _featureFlags != 0 ) << "Feature flags is zero? (" << _featureFlags << ")";

  CHECK( fread(&numFrames, sizeof(int32_t), 1, fp) == 1 );
  LOG(DEBUG) << "Num frames: " << numFrames;

	int32_t numFields = 0;
	CHECK(fread(&numFields, sizeof(int32_t), 1, fp) == 1);
  LOG(DEBUG) << "Num fields: " << numFields;

  CHECK( numFields > 0 && numFields < 5 ) << "Don't believe the number of fields: " << numFields;

  _fields.clear();
	for( int32_t i = 0; i < numFields; ++i ) {
		int32_t h, w, type, len;
    char buf[80];

		CHECK(fread(&h, sizeof(int32_t), 1, fp) == 1);
		CHECK(fread(&w, sizeof(int32_t), 1, fp) == 1);
		CHECK(fread(&type, sizeof(int32_t), 1, fp) == 1);
		CHECK(fread(&len, sizeof(int32_t), 1, fp) == 1);
    len = std::min(len,80);
		CHECK(fread(buf, sizeof(std::string::value_type), len, fp) == (size_t)len);

    LOG(DEBUG) << "Field " << i << " is " << h << " x " << w << "; type " << type << "  name \"" << std::string(buf,len) << "\"";

    _fields.emplace_back( std::string( buf, len), cv::Size( w, h ), static_cast<FieldType_t>(type) );

    const Field &f( _fields.back() );
    _data.emplace_back( f.nBytes() );

    _compressed.emplace_back( f.compressedBytes() );
	}

  // Read header
  return true;
}

bool LogReader::close( void )
{
  if( fp == NULL ) return false;

  fclose( fp );
  fp = NULL;
  return true;
}


bool LogReader::grab()
{
  currentFrame++;

  for( Fields::size_type i = 0; i < _fields.size(); ++i ) {
    CHECK( fp != NULL );
    CHECK( ferror( fp ) == 0 ) << "Error reading stream: " << ferror( fp );

    uint32_t len = 0;
    CHECK( fread( &len, sizeof(uint32_t), 1, fp ) == 1 );

    size_t nread = fread( _compressed[i].data.get(), sizeof(unsigned char), len, fp );
    CHECK( nread == len ) << "Expected " << len << " read " << nread;
    _compressed[i].size = len;
  }

  // Now decompress the data
  for( Fields::size_type i = 0; i < _fields.size(); ++i ) {
    uLongf destLen = _data[i].size;

    std::chrono::steady_clock::time_point start = std::chrono::steady_clock::now();
#ifdef USE_SNAPPY
    if( _featureFlags & SNAPPY_COMPRESSION) {
      // LOG(INFO) << "IsValidCompressedBuffer: " << snappy::IsValidCompressedBuffer( (char *)_compressed[i].data.get(), _compressed[i].size );

      if( !snappy::RawUncompress( (char *)_compressed[i].data.get(), _compressed[i].size, _data[i].data.get() )) {
        LOG(WARNING) << "Error uncompressing snappy data.";
        continue;
      }
    }
    else
#endif
    {
      if( uncompress( (Bytef *)_data[i].data.get(), &destLen, (Bytef *)_compressed[i].data.get(), _compressed[i].size ) != Z_OK) {
        LOG(WARNING) << "Error uncompressing zlib data.";
        continue;
      }
    }
    LOG(DEBUG) << "PNG uncompression required " << std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() << " ms";


    CHECK( destLen == _fields[i].nBytes() );
  }

  return true;
}

cv::Mat LogReader::retrieve( FieldHandle_t handle )
{
  CHECK( handle >= 0 && handle < (int)_fields.size() );


  return cv::Mat( _fields[handle].size, _fields[handle].cvType(), (void *)_data[handle].data.get() );
}


}
