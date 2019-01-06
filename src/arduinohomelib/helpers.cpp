#include "arduinohomelib/helpers.h"

const char* toKebabCase(const char* name) {
    String text = String(name);
    text.trim();
    text.toLowerCase();
    text.replace(" ", "_");

    return text.c_str();
}