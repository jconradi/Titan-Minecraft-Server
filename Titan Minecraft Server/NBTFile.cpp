#include <fstream>
#include <sstream>
#include <string>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/shared_ptr.hpp>
#include "NBTFile.h"
#include "NBTValue.h"

NBTFile::NBTFile(void)
{
}


NBTFile::~NBTFile(void)
{
}

bool NBTFile::Load(const std::string& filePath)
{
	std::ifstream inputFile(filePath, std::ios::in | std::ios::binary);
	std::stringstream s;

	if (!inputFile)
		return false;

	boost::iostreams::filtering_streambuf<boost::iostreams::input> in;

	in.push(boost::iostreams::gzip_decompressor());
	in.push(inputFile);

	try
	{
		boost::iostreams::copy(in, s);
		
		s.str().c_str();
		return Load(s.str().c_str(), s.str().length());
	}
	catch (boost::iostreams::zlib_error& e)
	{
		// TODO: Log etc...
		return false;
	}

	return true;
}

bool NBTFile::Load(const char* data, size_t size)
{
	boost::shared_ptr<NBTValue> rootValue(GetTagValue(data, 0, size));

	return true;
}

boost::shared_ptr<NBTValue> NBTFile::GetTagValue(const char* data, size_t readPos, size_t size)
{
	while (readPos < size)
	{
		uint8_t commandByte = data[readPos++];
		uint16_t stringLength = *(uint16_t *)&data[readPos];
		readPos += 2;
	}

	return boost::shared_ptr<NBTValue>();
}