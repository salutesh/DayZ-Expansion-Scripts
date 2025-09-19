/**
 * ExpansionSettingSerializationEnum.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSettingSerializationEnum: ExpansionSettingSerializationInt
{
	ref array< string > m_Values;

	void ExpansionSettingSerializationEnum(notnull Class instance, string variable)
	{
		m_Values = new array< string >();
	}

	void SetTempValue( string value )
	{
		int idx = m_Values.Find( value );
		if ( idx == -1 )
			return;

		SetTempValue( idx );
	}

	void SetValue( string value )
	{
		int idx = m_Values.Find( value );
		if ( idx == -1 )
			return;

		SetValue( idx );
	}
	
	override void SetValue( int value )
	{
		if ( value >= m_Values.Count() )
			value = m_Values.Count();

		if ( value < 0 )
			value = 0;

		super.SetValue( value );
	}
};