
#pragma once

#include "meta.hpp"
#include <cstdlib>
#include <string>
#include <iconv.h>

namespace stage {

class mbswcs
{
public:
	static size_t mb2wc(const char* const mbstr, wchar_t* const wcstr, std::size_t mbstr_len, std::size_t* converted_mbs_len)
	{
		// Get the size of wchar_t after converted
#ifdef WIN32
		int size = MultiByteToWideChar(CP_UTF8, 0, mbstr, -1, NULL, 0);
		MultiByteToWideChar(CP_UTF8, 0, mbstr, -1, wcstr, size);
		return size;
#else
//		size_t size = mbtowc(NULL, mbstr, 0);
//		if (size == static_cast<size_t>(-1))
//		{
//			CS_SAY("test failed, strlen(mbstr):" << strlen(mbstr));
//			CS_SAY("mbstr: [" << mbstr << "]")
//			std::exit(1);
//			return -1;
//		}
//		return mbstowcs(wcstr, mbstr, mbstr_len);

		size_t res_wcs_len = mbstowcs(wcstr, mbstr, mbstr_len);
		for (int try_times = 4; (res_wcs_len == static_cast<size_t>(-1)) && (try_times > 0); --try_times)
		{
			res_wcs_len = mbstowcs(wcstr, mbstr, --mbstr_len);
		}
		*converted_mbs_len = mbstr_len;
		return res_wcs_len;
#endif
	}

	static size_t wc2mb(const wchar_t* const wcstr, char* const mbstr, std::size_t wcstr_len, std::size_t* converted)
	{
		return wcstombs(mbstr, wcstr, wcstr_len);
		// Get the size of char after converted
//#ifdef WIN32
//		int size = WideCharToMultiByte(CP_UTF8, 0, wcstr, -1, NULL, 0, NULL, NULL);
//#else
//		size_t size = wctomb(NULL, wcstr);
//#endif
//
//		mbstr = new char[size+1];
//		if (mbstr)
//		{
//			memset(mbstr, 0, size * sizeof(char));
//#ifdef WIN32
//			int ret = WideCharToMultiByte(CP_UTF8, 0, wcstr, -1, mbstr, size, NULL, NULL);
//			if (ret == 0) // MultiByteToWideChar returns 0 if it does not succeed.
//#else
//			size_t ret = wcstombs(mbstr, wcstr, size+1);
//			if (ret == (size_t)-1)
//#endif
//			{
//				delete[] mbstr;
//				mbstr = NULL;
//			}
//		}
//
//		return mbstr;
	}

	static void setlocale(const char* lc_all = "")
	{
		std::setlocale(LC_ALL, lc_all);
	}

	static void code_convert(const char *from_charset, const char *to_charset, char *inbuf, size_t inlen, char *outbuf, size_t outlen)
	{
		 iconv_t cd;

		 CS_RETURN_IF((cd = iconv_open(to_charset, from_charset)) == NULL);
		 iconv(cd, &inbuf, &inlen, &outbuf, &outlen);

		 iconv_close(cd);
	}
};

}
