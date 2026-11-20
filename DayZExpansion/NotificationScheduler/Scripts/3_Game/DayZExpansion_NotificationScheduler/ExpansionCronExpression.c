/**
 * ExpansionCronExpression.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

//! Parsed cron-style schedule expression, used by the notification scheduler.
//!
//! Expression format (fields separated by whitespace):
//!
//!   minute hour day-of-month month day-of-week            (5 fields, fires at second 0)
//!   second minute hour day-of-month month day-of-week     (6 fields, leading seconds)
//!
//!   Field          Allowed values
//!   second         0-59
//!   minute         0-59
//!   hour           0-23
//!   day-of-month   1-31
//!   month          1-12
//!   day-of-week    0-7 (0 and 7 are Sunday)
//!
//! Each field is a comma separated list of items, each item being one of:
//!   *        every value
//!   */n     every n-th value (from the first allowed value of the field)
//!   a        a single value
//!   a-b      an inclusive range
//!   a-b/n    every n-th value in an inclusive range
//!
//! Like standard cron, if both day-of-month and day-of-week are restricted (do not start with *),
//! a day matches if EITHER of them matches.
//!
//! Names (JAN, MON, ...) and the non-standard characters ? L W # are NOT supported.
//!
//! The expression is parsed once in the constructor. Use IsValid() and GetError() to check the result.
//! An invalid expression never matches.
class ExpansionCronExpression
{
	protected string m_Expression;
	protected string m_Error;
	protected bool m_Valid;
	protected bool m_HasSeconds;
	protected bool m_DayOfMonthRestricted;
	protected bool m_DayOfWeekRestricted;

	//! Each array is indexed directly by the field value (so index 0 of day-of-month and month is unused)
	protected ref TBoolArray m_Seconds = new TBoolArray;
	protected ref TBoolArray m_Minutes = new TBoolArray;
	protected ref TBoolArray m_Hours = new TBoolArray;
	protected ref TBoolArray m_DaysOfMonth = new TBoolArray;
	protected ref TBoolArray m_Months = new TBoolArray;
	protected ref TBoolArray m_DaysOfWeek = new TBoolArray;

	void ExpansionCronExpression(string expression)
	{
		m_Expression = expression;
		m_Valid = Parse(expression);
	}

	string GetExpression()
	{
		return m_Expression;
	}

	bool IsValid()
	{
		return m_Valid;
	}

	//! Reason why the expression is not valid (empty if it is valid)
	string GetError()
	{
		return m_Error;
	}

	//! True if the expression has 6 fields (leading seconds), false if it has 5 (fires at second 0)
	bool HasSeconds()
	{
		return m_HasSeconds;
	}

	//! True if day-of-month, month or day-of-week actually restrict the schedule (i.e. are not equivalent to *)
	bool UsesDateFields()
	{
		if (!m_Valid)
			return false;

		if (!AllSet(m_DaysOfMonth, 1, 31))
			return true;

		if (!AllSet(m_Months, 1, 12))
			return true;

		if (!AllSet(m_DaysOfWeek, 0, 6))
			return true;

		return false;
	}

	//! Mark the expression as unusable (it will never match) and remember why
	void Invalidate(string reason)
	{
		m_Valid = false;
		m_Error = reason;
	}

	/**
	 * @param month 1-12
	 * @param dayOfMonth 1-31
	 * @param dayOfWeek 0-6, 0 = Sunday (same as CF_Date.GetDayOfWeek)
	 * @param hour 0-23
	 * @param minute 0-59
	 * @param second 0-59 (ignored if the expression has only 5 fields)
	 */
	bool Matches(int month, int dayOfMonth, int dayOfWeek, int hour, int minute, int second)
	{
		if (!m_Valid)
			return false;

		if (month < 1 || month > 12 || dayOfMonth < 1 || dayOfMonth > 31 || dayOfWeek < 0 || dayOfWeek > 6)
			return false;

		if (hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59)
			return false;

		if (m_HasSeconds && !m_Seconds[second])
			return false;

		if (!m_Minutes[minute] || !m_Hours[hour] || !m_Months[month])
			return false;

		bool dayOfMonthMatches = m_DaysOfMonth[dayOfMonth];
		bool dayOfWeekMatches = m_DaysOfWeek[dayOfWeek];

		//! Standard cron behavior: if both are restricted, either one matching is enough
		if (m_DayOfMonthRestricted && m_DayOfWeekRestricted)
			return dayOfMonthMatches || dayOfWeekMatches;

		return dayOfMonthMatches && dayOfWeekMatches;
	}

	protected bool Parse(string expression)
	{
		m_Error = "";

		string normalized = expression;
		normalized.Replace("\t", " ");
		normalized = normalized.Trim();

		TStringArray tokens = new TStringArray;
		normalized.Split(" ", tokens);

		//! Ignore empty tokens (repeated spaces)
		TStringArray fields = new TStringArray;
		foreach (string token: tokens)
		{
			if (token != "")
				fields.Insert(token);
		}

		int offset = 0;
		if (fields.Count() == 6)
		{
			m_HasSeconds = true;
			offset = 1;
		}
		else if (fields.Count() != 5)
		{
			m_Error = "expected 5 fields (minute hour day-of-month month day-of-week) or 6 fields (second minute hour day-of-month month day-of-week), found " + fields.Count();
			return false;
		}

		if (m_HasSeconds && !ParseField(fields[0], "second", 0, 59, m_Seconds))
			return false;

		if (!ParseField(fields[offset], "minute", 0, 59, m_Minutes))
			return false;

		if (!ParseField(fields[offset + 1], "hour", 0, 23, m_Hours))
			return false;

		if (!ParseField(fields[offset + 2], "day-of-month", 1, 31, m_DaysOfMonth))
			return false;

		if (!ParseField(fields[offset + 3], "month", 1, 12, m_Months))
			return false;

		if (!ParseField(fields[offset + 4], "day-of-week", 0, 7, m_DaysOfWeek))
			return false;

		//! 7 is an alias for 0 (Sunday)
		if (m_DaysOfWeek[7])
			m_DaysOfWeek[0] = true;

		//! Like standard cron, a day field that starts with * counts as unrestricted
		m_DayOfMonthRestricted = fields[offset + 2].Substring(0, 1) != "*";
		m_DayOfWeekRestricted = fields[offset + 4].Substring(0, 1) != "*";

		return true;
	}

	//! Parse one field (comma separated list of items) and set the matching entries of 'allowed' to true
	protected bool ParseField(string field, string fieldName, int lo, int hi, TBoolArray allowed)
	{
		allowed.Clear();

		int slot;
		for (slot = 0; slot <= hi; slot++)
		{
			allowed.Insert(false);
		}

		int fieldLength = field.Length();
		if (field.Substring(0, 1) == "," || field.Substring(fieldLength - 1, 1) == "," || field.IndexOf(",,") != -1)
		{
			m_Error = fieldName + " field \"" + field + "\" has an empty list item";
			return false;
		}

		TStringArray items = new TStringArray;
		field.Split(",", items);

		foreach (string item: items)
		{
			if (!ParseItem(item, fieldName, lo, hi, allowed))
				return false;
		}

		return true;
	}

	//! Parse a single list item (*, */n, a, a-b, a-b/n)
	protected bool ParseItem(string item, string fieldName, int lo, int hi, TBoolArray allowed)
	{
		if (item == "")
		{
			m_Error = fieldName + " field has an empty list item";
			return false;
		}

		int step = 1;
		bool hasStep = false;
		string rangePart = item;

		int slash = item.IndexOf("/");
		if (slash != -1)
		{
			hasStep = true;
			rangePart = SubText(item, 0, slash);
			string stepText = SubText(item, slash + 1, item.Length() - slash - 1);

			if (!IsNumber(stepText))
			{
				m_Error = fieldName + " field item \"" + item + "\" has an invalid step (must be a positive whole number)";
				return false;
			}

			step = ToNumber(stepText);
			if (step < 1)
			{
				m_Error = fieldName + " field item \"" + item + "\" has an invalid step (must be at least 1)";
				return false;
			}
		}

		int rangeStart;
		int rangeEnd;

		if (rangePart == "*")
		{
			rangeStart = lo;
			rangeEnd = hi;
		}
		else
		{
			int dash = rangePart.IndexOf("-");
			if (dash == -1)
			{
				if (!IsNumber(rangePart))
				{
					m_Error = fieldName + " field item \"" + item + "\" is not a valid number, range or *";
					return false;
				}

				if (hasStep)
				{
					m_Error = fieldName + " field item \"" + item + "\" is invalid (a step can only be used with * or a range)";
					return false;
				}

				rangeStart = ToNumber(rangePart);
				rangeEnd = rangeStart;
			}
			else
			{
				string startText = SubText(rangePart, 0, dash);
				string endText = SubText(rangePart, dash + 1, rangePart.Length() - dash - 1);

				if (!IsNumber(startText) || !IsNumber(endText))
				{
					m_Error = fieldName + " field item \"" + item + "\" is not a valid range";
					return false;
				}

				rangeStart = ToNumber(startText);
				rangeEnd = ToNumber(endText);
			}
		}

		if (rangeStart < lo || rangeEnd > hi)
		{
			m_Error = fieldName + " field item \"" + item + "\" is out of range (allowed: " + lo + "-" + hi + ")";
			return false;
		}

		if (rangeStart > rangeEnd)
		{
			m_Error = fieldName + " field item \"" + item + "\" is an invalid range (start is greater than end, ranges cannot wrap around)";
			return false;
		}

		int value;
		for (value = rangeStart; value <= rangeEnd; value += step)
		{
			allowed[value] = true;
		}

		return true;
	}

	//! Substring that returns an empty string for a length of zero or less
	protected string SubText(string text, int start, int length)
	{
		if (length <= 0)
			return "";

		return text.Substring(start, length);
	}

	//! True if text consists of 1 to 4 decimal digits only (no sign, no spaces)
	protected bool IsNumber(string text)
	{
		int textLength = text.Length();
		if (textLength < 1 || textLength > 4)
			return false;

		string digits = "0123456789";

		int pos;
		for (pos = 0; pos < textLength; pos++)
		{
			if (digits.IndexOf(text.Substring(pos, 1)) == -1)
				return false;
		}

		return true;
	}

	//! Only call with text that passed IsNumber (decimal digits only)
	protected int ToNumber(string text)
	{
		string digits = "0123456789";

		int result = 0;

		int pos;
		for (pos = 0; pos < text.Length(); pos++)
		{
			result = result * 10 + digits.IndexOf(text.Substring(pos, 1));
		}

		return result;
	}

	protected bool AllSet(TBoolArray values, int lo, int hi)
	{
		int index;
		for (index = lo; index <= hi; index++)
		{
			if (!values[index])
				return false;
		}

		return true;
	}
}
