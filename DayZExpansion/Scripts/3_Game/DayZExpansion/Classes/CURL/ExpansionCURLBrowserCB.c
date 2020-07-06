/**
 * ExpansionRestBrowserCB.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionRestBrowserCB extends RestCallback
{
	autoptr ScriptInvoker data_servers = new ScriptInvoker();
	
	override void OnError( int errorCode )
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Error Code:" + errorCode);
		#endif
	}

	override void OnTimeout()
	{
		#ifdef EXPANSIONEXLOGPRINT
		EXLogPrint("Request Timeout");
		#endif
	}

	override void OnSuccess( string data, int dataSize )
	{
		if( dataSize > 0 )
			data_servers.Invoke(data);
	}
}