/**
 * ExpansionMarketATM_Data.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionMarketATM_Data
{	
	string PlayerID;
	int MoneyDeposited;
	
	[NonSerialized()]
	string m_FileName;
	
	int GetMoney()
	{
		return MoneyDeposited;
	}
	
	void RemoveMoney(int amount)
	{
		MoneyDeposited -= amount;
	}
	
	void AddMoney(int amount)
	{
		MoneyDeposited += amount;
	}
	
	static ExpansionMarketATM_Data Load(string name)
	{
		ExpansionMarketATM_Data data = new ExpansionMarketATM_Data;
		
		JsonFileLoader<ExpansionMarketATM_Data>.JsonLoadFile(EXPANSION_ATM_FOLDER + name + ".json", data);

		data.m_FileName = name;
		
		return data;
	}
	
	void Save()
	{
		JsonFileLoader<ExpansionMarketATM_Data>.JsonSaveFile(EXPANSION_ATM_FOLDER + m_FileName + ".json", this);
	}
}
