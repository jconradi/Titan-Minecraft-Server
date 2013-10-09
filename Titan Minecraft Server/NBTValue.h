#pragma once
#include <cstdint>
#include <vector>
#include <map>
#include <string>
#include <boost/shared_ptr.hpp>

// http://mc.kev009.com/NBT
class NBTValue
{
public:
	NBTValue(void);
	NBTValue(const std::string& ourKey) : ourKey(ourKey) { }
	~NBTValue(void);

	typedef std::map<std::string, boost::shared_ptr<NBTValue> > NBTCompoundValue;
	typedef std::vector<boost::shared_ptr<NBTValue> > NBTListValue;

	void SetByteValue(int8_t data)
	{ 
		typeID = NBTValue_ByteType;
		value.byteValue = data;
	}
	void SetShortValue(int16_t data) 
	{ 
		typeID = NBTValue_ShortType;
		value.shortValue = data;
	}
	void SetIntValue(int32_t data) 
	{ 
		typeID = NBTValue_IntType;
		value.intValue = data;
	}
	void SetLongValue(int64_t data)
	{ 
		typeID = NBTValue_LongType;
		value.longValue = data;
	}
	void SetFloatValue(float data) 
	{ 
		typeID = NBTValue_FloatType;
		value.floatValue = data; 
	}
	void SetDoubleData(double data) 
	{
		typeID = NBTValue_DoubleType;
		value.doubleValue = data; 
	}
	void SetByteArrayData(const uint8_t* data, size_t size)
	{
		typeID = NBTValue_ByteArrayType;
		value.byteArrayValue = new std::vector<uint8_t>;
		value.byteArrayValue->insert(value.byteArrayValue->begin(), data, data + size);

	}
	void SetStringData(const std::string& data)
	{
		typeID = NBTValue_ByteType;
		value.stringValue = new std::string(data);
	}

	operator int8_t() { return value.byteValue; }
	operator int16_t() { return value.shortValue; }
	operator int32_t() { return value.intValue; }
	operator int64_t() { return value.longValue; }
	operator float() { return value.floatValue; }
	operator double() { return value.doubleValue; }
	operator std::string() { return *value.stringValue; }

	NBTListValue& operator[](int i) { return (*value.listValue); }
	NBTCompoundValue& operator[](const std::string& key) { return *value.compoundValue; }
	const std::string& Key() const { return ourKey; }

	enum NBTValueTypeID
	{
		NBTValue_EndType,
		NBTValue_ByteType,
		NBTValue_ShortType,
		NBTValue_IntType,
		NBTValue_LongType,
		NBTValue_FloatType,
		NBTValue_DoubleType,
		NBTValue_ByteArrayType,
		NBTValue_StringType,
		NBTValue_ListType,
		NBTValue_CompoundType
	};

private:
	union
	{
		int8_t byteValue;
		int16_t shortValue;
		int32_t intValue;
		int64_t longValue;
		float floatValue;
		double doubleValue;
		std::string* stringValue;
		std::vector<uint8_t>* byteArrayValue;
		NBTListValue* listValue;
		NBTCompoundValue* compoundValue;
	} value;

	std::string ourKey;
	NBTValueTypeID typeID;
};

