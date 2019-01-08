#include "arduinohomelib/helpers.h"

String toKebabCase(const char* name) {
    String text = String(name);

    text.trim();
    text.toLowerCase();
    text.replace(" ", "_");

    return text;
}