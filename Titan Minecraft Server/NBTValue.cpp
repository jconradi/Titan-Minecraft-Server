#include "NBTValue.h"


NBTValue::NBTValue(void)
{
	memset(&value, 0, sizeof(value));
}


NBTValue::~NBTValue(void)
{
	switch (typeID)
	{
	case NBTValue_StringType:
		if (value.stringValue != NULL)
		{
			delete value.stringValue;
		}
		break;
	case NBTValue_ByteArrayType:
		{
			if (value.byteArrayValue != NULL)
			{
				delete value.byteArrayValue;
			}
		}
	}
}
