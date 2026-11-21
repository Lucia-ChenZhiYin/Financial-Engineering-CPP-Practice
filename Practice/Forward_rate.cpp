#include<stdio.h>
#include<math.h>
int main(){
    double ZeroRate[8],Forward[9][9];
    for(int i=0;i<8;i++){
        printf("輸入第%d期零息利率:",i+1);
        scanf("%lf",&ZeroRate[i]);
    }
    for(int i=0;i<8;i++){
        Forward[0][i+1]=ZeroRate[i];
    }
    for(int i=0;i<8;i++){
        Forward[i][i]=0;
    }
    for(int a=0;a<8;a++){
        for(int b=a+1;b<=8;b++){
            Forward[a][b]=2*(pow(pow(1+Forward[0][b]/2,b)/pow(1+Forward[0][a]/2,a),1.0/(b-a))-1);
        }
    }
    printf("f(0,1)=%.4lf\n",ZeroRate[0]*100);
    for(int i=0;i<8;i++){
        printf("f(%d,%d)=%lf\n",i,i+1,Forward[i][i+1]*100);
    }
    return 0;
}