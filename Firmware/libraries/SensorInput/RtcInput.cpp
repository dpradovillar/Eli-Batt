#include <RtcInput.h>

int cs = 34;
//=====================================
int RTC_init(){
      pinMode(cs,OUTPUT); // chip select
      // start the SPI library:
      SPI.begin();
      SPI.setBitOrder(MSBFIRST);
      SPI.setDataMode(SPI_MODE3); // both mode 1 & 3 should work
      //set control register
      digitalWrite(cs, LOW);
      SPI.transfer(0x8E);
      SPI.transfer(0x60); //60= disable Osciallator and Battery SQ wave @1hz, temp compensation, Alarms disabled
      digitalWrite(cs, HIGH);
      delay(10);
}
//=====================================
int SetTimeDate(int d, int mo, int y, int h, int mi, int s){
    int TimeDate [7]={s,mi,h,0,d,mo,y};
    for(int i=0; i<=6;i++){
        if(i==3)
            i++;
        int b= TimeDate[i]/10;
        int a= TimeDate[i]-b*10;
        if(i==2){
            if (b==2)
                b=B00000010;
            else if (b==1)
                b=B00000001;
        }
        TimeDate[i]= a+(b<<4);

        digitalWrite(cs, LOW);
        SPI.transfer(i+0x80);
        SPI.transfer(TimeDate[i]);
        digitalWrite(cs, HIGH);
  }
}
//=====================================
MyDate ReadTimeDate(){
    int TimeDate [7]; //second,minute,hour,null,day,month,year
    for(int i=0; i<=6;i++){
        if(i==3)
            i++;
        digitalWrite(cs, LOW);
        SPI.transfer(i+0x00);
        unsigned int n = SPI.transfer(0x00);
        digitalWrite(cs, HIGH);
        int a=n & B00001111;
        if(i==2){
            int b=(n & B00110000)>>4; //24 hour mode
            if(b==B00000010)
                b=20;
            else if(b==B00000001)
                b=10;
            TimeDate[i]=a+b;
        }
        else if(i==4){
            int b=(n & B00110000)>>4;
            TimeDate[i]=a+b*10;
        }
        else if(i==5){
            int b=(n & B00010000)>>4;
            TimeDate[i]=a+b*10;
        }
        else if(i==6){
            int b=(n & B11110000)>>4;
            TimeDate[i]=a+b*10;
        }
        else{
            int b=(n & B01110000)>>4;
            TimeDate[i]=a+b*10;
            }
    }
    MyDate d;
    d.year = TimeDate[4];
    d.month = TimeDate[5];
    d.day = TimeDate[6];
    d.hour = TimeDate[2];
    d.minute = TimeDate[1];
    d.second = TimeDate[0];

    return d;
}

RtcClock::RtcClock() : m_ok(false) {

}

bool RtcClock::setup(SerialEndpoint *debugEndpoint) {
    d.setup(debugEndpoint);
    RTC_init();
    return (m_ok = true);
}

void RtcClock::adjust(const MyDate &dt) {
    SetTimeDate(11,12,13,14,15,16);
}

MyDate RtcClock::now() {
    return ReadTimeDate();
}

float RtcClock::getTempAsFloat() {
    return 0.0f;
}

int16_t RtcClock::getTempAsWord() {
    return 0;
}

bool RtcClock::isAllSetUp() {
    return m_ok;
}
