/*
 * Azan.h
 *
 *  Created on: Jan 2, 2014
 *      Author: M Hendra Herviawan
 */

#ifndef AZAN_H_
#define AZAN_H_

//#include "main.h"
#include <Wire/Wire.h>
#include "DS1307RTC.h"

#include "Time.h"
#include "TimeAlarms.h"

#include "PrayerTime.h"

typedef uint8_t prayer_t;

#define Fajr	0
#define Zuhr	1
#define Asr		2
#define Maghrib	3
#define Isha	4
#define SunRise	5

const char PrayerString0 [] = "Fajr";
const char PrayerString1 [] = "Zuhr";
const char PrayerString2 [] = "Asr";
const char PrayerString3 [] = "Mhrib";
const char PrayerString4 [] = "Isha";
const char PrayerString5 [] = "SunRise";

const double ihtiyati = 0.034;
const time_t tommorow = 86400;
const double lintang = -6.91;
const double bujur = 107.61;
const int timezone = 7;
const double fajrTwilight = -20;
const double ishaTwilight = -18;

boolean alarm_status = false; //volatile
prayer_t nextPrayer = Fajr;

double NextPrayerTime, FajrTime, ZuhurTime ;

void globalAzanSetup();
void setsunRiseAlarm(time_t value);
void sunRiseAlarm();
void setFajrAlarm(time_t value);
void FajrAlarm();
void setZuhrAlarm(time_t value);
void ZuhrAlarm();
void setAsrAlarm(time_t value);
void AsrAlarm();
void setMaghribAlarm(time_t value);
void MaghribAlarm();
void setIshaAlarm(time_t value);
void IshaAlarm();
double getNextPrayer(time_t value);
double getPrayerTime(prayer_t prayer, time_t value);

void BUZZ();

/////////////////////////////////////////////////////////////////
void globalAzanSetup() {

	Alarm.freeAll();
	Alarm.alarmOnce(0, 0, 0, globalAzanSetup);

	Serial.print(F("sunRise Time: "));
	setsunRiseAlarm(now());

	Serial.print(F("Fajar Time: "));
	setFajrAlarm(now());

	Serial.print(F("Zuhr Time: "));
	setZuhrAlarm(now());

	Serial.print(F("Asr Time: "));
	setAsrAlarm(now());

	Serial.print(F("Magrib Time: "));
	setMaghribAlarm(now());

	Serial.print(F("Isha Time: "));
	setIshaAlarm(now());

	nextPrayer = getNextPrayer(now());
	NextPrayerTime = getPrayerTime(nextPrayer, now());
}


void setsunRiseAlarm(time_t value) {
	double fajr, sunRise, zuhr, asr, maghrib, isha;
	//digitalDateDisplay(year(value), month(value), day(value));
	calcPrayerTimes(year(value), month(value), day(value), bujur, lintang,
			timezone, fajrTwilight, ishaTwilight, fajr, sunRise, zuhr, asr,
			maghrib, isha);

	uint8_t hours = 0, minutes = 0;
	fajr += ihtiyati;
	doubleToHrMin(sunRise, hours, minutes);
	Alarm.alarmOnce(hours, minutes, 0, sunRiseAlarm);
	serialClockDisplay(hours, minutes);
	NextPrayerTime = fajr;
}

void sunRiseAlarm() {
	alarm_status = true;
	Serial.println(F("Waktu Sun Rise"));
	time_t Tommorow = now() + tommorow;
	setsunRiseAlarm(Tommorow);
	BUZZ();
	alarm_status = false;
	nextPrayer = Fajr;
}

void setFajrAlarm(time_t value) {
	double fajr, sunRise, zuhr, asr, maghrib, isha;
	//digitalDateDisplay(year(value), month(value), day(value));
	calcPrayerTimes(year(value), month(value), day(value), bujur, lintang,
			timezone, fajrTwilight, ishaTwilight, fajr, sunRise, zuhr, asr,
			maghrib, isha);

	uint8_t hours = 0, minutes = 0;
	fajr += ihtiyati;
	doubleToHrMin(fajr, hours, minutes);
	Alarm.alarmOnce(hours, minutes, 0, FajrAlarm);
	serialClockDisplay(hours, minutes);

	FajrTime = fajr;
	NextPrayerTime = zuhr;
}

void FajrAlarm() {
	alarm_status = true;
	Serial.println(F("Waktu Fajar"));
	time_t Tommorow = now() + tommorow;
	setFajrAlarm(Tommorow);
	BUZZ();
	alarm_status = false;
	nextPrayer = Zuhr;
}

void setZuhrAlarm(time_t value) {
	double fajr, sunRise, zuhr, asr, maghrib, isha;
	//digitalDateDisplay(year(value), month(value), day(value));
	calcPrayerTimes(year(value), month(value), day(value), bujur, lintang,
			timezone, fajrTwilight, ishaTwilight, fajr, sunRise, zuhr, asr,
			maghrib, isha);

	uint8_t hours, minutes;
	zuhr += ihtiyati;
	doubleToHrMin(zuhr, hours, minutes);
	Alarm.alarmOnce(hours, minutes, 0, ZuhrAlarm);
	serialClockDisplay(hours, minutes);

	ZuhurTime = zuhr;
	NextPrayerTime = asr;
}

void ZuhrAlarm() {
	alarm_status = true;
	Serial.println(F("Waktu Zuhr"));
	time_t Tommorow = now() + tommorow;
	setZuhrAlarm(Tommorow);
	BUZZ();
	alarm_status = false;
	nextPrayer = Asr;
}

void setAsrAlarm(time_t value) {
	double fajr, sunRise, zuhr, asr, maghrib, isha;
	//digitalDateDisplay(year(value), month(value), day(value));
	calcPrayerTimes(year(value), month(value), day(value), bujur, lintang,
			timezone, fajrTwilight, ishaTwilight, fajr, sunRise, zuhr, asr,
			maghrib, isha);

	uint8_t hours, minutes;
	asr += ihtiyati;
	doubleToHrMin(asr, hours, minutes);
	Alarm.alarmOnce(hours, minutes, 0, AsrAlarm);
	serialClockDisplay(hours, minutes);

	NextPrayerTime = maghrib;
}

void AsrAlarm() {
	alarm_status = true;
	Serial.println(F("Waktu Asr"));
	time_t Tommorow = now() + tommorow;
	setAsrAlarm(Tommorow);
	BUZZ();
	alarm_status = false;
	nextPrayer = Maghrib;
}

void setMaghribAlarm(time_t value) {
	double fajr, sunRise, zuhr, asr, maghrib, isha;
	//digitalDateDisplay(year(value), month(value), day(value));
	calcPrayerTimes(year(value), month(value), day(value), bujur, lintang,
			timezone, fajrTwilight, ishaTwilight, fajr, sunRise, zuhr, asr,
			maghrib, isha);

	uint8_t hours = 19, minutes = 23;
	maghrib += ihtiyati;
	//doubleToHrMin(maghrib, hours, minutes);
	Alarm.alarmOnce(hours, minutes, 0, MaghribAlarm);
	serialClockDisplay(hours, minutes);

	NextPrayerTime = isha;
}

void MaghribAlarm() {
	alarm_status = true;
	Serial.println(F("Waktu Maghrib"));
	time_t Tommorow = now() + tommorow;
	setMaghribAlarm(Tommorow);
	BUZZ();
	alarm_status = false;
	nextPrayer = Isha;
}

//Magrib
void setIshaAlarm(time_t value) {
	double fajr, sunRise, zuhr, asr, maghrib, isha;
	//digitalDateDisplay(year(value), month(value), day(value));
	calcPrayerTimes(year(value), month(value), day(value), bujur, lintang,
			timezone, fajrTwilight, ishaTwilight, fajr, sunRise, zuhr, asr,
			maghrib, isha);

	uint8_t hours = 0, minutes = 0;
	isha += ihtiyati;
	doubleToHrMin(isha, hours, minutes);
	Alarm.alarmOnce(hours, minutes, 0, IshaAlarm);
	serialClockDisplay(hours, minutes);

	NextPrayerTime = sunRise;
}

void IshaAlarm() {
	alarm_status = true;
	Serial.println(F("Waktu Isha"));
	time_t Tommorow = now() + tommorow;
	setIshaAlarm(Tommorow);
	BUZZ();
	alarm_status = false;
	nextPrayer = Fajr;
}

double getNextPrayer(time_t value){
	double hours = (double)(hour(value));
	hours += (double)((double)(minute(value)) * 0.0167);

	double fajr, sunRise, zuhr, asr, maghrib, isha;
	//digitalDateDisplay(year(value), month(value), day(value));
	calcPrayerTimes(year(value), month(value), day(value), bujur, lintang,
			timezone, fajrTwilight, ishaTwilight, fajr, sunRise, zuhr, asr,
			maghrib, isha);

	if (hours >= isha)
		return Fajr;

	//if (hours <= Fajr)
		//return SunRise;

	if (hours <= sunRise)
		return SunRise;

	if (hours <= zuhr)
		return Zuhr;

	if (hours <= asr)
		return Asr;

	if (hours <= maghrib)
		return Maghrib;

	if (hours <= isha)
		return Isha;

	return -1;
}

double getPrayerTime(prayer_t prayer, time_t value) {
	double fajr, sunRise, zuhr, asr, maghrib, isha;
	//digitalDateDisplay(year(value), month(value), day(value));
	calcPrayerTimes(year(value), month(value), day(value), bujur, lintang,
			timezone, fajrTwilight, ishaTwilight, fajr, sunRise, zuhr, asr,
			maghrib, isha);

	switch (prayer) {
	case Fajr:
		return fajr+ihtiyati;
	case Zuhr:
		return zuhr+ihtiyati;
	case Asr:
		return asr+ihtiyati;
	case Maghrib:
		return maghrib+ihtiyati;
	case Isha:
		return isha+ihtiyati;
	case SunRise:
		return sunRise+ihtiyati;
	}

	return 0.0; // prayer not defined
}

const char* getPrayerName(prayer_t prayer){

	switch (prayer) {
	case Fajr:
		return PrayerString0;
	case Zuhr:
		return PrayerString1;
	case Asr:
		return PrayerString2;
	case Maghrib:
		return PrayerString3;
	case Isha:
		return PrayerString4;
	case SunRise:
		return PrayerString5;
	}

	return "err";
}
#endif /* AZAN_H_ */
