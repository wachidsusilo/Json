#include "FlashString.h"

FlashString::FlashString(const char *data)
    : data(data), startPos(0), dataLen(data ? strlen_P(data) : 0) {}

String FlashString::toString() {
    if (data) {
        char buf[dataLen + 1];
        buf[dataLen] = '\0';
        memcpy_P(buf, data + startPos, dataLen);
        return String(buf);
    }
    return String();
}

String FlashString::operator[](const uint32_t &index) {
    if (data) {
        if (index < dataLen) {
            char buf[2];
            memcpy_P(buf, data + startPos + index, 1);
            buf[1] = '\0';
            return String(buf);
        }
    }
    return String();
}

String FlashString::get(const uint32_t &index) {
    if (data) {
        if (index < dataLen) {
            char buf[2];
            memcpy_P(buf, data + startPos + index, 1);
            buf[1] = '\0';
            return String(buf);
        }
    }
    return String();
}

void FlashString::trim() {
    if (data) {
        char buf[2];
        buf[1] = '\0';
        memcpy_P(buf, data + startPos, 1);
        while (isspace(buf[0])) {
            startPos++;
            dataLen--;
            memcpy_P(buf, data + startPos, 1);
        }

        memcpy_P(buf, data + startPos + dataLen - 1, 1);
        while (isspace(buf[0])) {
            dataLen--;
            memcpy_P(buf, data + startPos + dataLen - 1, 1);
        }
    }
}

void FlashString::remove(const int &len, const bool &end) {
    if (data) {
        if (len >= 0) {
            if (!end) startPos += len;
            dataLen -= len;
        } else {
            startPos += dataLen;
            dataLen = 0;
        }
    }
}

String FlashString::substring(const int &start, const int &end) {
    if (data) {
        int len;
        if (end > 0 && end <= dataLen) {
            len = end - start;
        } else {
            if (start >= 0 && start < dataLen - 1) {
                len = dataLen - start;
            } else {
                len = dataLen;
            }
        }
        char buf[len + 1];
        buf[len] = '\0';
        memcpy_P(buf, data + startPos + start, len);
        return String(buf);
    }
    return String();
}

bool FlashString::startsWith(const String &str) {
    if (data) {
        int len = str.length();
        char buf[len + 1];
        buf[len] = '\0';
        memcpy_P(buf, data + startPos, len);
        return strcmp_P(buf, str.c_str()) == 0;
    }
    return false;
}

bool FlashString::endsWith(const String &str) {
    if (data) {
        int len = str.length();
        char buf[len + 1];
        buf[len] = '\0';
        memcpy_P(buf, data + startPos + dataLen - len, len);
        return strcmp_P(buf, str.c_str()) == 0;
    }
    return false;
}

int FlashString::indexOf(const String &str, const int &begin) {
    if (data) {
        int len = str.length();
        int index = 0;
        char buf[len + 1];
        buf[len] = '\0';
        if (begin < dataLen - len) {
            memcpy_P(buf, data + startPos + begin + index, len);
            while (index <= dataLen - len) {
                if (strcmp_P(buf, str.c_str()) == 0) {
                    return index;
                }
                index++;
                memcpy_P(buf, data + startPos + begin + index, len);
            }
        }
    }
    return -1;
}

int FlashString::length() {
    return data ? dataLen : 0;
}
