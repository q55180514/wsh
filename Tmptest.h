/*
test for template 
creted by wsh on 2024-1-2

*/
#ifndef TMPTEST_H_
#define TMPTEST_H_
namespace TmpTest{
template<typename T>
int compare(const T &temp1 , const T &temp2){
    if(temp1<temp2)return -1;
    if(temp1>temp2)return 1;
    return 0;
}

}




#endif
