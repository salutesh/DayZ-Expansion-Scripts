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
 * @brief		This module provides misc functionality like doing queued actions for an existing item on server start.
 **/

[CF_RegisterModule(ExpansionItemBaseModule)]
class ExpansionItemBaseModule : CF_ModuleWorld
{
	static const int SETUNLOOTABLE = 1;
	static const int DELETE = 2;

	static ExpansionItemBaseModule s_Instance;

	string m_QueuedEntityActionsFileName;
	ref map<int, ref map<int, ref map<int, ref map<int, int>>>> m_QueuedEntityActions;

	void ExpansionItemBaseModule()
	{
		s_Instance = this;
		m_QueuedEntityActions = new map<int, ref map<int, ref map<int, ref map<int, int>>>>;
	}

	override void OnInit()
	{
		super.OnInit();

		EnableMissionStart();
	}

	override void OnMissionStart(Class sender, CF_EventArgs args)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.GENERAL_ITEMS, this);
#endif
		super.OnMissionStart(sender, args);

#ifdef SERVER
		int instance_id = g_Game.ServerConfigGetInt( "instanceId" );
		string folder = "$mission:storage_" + instance_id + "\\expansion\\";
		m_QueuedEntityActionsFileName = folder + "queuedentityactions.bin";

		if (FileExist(m_QueuedEntityActionsFileName))
			LoadQueuedEntityActions();
		else
			ExpansionStatic.MakeDirectoryRecursive(folder);
#endif
	}

	int ProcessQueuedEntityActions(EntityAI entity, int actionsFilter = int.MAX)
	{
		int b1, b2, b3, b4;
		entity.GetPersistentID(b1, b2, b3, b4);

		int selectedActions;
		if (!PopQueuedEntityActions(b1, b2, b3, b4, actionsFilter, selectedActions))
			return 0;

		EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::ProcessQueuedEntityActions " + entity.ToString() + " - do actions (bitmask) " + selectedActions);

		if ((selectedActions & SETUNLOOTABLE) == SETUNLOOTABLE)
			SetLootable(entity, false);
		if ((selectedActions & DELETE) == DELETE)
			g_Game.ObjectDelete(entity);

		return selectedActions;
	}

	private void LoadQueuedEntityActions()
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.GENERAL_ITEMS, this);
#endif
		if (!FileExist(m_QueuedEntityActionsFileName))
			return;

		FileSerializer file = new FileSerializer;
		if (file.Open(m_QueuedEntityActionsFileName, FileMode.READ))
		{
			int count;

			while (true)
			{
				int b1 = 0, b2 = 0, b3 = 0, b4 = 0, actions = 0;

				file.Read(b1);
				file.Read(b2);
				file.Read(b3);
				file.Read(b4);

				if (!b1 && !b2 && !b3 && !b4)
					break;

				file.Read(actions);

				if (actions > 0)
					AddQueuedEntityActions(b1, b2, b3, b4, actions);
				else
					PopQueuedEntityActions(b1, b2, b3, b4, -actions);

				count++;
			}

			file.Close();

			EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::LoadQueuedEntityActions " + count);
		}

		DeleteFile(m_QueuedEntityActionsFileName);
	}

	static string FormatQueuedEntityActions(int b1, int b2, int b3, int b4, int actions)
	{
		return ExpansionStatic.IntToHex(b1) + " " + ExpansionStatic.IntToHex(b2) + " " + ExpansionStatic.IntToHex(b3) + " " + ExpansionStatic.IntToHex(b4) + " " + actions;
	}

	private void AddQueuedEntityActions(int b1, int b2, int b3, int b4, int actions)
	{
		if (EXTrace.GENERAL_ITEMS)
			EXTrace.Print(true, this, "::AddQueuedEntityActions " + FormatQueuedEntityActions(b1, b2, b3, b4, actions));

		map<int, ref map<int, ref map<int, int>>> m1 = m_QueuedEntityActions[b1];
		if (!m1)
		{
			m1 = new map<int, ref map<int, ref map<int, int>>>;
			m_QueuedEntityActions.Insert(b1, m1);
		}

		map<int, ref map<int, int>> m2 = m1[b2];
		if (!m2)
		{
			m2 = new map<int, ref map<int, int>>;
			m1[b2] = m2;
		}

		map<int, int> m3 = m2[b3];
		if (!m3)
		{
			m3 = new map<int, int>;
			m2[b3] = m3;
		}

		m3[b4] = m3[b4] | actions;  //! Operator '|=' not supported with array accessor...
	}

	private bool PopQueuedEntityActions(int b1, int b2, int b3, int b4, int actionsFilter, out int selectedActions = 0)
	{
		map<int, ref map<int, ref map<int, int>>> m1 = m_QueuedEntityActions[b1];
		if (!m1)
			return false;

		map<int, ref map<int, int>> m2 = m1[b2];
		if (!m2)
			return false;

		map<int, int> m3 = m2[b3];
		if (!m3 || !m3.Contains(b4))
			return false;

		int actions = m3[b4];
		selectedActions = actions & actionsFilter;
		if (!selectedActions)
			return false;

		if (EXTrace.GENERAL_ITEMS)
			EXTrace.Print(true, this, "::PopQueuedEntityActions " + FormatQueuedEntityActions(b1, b2, b3, b4, actionsFilter));

		int actionsRemaining = actions & ~actionsFilter;
		if (!actionsRemaining)
		{
			m3.Remove(b4);
			TruncateQueuedEntityActions(b1, b2, b3, m1, m2, m3);
		}
		else
		{
			m3[b4] = actionsRemaining;
		}

		return true;
	}

	private void TruncateQueuedEntityActions(int b1, int b2, int b3, map<int, ref map<int, ref map<int, int>>> m1, map<int, ref map<int, int>> m2, map<int, int> m3)
	{
		if (!m3.Count())
		{
			m2.Remove(b3);
			if (!m2.Count())
			{
				m1.Remove(b2);
				if (!m1.Count())
				{
					m_QueuedEntityActions.Remove(b1);
					if (!m_QueuedEntityActions.Count())
						EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "::TruncateQueuedEntityActions - queue empty");
				}
			}
		}
	}

	//! @brief queue entity actions for next server start.
	//! @note will only work correctly if OnStoreLoad/Save has been called on the entity at least once otherwise GetPersistentID won't return an actual persistent ID.
	//! Do not use directly in OnStoreLoad! Earliest is AfterStoreLoad or DeferredInit.
	void QueueEntityActions(EntityAI entity, int actions)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(EXTrace.GENERAL_ITEMS, this, entity.ToString());
#endif
		
		int b1, b2, b3, b4;
		entity.GetPersistentID(b1, b2, b3, b4);

		WriteQueuedEntityActions(b1, b2, b3, b4, actions);
	}

	private void WriteQueuedEntityActions(int b1, int b2, int b3, int b4, int actions)
	{
		FileSerializer file = new FileSerializer;
		if (file.Open(m_QueuedEntityActionsFileName, FileMode.APPEND))
		{
			WriteQueuedEntityActions(file, b1, b2, b3, b4, actions);
			file.Close();
		}
	}

	private void WriteQueuedEntityActions(FileSerializer file, int b1, int b2, int b3, int b4, int actions)
	{
		if (EXTrace.GENERAL_ITEMS)
			EXTrace.Print(true, this, "::WriteQueuedEntityActions " + FormatQueuedEntityActions(b1, b2, b3, b4, actions));

		file.Write(b1);
		file.Write(b2);
		file.Write(b3);
		file.Write(b4);
		file.Write(actions);
	}

/*
	private void SaveQueuedEntityActions()
	{
		FileSerializer file = new FileSerializer;
		if (file.Open(m_QueuedEntityActionsFileName, FileMode.WRITE))
		{
			foreach (int b1, map<int, ref map<int, ref map<int, int>>> m1: m_QueuedEntityActions)
			{
				foreach (int b2, map<int, ref map<int, int>> m2: m1)
				{
					foreach (int b3, map<int, int> m3: m2)
					{
						foreach (int b4, int actions: m3)
						{
							WriteQueuedEntityActions(file, b1, b2, b3, b4, actions);
						}
					}
				}
			}
			file.Close();
		}
	}
*/

	static void SetLootable(EntityAI entity, bool state)
	{
		ItemBase item;
		if (Class.CastTo(item, entity))
			item.Expansion_SetLootable(state);
		else
			entity.SetTakeable(state);
	}
};
