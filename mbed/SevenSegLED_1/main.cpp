
#include "mbed.h"
#include <stdio.h>
#include <string>

//pin
DigitalOut mPin_1( dp1);
DigitalOut mPin_2( dp2);
DigitalOut mPin_4( dp4);
DigitalOut mPin_5( dp5);
DigitalOut mPin_6( dp6);
DigitalOut mPin_9( dp9);
DigitalOut mPin_10(dp10);

DigitalOut mPin_11(dp11);
DigitalOut mPin_13(dp13);
DigitalOut mPin_14(dp14);
DigitalOut mPin_17(dp17);
DigitalOut mPin_18(dp18);
DigitalOut mPin_25(dp25);
DigitalOut mPin_26(dp26);

//Cathod
DigitalOut mDig10[7]={mPin_1, mPin_2 , mPin_4, mPin_5, mPin_6, mPin_9, mPin_10 };
DigitalOut mDig01[7]={mPin_11,mPin_13,mPin_14,mPin_17,mPin_18,mPin_25,mPin_26 };

Serial mPc(USBTX, USBRX);

bool Num_Array[10][8]={
// a b c d e f g dp 
{ 1,1,1,1,1,1,0,0}, //0 :0
{ 0,1,1,0,0,0,0,0}, //1 :1
{ 1,1,0,1,1,0,1,0}, //2 :2
{ 1,1,1,1,0,0,1,0}, //3 :3
{ 0,1,1,0,0,1,1,0}, //4 :4
{ 1,0,1,1,0,1,1,0}, //5 :5
{ 1,0,1,1,1,1,1,0}, //6 :6
{ 1,1,1,0,0,0,0,0}, //7 :7
{ 1,1,1,1,1,1,1,0}, //8 :8
{ 1,1,1,1,0,1,1,0}, //9 :9   
};
int mCathod_10[7]  ={4, 3, 2, 1, 0, 5, 6 };

int mSTAT =0;
int mSTAT_RSV_START=1;
int mSTAT_DISP=3;
string mResponse="";
string mReceive="";

//
void proc_display(int iTemp){
  int iDeg =iTemp;
  int iDeg_10= iDeg / 10;
  int iDeg_1 = iDeg % 10;

  // 10dig
  for(int i=0;i<7; i++){
//printf("i=%d ,d10=%d \n" ,i  ,Num_Array[iDeg_10][i]);
    if(Num_Array[iDeg_10][i]==1){
      mDig10[ mCathod_10[i] ]=0;
    }else{
      mDig10[ mCathod_10[i] ]=1;
    }
  }
  // 1-dig
  for(int i=0;i<7; i++){
    if(Num_Array[iDeg_1][i]==1){
      mDig01[ mCathod_10[i] ]=0;
    }else{
      mDig01[ mCathod_10[i] ]=1;
    }
  }

  wait(1.5);
  // 10dig-off
  for(int i=0;i<7; i++){
      mDig10[ mCathod_10[i] ]=1;
      mDig01[ mCathod_10[i] ]=1;
  }
}
//
void init_proc(){
    for (int i = 0; i < 7; i++) {
        mDig10[i]=1;
        mDig01[i]=1;
    }
}
// 
void proc_uart(){
   if( mPc.readable()) {
        char c= mPc.getc();
        mResponse+= c;
   }else{
       if(mSTAT ==mSTAT_RSV_START){
//printf("mResponse=%s\n" , mResponse.c_str() );                   
           if(mResponse.length() >= 6){
               string sHead=mResponse.substr(0,4);
               if( sHead== "tmp="){
                   mReceive =mResponse.substr(4 );
printf("sTmp=%s\n" , mReceive.c_str() );                   
                   mSTAT =mSTAT_DISP;
               }
               mResponse="";
           }
       }
   }
}

//
int main() {
   mSTAT = mSTAT_RSV_START;
   mPc.baud(9600 );
   mPc.printf("#Start-main \n");    
   init_proc();
   while(1){
       if(mSTAT != mSTAT_DISP){
           proc_uart();
           wait_ms(5);
       }else{
printf("#disp \n" );
           int iNum= atoi( mReceive.c_str());
           proc_display( iNum );
           wait(3);
           mSTAT =mSTAT_RSV_START;
       }
   }
}