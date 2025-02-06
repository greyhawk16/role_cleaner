#pragma once

#ifndef AWS_IAM_ROLE_H
#define AWS_IAM_ROLE_H


#include <string>
#include <vector>
#include <ctime>
#include <chrono>
#include <nlohmann/json.hpp>
using namespace::std;
using json = nlohmann::json;


struct Principal {
	string service;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Principal, service)
};


struct Statement {
	string effect;
	Principal principal;
	string action;
	
	NLOHMANN_DEFINE_TYPE_INTRUSIVE(Statement, effect, principal, action)
};


struct AssumeRolePolicyDocument {
	std::string version;
	std::vector<Statement> statement;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(AssumeRolePolicyDocument, version, statement)
};


class AwsIamRole
{
public:
	string path;
	string roleName;
	string roleId;
	string arn;
	time_t createDate;
	AssumeRolePolicyDocument assumeRolePolicy;
	string description;
	int maxSessionDuration;

	bool isOlderThanOneYear() const;
	vector<string> getPrincipals() const;
	static time_t parseIso8601(const string& iso8601);
	bool isUserCreated() const;

	NLOHMANN_DEFINE_TYPE_INTRUSIVE(AwsIamRole, path, roleName, roleId, arn, createDate, assumeRolePolicy, description, maxSessionDuration)
};

#endif // !AWS_IAM_ROLE_H

