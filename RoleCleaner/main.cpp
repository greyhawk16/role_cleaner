#include <iostream>
#include <string>
#include <sstream>
#include <memory>
#include <vector>
#include <nlohmann/json.hpp>

using namespace::std;
using json = nlohmann::json;

/*
* iam-role 저장하기 위한 클래스 멤버 변수 별 자료형

Path: string
RoleName: string
RoleId: string
Arn: string
CreateDate: time_t 
AssumeRolePolicyDocument: map {
    Version: string
    Statement: vector<map>
}
Description: string
MaxSessionDuration: int

*/


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
        else if (arg1 == "json") {

            // string -> json 변환 기능 테스트
            std::string jsonString = R"({
                "Path": "/aws-service-role/ops.apigateway.amazonaws.com/",
                "RoleName": "AWSServiceRoleForAPIGateway",
                "RoleId": "AROA5ZBTTCRBLMSWQZSED",
                "Arn": "arn:aws:iam::947148493890:role/aws-service-role/ops.apigateway.amazonaws.com/AWSServiceRoleForAPIGateway",
                "CreateDate": "2023-11-23T03:12:07+00:00",
                "AssumeRolePolicyDocument": {
                    "Version": "2012-10-17",
                    "Effect": "Allow",
                    "Service": "ops.apigateway.amazonaws.com"
                },
                "Description": "The Service Linked Role is used by Amazon API Gateway.",
                "MaxSessionDuration": 3600
            })";

            json JsonData = json::parse(jsonString);
            cout << JsonData << endl;

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
