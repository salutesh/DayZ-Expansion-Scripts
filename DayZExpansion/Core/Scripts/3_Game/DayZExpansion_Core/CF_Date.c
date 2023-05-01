/**
 * CF_Date.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

enum CF_Weekdays
{
	Sunday,
	Monday,
	Tuesday,
	Wednesday,
	Thursday,
	Friday,
	Saturday
}

modded class CF_Date
{
	int GetDaysToNextWeekDay(int weekDay)
  	{
		int weekDayOfCurrentWeek = GetDayOfWeek();
		if (weekDay > weekDayOfCurrentWeek)
			return weekDay - weekDayOfCurrentWeek;
		int daysRemainingInCurrentWeek = 7 - weekDayOfCurrentWeek;
		return daysRemainingInCurrentWeek + weekDay;
  	}

	string GetDayName()
	{
		int day = GetDayOfWeek();
		return typename.EnumToString(CF_Weekdays, day);
	}

	static int GetDayByName(string dayName)
	{
		return typename.StringToEnum(CF_Weekdays, dayName);
	}

	string GetISODate()
	{
		return m_Year.ToStringLen(4) + "-" + m_Month.ToStringLen(2) + "-" + m_Day.ToStringLen(2);
	}

	string GetISOTime(string delimHMS = ":")
	{
		return m_Hour.ToStringLen(2) + delimHMS + m_Minute.ToStringLen(2) + delimHMS + m_Second.ToStringLen(2);
	}

	string GetISODateTime(string delim = "T", string delimHMS = ":")
	{
		return GetISODate() + delim + GetISOTime(delimHMS);
	}

	//! Fix Format() for most common formats
	override string Format(string format)
	{
		switch (format)
		{
			case DATE:
				return GetISODate();
			case DATETIME:
				return GetISODateTime(" ");
			case TIME:
				return GetISOTime();
		}

		return super.Format(format);
	}
}
