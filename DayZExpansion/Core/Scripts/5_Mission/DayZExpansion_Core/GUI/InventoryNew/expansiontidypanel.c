class ExpansionTidyPanel
{
	protected Managed m_Owner;
	protected EntityAI m_Entity;
	protected Widget m_Panel;
	protected ButtonWidget m_Btn;
	protected ImageWidget m_BtnIcon;
	protected bool m_HasCallbacks;

	void ExpansionTidyPanel(Managed owner, Widget parent)
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "ExpansionTidyPanel " + owner + " " + parent);
	#endif

		if (parent)
		{
			m_Owner = owner;
			m_Panel = g_Game.GetWorkspace().CreateWidgets("DayZExpansion/Core/GUI/layouts/expansion_inventory_tidy.layout", parent);
			m_Btn = ButtonWidget.Cast(m_Panel.FindAnyWidget("TidyBtn"));
			m_BtnIcon = ImageWidget.Cast(m_Panel.FindAnyWidget("TidyBtnIcon"));
			WidgetEventHandler.GetInstance().RegisterOnClick(m_Btn, this, "Tidy");
			m_Panel.Show(false);
		}
	}

	void ~ExpansionTidyPanel()
	{
	#ifdef EXTRACE
		EXTrace trace;
	#endif

		if (g_Game)
		{
		#ifdef EXTRACE
			trace = EXTrace.Start(EXTrace.GENERAL_ITEMS, this);
		#endif

			RemoveCallbacks();
		}
	}

	void InsertCallbacks()
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "InsertCallbacks " + m_Owner + " " + m_Entity + " " + m_HasCallbacks);
	#endif

		if (!m_HasCallbacks)
		{
			ItemBase.s_Expansion_UpdateTidyPanelVisibilityExSI.Insert(UpdateVisibilityEx);
			ExpansionSettings.SI_Core.Insert(UpdateVisibility);
			m_HasCallbacks = true;
		}
	}

	void RemoveCallbacks()
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "RemoveCallbacks " + m_Owner + " " + m_Entity + " " + m_HasCallbacks);
	#endif

		if (m_HasCallbacks)
		{
			ItemBase.s_Expansion_UpdateTidyPanelVisibilityExSI.Remove(UpdateVisibilityEx);
			ExpansionSettings.SI_Core.Remove(UpdateVisibility);
			m_HasCallbacks = false;
		}
	}

	void SetEntity(EntityAI entity)
	{
	#ifdef EXTRACE
		EXTrace trace;
	#endif

		if (m_Entity != entity && m_Panel)
		{
		#ifdef EXTRACE
			trace = EXTrace.Start(EXTrace.GENERAL_ITEMS, this, "" + entity);
		#endif

			m_Entity = entity;

			if (m_Entity)
			{
				CargoBase cargo = m_Entity.GetInventory().GetCargo();
				if (cargo && cargo.GetWidth() > 0 && cargo.GetHeight() > 0)
				{
					UpdateVisibility();
					InsertCallbacks();
				}
				else
				{
					m_Panel.Show(false);
					RemoveCallbacks();
				}
			}
		}
	}

	void UpdateVisibility()
	{
	#ifdef DIAG_DEVELOPER
		EXTrace.Print(EXTrace.GENERAL_ITEMS, this, "UpdateVisibility " + m_Owner + " " + m_Entity);
	#endif

		auto settings = GetExpansionSettings().GetCore(false);
		if (settings.IsLoaded())
			m_Panel.Show(settings.EnableInventoryCargoTidy);

		m_Btn.Enable(true);
	}

	void UpdateVisibilityEx(EntityAI entity)
	{
		if (m_Entity == entity)
			UpdateVisibility();
	}

	void Tidy()
	{
		if (!m_Entity)
			return;

		m_Btn.Enable(false);

		ExpansionInventoryTools.RequestTidy(m_Entity, false);
	}
}

modded class Header
{
	protected ref ExpansionTidyPanel m_Expansion_TidyPanel;

	override void SetItemPreview(EntityAI entity_ai)
	{
		super.SetItemPreview(entity_ai);

		if (m_Expansion_TidyPanel)
			m_Expansion_TidyPanel.SetEntity(entity_ai);
	}
}

modded class ClosableHeader
{
	void ClosableHeader(LayoutHolder parent, string function_name)
	{
		m_Expansion_TidyPanel = new ExpansionTidyPanel(this, m_PanelWidget);
	}
}

modded class Attachments
{
	EntityAI Expansion_GetEntity()
	{
		return m_Entity;
	}
}

modded class AttachmentsWrapper
{
	protected ref ExpansionTidyPanel m_Expansion_TidyPanel;

	void AttachmentsWrapper(LayoutHolder parent)
	{
		//! lol vanilla typos :>
		m_Expansion_TidyPanel = new ExpansionTidyPanel(this, m_RootWidget.FindAnyWidget("arrachments_wrapper_header"));
	}

	override void SetParent(Attachments atts)
	{
		super.SetParent(atts);

		if (m_Expansion_TidyPanel)
		{
			EntityAI entity = atts.Expansion_GetEntity();
			if (entity)
			{
				EntityAI parent = entity.GetHierarchyParent();
				if (parent && !parent.IsMan())
					m_Expansion_TidyPanel.SetEntity(entity);
			}
		}
	}
}

modded class HandsHeader
{
	void HandsHeader(LayoutHolder parent, string function_name)
	{
		m_Expansion_TidyPanel = new ExpansionTidyPanel(this, m_ItemHeader);
	}
	
	override void UpdateInterval()
	{
		super.UpdateInterval();

		DayZPlayer player = g_Game.GetPlayer();
		if (player)
			m_Expansion_TidyPanel.SetEntity(player.GetHumanInventory().GetEntityInHands());
	}
}
