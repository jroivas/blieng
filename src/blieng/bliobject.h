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
	virtual void setValue(std::string key, boost::any value);
	virtual bool isValue(std::string key);

	virtual bool increase(std::string key);
	virtual bool decrease(std::string key);

	virtual boost::any getValue(std::string key);
	virtual std::string getStringValue(std::string key);
	virtual int getIntValue(std::string key);
	virtual double getDoubleValue(std::string key);
	virtual std::string toString();

	virtual std::list<std::string> getKeys();

	virtual void assignObject(BliObject *another);

protected:
	int getRandomInt(int limit_low, int limit_max);
	double getRandomDouble(double limit_low, double limit_max);

	bool changeIntValue(std::string key, int diff);
	std::map<std::string, boost::any> values;

private:
};

}

#endif
