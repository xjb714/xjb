#include <cmath>
#include <iostream>
#include "d2e_xjb.cpp"

void test_d2e()
{
    double v32[32];
    for(int i=0;i<32;++i) v32[i] = (i+1)*sin(1);
    for(int i=0;i<32;++i){
        char buffer[32];
        memset(buffer,0,32);
        d2e_xjb(v32[i],buffer);
        char buffer_f32[32];
        memset(buffer_f32,0,32);
        f2e_xjb(v32[i],buffer_f32);
        printf("i=%d,%.16le %s %s\n",i,v32[i],buffer,buffer_f32);
    }
    // char buffer_1024[1024];
    // int len = d2sci_32(v32,buffer_1024);
    // printf("len=%d\n%s",len,buffer_1024);
}
void test_d2e_i()
{
    int i = 0;
    while(i++ < 30)
    {
        printf("input a real number:");
        double num;
        scanf("%le",&num);

        char buffer[32];
        memset(buffer,0,32);
        d2e_xjb(num,buffer);
        char buffer_f32[32];
        memset(buffer_f32,0,32);
        f2e_xjb(num,buffer_f32);
        printf("%.16le %s %s\n",num,buffer,buffer_f32);
    }
}
void test_pow10(){
    for(int i=-324;i<=324;++i){
        u64 pow10 = powers_ten_reverse[343 - i];
        double v = _10en[i + 324];
        u64 u = *(u64*)&v;
        u64 u2 = ((u << 11) | (1ull<<63));
        printf("i=%d, u=0x%016lx u2=0x%016lx pow10=0x%016lx\n",i,u,u2,pow10);
    }
}
int main(){
    test_d2e();
    test_d2e_i();
    printf("\n");
    //test_pow10();
    return 0;
}