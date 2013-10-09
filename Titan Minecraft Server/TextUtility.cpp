#include "TextUtility.h"
#include <boost/asio.hpp>

using namespace boost::asio::detail::socket_ops;

namespace TextUtility
{
	std::string ASCIIToUCS2BE(const std::string& str)
	{
		std::string ret;

		for (std::string::const_iterator it = str.begin(); it != str.end(); it++)
		{
			int16_t ucs2Char = *it;
			ucs2Char = host_to_network_short(ucs2Char);

			ret += ucs2Char & 0xff;
			ret += (ucs2Char >> 8) & 0xff;
		}

		return ret;
	}
};