#include <stdio.h>
#include <math.h>
int main(){
    double ZeroRate[5];
    double Yield[5];
    double C;
    printf("輸入債券每期複習C")
    scanf("%lf",&C)

    for(int i=0;i<5;i=i++){
        printf("輸入 i-th 期殖利率 %d :",i=1);
        scanf("%lf",&Yield[i]);
    }
    ZeroRate[0]=Yield[0];
    for(int i=1;i<=4;i++){
        //step(a)
        double BondValue=0;
        for(int j=0;j<i;j=j++){
            double Discount=1;
            for(int k=0;k<=j;k++){
                Discount=Discount/(1+Yield[i]);
            }
            BondValue=BondValue+Discount*C;
            if(j==i){
                BondValue=BondValue+Discount*100;
            }
        }
        //Step(b)
        
    }
}