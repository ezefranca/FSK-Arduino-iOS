#include <SoftModem.h>
SoftModem modem;
 
void setup()
{
  modem.begin();
  Serial.begin(115200);
}
void loop()
{
  modem.write('C');
  Serial.write('C');
  delay(100);
}