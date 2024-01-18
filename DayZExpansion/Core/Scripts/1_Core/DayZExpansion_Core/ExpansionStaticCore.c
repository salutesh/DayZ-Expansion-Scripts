class ExpansionStaticCore
{
	string CF_GetISOTime()
	{
		int hour;
		int minute;
		int second;

		GetHourMinuteSecond(hour, minute, second);

		return hour.ToStringLen(2) + ":" + minute.ToStringLen(2) + ":" + second.ToStringLen(2);
	}
}
