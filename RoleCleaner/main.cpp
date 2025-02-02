#include <iostream>
#include <string>
using namespace::std;


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
        else {
            // 기능 수행
            cout << arg1 << "\n";   // 임시 기능

            // 기능-1. AWS가 직접 만든 role이 아니고, 사용한 지 1년 이상 지났으며, role을 맡은 존재가 없는 iam-role 조회.
            // 기능-2. 기능-1 에 나온 iam-role 들에 부여된 policy 조회
            // 기능-3. 기능-1에서 걸린 iam-role 제거
        }
    }

    return 0;
}
