/**
 * VicinitySlotsContainer.c
 *
 * Credit: @InclementDab
 *
*/
modded class VicinitySlotsContainer
{
	override void DoubleClick(Widget w, int x, int y, int button)
	{
		if (button != MouseState.LEFT) {
			return;
		}

		if (!w) {
			return;
		}

		ItemPreviewWidget iw = ItemPreviewWidget.Cast( w.FindAnyWidget( "Render" ) );
		if (!iw) {
		  string name = w.GetName();
		  name.Replace("PanelWidget", "Render");
		  iw = ItemPreviewWidget.Cast( w.FindAnyWidget( name ) );
		}

		if (!iw) {
		  iw = ItemPreviewWidget.Cast(w);
		}

		ItemBase item = ItemBase.Cast( iw.GetItem() );
		if (!item) {
			return;
		}

		if (GetGame().GetPlayer().GetInventory().HasInventoryReservation(item, null)) {
			return;
		}

		if (!item.GetInventory().CanRemoveEntity()) {
			return;
		}

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

		if (player.GetInventory().HasEntityInInventory(item) && GetGame().GetPlayer().GetHumanInventory().CanAddEntityInHands(item)) {
			player.PredictiveTakeEntityToHands(item);
		} else {
			InventoryLocation dst = new InventoryLocation;
			bool found;
			array<EntityAI> items = {};
			if (player.GetInventory().EnumerateInventory(InventoryTraversalType.PREORDER, items)) {
				foreach (EntityAI search_item: items) {
					if (search_item.CanBeCombined(item)) {
						search_item.CombineItemsClient(item);
						found = true;
					}
				}
			}

			if (!found) {
				player.GetInventory().FindFreeLocationFor( item, FindInventoryLocationType.ANY, dst );
				if (dst.IsValid() && player.GetInventory().LocationCanAddEntity(dst)) {
					SplitItemUtils.TakeOrSplitToInventoryLocation(player, dst);
				}
			}
		}

		ItemManager.GetInstance().HideTooltip();
		InventoryMenu menu = InventoryMenu.Cast( GetGame().GetUIManager().FindMenu( MENU_INVENTORY ) );
		if (menu) {
			menu.RefreshQuickbar();
		}
	}
};