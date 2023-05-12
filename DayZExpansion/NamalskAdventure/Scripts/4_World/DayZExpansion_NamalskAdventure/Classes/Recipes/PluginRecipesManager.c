modded class PluginRecipesManager
{
	override void RegisterRecipies()
	{
		super.RegisterRecipies();

		RegisterRecipe(new NA_CleanAmmo);
	}
}