#ifndef RTC_H
#define RTC_H

#include <stdint.h>

// Turn off alarm and timer code for now
// These are currently unimplemented in the simulator
// They may never be if there is an abstraction written 
// for them on the watch which is acessible to gui and event code
#undef RTC_ALARM
#undef RTC_TIMER

enum {
    PCF_TIMEFORMAT_HM,
    PCF_TIMEFORMAT_HMS,
    PCF_TIMEFORMAT_YYYY_MM_DD,
    PCF_TIMEFORMAT_MM_DD_YYYY,
    PCF_TIMEFORMAT_DD_MM_YYYY,
    PCF_TIMEFORMAT_YYYY_MM_DD_H_M_S,
};

class RTC_Date
{
public:
    RTC_Date();
    RTC_Date(const char* date, const char* time);
    RTC_Date(uint16_t year,
        uint8_t month,
        uint8_t day,
        uint8_t hour,
        uint8_t minute,
        uint8_t second
    );
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    bool operator==(RTC_Date d);
private:
    uint8_t StringToUint8(const char* pString);
};

#ifdef RTC_ALARM
class RTC_Alarm
{
public:
    RTC_Alarm(void);
    RTC_Alarm(
        uint8_t minute,
        uint8_t hour,
        uint8_t day,
        uint8_t weekday
    );
    uint8_t minute;
    uint8_t hour;
    uint8_t day;
    uint8_t weekday;
};
#endif //RTC_ALARM

class PCF8563_Class
{
public:
    PCF8563_Class(void);

    void check();

    void setDateTime(uint16_t year,
        uint8_t month,
        uint8_t day,
        uint8_t hour,
        uint8_t minute,
        uint8_t second);

    void setDateTime(RTC_Date date);
    RTC_Date getDateTime();

#ifdef RTC_ALARM
    RTC_Alarm getAlarm();
    void enableAlarm();
    void disableAlarm();
    bool alarmActive();
    void resetAlarm();
    void setAlarm(RTC_Alarm alarm);
    void setAlarm(uint8_t hour, uint8_t minute, uint8_t day, uint8_t weekday);
#endif //RTC_ALARM

    bool isValid();

#ifdef RTC_ALARM
    void setAlarmByWeekDay(uint8_t weekday);
    void setAlarmByHours(uint8_t hour);
    void setAlarmByDays(uint8_t day);
    void setAlarmByMinutes(uint8_t minute);
#endif //RTC_ALARM

#ifdef RTC_TIMER
    bool isTimerEnable();
    bool isTimerActive();
    void enableTimer();
    void disableTimer();
    void setTimer(uint8_t val, uint8_t freq, bool enIntrrupt);
    void clearTimer();
#endif RTC_TIMER

# if 0 // CLK functionality
    bool enableCLK(uint8_t freq);
    void disableCLK();
#endif 

    void syncToSystem();
    void syncToRtc();

    const char* formatDateTime(uint8_t sytle = PCF_TIMEFORMAT_HMS);
    uint32_t getDayOfWeek(uint32_t day, uint32_t month, uint32_t year);
#if 0 // Hardware status?
    uint8_t status2();
#endif

private:
    int offset_sec;
    char format[128];

};
#endif //RTC_H