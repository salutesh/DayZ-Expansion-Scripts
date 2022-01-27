/**
 * ExpansionItemBaseModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

/**@class		ExpansionItemBaseModule
 * @brief		
 **/
class ExpansionItemBaseModule : JMModuleBase
{
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

#ifdef CF_BUGFIX_REF
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ParamsReadContext ctx)
#else
	override void OnRPC(PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx)
#endif
	{
		switch (rpc_type)
		{
		case ExpansionItemBaseModuleRPC.PlayDestroySound:
			if (GetGame().IsServer())
				return;

			string sound;
			vector position;

			ctx.Read(position);
			ctx.Read(sound);

			PlayDestroySoundImpl(position, sound);
			break;
		}
	}
};
