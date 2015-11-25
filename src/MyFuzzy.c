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
double degree[5];
double odegree[5];
double getLi(int a,int b,int lqi)
{
    int  c = (a+b)/2;
    if(lqi<c)
        return ((double)(lqi-a))/((double)(c-a));
    if(lqi>=c)
        return ((double)(lqi-c))/((double)(b-c));
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
    if(lqi>=50&&lqi<=80){
        res += 1;
        if(id)
            odegree[0] = getLi(50,80,lqi);
        else
            degree[0] = getLi(50,80,lqi);
    }
    if(lqi>=75&&lqi<=87){
        res += 2;
        if(id)
            odegree[1] = getLi(75,87,lqi);
        else
            degree[1] = getLi(75,87,lqi);
    }
    if(lqi>=85&&lqi<=95){
        res += 4;
        if(id)
            odegree[2] = getLi(85,95,lqi);
        else
            degree[2] = getLi(85,95,lqi);
    }
    if(lqi>=93&&lqi<=102){
        res += 8;
        if(id)
            odegree[3] = getLi(93,102,lqi);
        else
            degree[3] = getLi(93,102,lqi);
    }
    if(lqi>=100&&lqi<=110){
        res += 16;
        if(id)
            odegree[4] = getLi(100,110,lqi);
        else
            degree[4] = getLi(100,110,lqi);
    }
    return res;
}
int fuzzyChangePower(int lqi,int oldlqi,int power)
{
    initialDegree();
    int list = fuzzy(lqi,0);
    int olist = fuzzy(oldlqi,1);
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
            BStep += degree[4]*10;
            sum += degree[4];
            flag = 0;
        }
        else if(x[j]==1)
        {
            BStep = degree[0]*10;
            sum += degree[0];
        }
        else if(x[j]==4){
        //当前的lqi为完美的情况，就不用调节了。
            sum += degree[2];
        }
        else if(x[j]==8)
        {
            if(y[0]!=0){
                double min = degree[3]<odegree[y[0]/2]?degree[3]:odegree[y[0]/2];
                if(y[0]<8){
                    TStep += min*2;
                }
                else{
                    SStep += min*5;
                }
                sum += min;
            }
            if(y[1]!=0){
                double min = degree[3]<odegree[y[1]/2]?degree[3]:odegree[y[1]/2];
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
                double min = degree[1]<odegree[y[0]/2]?degree[1]:odegree[y[0]/2];
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
                double min = degree[1]<odegree[y[1]/2]?degree[1]:odegree[y[1]/2];
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
    if(flag)
        power = power+adj;
    else
        power=  power-adj;
    return power;
}
int main(void) {
	int newpower = fuzzyChangePower(70,110,10);
	printf("after fuzzy control new power is%d\n",newpower);
	return EXIT_SUCCESS;
}
