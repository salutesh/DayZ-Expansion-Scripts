/**
 * InspectMenuNew.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

#ifdef EXPANSION_INSPECT_MENU_NEW_ENABLE
//! This is basically how vanilla SHOULD have done it
modded class InspectMenuNew
{
	override static void UpdateItemInfoLiquidType(Widget root_widget, EntityAI item)
	{
#ifdef EXPANSIONTRACE
		auto trace = CF_Trace_2(ExpansionTracing.UI, "InspectMenuNew", "UpdateItemInfoLiquidType").Add(root_widget).Add(item);
#endif

		if ( item.IsInherited( ZombieBase ) || item.IsTransport() ) return;
		
		ItemBase item_base = ItemBase.Cast( item );
		
		int color;

		if( item_base && item_base.GetQuantity() > 0 && item_base.IsBloodContainer() )
		{
			BloodContainerBase blood_container = BloodContainerBase.Cast( item_base );
			
			if (GetExpansionClientSettings().EnableLiquidTypeColors)
				color = Colors.RED;
			else
				color = Colors.COLOR_LIQUID;

			if( blood_container.GetBloodTypeVisible() )
			{
				string type;
				bool positive;
				string blood_type_name = BloodTypes.GetBloodTypeName(blood_container.GetLiquidType(), type, positive);
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "#inv_inspect_blood: " + blood_type_name, color);
			}
			else
			{
				WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "#inv_inspect_blood", color);
			}
		}
		else if( item_base && item_base.GetQuantity() > 0 && item_base.IsLiquidContainer() )
		{
			int liquid_type = item_base.GetLiquidType();

			string text = ExpansionWorld.GetLiquidDisplayName(liquid_type, true, color);

			WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", text, color);
		}
		else
		{
			WidgetTrySetText(root_widget, "ItemLiquidTypeWidget", "");
		}
	}

	override static void WidgetTrySetText(Widget root_widget, string widget_name, string text, int color = 0)
	{
		TextWidget widget = TextWidget.Cast( root_widget.FindAnyWidget(widget_name) );
		RichTextWidget rt_widget = RichTextWidget.Cast( root_widget.FindAnyWidget(widget_name) );
		if (widget)
		{
			widget.SetText(text);
			int textColor = COLOR_WHITE;
			if (color)
			{
				int a, r, g, b;
				ExpansionStatic.IntToARGB(color, a, r, g, b);
				if (a > 127)
				{
					//! If it's a light color, change text to black
					float lab_l, lab_a, lab_b;
					ExpansionColor.sRGBToLab(r, g, b, lab_l, lab_a, lab_b);
				#ifdef DIAG_DEVELOPER
					EXTrace.Print(EXTrace.GENERAL_ITEMS, ExpansionColor, string.Format("sRGBToLab A %1 R %2 G %3 B %4 -> L* %5 a* %6 b* %7 %8 %9", a, r, g, b, lab_l, lab_a, lab_b, widget_name, text));
				#endif
					if (lab_l > 90.0)
						textColor = 0xFF000000;
				}
			}
			widget.SetColor(textColor);
			Widget widget_background = root_widget.FindAnyWidget(widget_name+"Background");
			if (widget_background)
			{
				if (color != 0)
				{
					widget_background.Show( true );
					widget_background.SetColor(color | 0x7F000000);
				}
				else
				{
					widget_background.Show( false );
				}
			}
		}
		if( rt_widget )
		{
			rt_widget.Update();
		}
	}
};
#endif
