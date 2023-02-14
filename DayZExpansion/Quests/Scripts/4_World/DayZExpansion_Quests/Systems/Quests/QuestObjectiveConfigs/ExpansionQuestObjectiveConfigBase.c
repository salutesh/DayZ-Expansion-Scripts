/**
 * ExpansionQuestObjectiveConfigBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionQuestObjectiveConfig
{
	static const int CONFIGVERSION = 13;
	int ConfigVersion;
	int ID = -1; //! Unique objective ID
	int ObjectiveType = ExpansionQuestObjectiveType.NONE; //! Quest obecjtive type.
	string ObjectiveText = string.Empty;
	int TimeLimit = - 1;

	[NonSerialized()];
	protected ref array<string> m_ObjectiveAllocationClasses;

	void ExpansionQuestObjectiveConfig()
	{
		ConfigVersion = CONFIGVERSION;
	}

	void SetID(int id)
	{
		ID = id;
	}

	int GetID()
	{
		return ID;
	}

	void SetObjectiveType(int type)
	{
		ObjectiveType = type;
	}

	int GetObjectiveType()
	{
		return ObjectiveType;
	}

	void SetObjectiveText(string text)
	{
		ObjectiveText = text;
	}

	string GetObjectiveText()
	{
		return ObjectiveText;
	}

	void SetTimeLimit(int time)
	{
		TimeLimit = time;
	}

	int GetTimeLimit()
	{
		return TimeLimit;
	}

	int GetAmount()
	{
		return -1;
	}

	array<string> GetClassNames()
	{
		return NULL;
	}

	ExpansionQuestObjectiveTarget GetTarget()
	{
		return NULL;
	}

	vector GetPosition()
	{
		return vector.Zero;
	}

	float GetMaxDistance()
	{
		return -1;
	}

	string GetMarkerName()
	{
		return string.Empty;
	}

	array<string> GetActionNames()
	{
		return NULL;
	}

	array<string> GetItemNames()
	{
		return NULL;
	}

	float GetMaxDistRadius()
	{
		return 50;
	}

	float GetMinDistRadius()
	{
		return 150;
	}

	float GetDespawnRadius()
	{
		return 880;
	}

	bool CanLootAI()
	{
		return false;
	}

	array<ref ExpansionQuestObjectiveDelivery> GetDeliveries()
	{
		return NULL;
	}

	ExpansionQuestObjectiveTreasureHunt GetTreasureHunt()
	{
		return NULL;
	}

#ifdef EXPANSIONMODAI
	ExpansionQuestObjectiveAIPatrol GetAIPatrol()
	{
		return NULL;
	}

	ExpansionQuestObjectiveAICamp GetAICamp()
	{
		return NULL;
	}

	ExpansionQuestObjectiveAIVIP GetAIVIP()
	{
		return NULL;
	}
#endif

	array<string> GetAllowedClassNames()
	{
		return NULL;
	}

	array<string> GetExcludedClassNames()
	{
		return NULL;
	}

	int GetExecutionAmount()
	{
		return -1;
	}

#ifdef EXPANSIONMODMARKET
	bool AddItemsToNearbyMarketZone()
	{
		return false;
	}
#endif
	
	bool TriggerOnEnter()
	{
		return false;
	}
	
	bool TriggerOnExit()
	{
		return false;
	}

	void CollectAllocationClasses()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::CollectAllocationClasses - Start");
	#endif

		int objectiveType = GetObjectiveType();
		string baseClassName;
		array<string> validClassNames = new array<string>;
		switch (objectiveType)
		{
			case ExpansionQuestObjectiveType.ACTION:
			{
				validClassNames.Insert("ActionBase");
			}
			break;
			case ExpansionQuestObjectiveType.CRAFTING:
			{
				validClassNames.Insert("RecipeBase");
				validClassNames.Insert("ItemBase");
			}
			break;
			case ExpansionQuestObjectiveType.TARGET:
			{
				ExpansionQuestObjectiveTargetConfig targetConfig;
				if (Class.CastTo(targetConfig, this))
				{
					ExpansionQuestObjectiveTarget target = targetConfig.GetTarget();
					if (!target)
						return;

					validClassNames = target.GetClassNames();
				}
			}
			break;
			case ExpansionQuestObjectiveType.DELIVERY:
			{
				validClassNames.Insert("ItemBase");
			}
			break;
			case ExpansionQuestObjectiveType.COLLECT:
			{
				validClassNames.Insert("ItemBase");
			}
			break;
			case ExpansionQuestObjectiveType.TREASUREHUNT:
			{
				validClassNames.Insert("ItemBase");
			}
			break;
		#ifdef EXPANSIONMODAI
			case ExpansionQuestObjectiveType.AICAMP:
			{
				validClassNames.Insert("SurvivorBase");
			}
			break;
			case ExpansionQuestObjectiveType.AIPATROL:
			{
				validClassNames.Insert("SurvivorBase");
			}
			break;
			case ExpansionQuestObjectiveType.AIESCORD:
			{
				validClassNames.Insert("SurvivorBase");
				validClassNames.Insert("ActionBase");
			}
			break;
		#endif
		}

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		foreach (string name: validClassNames)
		{
			Print(ToString() + "::CollectAllocationClasses - Valid Class-Names: " + name);
		}
	#endif

		if (!validClassNames || validClassNames.Count() == 0)
			return;

		foreach (string className: validClassNames)
		{
			if (GetGame().ConfigIsExisting("CfgVehicles " + className))
			{
				GetGame().ConfigGetBaseName("CfgVehicles " + className, baseClassName);
				if (baseClassName != string.Empty)
				{
				#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
					Print(ToString() + "::CollectAllocationClasses - Name: " + baseClassName);
				#endif
					AllocationClassNameCheck(baseClassName);
				}
				else
				{
					//! Should never happening?!
					Error(ToString() + "::CollectAllocationClasses - Base class name for " + className + " is EMPTY?!");
				}
			}
			else
			{
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::CollectAllocationClasses - " + className + " is not a valid class name or has no RV-Config.");
			#endif
				string type = className;
				typename typeName = type.ToType();
			#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
				Print(ToString() + "::CollectAllocationClasses - Result type name: " + typeName.ToString());
			#endif
				AllocationTypeNameCheck(typeName);
			}
		}

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::CollectAllocationClasses - End");
	#endif
	}

	void AllocationClassNameCheck(string className)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::AllocationClassNameCheck - Start");
		Print(ToString() + "::AllocationClassNameCheck - " + className);
	#endif

		bool added;
		string baseName;
		if (InheritsFrom(className, "ZombieBase"))
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationClassNameCheck - ZombieBase");
		#endif
			baseName = "ZombieBase";
			added = true;
		}
		else if (InheritsFrom(className, "AnimalBase"))
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationClassNameCheck - AnimalBase");
		#endif
			baseName = "AnimalBase";
			added = true;
		}
		else if (className == "Man")
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationClassNameCheck - PlayerBase");
		#endif
			baseName = "PlayerBase";
			added = true;
		}
		else if (className == "ItemBase")
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationClassNameCheck - ItemBase");
		#endif
			baseName = className;
			added = true;
		}
		else if (className == "ActionBase")
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationClassNameCheck - ActionBase");
		#endif
			baseName = className;
			added = true;
		}
		else if (className == "RecipeBase")
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationClassNameCheck - RecipeBase");
		#endif
			baseName = className;
			added = true;
		}

		if (added)
		{
			if (!m_ObjectiveAllocationClasses)
				m_ObjectiveAllocationClasses = new array<string>;

			int index = m_ObjectiveAllocationClasses.Find(baseName);
			if (index == -1)
				m_ObjectiveAllocationClasses.Insert(baseName);
		}

		Print(ToString() + "::AllocationClassNameCheck - End");
	}

	void AllocationTypeNameCheck(typename typeName)
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::AllocationTypeNameCheck - Start");
		Print(ToString() + "::AllocationTypeNameCheck - " + typeName);
	#endif

		bool added;
		string baseName;
		if (typeName.IsInherited(ZombieBase))
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationTypeNameCheck - ZombieBase");
		#endif
			baseName = "ZombieBase";
			added = true;
		}
		else if (typeName.IsInherited(AnimalBase))
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationTypeNameCheck - AnimalBase");
		#endif
			baseName = "AnimalBase";
			added = true;
		}
		else if (typeName.IsInherited(Man) || typeName.ToString() == "Man")
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationTypeNameCheck - PlayerBase");
		#endif
			baseName = "PlayerBase";
			added = true;
		}
		else if (typeName.IsInherited(ItemBase) || typeName.ToString() == "ItemBase")
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationTypeNameCheck - ItemBase");
		#endif
			baseName = "ItemBase";
			added = true;
		}
		else if (typeName.IsInherited(ActionBase) || typeName.ToString() == "ActionBase")
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationTypeNameCheck - ActionBase");
		#endif
			baseName = "ActionBase";
			added = true;
		}
		else if (typeName.IsInherited(RecipeBase) || typeName.ToString() == "RecipeBase")
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::AllocationTypeNameCheck - RecipeBase");
		#endif
			baseName = "RecipeBase";
			added = true;
		}

		if (added)
		{
			if (!m_ObjectiveAllocationClasses)
				m_ObjectiveAllocationClasses = new array<string>;

			int index = m_ObjectiveAllocationClasses.Find(baseName);
			if (index == -1)
				m_ObjectiveAllocationClasses.Insert(baseName);
		}

	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print(ToString() + "::AllocationClassNameCheck - End");
	#endif
	}

	bool InheritsFrom(string value, string baseclass)
	{
		auto hitch = EXHitch(ToString() + "::InheritsFrom - ");

		string child = value;
		string parent;
		while (GetGame().ConfigGetBaseName("cfgVehicles " + child, parent))
		{
		#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
			Print(ToString() + "::InheritsFrom - Value: " + child + " | Parent: " + parent);
		#endif
			if (parent == baseclass)
			{
				return true;
			}
			child = parent;
		}

		return false;
	}

	array<string> GetAllocationClasseNames()
	{
		return m_ObjectiveAllocationClasses;
	}

	void Save(string fileName);

	void OnSend(ParamsWriteContext ctx)
	{
		ctx.Write(ID);
		ctx.Write(ObjectiveType);
		ctx.Write(ObjectiveText);
		ctx.Write(TimeLimit);
	}

	bool OnRecieve(ParamsReadContext ctx)
	{
		if (!ctx.Read(ID))
			return false;

		if (!ctx.Read(ObjectiveType))
			return false;

		if (!ctx.Read(ObjectiveText))
			return false;

		if (!ctx.Read(TimeLimit))
			return false;

		return true;
	}

	bool Validate()
	{
		if (ConfigVersion != CONFIGVERSION)
			return false;

		if (ID <= -1)
			return false;

		if (ObjectiveType == ExpansionQuestObjectiveType.NONE)
			return false;

		return true;
	}

	void QuestDebug()
	{
	#ifdef EXPANSIONMODQUESTSOBJECTIVEDEBUG
		Print("------------------------------------------------------------");
		Print(ToString() + "::QuestDebug - Objective config version: " + ConfigVersion);
		Print(ToString() + "::QuestDebug - Objective ID: " + ID);
		Print(ToString() + "::QuestDebug - ObjectiveType: " + ObjectiveType);
		Print(ToString() + "::QuestDebug - ObjectiveText: " + ObjectiveText);
		Print(ToString() + "::QuestDebug - TimeLimit: " + TimeLimit);
		Print("------------------------------------------------------------");
	#endif
	}
};