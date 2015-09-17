#ifndef __HELPERS_H__
#define __HELPERS_H__

#include "cocos2d.h"

namespace helpers {

#define CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper"

class Vibrator
{
public:
	static void vibrate(int time);
	static void cancelVibrate();
};

class URL
{
public:
	static void open(const char* url);
	static const std::string encode(const std::string& src);
};

class Locale
{
public:
	static const std::string getCountry();
};

class String
{
public:
	static const std::string format(const char* format, ...);
};

class Number
{
public:
	static void shuffle(int* array, const int size);
};

}; // namespace helpers {
#endif // __HELPERS_H__
