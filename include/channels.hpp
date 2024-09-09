

#ifndef _CHANNELS_HPP
#define _CHANNELS_HPP

#include "Parse.hpp"

class channels
{
	public:
		std::string					name;
		char						mode;
		int							op;
		std::string 				key;
		int							limit;
		std::vector<std::string>	invited;
		std::string					topic;
		channels();
		channels(std::string name, char mode, int op, std::vector<std::string>	invited);

};
#endif
