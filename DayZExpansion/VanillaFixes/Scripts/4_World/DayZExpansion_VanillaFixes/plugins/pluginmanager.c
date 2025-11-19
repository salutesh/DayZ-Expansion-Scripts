#ifndef DAYZ_1_28
#ifdef DIAG
modded class PluginManager
{
	override protected void RegisterPluginDebug(string plugin_class_name, bool reg_on_client, bool reg_on_server)
	{
		//! Fix NULL ptr errors related to hand events under diag 1.29 due to PluginInventoryDebug not getting registered under diag
		//! by forcing reg_on_release to true
		//! https://feedback.bistudio.com/T195696
		if (plugin_class_name == "PluginInventoryDebug")
			RegisterPlugin(plugin_class_name, reg_on_client, reg_on_server, true);
		else
			super.RegisterPluginDebug(plugin_class_name, reg_on_client, reg_on_server);
	}
}
#endif
#endif
