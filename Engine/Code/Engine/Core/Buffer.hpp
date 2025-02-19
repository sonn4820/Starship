#pragma once
#include <vector>

enum class eBufferEndian
{
	NATIVE,
	LITTLE,
	BIG
};

class BufferWriter
{

	unsigned char* AppendUnitializedByte();

	void AppendByte(unsigned char b);
	void AppendChar(char c);
	void AppendUShort16(unsigned short u);
	void AppendShort16(short s);
	void AppendUInt16(unsigned int u);
	void AppendInt16(int i);
	void AppendFloat(float f);
	void AppendDouble(double d);
	bool m_isOppositeEndianessFromNative = false;
};

class BufferParser
{
	BufferParser(unsigned char const* bufferData, size_t bufferSize, eBufferEndian endianMode);
	BufferParser(std::vector<unsigned char> const& buffer, eBufferEndian endianMode);

	void SetEndianMode(eBufferEndian endianMode);
	eBufferEndian GetPlatformNativeEndian() const;

	float ParseByte();
	float ParseFloat();

	unsigned char const* m_scanPosition;
	unsigned char const* m_scanStart;
	unsigned char const* m_scanEnd;

	eBufferEndian m_endianMode = eBufferEndian::NATIVE;


};

inline void Reverse4BytesInPlace(void* ptrTo32BitDword)
{
	unsigned int* asUint32Ptr = reinterpret_cast<unsigned int*>(ptrTo32BitDword);
	unsigned int originalUint32 = *asUint32Ptr;
	unsigned int reversedUint32 = ((originalUint32 & 0x000000ff) << 24 |
									(originalUint32 & 0x0000ff00) << 8 |
									(originalUint32 & 0x00ff0000) >> 8 |
									(originalUint32 & 0xff000000) >> 24);
	*asUint32Ptr = reversedUint32;
}
inline void Reverse8BytesInPlace(void* ptrTo64BitQword)
{
	int64_t u = *(int64_t*)ptrTo64BitQword;

	*(int64_t*)ptrTo64BitQword = ((u & 0x00000000000000ff) << 56 |
									(u & 0x000000000000ff00) << 40 |
									(u & 0x0000000000ff0000) << 24 |
									(u & 0x00000000ff000000) << 8 |
									(u & 0x000000ff00000000) >> 8 |
									(u & 0x0000ff000000000) >> 24 |
									(u & 0x00ff00000000000) >> 40 |
									(u & 0xff0000000000000) >> 56);
}
inline void ReverseSizeTInPlace(void* ptrToSizeT)
{
	if (sizeof(ptrToSizeT) == 8)
	{
		Reverse8BytesInPlace(ptrToSizeT);
	}
	else
	{
		Reverse4BytesInPlace(ptrToSizeT);
	}
}