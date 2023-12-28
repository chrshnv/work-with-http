#ifndef STRING_UTILS_H
#define STRING_UTILS_H
#include <string>

// trim from start (in place)
static void ltrim(std::string&s) {
    s.erase( s.begin(), std::find_if( s.begin(), s.end(), []( const unsigned char ch ) {
        return !std::isspace(ch);
    } ) );
}

// trim from end (in place)
static void rtrim(std::string&s) {
    s.erase( std::find_if( s.rbegin(), s.rend(), []( const unsigned char ch ) {
        return !std::isspace(ch);
    } ).base(), s.end() );
}

// trim from both ends (in place)
static void trim(std::string&s) {
    rtrim(s);
    ltrim(s);
}

// trim from both ends (copying)
static std::string trim_copy(std::string s) {
    trim(s);
    return s;
}

#endif //STRING_UTILS_H
