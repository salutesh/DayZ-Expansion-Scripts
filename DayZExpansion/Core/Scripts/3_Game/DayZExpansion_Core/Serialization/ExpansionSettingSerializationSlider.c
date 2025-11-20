/**
 * ExpansionSettingSerializationSlider.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionSettingSerializationSlider: ExpansionSettingSerializationBase
{
	float m_Min;
	float m_Max;
	float m_Step;

	float m_TempValue;

	typename m_VariableType;

	override protected bool FindClassInstanceAndVariable()
	{
		if (super.FindClassInstanceAndVariable())
		{
			typename type = m_ActualInstance.Type();
			int count = type.GetVariableCount();

			for (int i = 0; i < count; i++)
			{
				if (type.GetVariableName(i) == m_ActualVariable)
				{
					m_VariableType = type.GetVariableType(i);
					return true;
				}
			}
		}

		return false;
	}

	void SetTempValue( float value )
	{
		m_TempValue = value;
		m_IsTempSet = true;
	}

	override void SetFromTemp()
	{
		super.SetFromTemp();
		
		SetValue( m_TempValue );
	}

	float GetValue()
	{
		switch (m_VariableType)
		{
			case int:
				int i;
				EnScript.GetClassVar(m_ActualInstance, m_ActualVariable, 0, i);
				return i;

			case float:
				float f;
				EnScript.GetClassVar(m_ActualInstance, m_ActualVariable, 0, f);
				return f;
		}

		return 0;
	}

	void SetValue( float value )
	{
		value = Math.Clamp( value, m_Min, m_Max );

		switch (m_VariableType)
		{
			case int:
				int i = Math.Round(value);
				EnScript.SetClassVar(m_ActualInstance, m_ActualVariable, 0, i);
				break;

			case float:
				float f = value;
				EnScript.SetClassVar(m_ActualInstance, m_ActualVariable, 0, f);
				break;
		}

		auto param = new Param2< typename, ExpansionSettingSerializationBase >( Type(), this );
		g_Game.GameScript.Call( m_Instance, "OnSettingsUpdated", param );
	}
};