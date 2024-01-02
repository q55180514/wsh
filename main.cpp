#include <iostream>
#include"CSingleton.h"
#include"Tmptest.h"
#include <vector>
using namespace std;

int main(){
 std::cout<<"just for test"<<std::endl;
 Singletest::GetInstance();

std::cout<<TmpTest::compare(-1,1)<<std::endl;
vector<int> v1{1,2,3};
vector<int> v2{2,3,4};
std::cout<<TmpTest::compare(v1,v2)<<std::endl;

std::cout<<"just for test2"<<std::endl;
 
 return 0;




}
