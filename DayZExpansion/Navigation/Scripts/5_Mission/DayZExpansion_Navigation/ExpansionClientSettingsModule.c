modded class ExpansionClientSettingsModule
{
	override void OnSettingsChanged(Class sender, CF_EventArgs args)
	{
		super.OnSettingsChanged(sender, args);

		ExpansionMarkerModule module = ExpansionMarkerModule.Cast(CF_ModuleCoreManager.Get(ExpansionMarkerModule));
		if (module)
		{
			// PERSONAL MARKERS
			if (GetExpansionClientSettings().Show2DClientMarkers)
				module.SetVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_MAP);
			else
				module.RemoveVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_MAP);

			if (GetExpansionClientSettings().Show3DClientMarkers)
				module.SetVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD);
			else
				module.RemoveVisibility(ExpansionMapMarkerType.PERSONAL, EXPANSION_MARKER_VIS_WORLD);

			// PARTY MARKERS
			if (GetExpansionClientSettings().Show2DPartyMarkers)
				module.SetVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_MAP);
			else
				module.RemoveVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_MAP);

			if (GetExpansionClientSettings().Show3DPartyMarkers)
				module.SetVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD);
			else
				module.RemoveVisibility(ExpansionMapMarkerType.PARTY, EXPANSION_MARKER_VIS_WORLD);

			// SERVER MARKERS
			if (GetExpansionClientSettings().Show2DGlobalMarkers)
				module.SetVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_MAP);
			else
				module.RemoveVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_MAP);

			if (GetExpansionClientSettings().Show3DGlobalMarkers)
				module.SetVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD);
			else
				module.RemoveVisibility(ExpansionMapMarkerType.SERVER, EXPANSION_MARKER_VIS_WORLD);

			// MEMBER MARKERS
			if (GetExpansionClientSettings().Show2DPlayerMarkers)
				module.SetVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_MAP);
			else
				module.RemoveVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_MAP);

			if (GetExpansionClientSettings().Show3DPlayerMarkers)
				module.SetVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD);
			else
				module.RemoveVisibility(ExpansionMapMarkerType.PLAYER, EXPANSION_MARKER_VIS_WORLD);
		}
	}
};
