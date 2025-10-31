/**
 * ExpansionP2PMarketContainerItem.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2025 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionP2PMarketContainerItem: ExpansionP2PMarketListingBase
{
	protected bool m_IsWeapon = false;
	protected bool m_IsMagazine = false;
	protected bool m_IsAttached = false;
	
	override void SetFromItem(EntityAI object, PlayerBase owner = null)
	{
		super.SetFromItem(object, owner);

		if (object.IsWeapon())
			m_IsWeapon = true;

		if (object.GetInventory().IsAttachment())
		{
			if (!object.GetHierarchyParent().IsInherited(SurvivorBase))
				m_IsAttached = true;
		}

		if (object.IsInherited(MagazineStorage))
			m_IsMagazine = true;
	}

	bool IsAttached()
	{
		return m_IsAttached;
	}

	bool IsMagazine()
	{
		return m_IsMagazine;
	}

	bool IsWeapon()
	{
		return m_IsWeapon;
	}
	
	void OnLoad()
	{
		if (m_SkinName)
			m_SkinIndex = ExpansionSkinModule.s_Instance.GetSkinIndex(m_ClassName, m_SkinName);

		foreach (auto containerItem: m_ContainerItems)
		{
			containerItem.OnLoad();
		}
	}
	
	void OnSendBasic(ParamsWriteContext ctx)
	{
		OnSendClassName(ctx);

		ctx.Write(m_Quantity);
		ctx.Write(m_SkinIndex);
		ctx.Write(m_QuantityType);
		ctx.Write(m_HealthLevel);
		ctx.Write(m_LiquidType);
		ctx.Write(m_IsBloodContainer);
		ctx.Write(m_FoodStageType);
		#ifdef EXPANSIONMODHARDLINE
		ctx.Write(m_Rarity);
		#endif
		
		int containerItemsCount = m_ContainerItems.Count();
		ctx.Write(containerItemsCount);
		for (int j = 0; j < containerItemsCount; j++)
		{
			ExpansionP2PMarketContainerItem containerItem = m_ContainerItems[j];
			containerItem.OnSendBasic(ctx);
		}
	}
	
	bool OnRecieveBasic(ParamsReadContext ctx)
	{
		if (!OnReceiveClassName(ctx))
			return false;
		
		if (!ctx.Read(m_Quantity))
			return false;
		
		if (!ctx.Read(m_SkinIndex))
			return false;
		
		if (!ctx.Read(m_QuantityType))
			return false;
		
		if (!ctx.Read(m_HealthLevel))
			return false;
		
		if (!ctx.Read(m_LiquidType))
			return false;
		
		if (!ctx.Read(m_IsBloodContainer))
			return false;
		
		if (!ctx.Read(m_FoodStageType))
			return false;
		
		#ifdef EXPANSIONMODHARDLINE
		if (!ctx.Read(m_Rarity))
			return false;
		#endif
		
		int containerItemsCount;
		if (!ctx.Read(containerItemsCount))
			return false;

		if (m_ContainerItems.Count())
			m_ContainerItems.Clear();

		for (int i = 0; i < containerItemsCount; ++i)
		{
			ExpansionP2PMarketContainerItem containerItem = new ExpansionP2PMarketContainerItem();
			if (!containerItem.OnRecieveBasic(ctx))
				return false;

			m_ContainerItems.Insert(containerItem);
		}		
		
		return true;
	}
	
	void OnSendDetails(ParamsWriteContext ctx)
	{
		OnSendBasic(ctx);

		ctx.Write(m_IsWeapon);
		ctx.Write(m_IsMagazine);
		ctx.Write(m_IsAttached);
	}
	
	bool OnRecieveDetails(ParamsReadContext ctx)
	{
		if (!OnRecieveBasic(ctx))
			return false;
		
		if (!ctx.Read(m_IsWeapon))
			return false;
		
		if (!ctx.Read(m_IsMagazine))
			return false;
		
		if (!ctx.Read(m_IsAttached))
			return false;
		
		return true;
	}
};