/**
 * ExpansionGarageMenuVehicle.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionGarageMenuVehicle: ExpansionScriptView
{
	protected ref ExpansionGarageMenuVehicleController m_GarageMenuVehicleController;
	protected ref ExpansionGarageMenu m_GarageMenu;
	protected ref ExpansionGarageVehicleData m_Vehicle;
	protected ref ExpansionGarageModule m_GarageModule;

	EntityAI m_Object;
	protected bool m_IsStored;
	
	protected ref ExpansionGarageMenuTooltip m_Tooltip;
	
	protected ButtonWidget garage_vehicle_element_button;
	protected Widget garage_vehicle_price_info_panel;

	void ExpansionGarageMenuVehicle(ExpansionGarageMenu menu, ExpansionGarageVehicleData vehicle, bool isStored = false)
	{
		m_GarageMenu = menu;
		m_Vehicle = vehicle;
		m_IsStored = isStored;

		Class.CastTo(m_GarageMenuVehicleController, GetController());
		Class.CastTo(m_GarageModule, CF_ModuleCoreManager.Get(ExpansionGarageModule));
		
		SetView();
	}

	void ~ExpansionGarageMenuVehicle()
	{
		if (m_Object && m_Object != m_Vehicle.m_VehicleObject)
			GetGame().ObjectDelete(m_Object);
	}

	void SetView()
	{
		if (!m_Vehicle)
			return;

		m_GarageMenuVehicleController.VehicleName = ExpansionStatic.GetItemDisplayNameWithType(m_Vehicle.m_ClassName);
		m_GarageMenuVehicleController.NotifyPropertyChanged("VehicleName");

		if (m_IsStored)
		{
			m_GarageMenuVehicleController.ButtonLabel = "RETRIEVE";
			m_GarageMenuVehicleController.NotifyPropertyChanged("ButtonLabel");
		}
		else
		{
			m_GarageMenuVehicleController.ButtonLabel = "DEPOSIT";
			m_GarageMenuVehicleController.NotifyPropertyChanged("ButtonLabel");
		}

		UpdatePreview();
		
	#ifdef EXPANSIONMODMARKET
		if (GetExpansionSettings().GetGarage().EnableMarketFeatures && GetExpansionSettings().GetMarket().MarketSystemEnabled)
		{
			if (!m_IsStored)
			{
				garage_vehicle_price_info_panel.Show(true);
				SetPrice(m_Vehicle.m_StorePrice);
			}
			else
			{
				garage_vehicle_price_info_panel.Show(false);
			}
		}
	#endif
	}

#ifdef EXPANSIONMODMARKET
	void SetPrice(int price)
	{
		m_GarageMenuVehicleController.Price = price.ToString();
		m_GarageMenuVehicleController.NotifyPropertyChanged("Price");
	}
#endif
	
	override string GetLayoutFile()
	{
		return "DayZExpansion/Garage/GUI/layouts/expansion_garage_menu_vehicle_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionGarageMenuVehicleController;
	}

	void UpdatePreview()
	{
		if (m_IsStored || !m_Vehicle.m_VehicleObject)
		{
			CreatePreviewObject(m_Vehicle.m_ClassName, m_Object);
			if (m_Object)
			{
				Transport transportEntity;
				if (Class.CastTo(transportEntity, m_Object))
				{
					dBodyActive(m_Object, ActiveState.INACTIVE);
					dBodyDynamic(m_Object, false);
					transportEntity.DisableSimulation(true);
				}

				if (m_Object.HasSelection("antiwater"))
					m_Object.HideSelection("antiwater");

				ExpansionVehicle vehicle;
				if (m_Vehicle.m_SkinIndex > -1 && ExpansionVehicle.Get(vehicle, m_Object) && vehicle.HasSkin(m_Vehicle.m_SkinIndex))
					vehicle.SetSkin(m_Vehicle.m_SkinIndex);
			}
		}
		else
		{
			m_Object = EntityAI.Cast(m_Vehicle.m_VehicleObject);
		}

		m_GarageMenuVehicleController.Preview = m_Object;
		m_GarageMenuVehicleController.NotifyPropertyChanged("Preview");
	}

	void CreatePreviewObject(string className, inout EntityAI preview)
	{
		if (preview)
		{
			if (CF_String.EqualsIgnoreCase(preview.GetType(), className))
			{
				//! Same classname
				return;
			}
			else
			{
				//! Different classname, delete old preview object
				GetGame().ObjectDelete(preview);
			}
		}

		if (!GetGame().IsKindOf(className, "DZ_LightAI"))
		{
			preview = EntityAI.Cast(GetGame().CreateObjectEx(className, vector.Zero, ECE_LOCAL|ECE_NOLIFETIME));
		#ifdef EXPANSIONMODHARDLINE
			ItemBase item;
			if (GetExpansionSettings().GetHardline().EnableItemRarity && Class.CastTo(item, preview))
				item.Expansion_SetRarity(GetExpansionSettings().GetHardline().GetItemRarityByType(item.GetType()));
		#endif
		}
	}

	void OnButtonClick()
	{
		if (!m_GarageMenu)
			return;

		if (!m_IsStored)
			m_GarageMenu.RequestDeposit(m_Vehicle);
		else
			m_GarageMenu.RequestRetrieve(m_Vehicle);
	}
	
	void CreateTooltip()
	{
		if (!m_Tooltip)
		{
			m_Tooltip = new ExpansionGarageMenuTooltip();
			
			if (m_IsStored)
			{
				m_Tooltip.SetTitle("Stored Vehicle Info");  //! TODO localize
				//m_Tooltip.SetText("");
				m_Tooltip.AddEntry(ExpansionStatic.VectorToString(m_Vehicle.m_Position, ExpansionVectorToString.Labels), ARGB(255, 160, 223, 59));
			}
			else
			{
				m_Tooltip.SetTitle("World Vehicle Info");  //! TODO localize
				//m_Tooltip.SetText("");
				m_Tooltip.AddEntry(ExpansionStatic.VectorToString(m_Vehicle.m_Position, ExpansionVectorToString.Labels), ARGB(255, 160, 223, 59));
			}
			
			m_Tooltip.Hide();
		}
	}

	void DestroyTooltip()
	{
		if (m_Tooltip)
		{
			m_Tooltip.Destroy();
		}
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case garage_vehicle_element_button:
			CreateTooltip();
			m_Tooltip.Show();
			break;
		}
		
		return super.OnMouseEnter(w, x, y);
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case garage_vehicle_element_button:
			DestroyTooltip();
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
};

class ExpansionGarageMenuVehicleController: ExpansionViewController
{
	string VehicleName;
	Object Preview;
	string ButtonLabel;
	string Price;
};