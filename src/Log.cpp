#include "Log.h"

void Log::attachSerial(HardwareSerial &serial) {
    this->serial = &serial;
}

void Log::detachSerial() {
    this->serial = NULL;
}

void Log::e(const String &scope, const String &message) {
    if (serial) {
        serial->println("[ERROR][" + scope + "]: " + message);
    }
}

void Log::w(const String &scope, const String &message) {
    if (serial) {
        serial->println("[WARNING][" + scope + "]: " + message);
    }
}

void Log::i(const String &scope, const String &message) {
    if (serial) {
        serial->println("[INFO][" + scope + "]: " + message);
    }
}

Log fb_log;
