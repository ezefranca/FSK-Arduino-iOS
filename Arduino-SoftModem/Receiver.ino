#include <SoftModem.h>
SoftModem modem;
int dat;
 
void setup()
{
  modem.begin();
  Serial.begin(115200);
  pinMode(13,OUTPUT);
}
void loop()
{
  if(modem.available())
  {
    dat = modem.read();
    if(dat == 'C')
    {
      Serial.write('Y');
      digitalWrite(13,HIGH);
    }
    else
    {
      Serial.write('N');
      digitalWrite(13,LOW);     
    }
  }
}