/**
 * ExpansionClientSetting.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionClientSettingCategory
{
	autoptr array< ref ExpansionClientSetting > m_Settings;

	string m_Name;
	string m_DisplayName;

	void ExpansionClientSettingCategory()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettingCategory::ExpansionClientSettingCategory - Start");
		#endif
		
		m_Settings = new array< ref ExpansionClientSetting >;
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettingCategory::ExpansionClientSettingCategory - End");
		#endif
	}
}

class ExpansionClientSetting
{
	string m_Variable;
	string m_Name;

	string m_DetailLabel;
	string m_DetailContent;
}

class ExpansionClientSettingToggle: ExpansionClientSetting
{
	bool GetValue()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettingToggle::GetValue - Start");
		#endif
		
		bool toggled = false;

		EnScript.GetClassVar( GetExpansionClientSettings(), m_Variable, 0, toggled );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionClientSettingToggle::GetValue - Return: " + toggled.ToString() );
		#endif
		
		return toggled;
	}

	void SetValue( bool toggled )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettingToggle::SetValue - Start");
		#endif
		
		EnScript.SetClassVar( GetExpansionClientSettings(), m_Variable, 0, toggled );
		
		GetExpansionClientSettings().SI_UpdateSetting.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettingToggle::SetValue - End");
		#endif
	}
}

class ExpansionClientSettingSlider: ExpansionClientSetting
{
	float m_Min;
	float m_Max;

	float GetValue()
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettingToggle::GetRawValue - Start");
		#endif
		
		float amt = 0;

		EnScript.GetClassVar( GetExpansionClientSettings(), m_Variable, 0, amt );

		#ifdef EXPANSIONEXPRINT
		EXPrint( "ExpansionClientSettingToggle::GetRawValue - Return: " + amt.ToString() );
		#endif

		return amt;
	}

	void SetValue( float amt )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettingToggle::SetRawValue - Start");
		#endif
		
		EnScript.SetClassVar( GetExpansionClientSettings(), m_Variable, 0, amt );
		
		GetExpansionClientSettings().SI_UpdateSetting.Invoke();
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("ExpansionClientSettingToggle::SetRawValue - End");
		#endif
	}
}