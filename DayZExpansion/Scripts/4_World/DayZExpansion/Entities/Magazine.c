/**
 * Magazine.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2021 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

modded class MagazineStorage
{
	override bool CanDetachAttachment (EntityAI parent)
	{
		DayZPlayerImplement player = DayZPlayerImplement.Cast(GetHierarchyRootPlayer());
		if(player)
		{
			Weapon_Base wpn = Weapon_Base.Cast(parent);
			if(wpn)
			{
				return player.GetWeaponManager().CanDetachMagazine(wpn,this);
			}
		}
		return super.CanDetachAttachment(parent);
	}
	
	override void OnInventoryEnter(Man player)
	{		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MagazineStorage::OnInventoryEnter Start");
		#endif
		DayZPlayerImplement p = DayZPlayerImplement.Cast(player);
		p.GetWeaponManager().OnMagazineInventoryEnter(this);

		PlayerBase nplayer;
		if ( PlayerBase.CastTo(nplayer, player) )
		{
			//nplayer.OnItemInventoryEnter(this);
			nplayer.SetEnableQuickBarEntityShortcut(this,true);	
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("MagazineStorage::OnInventoryEnter End");
		#endif
	}
	
	override void OnInventoryExit(Man player)
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MagazineStorage::OnInventoryExit Start");
		#endif
		DayZPlayerImplement p = DayZPlayerImplement.Cast(player);
		p.GetWeaponManager().OnMagazineInventoryExit(this);

		PlayerBase nplayer;
		if ( PlayerBase.CastTo(nplayer,player) )
		{		
			//nplayer.OnItemInventoryExit(this);
			nplayer.SetEnableQuickBarEntityShortcut(this,false);
		}
		
		if(GetGame().IsClient() || !GetGame().IsMultiplayer())
			player.GetHumanInventory().ClearUserReservedLocationForContainer(this);
		
		
		if ( HasEnergyManager() )
		{
			GetCompEM().UpdatePlugState(); // Unplug the el. device if it's necesarry.
		}
		
		#ifdef EXPANSIONEXPRINT
		EXPrint("MagazineStorage::OnInventoryExit End");
		#endif
	}
	
	override void OnWasAttached( EntityAI parent, int slot_id )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MagazineStorage::OnWasAttached Start");
		#endif
		DayZPlayerImplement player = DayZPlayerImplement.Cast(GetHierarchyRootPlayer());
		Weapon_Base wpn = Weapon_Base.Cast(parent);
		if(wpn && player)
		{
			player.GetWeaponManager().OnMagazineAttach(this);
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("MagazineStorage::OnWasAttached End");
		#endif
		/*DayZPlayerImplement player = DayZPlayerImplement.Cast(parent);
		if (player)
			//player.SwitchItemTypeAttach(this, slot_id);
		//{
		//	player.UpdateQuickBarEntityVisibility(this);
		//}*/
		//! Print("OnWasAttached: " + GetType());
	}
	
	override void OnWasDetached( EntityAI parent, int slot_id )
	{
		#ifdef EXPANSIONEXPRINT
		EXPrint("MagazineStorage::OnWasDetached Start");
		#endif
		DayZPlayerImplement player = DayZPlayerImplement.Cast(GetHierarchyRootPlayer());
		Weapon_Base wpn = Weapon_Base.Cast(parent);
		
		if(wpn && player)
		{
			player.GetWeaponManager().OnMagazineDetach(this);
		}
		#ifdef EXPANSIONEXPRINT
		EXPrint("MagazineStorage::OnWasDetached End");
		#endif
	}
}