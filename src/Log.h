#ifndef LOG_H
#define LOG_H

#include "Arduino.h"

class Log {
   public:
    Log() {}
    ~Log() {}
    void attachSerial(HardwareSerial &serial);
    void detachSerial();
    void e(const String &scope, const String &message);
    void w(const String &scope, const String &message);
    void i(const String &scope, const String &message);

   private:
    HardwareSerial *serial;
};

extern Log fb_log;

#endif
