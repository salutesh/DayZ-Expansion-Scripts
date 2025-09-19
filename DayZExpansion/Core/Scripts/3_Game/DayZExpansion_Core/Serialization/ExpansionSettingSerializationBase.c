/**
 * ExpansionSettingSerializationBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSettingSerializationBase
{
	protected string m_Variable;
	string m_Name;

	string m_DetailContent;

	protected Class m_Instance;

	protected Class m_ActualInstance;
	protected string m_ActualVariable;

	Managed m_WidgetHandle;

	bool m_IsTempSet;

	void ExpansionSettingSerializationBase(notnull Class instance, string variable)
	{
		m_Instance = instance;
		m_Variable = variable;

		if (!FindClassInstanceAndVariable())
			CF.FormatError("Variable '%1' not found on %2", m_Variable, m_Instance.ToString());
	}

	void SetFromTemp()
	{
		ResetTemp();
	}

	void ResetTemp()
	{
		m_IsTempSet = false;
	}

	bool IsTempSet()
	{
		return m_IsTempSet;
	}

	protected bool FindClassInstanceAndVariable()
	{
		array<string> arr = {};
		m_Variable.Split( ".", arr );

		m_ActualInstance = m_Instance;

		for ( int i = 0; i < arr.Count() - 1; ++i )
		{
			EnScript.GetClassVar( m_ActualInstance, arr[i], 0, m_ActualInstance );

			if ( m_ActualInstance == NULL )
				return false;
		}

		m_ActualVariable = arr[arr.Count() - 1];

		return true;
	}
};