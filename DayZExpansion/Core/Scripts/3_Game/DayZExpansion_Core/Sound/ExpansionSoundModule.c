/**
 * ExpansionSoundModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2023 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionSoundModule
 * @brief		This module provides functionality like playing sound for near players at given position
 **/

[CF_RegisterModule(ExpansionSoundModule)]
class ExpansionSoundModule: CF_ModuleGame
{
	static ExpansionSoundModule s_Instance;

	void ExpansionSoundModule()
	{
		s_Instance = this;
	}

	override void OnInit()
	{
		super.OnInit();

		Expansion_EnableRPCManager();
		Expansion_RegisterClientRPC("RPC_Play");
		Expansion_RegisterClientRPC("RPC_Stop");
		Expansion_RegisterClientRPC("RPC_FadeIn");
		Expansion_RegisterClientRPC("RPC_FadeOut");
		Expansion_RegisterClientRPC("RPC_SetVolume");
	}

	void RPC_Play(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		int soundSetID;
		vector position;
		auto params = new Param3<float, float, bool>(-1.0, -1.0, false);  //! fadeIn, fadeOut, loop

		if (!ReadParams(target, ctx, soundSetID, position, params))
			return;

		if (target)
			ExpansionSound.Play(soundSetID, target, params.param1, params.param2, params.param3);
		else
			ExpansionSound.Play(soundSetID, position, params.param1, params.param2, params.param3);
	}

	void RPC_Stop(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		int soundSetID;
		vector position;
		auto params = new Param1<float>(0.0);  //! fadeOut

		if (!ReadParams(target, ctx, soundSetID, position, params))
			return;

		if (target)
			ExpansionSound.Stop(soundSetID, target, params.param1);
		else
			ExpansionSound.Stop(soundSetID, position, params.param1);
	}

	void RPC_FadeIn(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		int soundSetID;
		vector position;
		auto params = new Param1<float>(0.0);  //! duration

		if (!ReadParams(target, ctx, soundSetID, position, params))
			return;

		if (target)
			ExpansionSound.FadeIn(soundSetID, target, params.param1);
		else
			ExpansionSound.FadeIn(soundSetID, position, params.param1);
	}

	void RPC_FadeOut(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		int soundSetID;
		vector position;
		auto params = new Param1<float>(0.0);  //! duration

		if (!ReadParams(target, ctx, soundSetID, position, params))
			return;

		if (target)
			ExpansionSound.FadeOut(soundSetID, target, params.param1);
		else
			ExpansionSound.FadeOut(soundSetID, position, params.param1);
	}

	void RPC_SetVolume(PlayerIdentity sender, Object target, ParamsReadContext ctx)
	{
		int soundSetID;
		vector position;
		auto params = new Param1<float>(0.0);  //! volume

		if (!ReadParams(target, ctx, soundSetID, position, params))
			return;

		if (target)
			ExpansionSound.SetVolume(soundSetID, target, params.param1);
		else
			ExpansionSound.SetVolume(soundSetID, position, params.param1);
	}

	protected bool ReadParams(Object target, ParamsReadContext ctx, out int soundSetID, out vector position, Param params)
	{
		if (!ctx.Read(soundSetID))
			return false;

		if (!target && !ctx.Read(position))
			return false;

		if (!params.Deserializer(ctx))
			return false;

		return true;
	}
}
