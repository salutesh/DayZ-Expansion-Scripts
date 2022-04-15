/**
 * ExpansionBookMenu.c
 *
 * DayZ Expansion Mod
 * www.dayzexpansion.com
 * © 2022 DayZ Expansion Mod Team
 *
 * This work is licensed under the Creative Commons Attribution-NonCommercial-NoDerivatives 4.0 International License.
 * To view a copy of this license, visit http://creativecommons.org/licenses/by-nc-nd/4.0/.
 *
*/

class ExpansionBookMenu: ExpansionScriptViewMenu
{
	ref ExpansionBookMenuManager m_BookManager;
	ref ExpansionBookMenuController m_BookMenuController;
	
	ButtonWidget back_button;
	TextWidget back_button_label;
	Widget book_content_panel;
	
	static ref ExpansionBookMenuTabBase m_LastOpenedTab = NULL;
	static const int m_MaxNumberOfElements = 20;
	static const int m_MaxNumberOfBookmarks = m_MaxNumberOfElements;
	
	bool m_IsMainContentVisible = true;
	
	void ExpansionBookMenu()
	{
		if (!m_BookMenuController)
			m_BookMenuController = ExpansionBookMenuController.Cast(GetController());		
		
		if (!m_BookManager)
		{
			m_BookManager = new ExpansionBookMenuManager();
			m_BookManager.RegisterBookMenuTabs(this);
		}
		
		UpdateBookTabs();
	}
	
	void ~ExpansionBookMenu()
	{		
		if (m_BookManager)
		{	
			m_BookManager.DestroyBookMenuTabs();
			delete m_BookManager;
		}
	}
	
	override string GetLayoutFile() 
	{
		return "DayZExpansion/Book/GUI/layouts/expansion_book.layout";
	}
	
	override typename GetControllerType() 
	{
		return ExpansionBookMenuController;
	}
	
	void UpdateBookTabs()
	{
		if (m_BookManager)
		{
			m_BookManager.UpdateBookMenuTabs(this);
			
			if (m_BookMenuController.BookTabs && m_BookMenuController.BookTabs.Count() >0)
				m_BookMenuController.BookTabs.Clear();
			
			for (int i = 0; i < m_BookManager.GetBookTabs().Count(); i++)
			{
				ExpansionBookMenuTabBase tab = m_BookManager.GetBookTabs()[i];
				if (tab && !tab.IsChildTab() && tab.CanShow())
				{
					m_BookMenuController.BookTabs.Insert(tab);
				}
			}
		}
	}
	
	ExpansionBookMenuManager GetBookMenuManager()
	{
		return m_BookManager;
	}
	
	void RegisterTab(ExpansionBookMenuTabBase tab)
	{
		if (GetBookMenuManager())
			GetBookMenuManager().RegisterBookMenuTab(tab);
	}
	
	void UnregisterTab(ExpansionBookMenuTabBase tab)
	{
		if (GetBookMenuManager())
			GetBookMenuManager().RemoveBookMenuTab(tab);
	}
	
	void AddTab(ExpansionBookMenuTabBase tab)
	{
		if (m_BookMenuController.ElementsSpacerRight.Count() >= m_MaxNumberOfElements)
			return;
		
		ExpansionBookMenuTabElement element = new ExpansionBookMenuTabElement(tab);
		if (m_BookMenuController.ElementsSpacerLeft.Count() < (m_MaxNumberOfElements / 2))
		{
			m_BookMenuController.ElementsSpacerLeft.Insert(element);
		} 
		else if (m_BookMenuController.ElementsSpacerLeft.Count() >= (m_MaxNumberOfElements / 2) && m_BookMenuController.ElementsSpacerRight.Count() < m_MaxNumberOfElements)
		{
			m_BookMenuController.ElementsSpacerRight.Insert(element);
		}
		
		element.Show();
	}
	
	void RemoveElements()
	{
		m_BookMenuController.ElementsSpacerLeft.Clear();
		m_BookMenuController.ElementsSpacerRight.Clear();
	}
		
	void AddTabs()
	{
		RemoveElements();
		
		for (int i = 0; i < m_BookMenuController.BookTabs.Count(); i++)
		{
			ExpansionBookMenuTabBase tab = m_BookMenuController.BookTabs[i];
			if (tab && tab.IsParentTab() && !tab.IsChildTab() && tab.CanShow())
			{
				AddTab(tab);
			}
		}
	}
	
	void AddBookmark(ExpansionBookMenuTabBase tab)
	{
		if (m_BookMenuController.BookmarksSpacerRight.Count() >= m_MaxNumberOfBookmarks)
			return;
		
		ExpansionBookMenuTabBookmark bookmark = new ExpansionBookMenuTabBookmark(tab);
		if (m_BookMenuController.BookmarksSpacerLeft.Count() < (m_MaxNumberOfBookmarks / 2))
		{
			m_BookMenuController.BookmarksSpacerLeft.Insert(bookmark);
		} 
		else if (m_BookMenuController.BookmarksSpacerLeft.Count() >= (m_MaxNumberOfBookmarks / 2) && m_BookMenuController.BookmarksSpacerRight.Count() < m_MaxNumberOfBookmarks)
		{
			m_BookMenuController.BookmarksSpacerRight.Insert(bookmark);
		}
		
		bookmark.Show();
	}

	void OnBackButtonClick()
	{
		ExpansionBookMenuTabBase currentTab = GetLastOpenedTab();
		
		if (currentTab)
		{
			if (GetLastOpenedTab())
				GetLastOpenedTab().OnBackButtonClick();
			
			if (!currentTab.IgnoreBackButtonBase())
			{
				if (currentTab.IsParentTab())
				{
					currentTab.Hide();
					return;
				}
				else if (GetLastOpenedTab().IsChildTab())
				{
					currentTab.Hide();
					currentTab.GetParentTab().Show();				
					return;
				}
				
				GetDayZExpansion().GetExpansionUIManager().CloseMenu();
				return;
			}
			
			UpdateBookTabs();
		}
		
		if (GetDayZExpansion().GetExpansionUIManager().GetMenu())
			GetDayZExpansion().GetExpansionUIManager().CloseMenu();
	}

	ExpansionBookMenuTabBase GetLastOpenedTab()
	{
		return m_LastOpenedTab;
	}
	
	void SetLastOpenedTab(ExpansionBookMenuTabBase tab)
	{
		m_LastOpenedTab = tab;
		
		if (GetLastOpenedTab())
		{
			ShowBookMainContent(false);
			RemoveElements();
		}
		else 
		{
			ShowBookMainContent(true);
			AddTabs();
		}
	}

	void PlayOpenBookSound()
	{
		EffectSound m_Sound = SEffectManager.PlaySound("Expansion_OpenBook_SoundSet", GetGame().GetPlayer().GetPosition());
		m_Sound.SetSoundAutodestroy(true);
	}

	void PlaySwitchPageSound()
	{
		EffectSound m_Sound = SEffectManager.PlaySound("Expansion_SwitchPage_SoundSet", GetGame().GetPlayer().GetPosition());
		m_Sound.SetSoundAutodestroy(true);
	}

	void PlayCloseBookSound()
	{
		EffectSound m_Sound = SEffectManager.PlaySound("Expansion_CloseBook_SoundSet", GetGame().GetPlayer().GetPosition());
		m_Sound.SetSoundAutodestroy(true);
	}

	void PlayDrawSound()
	{
		EffectSound m_Sound = SEffectManager.PlaySound("Expansion_Draws_SoundSet", GetGame().GetPlayer().GetPosition());
		m_Sound.SetSoundAutodestroy(true);
	}
	
	void ShowBookMainContent(bool state)
	{
		m_IsMainContentVisible = state;
		book_content_panel.Show(state);
	}
	
	override void OnShow()
	{
		super.OnShow();
		
		GetUApi().GetInputByName("UAExpansionBookToggle").ForceDisable(false);		
		GetUApi().UpdateControls();

		PlayOpenBookSound();
		AddTabs();
	}
	
	override void OnHide()
	{
		super.OnHide();
		
		PlayCloseBookSound();
		
		if (GetLastOpenedTab())
			GetLastOpenedTab().Hide();
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		switch (w)
		{
		case back_button:
			back_button_label.SetColor(ARGB(255, 220, 220, 220));
			break;
		}
		
		return super.OnMouseEnter(w, x, y);;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		switch (w)
		{
		case back_button:
			back_button_label.SetColor(ARGB(255, 0, 0, 0));
			break;
		}
		
		return super.OnMouseLeave(w, enterW, x, y);
	}
	
	ObservableCollection<ref ExpansionScriptView> GetBookmarksSideSpacerLeft()
	{
		return m_BookMenuController.SideButtonsLeft;
	}
	
	override void LockControls(bool lockMovement = false)
	{
		super.LockControls(false);
	}

	void SwitchMovementLockState(bool lockMovement)
	{
		if (!lockMovement)
			UnlockInputs();
		
		LockInputs(true, lockMovement);
		GetUApi().UpdateControls();
	}
	
	#ifdef EXPANSIONMODGROUPS
	override void Refresh()
	{
		ExpansionBookMenuTabParty partyTab;
		ExpansionBookMenuTabPartyInvites partyInvitesTab;
		if (m_LastOpenedTab)
		{
			if (Class.CastTo(partyTab, m_LastOpenedTab))
			{
				partyTab.Refresh();
			}
			else if (Class.CastTo(partyInvitesTab, m_LastOpenedTab))
			{
				partyInvitesTab.Refresh();
			}
		}
	}
	#endif
};

class ExpansionBookMenuController: ExpansionViewController
{
	ref ObservableCollection<ref ExpansionBookMenuTabBase> BookTabs = new ObservableCollection<ref ExpansionBookMenuTabBase>(this);
	ref ObservableCollection<ref ExpansionBookMenuTabElement> ElementsSpacerLeft = new ObservableCollection<ref ExpansionBookMenuTabElement>(this);
	ref ObservableCollection<ref ExpansionBookMenuTabElement> ElementsSpacerRight = new ObservableCollection<ref ExpansionBookMenuTabElement>(this);
	ref ObservableCollection<ref ExpansionBookMenuTabBookmark> BookmarksSpacerLeft = new ObservableCollection<ref ExpansionBookMenuTabBookmark>(this);
	ref ObservableCollection<ref ExpansionBookMenuTabBookmark> BookmarksSpacerRight = new ObservableCollection<ref ExpansionBookMenuTabBookmark>(this);
	
	ref ObservableCollection<ref ExpansionScriptView> SideButtonsLeft = new ObservableCollection<ref ExpansionScriptView>(this);
	
	int PageNumberLeft;
	int PageNumberRight;
};

