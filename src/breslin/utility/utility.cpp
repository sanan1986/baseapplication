#include "utility.h"
using namespace std;

#include <sstream>
#include <string> // this should be already included in <sstream>

//log
#include "../clientside/tdreamsock/dreamSockLog.h"

//random
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

//time
#include <sys/time.h>

Utility::Utility()
{
}

Utility::~Utility()
{
}

std::string Utility::intToString(int i)
{
        ostringstream ss;
        ss << i;
	std::string s = ss.str();
	return s;
}
