#include "AwsIamRole.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <time.h>
#include <stdexcept>


time_t AwsIamRole::parseIso8601(const string& iso8601) {
	std::tm timeStamp = {};
	std::istringstream ss(iso8601);

	ss >> std::get_time(&timeStamp, "%Y-%m-%dT%H:%M:%S");
	
	if (ss.fail()) {
		return -1;
	}

	size_t offsetPos = iso8601.find_last_of("+-");
	// Time Offset implementation

	if (offsetPos != string::npos && iso8601[offsetPos] != 'T') {
		int offsetHrs = 0;
		int offsetMins = 0;
		char sign = iso8601[offsetPos];

		try {
			offsetHrs = std::stoi(iso8601.substr(offsetPos + 1, 2));
			offsetMins = std::stoi(iso8601.substr(offsetPos + 4, 2));
		}
		catch (const std::invalid_argument& e) {
			return -1;
		}

		time_t parsedTime = std::mktime(&timeStamp);
		if (sign == '-') {
			parsedTime += (offsetHrs * 3600 + offsetMins * 60);
		}
		else if (sign == '+') {
			parsedTime -= (offsetHrs * 3600 +offsetMins * 60);
		}

		return parsedTime;
	}

	return std::mktime(&timeStamp);
}


bool AwsIamRole::isOlderThanOneYear() const {
	using namespace::chrono;

	std::time_t cur = system_clock::to_time_t(system_clock::now());
	std::time_t afterCreation = createDate + (365 * 24 * 60 * 60);

	bool res = cur > afterCreation;

	return res;
}


vector<string> AwsIamRole::getPrincipals() const {
	vector<string> principals;

	for (const auto& stmt : assumeRolePolicy.statement) {
		if (!stmt.principal.service.empty()) {
			principals.push_back(stmt.principal.service);
		}
	}

	return principals;
}


bool AwsIamRole::isUserCreated() const {
	string tgt = "AWSServiceRoleFor";

	if (roleName.rfind(tgt, 0) == 0) {
		return true;
	}
	else {
		return false;
	}
}