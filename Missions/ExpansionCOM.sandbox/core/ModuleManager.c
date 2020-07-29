#include "$CurrentDir:missions\\ExpansionCOM.sandbox\\core\\Module.c"
#include "$CurrentDir:missions\\ExpansionCOM.sandbox\\core\\KeyMouseBinding.c"

#include "$CurrentDir:missions\\ExpansionCOM.sandbox\\core\\modules\\ComEditor\\module.c"
#include "$CurrentDir:missions\\ExpansionCOM.sandbox\\core\\modules\\ComKeyBinds\\module.c"
#include "$CurrentDir:missions\\ExpansionCOM.sandbox\\core\\modules\\DebugMonitor\\module.c"

class COM_KeyMouseActionType
{
	static int PRESS = 1;
	static int RELEASE = 2;
	static int HOLD = 4;
	static int DOUBLECLICK = 8;
	static int VALUE = 16;
}

class COM_ModuleManager
{
	protected ref array< ref COM_Module > m_Modules;

	void COM_ModuleManager()
	{
		Print( "COM_ModuleManager::COM_ModuleManager()" );

		RegisterModules();
	}

	void ~COM_ModuleManager()
	{
		Print( "COM_ModuleManager::~COM_ModuleManager()" );

		m_Modules.Clear();

		delete m_Modules;
	}

	void RegisterModule( ref COM_Module module )
	{
		m_Modules.Insert( module );
	}

	void RegisterModules()
	{
		Print( "COM_ModuleManager::RegisterModules()" );

		m_Modules = new array< ref COM_Module >;

		RegisterModule( new ObjectEditor );
		RegisterModule( new COMKeyBinds );
		RegisterModule( new COM_CustomDebugMonitor );
	}

	void OnSettingsUpdated()
	{
		for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			// m_Modules.Get(i).OnSettingsUpdated();
		}
	}

	ref array< ref COM_Module > GetModules()
	{
		return m_Modules;
	}

	void OnInit()
	{
		Print( "COM_ModuleManager::OnInit()" );

		for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			m_Modules.Get(i).Init();
		}
	}

	void OnMissionStart()
	{
		Print( "COM_ModuleManager::OnMissionStart()" );

		for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			m_Modules.Get(i).onMissionStart();
		}
	}

	void OnMissionFinish()
	{
		Print( "COM_ModuleManager::OnMissionFinish()" );

		for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			m_Modules.Get(i).onMissionFinish();
		}
	}

	void OnMissionLoaded()
	{
		Print( "COM_ModuleManager::OnMissionLoaded()" );

		for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			m_Modules.Get(i).onMissionLoaded();
		}
	}

	void OnUpdate( float timeslice )
	{
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() ) return; 

		for ( int i = 0; i < m_Modules.Count(); ++i)
		{
			COM_Module module = m_Modules.Get(i);

			if ( !module.IsPreventingInput() )
			{
				auto bindings = module.GetBindings();

				for ( int nBinding = 0; nBinding < bindings.Count(); ++nBinding )
				{
					auto k_m_Binding = bindings[ nBinding ];

					if ( !k_m_Binding.CanBeUsedInMenu() && GetGame().GetUIManager().GetMenu())
					{
						continue;
					}

					UAInput input = GetUApi().GetInputByName( k_m_Binding.GetUAInputName() );

					int action = k_m_Binding.GetActionType();

					if ( action & COM_KeyMouseActionType.PRESS && input.LocalPress() )
					{
						GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
					}

					if ( action & COM_KeyMouseActionType.RELEASE && input.LocalRelease() )
					{
						GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
					}

					if ( action & COM_KeyMouseActionType.HOLD && input.LocalHold() )
					{
						GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
					}

					if ( action & COM_KeyMouseActionType.DOUBLECLICK && input.LocalDoubleClick() )
					{
						GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, 0 );
					}

					if ( action & COM_KeyMouseActionType.VALUE && input.LocalValue() != 0 ) 
					{
						GetGame().GameScript.CallFunction( GetModule( k_m_Binding.GetObject() ), k_m_Binding.GetCallBackFunction(), NULL, input.LocalValue() );
					}
				}
			}

			module.onUpdate( timeslice );
		}
	}

	ref COM_Module GetModule( typename module_Type )
	{
		for ( int i = 0; i < m_Modules.Count(); ++i )
		{
			ref COM_Module module = m_Modules.Get(i);

			if ( module.GetModuleType() == module_Type)
			{
				return module;
			}
		}

		return NULL;
	}

	ref COM_Module GetModuleByName( string module_name )
	{
		for ( int i = 0; i < m_Modules.Count(); ++i )
		{
			ref COM_Module module = m_Modules.Get( i );

			if (module.GetModuleName() == module_name)
			{
				return module;
			}
		}

		return NULL;
	}
}

ref COM_ModuleManager g_com_ModuleManager;

ref COM_ModuleManager GetCOMModuleManager()
{
	if( !g_com_ModuleManager )
	{
		g_com_ModuleManager = new COM_ModuleManager();
	}

	return g_com_ModuleManager;
}

ref COM_ModuleManager NewCOMModuleManager()
{
	if ( g_com_ModuleManager )
	{
		delete g_com_ModuleManager;
	}

	g_com_ModuleManager = new COM_ModuleManager();

	return g_com_ModuleManager;
}