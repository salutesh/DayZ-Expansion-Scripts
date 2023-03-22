/**
 * ExpansionPersonalStorageConfig.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionPersonalStorageConfigBase
{
	int ConfigVersion;
	int StorageID;
	string ClassName;
	string DisplayName;
	string DisplayIcon;
	vector Position;
	vector Orientation;
#ifdef EXPANSIONMODQUESTS
	int QuestID = -1;
#endif
#ifdef EXPANSIONMODHARDLINE
	int Reputation;
#endif
#ifdef EXPANSIONMODAI
	string Faction;
#endif
};
class ExpansionPersonalStorageConfig: ExpansionPersonalStorageConfigBase
{
	[NonSerialized()];
	static const int VERSION = 2;
	
	bool IsGlobalStorage;
	
	void ExpansionPersonalStorageConfig()
	{
		ConfigVersion = VERSION;
	}
	
	void CopyFromBaseClass(ExpansionPersonalStorageConfigBase base)
	{
		StorageID = base.StorageID;
		ClassName = base.ClassName;
		DisplayName = base.DisplayName;
		DisplayIcon = base.DisplayIcon;
		Position = base.Position;
		Orientation = base.Orientation;
#ifdef EXPANSIONMODQUESTS
		QuestID = base.QuestID;
#endif
#ifdef EXPANSIONMODHARDLINE
		Reputation = base.Reputation;
#endif
#ifdef EXPANSIONMODAI
		Faction = base.Faction;
#endif
	}
	
	static ExpansionPersonalStorageConfig Load(string fileName)
	{
		CF_Log.Info("[ExpansionPersonalStorageConfig] Load existing personal storage file:" + fileName);
		ExpansionPersonalStorageConfigBase personalStorageConfigBase;
		if (!ExpansionJsonFileParser<ExpansionPersonalStorageConfigBase>.Load(fileName, personalStorageConfigBase))
			return NULL;
		
		bool save;
		ExpansionPersonalStorageConfig personalStorageConfig = new ExpansionPersonalStorageConfig();
		if (personalStorageConfigBase.ConfigVersion < VERSION)
		{
			save = true;
			personalStorageConfig.CopyFromBaseClass(personalStorageConfigBase); //! Copy over old data that has not changed.			
			personalStorageConfig.ConfigVersion = VERSION;
						
			if (save)
				Save(personalStorageConfig);
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionPersonalStorageConfig>.Load(fileName, personalStorageConfig))
				return NULL;
		}
		
		return personalStorageConfig;
	}

	void Save()
	{
		Save(this);
	}
	
	static void Save(ExpansionPersonalStorageConfig personalStorageConfig)
	{
		ExpansionJsonFileParser<ExpansionPersonalStorageConfig>.Save(ExpansionPersonalStorageModule.s_PersonalStorageConfigFolderPath + "PersonalStorage_" + personalStorageConfig.StorageID + ".json", personalStorageConfig);
	}
	
	void Spawn()
	{
		Object obj = GetGame().CreateObjectEx(ClassName, Position, ECE_SETUP | ECE_UPDATEPATHGRAPH | ECE_CREATEPHYSICS | ECE_NOLIFETIME);
		
		if (!obj || !obj.IsInherited(ExpansionPersonalStorageBase))
		{
			if (obj)
				GetGame().ObjectDelete(obj);
			Error(ToString() + "::Spawn - Tried to spawn personal storage object with unsuported type name: " + ClassName);
			return;
		}
		
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
	
	bool IsGlobalStorage()
	{
		return IsGlobalStorage;
	}
};