#ifndef __BLIOBJ
#define __BLIOBJ

#include <string>
#include <map>
#include <list>
#include <boost/any.hpp>

namespace blieng {

class BliObject
{
public:
	BliObject();
	void setValue(std::string key, boost::any value);
	bool isValue(std::string key);

	bool increase(std::string key);
	bool decrease(std::string key);

	boost::any getValue(std::string key);
	std::string getStringValue(std::string key);
	int getIntValue(std::string key);
	double getDoubleValue(std::string key);
	virtual std::string toString();

	std::list<std::string> getKeys();

protected:
	int getRandomInt(int limit_low, int limit_max);
	double getRandomDouble(double limit_low, double limit_max);

	bool changeIntValue(std::string key, int diff);
	std::map<std::string, boost::any> values;

private:
};

}

#endif
