#include "AwsIamRole.h"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <ctime>


time_t AwsIamRole::parseIso8601(const string& iso8601) {
	std::tm timeStamp = {};
	std::istringstream ss(iso8601);
	
	if (ss.fail()) {
		return -1;
	}

	return std::mktime(&timeStamp);
}


bool AwsIamRole::isOlderThanOneYear() const {
	using namespace::chrono;

	std::time_t cur = system_clock::to_time_t(system_clock::now());
	std::time_t afterCreation = createDate + (365 * 24 * 60 * 60);

	return cur > afterCreation;
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
	return roleName.find("AWSServiceRoleFor") != 0;
}