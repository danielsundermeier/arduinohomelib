#include "arduinohomelib/helpers.h"

String toKebabCase(String text) {

    text.trim();
    text.toLowerCase();
    text.replace(" ", "_");

    return text;
}