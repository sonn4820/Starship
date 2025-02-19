#include "Buffer.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"

void BufferWriter::AppendFloat(float f)
{
	float* addressOfFloat = &f;
	unsigned char* addressOfFloatInByteArray = reinterpret_cast<unsigned char*>(addressOfFloat);
	if (m_isOppositeEndianessFromNative)
	{
		Reverse4BytesInPlace(addressOfFloatInByteArray);
	}

	AppendByte(addressOfFloatInByteArray[0]);
	AppendByte(addressOfFloatInByteArray[1]);
	AppendByte(addressOfFloatInByteArray[2]);
	AppendByte(addressOfFloatInByteArray[3]);
}

void BufferParser::SetEndianMode(eBufferEndian endianModeSub)
{
	eBufferEndian platformEndian = GetPlatformNativeEndian();
	//m_endianMode = endianModeSub
}

eBufferEndian BufferParser::GetPlatformNativeEndian() const
{
	unsigned int endianTest = 0x12345678;
	unsigned char* asByteArray = reinterpret_cast<unsigned char*>(&endianTest);
	if (asByteArray[0] == 0x12)
	{
		return eBufferEndian::LITTLE;
	}
	else if (asByteArray[0] == 0x78)
	{
		return eBufferEndian::BIG;
	}
	else
	{
		ERROR_AND_DIE("SOMETHING WRONG");
	}
}

float BufferParser::ParseFloat()
{
	float finalValue = 0;
	return finalValue;
}


