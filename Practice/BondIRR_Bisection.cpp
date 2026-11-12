#include<stdio.h>
int main(){
    float C,X,Discount,f_r;
    float Low=0,High=1;
    int N;
    printf("$C:");
    scanf("%f",&C);
    printf("$X:");
    scanf("%f",&X);
    printf("N-th:");
    scanf("%d",&N);
    while (High-Low>=0.0001){
        float middle=(High+Low)/2,BondPV=0;
        for(int i=1;i<=N;i++){
            Discount=1;
            for(int j=1;j<=i;j++){
                Discount=Discount/(1+middle);
            }
            BondPV=BondPV+Discount*C;
            if(i==N){
                BondPV=BondPV+Discount*100;
            }
        }
        f_r=BondPV-X;
        if(f_r>0){
            Low=middle;
        }
        else{
            High=middle;
        }
    }
    printf("IRR_Bisection=%8.4f",High);
    return 0;

}