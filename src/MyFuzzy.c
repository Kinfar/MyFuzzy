/*
 ============================================================================
 Name        : MyFuzzy.c
 Author      : MoMo
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
double degree[5];
double odegree[5];
/*
 * 产生一个随机数
 */
float randf(){
	return ((float)(rand()%1001))*(0.001f);
}
int getLqi(int power){
	int mid = 50+2*power-5;
	int a,b;
	if(randf()>0.1){
		a = mid-5;
		b = mid+5;
	}
	else{
		if(randf()>0.5){
			a = 50;
			b = 60;
		}
		else{
			a = mid+5;
			b = 110;
		}
	}
	return rand()%(b-a)+a;
}
double getLi(int a,int b,int lqi)
{
    int  c = (a+b)/2;
    double result;
    if(lqi<c){
    	result = ((double)(lqi-a))/((double)(c-a));
    }
    if(lqi>=c){
    	result = ((double)(b-lqi))/((double)(b-c));
    }
    return result;
}
void initialDegree()
{
    int i = 0;
	for(i=0;i<5;i++)
    {
        degree[i] = 0;
        odegree[i] = 0;
    }
}
int fuzzy(int lqi,int id)
{
//    bool *list = new bool[5];
    int res = 0;
    if(lqi>=50&&lqi<=70){
        res += 1;
        if(id)
        	odegree[0] = ((double)(70-lqi))/((double)20);
        else
        	degree[0] = ((double)(70-lqi))/((double)20);
    }
    if(lqi>=60&&lqi<=80){
        res += 2;
        if(id)
            odegree[1] = getLi(60,80,lqi);
        else
            degree[1] = getLi(60,80,lqi);
    }
    if(lqi>=70&&lqi<=90){
        res += 4;
        if(id)
            odegree[2] = getLi(70,90,lqi);
        else
            degree[2] = getLi(70,90,lqi);
    }
    if(lqi>=80&&lqi<=100){
        res += 8;
        if(id)
            odegree[3] = getLi(80,100,lqi);
        else
            degree[3] = getLi(80,100,lqi);
    }
    if(lqi>=90&&lqi<=110){
        res += 16;
        if(id)
        	odegree[4] = ((double)(lqi-90))/((double)20);
        else
        	degree[4] = ((double)(lqi-90))/((double)20);
    }
    return res;
}
int countPower(int num){
	int count = 0;
	while(num!=1){
		num = num/2;
		count++;
	}
	return count;
}
int fuzzyChangePower(int lqi,int oldlqi,int power)
{
    initialDegree();
    int list = fuzzy(lqi,0);
    int olist = fuzzy(oldlqi,1);
    int k=0;
    for(;k<5;k++)
    	printf("degree[%d]=%lf.",k,degree[k]);
    puts("");
    //得到之前lqi的模糊值和当前lqi的模糊值
    int *x = malloc(sizeof(int)*2);
    x[0] = 0;
    x[1] = 0;
    int index = 0;
    while(list!=0)
    {
        if(list>=16)
        {
            list -= 16;
            x[index++] = 16;
            continue;
        }
        if(list>=8)
        {
            list -= 8;
            x[index++] = 8;
            continue;
        }
        if(list>=4)
        {
          list -= 4;
          x[index++] = 4;
          continue;
        }
        if(list>=2)
        {
          list -= 2;
          x[index++] = 2;
          continue;
        }
        if(list>=1)
        {
          list -= 1;
          x[index++] = 1;
          continue;
        }
    }
    int *y = malloc(sizeof(int)*2);
    y[0] = 0;
    y[1] = 0;
    index = 0;
    while(olist!=0)
    {
        if(olist>=16)
        {
            olist -= 16;
            y[index++] = 16;
            continue;
        }
        if(olist>=8)
        {
            olist -= 8;
            y[index++] = 8;
            continue;
        }
        if(olist>=4)
        {
          olist -= 4;
          y[index++] = 4;
          continue;
        }
        if(olist>=2)
        {
          olist -= 2;
          y[index++] = 2;
          continue;
        }
        if(olist>=1)
        {
          olist -= 1;
          y[index++] = 1;
          continue;
        }
    }
    int  flag = 1;
    double BStep=0;
    double SStep=0;
    double TStep=0;
    double sum = 0;
    int j;
    for(j=0;j<2;j++){
        if(x[j]==16)
        {
            BStep += -degree[4]*10;
            sum += degree[4];
            flag = 0;
        }
        else if(x[j]==1)
        {
            BStep += degree[0]*10;
            sum += degree[0];
        }
        else if(x[j]==4){
        //当前的lqi为完美的情况，就不用调节了。
            sum += degree[2];
        }
        else if(x[j]==8)
        {
            if(y[0]!=0){
                double min = degree[3]<odegree[countPower(y[0])]?degree[3]:odegree[countPower(y[0])];
                if(y[0]<8){
                    TStep += min*2;
                }
                else{
                    SStep += -min*5;
                }
                sum += min;
            }
            if(y[1]!=0){
                double min = degree[3]<odegree[countPower(y[1])]?degree[3]:odegree[countPower(y[1])];
                if(y[1]<8){
                    TStep += min*2;
                }
                else{
                    SStep += min*5;
                }
                sum += min;
            }
        }
        else if(x[j]==2){
            if(y[0]!=0){
                double min = degree[1]<odegree[countPower(y[0])]?degree[1]:odegree[countPower(y[0])];
                if(y[0]<4)
                {
                    SStep += min*5;
                }
                else
                {
                    TStep += min*2;
                }
                sum += min;
            }
            if(y[1]!=0)
            {
                double min = degree[1]<odegree[countPower(y[1])]?degree[1]:odegree[countPower(y[1])];
                if(y[1]<4)
                    SStep += min*5;
                else
                    TStep += min*2;
                sum += min;
            }
        }
    }
    int adj = 0;
    printf("BStep = %lf\n",BStep);
    printf("SStep = %lf\n",SStep);
    printf("TStep = %lf\n",TStep);
    if(sum!=0)
        adj = (BStep+SStep+TStep)/sum;
    printf("sum = %lf\n",sum);
    printf("adj = %lf\n",adj);

    return power+adj;
}
int main(void) {
//	int newpower = fuzzyChangePower(95,110,25);
//	printf("after fuzzy control new power is%d\n",newpower);
	int i = 1;
	for(;i<31;i++){
		printf("功率为%d产生LQI为%d\n",i,getLqi(i));
	}
	return EXIT_SUCCESS;
}
