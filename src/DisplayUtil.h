#pragma once
#include <cstring>

// Remove display-only tokens and cosmetics: '_B_'
inline void strip_display_tokens(char* s) {
  if (!s) return;
  for (char* p = std::strstr(s, "_B_"); p != nullptr; p = std::strstr(p, "_B_")) {
    std::memmove(p, p + 3, std::strlen(p + 3) + 1);
  }
}