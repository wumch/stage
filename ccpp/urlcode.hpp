
#include "meta.hpp"
#include <string>
#include <string.h>

namespace stage {

typedef unsigned char BYTE;

inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x -10 + 'A': x + '0';
}

inline BYTE fromHex(const BYTE &x)
{
	return isdigit(x) ? x-'0' : x-'A'+10;
}

inline std::string URLEncode(const std::string &sIn)
{
	std::string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{
		BYTE buf[4];
		memset( buf, 0, 4 );
		if( isalnum( (BYTE)sIn[ix] ) )
		{
			buf[0] = sIn[ix];
		}
		//else if ( isspace( (BYTE)sIn[ix] ) ) //貌似把空格编码成%20或者+都可以
		//{
		//    buf[0] = '+';
		//}
		else
		{
			buf[0] = '%';
			buf[1] = toHex( (BYTE)sIn[ix] >> 4 );
			buf[2] = toHex( (BYTE)sIn[ix] % 16);
		}
		sOut += (char *)buf;
	}
	return sOut;
};

inline std::string URLDecode(const std::string &sIn)
{
	std::string sOut;
	for( size_t ix = 0; ix < sIn.size(); ix++ )
	{
		BYTE ch = 0;
		if(sIn[ix]=='%')
		{
			ch = (fromHex(sIn[ix+1])<<4);
			ch |= fromHex(sIn[ix+2]);
			ix += 2;
		}
		else if(sIn[ix] == '+')
		{
			ch = ' ';
		}
		else
		{
			ch = sIn[ix];
		}
		sOut += (char)ch;
	}
	return sOut;
}

inline size_t urldecode(const char* const in, char* const out, std::size_t len, std::size_t* const decoded)
{
//	uint8_t chr;
	size_t outlen = 0;
	for (size_t i = 0, _end = len - 3; i < len; ++i)
	{
//		chr = 0;
		if (CS_BLIKELY(in[i] == '%'))
		{
			if (i > _end)
			{
				*decoded = outlen;
				return len - i;
			}
			out[outlen++] = (fromHex(in[i+1]) << 4) | fromHex(in[i+2]);
			i += 2;
//			out[outlen] |= fromHex(in[i+2]);
//			i += 2;
//			++outlen;
		}
		else if (in[i] == '+')
		{
			out[outlen++] = ' ';
		}
		else
		{
			out[outlen++] = in[i];
		}
	}
	*decoded = outlen;
	return 0;
}

}
