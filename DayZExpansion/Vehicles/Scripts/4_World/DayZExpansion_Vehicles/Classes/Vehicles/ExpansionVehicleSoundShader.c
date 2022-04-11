/**
 * ExpansionVehicleSoundShader.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleSoundShader
{
	ref Expression m_Volume;
	ref Expression m_Frequency;

	void ExpansionVehicleSoundShader(string name)
	{
		string path;
		string expression;

		expression = "1";
		path = "CfgSoundShaders " + name + " volume";
		if (GetGame().ConfigIsExisting(path))
		{
			expression = GetGame().ConfigGetTextOut(path);
		}

#ifdef CF_EXPRESSION
		Class.CastTo(m_Volume, CF_ExpressionVM.Compile(expression, ExpansionVehicleSoundManager.s_SoundShaderParameters, Expression));
#endif

		expression = "1";
		path = "CfgSoundShaders " + name + " frequency";
		if (GetGame().ConfigIsExisting(path))
		{
			expression = GetGame().ConfigGetTextOut(path);
		}

#ifdef CF_EXPRESSION
		Class.CastTo(m_Frequency, CF_ExpressionVM.Compile(expression, ExpansionVehicleSoundManager.s_SoundShaderParameters, Expression));
#endif
	}

	void Calculate(array<float> variables, inout float volume, inout float frequency)
	{
#ifdef CF_EXPRESSION
		frequency = m_Frequency.Evaluate(variables);
		frequency = Math.Clamp(frequency, 0.5, 2.0);

		volume = m_Volume.Evaluate(variables);
#endif
	}
};
