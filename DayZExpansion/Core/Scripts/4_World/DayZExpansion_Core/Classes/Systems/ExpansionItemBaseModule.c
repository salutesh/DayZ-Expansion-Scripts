/**
 * ExpansionItemBaseModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionItemBaseModule
 * @brief		
 **/

[CF_RegisterModule(ExpansionItemBaseModule)]
class ExpansionItemBaseModule : CF_ModuleWorld
{
	override void OnInit()
	{
		super.OnInit();

		EnableRPC();
	}

	void PlayDestroySound(vector position, string sound)
	{
		if (sound == string.Empty)
			return;

		if (!GetGame().IsMultiplayer())
		{
			PlayDestroySoundImpl(position, sound);
			return;
		}

		ScriptRPC rpc = new ScriptRPC();
		rpc.Write(position);
		rpc.Write(sound);

		PlayerBase.Expansion_SendNear(rpc, ExpansionItemBaseModuleRPC.PlayDestroySound, position, 100);
	}

	void PlayDestroySoundImpl(vector position, string sound)
	{
		SEffectManager.PlaySound(sound, position);
	}

	override int GetRPCMin()
	{
		return ExpansionItemBaseModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionItemBaseModuleRPC.COUNT;
	}

	override void OnRPC(Class sender, CF_EventArgs args)
	{
		super.OnRPC(sender, args);

		auto rpc = CF_EventRPCArgs.Cast(args);

		switch (rpc.ID)
		{
		case ExpansionItemBaseModuleRPC.PlayDestroySound:
			if (GetGame().IsServer())
				return;

			string sound;
			vector position;

			rpc.Context.Read(position);
			rpc.Context.Read(sound);

			PlayDestroySoundImpl(position, sound);
			break;
		}
	}
};
