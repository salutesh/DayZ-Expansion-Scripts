/**
 * CF_Localiser.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class CF_Localiser
{
	override CF_Localiser Set(int index, string value)
	{
		string valueUpper = value;
		valueUpper.ToUpper();
		bool translates = valueUpper.IndexOf("STR_") == 0;
		return Set(index, value, translates);
	}

#ifdef DIAG_DEVELOPER
	override CF_Localiser Set(int index, string value, bool translates)
	{
		if (translates)
		{
			string valueUpper = value;
			valueUpper.ToUpper();
			if (valueUpper.IndexOf("#STR_") == 0)
				Error("Wrong use of CF_Localiser, localizable strings must not start with #");
		}

		return super.Set(index, value, translates);
	}
#endif

	void SetTranslates(int index, bool translates)
	{
		int realIndex = index + 1;
		if (realIndex < 0 || realIndex >= m_Count)
			return;
		m_Translates[realIndex] = translates;
	}
}
