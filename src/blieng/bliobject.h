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
	bool changeIntValue(std::string key, int diff);

	virtual boost::any getValue(std::string key);
	virtual std::string getStringValue(std::string key);
	virtual bool getBoolValue(std::string key);
	virtual int getIntValue(std::string key);
	virtual unsigned int getUIntValue(std::string key);
	virtual double getDoubleValue(std::string key);
	virtual std::string toString();
	const std::type_info *getValueType(std::string key);

	virtual std::list<std::string> getKeys();

	virtual void assignObject(BliObject *another);

	static int getRandomInt(int limit_low, int limit_max);
	static double getRandomDouble(double limit_low, double limit_max);

protected:
	std::map<std::string, boost::any> values;

private:
};

}

#endif
