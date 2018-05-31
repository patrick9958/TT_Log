#include "Arduino.h"
#include "TTLog.h"

TTLog* TTLog::m_pInstance = NULL;

TTLog* TTLog::log()
{
  if(!m_pInstance)    // only allows one instance of class to be generated
  {
    m_pInstance = new TTLog;
  }
  return m_pInstance;
}

void entry(const char *rgMessage, String &sFilename,
           bool bPrintSerial, bool bPrintSDCard)
{
  String sMessage = String(rgMessage);
  sMessage += "\n";
  //printDateTime(sMessage);

  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  File dataFile = SD.open(sFilename, FILE_WRITE);

  // if the file is available, write to it if...
  if (dataFile)
  {
    // an intruder has entered or left...
    if(sMessage != NO_DATA && sMessage != "")
    {
      if(bPrintSDCard)
      {
        dataFile.println(sMessage);
      }
      if(bPrintSerial)
      {
        Serial.println(sMessage);
      }
    }
    // otherwise just close the file
    dataFile.close();
  }

  // if the file isn't open, pop up an error:
  else
  {
    Serial.println("error opening" + String(sFilename));
  }
}


void TTLog::initializeSD(int iCS_pin)
{
  m_iCS_pin = iCS_pin;
  while (!Serial);
  Serial.print("Initializing SD card...");

  // see if the card is present and can be initialized:
  if (!SD.begin(m_iCS_pin))
  {
    Serial.println("Card failed, or not present");
    // don't do anything more:
    return;
  }
  Serial.println("card initialized.");
}


TTLog::TTLog() : m_cs_pin(DEFAULT_CS_PIN) {}


void TTLog::setDateTime(int iHour, int iMinute,
                        int iDay, int iMonth, int iYear)
{
  // user entered time as params
  if(iHour != -1 && iMinute != -1 && iDay != -1 && iMonth != -1 && iYear != -1)
  {
    int iSecond = 0;
    setTime(iHour, iMinute, iSecond, iDay, iMonth, iYear);
    return;
  }

  // otherwise set time through Serial monitor
  // wait for Serial port to connect
  if(!Serial)
  {
    delay(1000);
  }
  if(!Serial)
  {
    return;
  }

  char rg_Input[5];
  Serial.println("Please set the time");
  Serial.println();

  memset(rg_Input, '\0', sizeof(rg_Input));
  Serial.print("Hour: ");
  while(!Serial.available());
  int i = 0;
  while(Serial.available())
  {
    delay(10);
    rg_Input[i] = Serial.read();
    i++;
    if(i > 1)
    {
      break;
    }
  }
  iHour = atoi(rg_Input);
  Serial.println(iHour);

  memset(rg_Input, '\0', sizeof(rg_Input));
  Serial.print("Minute: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available())
  {
    delay(10);
    rg_Input[i] = Serial.read();
    i++;
    if(i > 1)
    {
      break;
    }
  }
  iMinute = atoi(rg_Input);
  Serial.println(iMinute);

  memset(rg_Input, '\0', sizeof(rg_Input));
  Serial.print("Month: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available())
  {
    delay(10);
    rg_Input[i] = Serial.read();
    i++;
    if(i > 1)
    {
      break;
    }
  }
  iMonth = atoi(rg_Input);
  Serial.println(iMonth);

  memset(rg_Input, '\0', sizeof(rg_Input));
  Serial.print("Day: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available())
  {
    delay(10);
    rg_Input[i] = Serial.read();
    i++;
    if(i > 1)
    {
      break;
    }
  }
  iDay = atoi(rg_Input);
  Serial.println(iDay);

  memset(rg_Input, '\0', sizeof(rg_Input));
  Serial.print("Year: ");
  while(!Serial.available());
  i = 0;
  while(Serial.available())
  {
    delay(10);
    rg_Input[i] = Serial.read();
    i++;
    if(i > 3)
    {
      break;
    }
  }

  iYear = atoi(rg_Input);
  Serial.println(iYear);
  int iSecond = 0;

  setTime(iHour, iMinute, iSecond, iDay, iMonth, iYear);

  String sNow;
  printDateTime(sNow);
  Serial.println("It is now: ");
  Serial.println(sNow);
}


void TTLog::printDateTime(String &sDateTime)
{
  printTime(sDateTime);
  printDate(sDateTime);
}

void TTLog::printTime(String &sTime)
{
  String sHour; String sMinute;
  time_t t = now();

  if(hour(t) < 10)
  {
    sHour = "0" + String(hour(t));
  }
  else
  {
    sHour = String(hour(t));
  }

  if(minute(t) < 10)
  {
    sMinute = "0" + String(minute(t));
  }
  else
  {
    sMinute = String(minute(t));
  }

  sTime += sHour + ":" + sMinute + "\n";
}

void TTLog::printDate(String &sDate)
{
  time_t t = now();
  String sMonth = String(monthShortStr(month()));
  sDate += sMonth + " ";
  sDate += String(day(t)) + " ";
  sDate += String(year(t)) + "\n";
}
