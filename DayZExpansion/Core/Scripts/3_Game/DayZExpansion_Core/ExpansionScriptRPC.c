/**
 * ExpansionScriptRPC.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionScriptRPC
{
	static const int MAGIC_NUMBER = 1506850293;  //! "DayZ Expansion".Hash()

	static ScriptRPC Create()
	{
		ScriptRPC rpc = new ScriptRPC();
	//#ifndef EXPANSION_RPC_DISABLE_MAGIC_NUMBER
		//rpc.Write(MAGIC_NUMBER);
	//#endif
		return rpc;
	}

	static bool CheckMagicNumber(ParamsReadContext ctx)
	{
	//#ifndef EXPANSION_RPC_DISABLE_MAGIC_NUMBER
		//int magic_number;

		//if (!ctx.Read(magic_number))
		//{
			//Error("Couldn't read magic number");
			//return false;
		//}

		//if (magic_number != MAGIC_NUMBER)
		//{
			//Error("Magic number mismatch");
			//return false;
		//}
	//#endif

		return true;
	}
}
