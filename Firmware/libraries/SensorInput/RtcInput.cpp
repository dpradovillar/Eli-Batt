#include <RtcInput.h>

MyDate::MyDate() :
    year(0),
    month(0),
    day(0),

    hour(0),
    minute(0),
    second(0)
{
}

void RtcClock::RTC_init(){
      pinMode(m_pin, OUTPUT); // chip select
      // start the SPI library:
      //SPI.begin();
      SPI.setBitOrder(MSBFIRST);
      SPI.setDataMode(SPI_MODE3); // both mode 1 & 3 should work
      //set control register
      digitalWrite(m_pin, LOW);
      SPI.transfer(0x8E);
      SPI.transfer(0x60); //60= disable Osciallator and Battery SQ wave @1hz, temp compensation, Alarms disabled
      digitalWrite(m_pin, HIGH);
      delay(10);
}

void RtcClock::RTC_setTimeDate(const MyDate &d){
    //day(1-31), month(1-12), year(0-99), hour(0-23), minute(0-59), second(0-59)
    int TimeDate [7]={d.second,d.minute,d.hour,0,d.day,d.month,d.year};
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

        digitalWrite(m_pin, LOW);
        SPI.transfer(i+0x80);
        SPI.transfer(TimeDate[i]);
        digitalWrite(m_pin, HIGH);
    }
}
//=====================================
void RtcClock::RTC_readTimeDate(){
    int TimeDate [7]; //second,minute,hour,null,day,month,year
    for(int i=0; i<=6;i++){
        if(i==3)
            i++;
        digitalWrite(m_pin, LOW);
        SPI.transfer(i+0x00);
        unsigned int n = SPI.transfer(0x00);
        digitalWrite(m_pin, HIGH);
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
    m_lastDate.year = 2000 + TimeDate[6];
    m_lastDate.month = TimeDate[5];
    m_lastDate.day = TimeDate[4];
    m_lastDate.hour = TimeDate[2];
    m_lastDate.minute = TimeDate[1];
    m_lastDate.second = TimeDate[0];
}

RtcClock::RtcClock() : m_ok(false) {

}

bool RtcClock::setup(int pin) {
    m_pin = pin;
    RTC_init();
    return (m_ok = true);
}

void RtcClock::adjust(const MyDate &dt) {
    RTC_setTimeDate(dt);
}

MyDate RtcClock::readDate() {
    RTC_readTimeDate();
    return m_lastDate;
}

MyDate RtcClock::getLastDate() {
    return m_lastDate;
}

bool RtcClock::isAllSetUp() {
    return m_ok;
}
