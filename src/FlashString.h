#ifndef FLASHSTRING_H
#define FLASHSTRING_H

#include "Arduino.h"

class FlashString {
   public:
    FlashString(const char *data);
    String toString();
    String operator[](const uint32_t &index);
    String get(const uint32_t &index);
    String substring(const int &start, const int &end = -1);
    void trim();
    void remove(const int &len, const bool &end = false);
    bool startsWith(const String &str);
    bool endsWith(const String &str);
    int indexOf(const String &str, const int &begin = 0);
    int length();

   private:
    const char *data;
    int startPos;
    int dataLen;
};

#endif
