#pragma once
#include <cstring>

// Remove display-only tokens and cosmetics: '_B_'
inline void strip_display_tokens(char* s) {
  if (!s) return;
  for (char* p = strstr(s, "_B_"); p != nullptr; p = strstr(p, "_B_")) {
    memmove(p, p + 3, strlen(p + 3) + 1);
  }
}