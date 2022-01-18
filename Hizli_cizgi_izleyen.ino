
#include <QTRSensors.h>
// 130 255 buradaki ayar robotun sıfır hata ile giderken gittiği hızı gösterir.
//Pistteki virajlar arttıkça 80 e kadar düşürülebilir. En yüksek verilebilecek değer 255 tir.

//****************************************************
int sagmotor1 = 9;
int sagmotor1_2 = 10;
int sagmotor2 = 11 ;

int solmotor1 = 7;
int solmotor1_2 = 6;
int solmotor2 = 5;

int stby = 8;

int sag90_0 = B01111111;
int sag90_1 = B00111111;
int sag90_2 = B00011111;
int sag90_3 = B00001111;

int sol90_0 = B11111110;
int sol90_1 = B11111100;
int sol90_2 = B11111000;
int sol90_3 = B11110000;

int fullbeyaz = B11111111;
//****************************************************

#define MZ80 12//3-4

int se = 100;
int Buzzer = 13;
volatile int      sensor_flag = B00011000;
//unsigned long  zaman1=0, zaman2=0;
int zemin = 1;
QTRSensorsAnalog qtra((unsigned char[]) {  A7, A6, A5, A4, A3 , A2, A1, A0} , 8);
unsigned int sensors[8];
int sonhata = 0;
float Kp = 0.1;   //0.05---0.13   Kp oturmuş değeri---- 0.10
float Kd = 0.95;    //0.7---1.2    Kd oturmuş değeri---- 0.95
float Ki = 0.00001;  //0.001       Ki oturmuş değeri---- 0.00001



int tabanhiz = 150;
int sagmotorpwm = 0;
int solmotorpwm = 0;
int integral = 0;
int hata = 0;
int i;
int oldzaman;
int zaman;
int sayac=0;
int donuszaman=0;
int daire=70;
int cizgi=40;
int hizzaman;
int hizbit=0;
void setup()
{
  Serial.begin(9600);
  Serial.println("Robot Calisti!!");

  pinMode(sagmotor1, OUTPUT);
  pinMode(sagmotor1_2, OUTPUT);
  pinMode(sagmotor2, OUTPUT);
  pinMode(solmotor1, OUTPUT);
  pinMode(solmotor1_2, OUTPUT);
  pinMode(solmotor2, OUTPUT);
  pinMode(stby, OUTPUT);

  pinMode(Buzzer, OUTPUT);
  pinMode(2, OUTPUT),   pinMode(3, OUTPUT),  pinMode(4, OUTPUT);
  pinMode(MZ80, INPUT);

  //**** kalibrasyon kodları kalibrasyon arduino üzerindeki led yanık oldugu sürece devam eder robotun çizgi sensörünü tam ortalayarak  yerleştirilmelidir
  for (int i = 0; i < 120; i++)
  {
    qtra.calibrate();

    for (int j = 14; j < 22; j++)
    {
      int z = analogRead(j);
      switch (j) 
      {
        case 14:     sensors[7] = z;     break;
        case 15:     sensors[6] = z;     break;
        case 16:     sensors[5] = z;     break;
        case 17:     sensors[4] = z;     break;
        case 18:     sensors[3] = z;     break;
        case 19:     sensors[2] = z;     break;
        case 20:     sensors[1] = z;     break;
        case 21:     sensors[0] = z;     break;
      }
    }

    sensor_flag_read();//ANALoG BİLGİYİ DİJİTALE ÇEVİR
    shift_gonder(sensor_flag);//BİLGİYİ LEDLERE GÖNDER
  }


  // digitalWrite(Buzzer, HIGH);  delay(500);   digitalWrite(Buzzer, LOW);delay(500);
  digitalWrite(Buzzer, HIGH);  delay(500);   digitalWrite(Buzzer, LOW); delay(500);
  digitalWrite(stby, 1);

  while (digitalRead(MZ80) == 0) {}
}

//*******************************************************************************************
void loop()
{
  sensor_flag_read();
  shift_gonder(sensor_flag);



            
            if (sensor_flag == B11111100 or sensor_flag == B11111000 or sensor_flag == B11110000 or sensor_flag == B11111111) 
                      { sayac++;
                        donuszaman=1;
                        oldzaman = millis();
                       
                      
                        switch(sayac)//dönüşlerde dairesel ve açısal hız değerleri
                          {
                            case 1:  frenle(); delay(30);  tabanhiz = daire;    break;  //dairesel
                            case 2:  frenle(); delay(30);  tabanhiz = daire;    break;  //dairesel
                            case 3:  frenle(); delay(30);  tabanhiz = cizgi;    break;  //çizgisel  
                                    case 4:    tabanhiz = 140;  donuszaman=0; delay(20);  break;
                                    case 5:    tabanhiz = 140;  donuszaman=0; delay(20);   break;
                                    case 6:    tabanhiz = 140;  donuszaman=0;  delay(20);  break;
                                    case 7:    tabanhiz = 140;  donuszaman=0;  delay(20);  break;
                                    
                            case 8:  frenle(); delay(30);  tabanhiz = 50 ;  /* buz();*/  break;  
                            case 9:  frenle(); delay(30);  tabanhiz = 50;    break;  //dairesel
                            case 10:  frenle(); delay(30);  tabanhiz = 60;    break;  //çizgisel 
                            case 11:  frenle(); delay(30);  tabanhiz = cizgi;    break;  //çizgisel  
                                    case 12:    tabanhiz = 140;  donuszaman=0; delay(20);   break;
                                    case 13:    tabanhiz = 140;  donuszaman=0; delay(20);   break;
                                    case 14:    tabanhiz = 140;  donuszaman=0;  delay(20);  break;
                                    case 15:    tabanhiz = 140;  donuszaman=0; delay(20);   break;
                           case 16: frenle(); delay(30);   tabanhiz = 60;    break;  //çizgisel       

                           case 17: frenle();delay(5000);sayac=0; break;
                          }  
                                         
                      
                      }

   
   
//*************************************************************************************************************
          if(donuszaman==1) 
              {   zaman = millis();   
                  if ( zaman - oldzaman > 800) 
                        {
                           hizzaman=millis();
                           hizbit=1;
                          //tabanhiz = 140;
                          donuszaman=0;}            
                            
        
              }

              
              
//********************************************************************************************************
                if(hizbit==1)
                {
                       switch(sayac)
                       {
                        case 1: tabanhiz = 250;if (millis()- hizzaman >450)   { tabanhiz = 100 ;hizbit=0;}    break;
                        case 2: tabanhiz = 250;if (millis()- hizzaman >900)   { tabanhiz = 70  ;hizbit=0;}    break;
                        case 3: tabanhiz = 250;if (millis()- hizzaman >100)   { tabanhiz = 100 ;hizbit=0;}    break;
                        case 8: tabanhiz = 200;if (millis()- hizzaman >300)   { tabanhiz = 110 ;hizbit=0;}    break;
                        case 9: tabanhiz = 200;if (millis()- hizzaman >350)   { tabanhiz = 110 ;hizbit=0;}    break;
                        case 10: tabanhiz = 200;if (millis()- hizzaman >300)  { tabanhiz = 100;hizbit=0;}    break;
                        case 11: tabanhiz = 250;if (millis()- hizzaman >150)  { tabanhiz = 50;hizbit=0;}    break;
                        case 16: tabanhiz = 250;    break;
                        case 17: frenle();delay(5000);    break;
                       }
                }

//***********************************************************************************************


  unsigned int position = qtra.readLine(sensors, 1, zemin);
  hata = position - 3500;

  int duzeltmehizi = Kp * hata + Kd * (hata - sonhata); //+ Ki*integral;
  sonhata = hata;

  //************* Motorlara uygulanacak kesin hız ayarları****
  solmotorpwm = tabanhiz + duzeltmehizi   ;
  sagmotorpwm = tabanhiz - duzeltmehizi  ;  //solmotorpwm = tabanhiz - duzeltmehizi + ekhiz ;
  //********************


  /// motorlara hız ayarlarının uygulanması********
  sagmotorpwm = constrain(sagmotorpwm, -250, 250); // -254,254 İDİ Burada motorlara uygulanacak PWM değerlerine sınırlandırma getirilmiştir.
  solmotorpwm = constrain(solmotorpwm, -250, 250);
  motorkontrol(solmotorpwm, sagmotorpwm);

  //**************
}

void buz()
{
   digitalWrite(Buzzer, HIGH);  delay(25);   digitalWrite(Buzzer, LOW);
}

void sensor_flag_read()
{




  if ( sensors[7] >= se ) bitClear(sensor_flag , 7); else if ( sensors[7] < se ) bitSet(sensor_flag , 7);
  if ( sensors[6] >= se ) bitClear(sensor_flag , 6); else if ( sensors[6] < se ) bitSet(sensor_flag , 6);
  if ( sensors[5] >= se ) bitClear(sensor_flag , 5); else if ( sensors[5] < se ) bitSet(sensor_flag , 5);
  if ( sensors[4] >= se ) bitClear(sensor_flag , 4); else if ( sensors[4] < se ) bitSet(sensor_flag , 4);
  if ( sensors[3] >= se ) bitClear(sensor_flag , 3); else if ( sensors[3] < se ) bitSet(sensor_flag , 3);
  if ( sensors[2] >= se ) bitClear(sensor_flag , 2); else if ( sensors[2] < se ) bitSet(sensor_flag , 2);
  if ( sensors[1] >= se ) bitClear(sensor_flag , 1); else if ( sensors[1] < se ) bitSet(sensor_flag , 1);
  if ( sensors[0] >= se ) bitClear(sensor_flag , 0); else if ( sensors[0] < se ) bitSet(sensor_flag , 0);
}
void shift_gonder(int numberToDisplay) {

  digitalWrite(3, LOW);
  shiftOut(4, 2, MSBFIRST, numberToDisplay);
  digitalWrite(3, HIGH);
}

void shiftOut(int myDataPin, int myClockPin, byte myDataOut) {
  int i = 0;
  int pinState;

  digitalWrite(myDataPin, 0);
  digitalWrite(myClockPin, 0);

  for (i = 7; i >= 0; i--)  {
    digitalWrite(myClockPin, 0);

    if ( myDataOut & (1 << i) ) {
      pinState = 1;
    }
    else {
      pinState = 0;
    }
    digitalWrite(myDataPin, pinState);
    digitalWrite(myClockPin, 1);
    digitalWrite(myDataPin, 0);
  }
  digitalWrite(myClockPin, 0);
}
