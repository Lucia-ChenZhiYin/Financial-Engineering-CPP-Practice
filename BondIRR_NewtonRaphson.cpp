#include<stdio.h>
#include<math.h>
int main(){
    int N;
    double C,BondPV=0,Discount,f_r,X,rk,df_r,initialr=0.01;
    printf("put C :");
    scanf("%lf",&C);
    printf("put $X :");
    scanf("%lf",&X);
    printf("put N-th:");
    scanf("%d",&N);

    f_r=1;
    rk=initialr;
    while (fabs(f_r)>=0.001){
        BondPV=0;
        df_r=0;
        for(int i=1;i<=N;i++){
            Discount=1;
            for(int j=1;j<=i;j++){
                Discount=Discount/(1+rk);
            }
            BondPV=BondPV+Discount*C;
            df_r=df_r-(Discount/(1+rk))*i*C;
            if(i==N){
                BondPV=BondPV+Discount*100;
                df_r=df_r-(Discount/(1+rk))*N*100;
            }
        }
        f_r=BondPV-X;
        rk=rk-(f_r/df_r);
    }
    printf("IRR_Newton=%lf\n",rk);
    return 0;
}