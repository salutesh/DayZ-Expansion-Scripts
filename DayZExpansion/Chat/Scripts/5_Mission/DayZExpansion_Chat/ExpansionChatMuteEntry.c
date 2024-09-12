/**
 * ExpansionChatMuteEntry.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionChatMuteEntry: ExpansionScriptView
{
	private ref ExpansionChatMuteEntryController m_ChatMuteController;
	private ref ExpansionChatUIWindow m_ChatWindow;
	private string m_PlayerUID;
	private ButtonWidget ChatMuteButton;
	private ImageWidget ChatMuteButtonIcon;

	void ExpansionChatMuteEntry(ExpansionChatUIWindow chatWindow, SyncPlayer data)
	{
#ifdef EXTRACE
		auto trace = EXTrace.Start(ExpansionTracing.CHAT, this);
#endif

		m_ChatMuteController = ExpansionChatMuteEntryController.Cast(GetController());
		m_ChatWindow = chatWindow;
		m_PlayerUID = data.m_RUID;

		SetPlayerName(data.m_PlayerName);
	}

	void SetPlayerName(string name)
	{
		if (!name)
			return;

		m_ChatMuteController.Name = name;
		m_ChatMuteController.NotifyPropertyChanged("Name");
	}

	void OnEntryClick()
	{
		string name = m_ChatMuteController.Name;
		m_ChatWindow.Unmute(m_PlayerUID, name);
		Destroy();
	}

	override string GetLayoutFile()
	{
		return "DayZExpansion/Chat/GUI/layouts/expansion_chat_mute_entry.layout";
	}

	override typename GetControllerType()
	{
		return ExpansionChatMuteEntryController;
	}

	override bool OnMouseEnter(Widget w, int x, int y)
	{
		super.OnMouseEnter(w, x, y);

		if (w == ChatMuteButton)
		{
			ChatMuteButtonIcon.SetColor(ARGB(200, 0 , 0, 0));
			return true;
		}
		
		return false;
	}

	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		super.OnMouseLeave(w, enterW, x, y);
		
		if (w == ChatMuteButton)
		{
			ChatMuteButtonIcon.SetColor(ARGB(200, 255 , 255, 255));
			return true;
		}

		return false;
	}
};

class ExpansionChatMuteEntryController: ExpansionViewController
{
	string Name;
};