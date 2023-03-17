/**
 * ExpansionPersonalStorageData.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageDataBase
{
	int ConfigVersion;
};
class ExpansionPersonalStorageData: ExpansionPersonalStorageDataBase
{
	[NonSerialized()];
	static const int VERSION = 0;
	
	int StorageID;
	string ClassName;
	string DisplayName;
	string DisplayIcon;
	vector Position;
	vector Orientation;
	bool NeedUnlock = false;
#ifdef EXPANSIONMODQUESTS
	int QuestID = -1;
#endif
#ifdef EXPANSIONMODHARDLINE
	int Reputation;
#endif
#ifdef EXPANSIONMODAI
	string Faction;
#endif
	
	void ExpansionPersonalStorageData()
	{
		ConfigVersion = VERSION;
	}
	
	void CopyFromBaseClass(ExpansionPersonalStorageDataBase base)
	{
		//! Nothing to do here yet
	}
	
	static ExpansionPersonalStorageData Load(string fileName)
	{
		CF_Log.Info("[ExpansionPersonalStorageData] Load existing personal storage file:" + fileName);
		ExpansionPersonalStorageDataBase personalStorageDataBase;
		ExpansionJsonFileParser<ExpansionPersonalStorageDataBase>.Load(fileName, personalStorageDataBase);
		
		bool save;
		ExpansionPersonalStorageData personalStorageData = new ExpansionPersonalStorageData();
		if (personalStorageDataBase.ConfigVersion < VERSION)
		{
			save = true;
			personalStorageData.CopyFromBaseClass(personalStorageDataBase); //! Copy over old data that has not changed.			
			personalStorageData.ConfigVersion = VERSION;
						
			if (save)
				Save(personalStorageData);
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionPersonalStorageData>.Load(fileName, personalStorageData))
				return NULL;
		}
		
		return personalStorageData;
	}

	void Save()
	{
		ExpansionJsonFileParser<ExpansionPersonalStorageData>.Save(ExpansionPersonalStorageModule.s_PersonalStorageDataFolderPath + "PersonalStorage_" + StorageID + ".json", this);
	}
	
	static void Save(ExpansionPersonalStorageData personalStorageData)
	{
		ExpansionJsonFileParser<ExpansionPersonalStorageData>.Save(ExpansionPersonalStorageModule.s_PersonalStorageDataFolderPath + "PersonalStorage_" + personalStorageData.StorageID + ".json", personalStorageData);
	}
	
	void Spawn()
	{
		typename chestType = ClassName.ToType();
		if (!chestType || !chestType.IsInherited(ExpansionPersonalStorageBase))
		{
			Error(ToString() + "::Spawn - Tried to spawn personal storage object with unsuported type name: " + ClassName);
			return;
		}
		
		Object obj = GetGame().CreateObjectEx(ClassName, Position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME);		
		auto personalStorage = ExpansionPersonalStorageBase.Cast(obj);
		if (!personalStorage)
		{
			GetGame().ObjectDelete(obj);
			Error(ToString() + "::Spawn - Could not spawn Personal Storage!");
			return;
		}
	
		personalStorage.SetOrientation(Orientation);
		personalStorage.SetStorageID(StorageID);
		personalStorage.m_Expansion_NetsyncData.Set(0, DisplayName);
		personalStorage.m_Expansion_NetsyncData.Set(1, DisplayIcon);
	}
	
	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(StorageID);
	#ifdef EXPANSIONMODHARDLINE
		ctx.Write(Reputation);
	#endif
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(StorageID))
			return false;
		
	#ifdef EXPANSIONMODHARDLINE
		if (!ctx.Read(Reputation))
			return false;
	#endif

		return true;
	}
	
	void SetStorageID(int id)
	{
		StorageID = id;
	}
	
	int GetStorageID()
	{
		return StorageID;
	}
	
	void SetClassName(string name)
	{
		ClassName = name;
	}
	
	string GetClassName()
	{
		return ClassName;
	}
	
	void SetDisplayName(string name)
	{
		DisplayName = name;
	}
	
	string GetDisplayName()
	{
		return DisplayName;
	}
	
	void SetDisplayIcon(string icon)
	{
		DisplayIcon = icon;
	}
	
	string GetDisplayIcon()
	{
		return DisplayIcon;
	}
	
	void SetPosition(vector pos)
	{
		Position = pos;
	}
	
	vector GetPosition()
	{
		return Position;
	}
	
	void SetOrientation(vector pos)
	{
		Orientation = pos;
	}
	
	vector GetOrientation()
	{
		return Orientation;
	}
	
	void SetNeedUnlock(bool state)
	{
		NeedUnlock = state;
	}
	
	bool NeedUnlock()
	{
		return NeedUnlock;
	}
#ifdef EXPANSIONMODQUESTS
	void SetQuestID(int questID)
	{
		QuestID = questID;
	}
	
	int GetQuestID()
	{
		return QuestID;
	}
#endif
	
#ifdef EXPANSIONMODHARDLINE
	void SetReputation(int rep)
	{
		Reputation = rep;
	}
	
	int GetReputation()
	{
		return Reputation;
	}
#endif
	
#ifdef EXPANSIONMODAI
	void SetFaction(string faction)
	{
		Faction = faction;
	}
	
	string GetFaction()
	{
		return Faction;
	}
#endif
};