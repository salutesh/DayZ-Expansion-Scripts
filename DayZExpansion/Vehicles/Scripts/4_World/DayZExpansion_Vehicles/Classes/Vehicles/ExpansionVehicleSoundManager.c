/**
 * ExpansionVehicleSoundManager.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionVehicleSoundManager
{
	static ref array<string> s_SoundShaderParameters = {
		"rpm",
		"engineOn",
		"campos",
		"doors",
		"speed",
		"thrust",
		"water",
		"rock",
		"grass",
		"gravel",
		"asphalt",
		"latSlipDrive",
		"steerdelta",
		"rain",
		"damperLeft",
		"damperRight",
	};

	static ref map<string, ref ExpansionVehicleSoundShader> s_SoundShaders = new map<string, ref ExpansionVehicleSoundShader>();

	static void Get(string name, out ExpansionVehicleSoundShader shader)
	{
		if (s_SoundShaders.Find(name, shader))
			return;

		shader = new ExpansionVehicleSoundShader(name);
		s_SoundShaders[name] = shader;
	}
};
