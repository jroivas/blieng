#ifndef __CHARACTER_H
#define __CHARACTER_H

/** Basic character building blocks
 */

#include <string>
#include <map>
#include <boost/any.hpp>

class Character
{
public:
	Character();

	void setValue(std::string key, boost::any value);

	boost::any getValue(std::string key);
	std::string getStringValue(std::string key);
	int getIntegerValue(std::string key);

private:
	std::map<std::string, boost::any> values;
};

#endif
