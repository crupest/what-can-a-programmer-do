#pragma once
#include "StringUtil.hpp"

#include <iostream>
#include <sstream>
#include <string>
#include <string_view>

#ifdef WIN32
using Char = wchar_t;
using String = std::wstring;
using StringView = std::wstring_view;
using StringStream = std::wstringstream;
inline auto &input_stream = std::wcin;
inline auto &output_stream = std::wcout;
inline auto &error_stream = std::wcerr;
#define CRUT(string_literal) L##string_literal

inline String ConvertCharString(std::string_view s) {
  return cru::ToUtf16WString(s);
}

inline std::string ConvertCharStringBack(StringView s) {
  return cru::ToUtf8(s);
}
#else
using Char = char;
using String = std::string;
using StringView = std::string_view;
using StringStream = std::stringstream;
inline auto &input_stream = std::cin;
inline auto &output_stream = std::cout;
inline auto &error_stream = std::cerr;
#define CRUT(string_literal) string_literal

inline String ConvertCharString(std::string_view s) { return String(s); }
inline std::string ConvertCharStringBack(StringView s) {
  return std::string(s);
}
#endif

int Main();

[[noreturn]] void PrintErrorMessageAndExit(StringView message,
                                           bool print_last_error = true);

int CloseSocket(int socket);

void BeforeExit();

// Return false for error.
bool SafeSend(int socket, std::string_view buffer);
// Return false for error.
bool SafeReadUntil(int socket, char c, std::string &data, std::string &rest);
