/**
 * ExpansionCOTModuleBase.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2020 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License. 
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionCOTModuleBase : JMRenderableModuleBase
{
	ref array< ref ExpansionSettingSerializationBase > m_Settings;

	override void OnInit()
	{
		super.OnInit();

		m_Settings = new array< ref ExpansionSettingSerializationBase >;

		OnRegisterSettings();
	}

	void ~ExpansionCOTModuleBase()
	{
		if ( m_Settings )
			delete m_Settings;
	}

	override void GetWebhookTypes( out array< string > types )
	{
		types.Insert( "SettingsUpdate" );
	}

	void OnRegisterSettings()
	{
	}

	void OnSend( ref ExpansionSettingBase setting )
	{
		Error( "Not implemented" );
	}

	void OnSend_RPC( ref ParamsReadContext ctx, PlayerIdentity senderRPC, Object target )
	{
		Error( "Not implemented" );
	}

	int GetRPCUpdate()
	{
		return -1;
	}

	override void OnRPC( PlayerIdentity sender, Object target, int rpc_type, ref ParamsReadContext ctx )
	{
		switch ( rpc_type )
		{
		case GetRPCUpdate():
			OnSend_RPC( ctx, sender, target );
			break;
		}
	}

	void UpdateServer( ref ExpansionSettingBase setting, ref JMPlayerInstance instance  )
	{
		GetSettingsInstance().Update( setting );
		GetSettingsInstance().Save();
		
		GetSettingsInstance().Send( NULL );

		SendWebhook( "SettingsUpdate", instance, "Updated the settings" );
	}

	void Update()
	{
		ExpansionSettingBase new_setting = ExpansionSettingBase.Cast( GetSettingsInstance().Type().Spawn() );
		if ( !new_setting.Copy( GetSettingsInstance() ) )
			return;

		bool is_changed = false;

		for ( int i = 0; i < m_Settings.Count(); ++i )
		{
			if ( m_Settings[i].m_IsTempSet )
			{
				m_Settings[i].m_Instance = new_setting;
				m_Settings[i].SetFromTemp();
				m_Settings[i].m_Instance = GetSettingsInstance();

				is_changed = true;
			}
		}

		if ( is_changed )
		{
			OnSend( new_setting );
		}
	}

	ref ExpansionSettingBase GetSettingsInstance()
	{
		return NULL;
	}

	ref array< ref ExpansionSettingSerializationBase > GetSettings()
	{
		return m_Settings;
	}

	private bool FindClassInstanceAndVariable( string variable, ref ExpansionSettingSerializationBase setting )
	{
		array<string> arr();
		variable.Split( ".", arr );

		setting.m_Instance = GetSettingsInstance();

		for ( int i = 0; i < arr.Count() - 1; ++i )
		{
			EnScript.GetClassVar( setting.m_Instance, arr[i], 0, setting.m_Instance );

			if ( setting.m_Instance == null )
				return false;
		}

		setting.m_Variable = arr[arr.Count() - 1];

		return true;
	}

	protected void CreateToggle( string variable, string name, string detailLabel, string detailContent )
	{
		ExpansionSettingSerializationToggle setting = new ref ExpansionSettingSerializationToggle;

		if ( FindClassInstanceAndVariable( variable, setting ) )
		{
			setting.m_Name = name;
			setting.m_DetailLabel = detailLabel;
			setting.m_DetailContent = detailContent;

			m_Settings.Insert( setting );
		}
	}

	protected void CreateSlider( string variable, string name, string detailLabel, string detailContent, float min = 0.0, float max = 1.0, float step = 0.0 )
	{
		ExpansionSettingSerializationSlider setting = new ref ExpansionSettingSerializationSlider;

		if ( FindClassInstanceAndVariable( variable, setting ) )
		{
			setting.m_Name = name;
			setting.m_DetailLabel = detailLabel;
			setting.m_DetailContent = detailContent;
			setting.m_Min = min;
			setting.m_Max = max;
			setting.m_Step = step;

			m_Settings.Insert( setting );
		}
	}

	//! Not working.
	protected void CreateInt( string variable, string name, string detailLabel, string detailContent )
	{
		ExpansionSettingSerializationInt setting = new ref ExpansionSettingSerializationInt;

		if ( FindClassInstanceAndVariable( variable, setting ) )
		{
			setting.m_Name = name;
			setting.m_DetailLabel = detailLabel;
			setting.m_DetailContent = detailContent;

			m_Settings.Insert( setting );
		}
	}

	protected void CreateEnum( string variable, array< string > values, string name, string detailLabel, string detailContent )
	{
		ExpansionSettingSerializationEnum setting = new ref ExpansionSettingSerializationEnum;

		if ( FindClassInstanceAndVariable( variable, setting ) )
		{
			setting.m_Name = name;
			setting.m_DetailLabel = detailLabel;
			setting.m_DetailContent = detailContent;

			for ( int j = 0; j < values.Count(); ++j )
			{
				setting.m_Values.Insert( values[j] );
			}

			m_Settings.Insert( setting );
		}
	}

	protected void CreateEnum( string variable, typename enm, string name, string detailLabel, string detailContent )
	{
		ExpansionSettingSerializationEnum setting = new ref ExpansionSettingSerializationEnum;

		if ( FindClassInstanceAndVariable( variable, setting ) )
		{
			setting.m_Name = name;
			setting.m_DetailLabel = detailLabel;
			setting.m_DetailContent = detailContent;

			for ( int j = 0; j < enm.GetVariableCount(); ++j )
			{
				if ( enm.GetVariableType( j ) == int )
				{
					setting.m_Values.Insert( enm.GetVariableName( j ) );
				}
			}

			m_Settings.Insert( setting );
		}
	}
};