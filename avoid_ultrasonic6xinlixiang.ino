//#include <Servo.h> 
#include <LiquidCrystal.h> //申明1602液晶的函数库
//申明1602液晶的引脚所连接的Arduino数字端口，8线或4线数据模式，任选其一
//LiquidCrystal lcd(12,11,10,9,8,7,6,5,4,3,2);   //8数据口模式连线声明
LiquidCrystal lcd(13,12,7,6,5,4,3); //4数据口模式连线声明 

int Echo = A1;  // Echo回声脚(P2.0)
int Trig =A0;  //  Trig 触发脚(P2.1)

int Distance = 0;
String xy = "";
int s=0;

int a=0;int b=0;
int k=0,x=0,y=0;

int Left_motor_back=8;     //左电机后退(IN1)
int Left_motor_go=9;     //左电机前进(IN2)

int Right_motor_go=10;    // 右电机前进(IN3)
int Right_motor_back=11;    // 右电机后退(IN4)

//int key=A2;//定义按键 A2 接口
//int beep=A3;//定义蜂鸣器 A3 接口

//const int SensorRight = 3;   	//右循迹红外传感器(P3.2 OUT1)
//const int SensorLeft = 4;     	//左循迹红外传感器(P3.3 OUT2)

//const int SensorRight_2 = 6;   	//右红外传感器(P3.5 OUT4)
//const int SensorLeft_2 = 5;     //左红外传感器(P3.4 OUT3)

//int SL;    //左循迹红外传感器状态
//int SR;    //右循迹红外传感器状态
//int SL_2;    //左红外传感器状态
//int SR_2;    //右红外传感器状态

void setup()
{
  Serial.begin(9600);     // 初始化串口
  //初始化电机驱动IO为输出方式
  pinMode(Left_motor_go,OUTPUT); // PIN 8 (PWM)
  pinMode(Left_motor_back,OUTPUT); // PIN 9 (PWM)
  pinMode(Right_motor_go,OUTPUT);// PIN 10 (PWM) 
  pinMode(Right_motor_back,OUTPUT);// PIN 11 (PWM)
  //pinMode(key,INPUT);//定义按键接口为输入接口
 // pinMode(beep,OUTPUT);
  // pinMode(SensorRight, INPUT); //定义右循迹红外传感器为输入
  // pinMode(SensorLeft, INPUT); //定义左循迹红外传感器为输入
  //pinMode(SensorRight_2, INPUT); //定义右红外传感器为输入
  //pinMode(SensorLeft_2, INPUT); //定义左红外传感器为输入
  //初始化超声波引脚
  pinMode(Echo, INPUT);    // 定义超声波输入脚
  pinMode(Trig, OUTPUT);   // 定义超声波输出脚
  lcd.begin(16,2);      //初始化1602液晶工作                       模式
  //定义1602液晶显示范围为2行16列字符  
}

String readTtl() {
  String comdata = "";
  while (Serial.available())
  {
    comdata += char(Serial.read());
    delay(2);
  }
  return comdata;
  //comdata说是一个字符串，也是一个数组，引用每个字的话可以用comdata[0]，comdata[1]。。。comdata[n]。如果我们要每个字节取出的话，可以每个引用。
}

//=======================智能小车的基本动作=========================
//void run(int time)     // 前进
void run(int s)     // 前进
{
  digitalWrite(Right_motor_go,HIGH);  // 右电机前进
  digitalWrite(Right_motor_back,LOW);     
  analogWrite(Right_motor_go,s*0.225);//PWM比例0~255调速，左右轮差异略增减
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,HIGH);  // 左电机前进
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,s*0.225);//PWM比例0~255调速，左右轮差异略增减
  analogWrite(Left_motor_back,0);
  //delay(time * 100);   //执行时间，可以调整  
}

void brake()  //刹车，停车
{
  digitalWrite(Right_motor_go,LOW);
  digitalWrite(Right_motor_back,LOW);
  digitalWrite(Left_motor_go,LOW);
  digitalWrite(Left_motor_back,LOW);
  //delay(time * 100);//执行时间，可以调整  
}

//void left(int time)         //左转(左轮不动，右轮前进)
void runleft(int s,int m)         //前行左转(左轮低速，右轮前进)
{
  digitalWrite(Right_motor_go,HIGH);  // 右电机前进
  digitalWrite(Right_motor_back,LOW);  
   
  analogWrite(Right_motor_go,s*0.225-m*0.225);//PWM比例0~255调速，左右轮差异略增减
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,HIGH);  // 左电机前进
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,s*0.22);//PWM比例0~255调速，左右轮差异略增减
  analogWrite(Left_motor_back,0);
  //delay(time * 100);   //执行时间，可以调整  
}

void left()         //左转(左轮不动，右轮前进)
{
  digitalWrite(Right_motor_go,HIGH);	// 右电机前进
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,100); 
  analogWrite(Right_motor_back,0);//PWM比例0~255调速
  digitalWrite(Left_motor_go,LOW);   //左轮后退
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,0);//PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整  
}


void spin_left()         //左转(左轮后退，右轮前进)
{
  digitalWrite(Right_motor_go,HIGH);	// 右电机前进
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,110); 
  analogWrite(Right_motor_back,0);//PWM比例0~255调速
  digitalWrite(Left_motor_go,LOW);   //左轮后退
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0); 
  analogWrite(Left_motor_back,110);//PWM比例0~255调速
  //delay(time * 100);	//执行时间，可以调整  
}

void runright(int s,int m)
//void right()        //前行右转(右轮低速，左轮前进)
{
   digitalWrite(Right_motor_go,HIGH);  // 右电机前进
  digitalWrite(Right_motor_back,LOW);     
  analogWrite(Right_motor_go,s*0.225);//PWM比例0~255调速，左右轮差异略增减
  analogWrite(Right_motor_back,0);
  digitalWrite(Left_motor_go,HIGH);  // 左电机前进
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,s*0.225+m*0.225);//PWM比例0~255调速，左右轮差异略增减
  analogWrite(Left_motor_back,0);
  //delay(time * 100);   //执行时间，可以调整  
}


void right(int time)
//void right()        //右转(右轮不动，左轮前进)
{
  digitalWrite(Right_motor_go,LOW);   //右电机后退
  digitalWrite(Right_motor_back,LOW);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,0);//PWM比例0~255调速
  digitalWrite(Left_motor_go,HIGH);//左电机前进
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,100); 
  analogWrite(Left_motor_back,0);//PWM比例0~255调速
  delay(time * 100);	//执行时间，可以调整  
}

void spin_right()        //右转(右轮后退，左轮前进)
{
  digitalWrite(Right_motor_go,LOW);   //右电机后退
  digitalWrite(Right_motor_back,HIGH);
  analogWrite(Right_motor_go,0); 
  analogWrite(Right_motor_back,110);//PWM比例0~255调速
  digitalWrite(Left_motor_go,HIGH);//左电机前进
  digitalWrite(Left_motor_back,LOW);
  analogWrite(Left_motor_go,110); 
  analogWrite(Left_motor_back,0);//PWM比例0~255调速
 // delay(time * 100);	//执行时间，可以调整    
}

void back(int time)          //后退
{
  digitalWrite(Right_motor_go,LOW);  //右轮后退
  digitalWrite(Right_motor_back,HIGH);
  analogWrite(Right_motor_go,0);
  analogWrite(Right_motor_back,s*50);//PWM比例0~255调速
  digitalWrite(Left_motor_go,LOW);  //左轮后退
  digitalWrite(Left_motor_back,HIGH);
  analogWrite(Left_motor_go,0);
  analogWrite(Left_motor_back,s*50);//PWM比例0~255调速
  delay(time * 100);     //执行时间，可以调整  
}
//==========================================================

void Distance_test()   // 量出前方距离 
{
  digitalWrite(Trig, LOW);   // 给触发脚低电平2μs
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  // 给触发脚高电平10μs，这里至少是10μs
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    // 持续给触发脚低电
  float Fdistance = pulseIn(Echo, HIGH);  // 读取高电平时间(单位：微秒)
  Fdistance= Fdistance/58;       //为什么除以58等于厘米，  Y米=（X秒*344）/2
  // X秒=（ 2*Y米）/344 ==》X秒=0.0058*Y米 ==》厘米=微秒/58
  //Serial.print("Distance:");      //输出距离（单位：厘米）
  //Serial.println(Fdistance);         //显示距离
  Distance = Fdistance;
}  

void Distance_display()//液晶屏显示距离和位置
{ 
  if((2<Distance)&(Distance<400))//超声波测距范围2cm到400cm
  {
    lcd.home();        //把光标移回左上角，即从头开始输出   
    lcd.print("Distance: "); 
    lcd.print(Distance); 
    lcd.print("cm");    //显示
    lcd.setCursor(2,2);   //把光标定位在第2行，第6列
    while (Serial.available() > 0)
  {
  String s = readTtl();
  lcd.print(s);       //显示距离
  //Serial.println(s); 
  for(int i = 0,x=0; i < s.length() ; i++)
  {
  if(s[i] == ' ')
  {a=x;x=0;}
  else
   x = x * 10 + (s[i] - '0');
   b=x;
  }
    Serial.print(a);Serial.print(',');Serial.println(b);
   
   if(k<9)
  {x=x+a;y=y+b;k++;}
  else
  {x=x+a;y=y+b;x=x/10;y=y/10;
  Serial.print("Average:");Serial.print(x);Serial.print(',');Serial.println(y);
  x=0;y=0;k=0; 
  }
 
   
  }
  }
  else
  {
    lcd.home();        //把光标移回左上角，即从头开始输出  
    lcd.print("!!! Out of range");  
    lcd.setCursor(2,2);   //把光标定位在第2行，第6列
    while (Serial.available() > 0)
  {
  String s = readTtl(); //获取相对位置
  lcd.print(s);       //显示距离
  //Serial.println(s);
  for(int i = 0,x=0; i < s.length() ; i++)
  {
  if(s[i] == ' ')
  {a=x;x=0;}
  else
   x = x * 10 + (s[i] - '0');
   b=x;
  } //获取相对位置
 
   Serial.print(a);Serial.print(',');Serial.println(b);
  if(k<9)
  {x=x+a;y=y+b;k++;}
  else
  {x=x+a;y=y+b;x=x/10;y=y/10;
  Serial.print("Average:");Serial.print(x);Serial.print(',');Serial.println(y);
  x=0;y=0;k=0; 
  }
  }
  }
  delay(250);
  
}

void loop()
{
 // keysacn();	   //调用按键扫描函数
    
    Distance_test();//测量前方距离
    Distance_display();//液晶屏显示距离和位置
   
    if(Distance < 20)//数值为碰到障碍物的距离，可以按实际情况设置
      while(Distance < 20)//再次判断是否有障碍物，若有则转动方向后，继续判断
      {if(a<=100&&a>=-100&&b<=100&&b>=-100)
         brake;
    else if(a<-100)
       spin_left();
    else if(a>100||b>=100||b<=-100)
       spin_right();
        Distance_test();//测量前方距离
        Distance_display();//液晶屏显示距离
      }
    else if(a==0)
       run(b);
    else if(a<0)
       runleft(b,a);
    else if(a>0)
       runright(b,a);
    
  
}










