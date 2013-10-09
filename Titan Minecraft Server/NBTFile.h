#pragma once
#include <boost/shared_ptr.hpp>
#include <string>

class NBTValue;

/**
* Minecraft Tag File.
*/
class NBTFile
{
public:
	NBTFile();
	NBTFile(const std::string& filePath);
	~NBTFile(void);

	bool Load(const std::string& filePath);
	bool Load(const char* data, size_t size);

private:
	boost::shared_ptr<NBTValue> GetTagValue(const char* data, size_t readPos, size_t size);
};

