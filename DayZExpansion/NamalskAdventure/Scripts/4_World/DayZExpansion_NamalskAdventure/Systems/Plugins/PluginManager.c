modded class PluginManager
{
	override void Init()
	{
		super.Init();
		RegisterPlugin("PluginCleaning", true, true);
    }
}