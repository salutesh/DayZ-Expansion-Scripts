modded class JMModuleConstructor
{
	override void RegisterModules( out TTypenameArray modules )
	{
		modules.Insert( ExpansionGlobalChatModule );
		
		super.RegisterModules( modules );
	}
};
