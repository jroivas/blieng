#include "path.h"
#include <boost/foreach.hpp>

std::string blieng::Path::toString() {
	std::string res = "";
	bool first = true;
	BOOST_FOREACH(blieng::Point *pt, points) {
		if (!first) res += ";";
		first = false;
		res += pt->toString();
	}
	return res;
}
