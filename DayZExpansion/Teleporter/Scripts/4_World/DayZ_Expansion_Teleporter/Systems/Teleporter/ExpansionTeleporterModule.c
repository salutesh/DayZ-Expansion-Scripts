/**
 * ExpansionTeleporterModule.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionTeleportPosition
{
	vector m_Position;
	vector m_Orientation;
	
	void ExpansionTeleportPosition(vector pos, vector ori)
	{
		m_Position = pos;
		m_Orientation = ori;
	}
	
	vector GetPosition()
	{
		return m_Position;
	}
	
	vector GetOrientation()
	{
		return m_Orientation;
	}
};

class ExpansionTeleportDataBase 
{
	int ConfigVersion;
};

class ExpansionTeleportData: ExpansionTeleportDataBase
{
	[NonSerialized()];
	static const int VERSION = 0;
	
	protected int m_ID;
	protected vector m_ObjectPosition;
	protected vector m_ObjectOrientation;
	protected ref array<ref ExpansionTeleportPosition> m_TeleportPositions;

#ifdef EXPANSIONMODAI
	protected string m_FactionName;
#endif
#ifdef EXPANSIONMODHARDLINE
	protected int m_Reputation;
#endif
#ifdef EXPANSIONMODQUESTS
	protected int m_QuestID = -1;
#endif
	
	void ExpansionTeleportData()
	{
		ConfigVersion = VERSION;
		
		if (!m_TeleportPositions)
			m_TeleportPositions = new array<ref ExpansionTeleportPosition>;
	}
	
	void CopyFromBaseClass(ExpansionTeleportDataBase base)
	{
		//! Nothing to do here yet
	}
	
	static ExpansionTeleportData Load(string fileName)
	{
		CF_Log.Info("[ExpansionTeleportData] Load existing teleporter file:" + fileName);
		ExpansionTeleportDataBase teleporterDataBase;
		ExpansionJsonFileParser<ExpansionTeleportDataBase>.Load(fileName, teleporterDataBase);
		
		bool save;
		ExpansionTeleportData teleporterData = new ExpansionTeleportData();
		if (teleporterDataBase.ConfigVersion < VERSION)
		{
			save = true;
			teleporterData.CopyFromBaseClass(teleporterDataBase); //! Copy over old data that has not changed.			
			teleporterData.ConfigVersion = VERSION;
						
			if (save)
				Save(teleporterData);
		}
		else
		{
			if (!ExpansionJsonFileParser<ExpansionTeleportData>.Load(fileName, teleporterData))
				return NULL;
		}
		
		return teleporterData;
	}
	
	void Save()
	{
		ExpansionJsonFileParser<ExpansionTeleportData>.Save(ExpansionTeleporterModule.s_TeleporterDataFolderPath + "Teleporter_" + m_ID + ".json", this);
	}
	
	static void Save(ExpansionTeleportData teleporterData)
	{
		ExpansionJsonFileParser<ExpansionTeleportData>.Save(ExpansionTeleporterModule.s_TeleporterDataFolderPath + "Teleporter_" + teleporterData.GetID() + ".json", teleporterData);
	}
	
	void AddTeleportPosition(vector pos, vector ori)
	{
		ExpansionTeleportPosition teleportPos = new ExpansionTeleportPosition(pos, ori);
		m_TeleportPositions.Insert(teleportPos);
	}
	
	void SetID(int id)
	{
		m_ID = id;
	}
	
	int GetID()
	{
		return m_ID;
	}
	
	void SetObjectPosition(vector objPos)
	{
		m_ObjectPosition = objPos;
	}
	
	void SetObjectOrientation(vector objOri)
	{
		m_ObjectOrientation = objOri;
	}
	
	vector GetObjectPosition()
	{
		return m_ObjectPosition;
	}
	
	vector GetObjectOrientation()
	{
		return m_ObjectOrientation;
	}
	
#ifdef EXPANSIONMODAI
	void SetFactionName(string factionName)
	{
		m_FactionName = factionName;
	}

	string GetFactionName()
	{
		return m_FactionName;
	}
#endif
	
#ifdef EXPANSIONMODHARDLINE
	void SetReputation(int reputation)
	{
		m_Reputation = reputation;
	}
	
	int GetReputation()
	{
		return m_Reputation;
	}
#endif
	
#ifdef EXPANSIONMODQUESTS
	void SetQuestID(int questID)
	{
		m_QuestID = questID;
	}

	int GetQuestID()
	{
		return m_QuestID;
	}
#endif
	
	array<ref ExpansionTeleportPosition> GetTeleportPositions()
	{
		return m_TeleportPositions;
	}
	
	void SpawnTeleporter()
	{
		Object obj = GetGame().CreateObjectEx("Expansion_Teleporter_Big", m_ObjectPosition, ECE_NONE);
		Expansion_Teleporter_Big teleportObj = Expansion_Teleporter_Big.Cast(obj);
		if (!teleportObj)
			GetGame().ObjectDelete(obj);
		
		teleportObj.SetPosition(m_ObjectPosition);
		teleportObj.SetOrientation(m_ObjectOrientation);
		teleportObj.SetTeleportData(this);
	}
};

[CF_RegisterModule(ExpansionTeleporterModule)]
class ExpansionTeleporterModule: CF_ModuleWorld
{
	protected static ExpansionTeleporterModule s_Instance;
	static string s_TeleporterDataFolderPath = "$mission:expansion\\teleporter\\";

	void ExpansionTeleporterModule()
	{
		s_Instance = this;
	}

	override void OnInit()
	{
		EnableMissionStart();
		EnableRPC();
	}

	protected void CreateDirectoryStructure()
	{
		if (!FileExist(s_TeleporterDataFolderPath))
			ExpansionStatic.MakeDirectoryRecursive(s_TeleporterDataFolderPath);
	}
	
	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
		if (GetGame().IsServer() && GetGame().IsMultiplayer())
		{		
			CreateDirectoryStructure();
			LoadTeleporterServerData();
		}
	}
	
	protected void LoadTeleporterServerData()
	{
		array<string> teleporterFiles = ExpansionStatic.FindFilesInLocation(s_TeleporterDataFolderPath, ".json");
		if (teleporterFiles && teleporterFiles.Count() > 0)
		{
			foreach (string fileName: teleporterFiles)
			{
				GetTeleporterData(fileName, s_TeleporterDataFolderPath);
			}
		}
		else
		{
			CreateDefaultTeleporterData();
		}
	}

	protected void CreateDefaultTeleporterData()
	{
	#ifdef EXPANSION_NAMALSK_ADVENTURE		
		ExpansionTeleportData teleporterData = new ExpansionTeleportData();
		teleporterData.SetID(1);
		teleporterData.SetObjectPosition(Vector(8551.52, 15.643, 10530.8));
		teleporterData.SetObjectOrientation(Vector(-53.5181, 0, 0));
		teleporterData.AddTeleportPosition(Vector(5079.96, 2085.61, 11720.7),  Vector(39.18, 0, 0));
		teleporterData.AddTeleportPosition(Vector(5075.36, 2085.61, 11715.3),  Vector(132.59, 0, 0));
		teleporterData.Save();

		teleporterData.SpawnTeleporter();

		teleporterData = new ExpansionTeleportData();
		teleporterData.SetID(2);
		teleporterData.SetObjectPosition(Vector(5080.87, 2085.49, 11714.6));
		teleporterData.SetObjectOrientation(Vector(130.388, 0, 0));
		teleporterData.AddTeleportPosition(Vector(8564.36, 14.8901, 10509.8),  Vector(0, 0, 0));
		teleporterData.AddTeleportPosition(Vector(8599.56, 14.793, 10511.9),  Vector(0, 0, 0));
		teleporterData.Save();

		teleporterData.SpawnTeleporter();
	#endif
	}
	
	protected void GetTeleporterData(string fileName, string path)
	{
		ExpansionTeleportData teleporterData = ExpansionTeleportData.Load(path + fileName);
		if (!teleporterData)
			return;

		teleporterData.SpawnTeleporter(); //! Spawn the teleporter.
	}
	
	override int GetRPCMin()
	{
		return ExpansionTeleporterModuleRPC.INVALID;
	}

	override int GetRPCMax()
	{
		return ExpansionTeleporterModuleRPC.COUNT;
	}

	override void OnRPC(Class sender, CF_EventArgs args)
	{
		super.OnRPC(sender, args);
		auto rpc = CF_EventRPCArgs.Cast(args);

		switch (rpc.ID)
		{
			case ExpansionTeleporterModuleRPC.PlayTeleportSound:
			{
				RPC_PlayTeleportSound(rpc.Context, rpc.Sender, rpc.Target);
				break;
			}
		}
	}
	
	void PlayTeleportSound(vector teleportPos)
	{
		if (!GetGame().IsServer() && !GetGame().IsMultiplayer())
		{
			Error(ToString() + "::PlayTeleportSound - Tryed to call PlayTeleportSound on Client!");
			return;
		}
		
		auto rpc = ExpansionScriptRPC.Create();
		rpc.Write(teleportPos);
		
		array<Object> objects = new array<Object>;
		GetGame().GetObjectsAtPosition3D(teleportPos, 300, objects, null);
		
		foreach (Object obj: objects)
		{
			PlayerBase player = PlayerBase.Cast(obj);
			if (player && player.GetIdentity())
				rpc.Send(NULL, ExpansionTeleporterModuleRPC.PlayTeleportSound, true, player.GetIdentity());
		}
	}
	
	protected void RPC_PlayTeleportSound(ParamsReadContext ctx, PlayerIdentity senderRPC, Object target)
	{
		if (!ExpansionScriptRPC.CheckMagicNumber(ctx))
		{
			Error(ToString() + "::RPC_PlayTeleportSound - Magic number check failed!");
			return;
		}

		if (!GetGame().IsClient())
		{
			Error(ToString() + "::RPC_PlayTeleportSound - Tryed to call RPC_PlayTeleportSound on Server!");
			return;
		}
		
		vector teleportPos;
		if (!ctx.Read(teleportPos))
		{
			Error(ToString() + "::RPC_PlayTeleportSound - Could not read teleportPos");
			return;
		}

	#ifndef EDITOR
		EffectSound sound = SEffectManager.PlaySound("Blowout_Teleport", teleportPos, 0, 0, false);
		if (!sound)
			return;

		sound.SetParent(target);
		sound.SetSoundAutodestroy(true);
	#endif
	}

	static ExpansionTeleporterModule GetModuleInstance()
	{
		return s_Instance;
	}
};
