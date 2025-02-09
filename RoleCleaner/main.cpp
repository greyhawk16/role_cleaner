#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>
#include "AwsIamRole.h"
using namespace::std;
using json = nlohmann::json;


string ExecuteCmd(const string cmd)
{
    string res;
    FILE* pipe = _popen(cmd.c_str(), "r");
    vector<char> buffer(4096);

    if (!pipe) {
        res = "Invalid Command";
        return res;
    }

    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe) != nullptr) {
        res.append(buffer.data());
    }

    _pclose(pipe);
    return res;
}


vector<AwsIamRole> ParseIamRoles(const string& jsonStr) {
    vector<AwsIamRole> roles;

    try {
        json jsonData = json::parse(jsonStr);

        if (!jsonData.contains("Roles") || !jsonData["Roles"].is_array()) {
            throw runtime_error("Invalid JSON format: 'Roles' array not found");
        }

        for (const auto& roleJson : jsonData["Roles"]) {
            AwsIamRole role;

            role.path = roleJson.value("Path", "");
            role.roleName = roleJson.value("RoleName", "");
            role.roleId = roleJson.value("RoleId", "");
            role.arn = roleJson.value("Arn", "");
            // cout << roleJson.value("CreateDate", "") << endl;
            role.createDate = AwsIamRole::parseIso8601(roleJson.value("CreateDate", "")); // parseIso8601: "2023-11-23T03:12:07+00:00" 형식에 맞게 계산하도록 수정 필요
            role.description = roleJson.value("Description", "");
            role.maxSessionDuration = roleJson.value("MaxSessionDuration", 3600);

            if (roleJson.contains("AssumeRolePolicyDocument") && roleJson["AssumeRolePolicyDocument"].is_object()) {
                role.assumeRolePolicy.version = roleJson["AssumeRolePolicyDocument"].value("Version", "");

                if (roleJson["AssumeRolePolicyDocument"].contains("Statement") && roleJson["AssumeRolePolicyDocument"]["Statement"].is_array()) {
                    for (const auto& stmtJson : roleJson["AssumeRolePolicyDocument"]["Statement"]) {
                        Statement stmt;
                        stmt.effect = stmtJson.value("Effect", "");
                        stmt.action = stmtJson.value("Action", "");

                        if (stmtJson.contains("Principal") && stmtJson["Principal"].is_object()) {
                            stmt.principal.service = stmtJson["Principal"].value("Service", "");
                        }

                        role.assumeRolePolicy.statement.push_back(stmt);
                    }
                }
            }
            roles.push_back(role);
        }
    }
    catch (const std::exception& e) {
        cerr << "Error parsing IAM roles JSON: " << e.what() << endl;
    }

    return roles;
}


int main()
{
    while (true) {
        cout << "Enter argument -> ";

        string arg1;
        cin >> arg1;

        if (arg1 == "quit") {
            // 프로그램 종료
            break;
        }
        else if (arg1 == "roles") {
            string cmd = "aws iam list-roles";
            string res = ExecuteCmd(cmd);
            vector<AwsIamRole> roles = ParseIamRoles(res);

            cout << "Role Name        " << "isAwsCreated        " << "One year old        " << "Number of entities        " << endl;

            for (const auto& role : roles) {
                cout <<  role.roleName << "    " << role.isUserCreated() << "    " << role.isOlderThanOneYear() << "    " << role.getPrincipals().size() << endl;
                // 문자열을 표처럼 정렬해주는 함수 적용
            }
        }
        else {
            // 기능 수행

            // 임시 기능: echo 명령 수행
            string cmd = "echo " + arg1;
            string result = ExecuteCmd(cmd);
            cout << result << endl;


            // 기능-1. AWS가 직접 만든 role이 아니고, 사용한 지 1년 이상 지났으며, role을 맡은 존재가 없는 iam-role 조회.
            // 기능-2. 기능-1 에 나온 iam-role 들에 부여된 policy 조회
            // 기능-3. 기능-1에서 걸린 iam-role 제거
        }
    }

    return 0;
}
