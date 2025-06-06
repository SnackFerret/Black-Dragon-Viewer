/**
 * @file llpanelpeople.h
 * @brief Side tray "People" panel
 *
 * $LicenseInfo:firstyear=2009&license=viewerlgpl$
 * Second Life Viewer Source Code
 * Copyright (C) 2010, Linden Research, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation;
 * version 2.1 of the License only.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Linden Research, Inc., 945 Battery Street, San Francisco, CA  94111  USA
 * $/LicenseInfo$
 */

#ifndef LL_LLPANELPEOPLE_H
#define LL_LLPANELPEOPLE_H

#include <llpanel.h>

#include "llcallingcard.h" // for avatar tracker
#include "llfloaterwebcontent.h"
#include "llvoiceclient.h"

class LLAvatarList;
class LLAvatarName;
class LLFilterEditor;
class LLGroupList;
class LLMenuButton;
class LLTabContainer;
class LLNetMap;
class LLBlockList;
class LLMenuItemBranchGL;

class LLPanelPeople
    : public LLPanel
    , public LLVoiceClientStatusObserver
{
    LOG_CLASS(LLPanelPeople);
public:
    LLPanelPeople();
    virtual ~LLPanelPeople();

    bool postBuild() override;
    void onOpen(const LLSD& key) override;
    bool notifyChildren(const LLSD& info) override;
    // Implements LLVoiceClientStatusObserver::onChange() to enable call buttons
    // when voice is available
    void onChange(EStatusType status, const LLSD& channelInfo, bool proximal) override;

    //bool mTryToConnectToFacebook;

	//BD
	LLAvatarList* getNearbyList() { return mNearbyList; }
	void          updateNearbyList();

	//BD - Empower someone with rights or revoke them.
	void		onEmpowerFriend(const LLSD& userdata);
	LLAvatarList* getFriendList() { return mAllFriendList; }

	// internals
	class Updater;

    bool updateNearbyArrivalTime();

private:

	typedef enum e_sort_oder {
		E_SORT_BY_NAME = 0,
		E_SORT_BY_STATUS = 1,
		E_SORT_BY_MOST_RECENT = 2,
		E_SORT_BY_DISTANCE = 3,
		E_SORT_BY_RECENT_SPEAKERS = 4,
		E_SORT_BY_RECENT_ARRIVAL = 5,
		E_SORT_BY_TYPE = 6
	} ESortOrder;

    void				    removePicker();

	// methods indirectly called by the updaters
	void					updateFriendListHelpText();
	void					updateFriendList();
	void					updateRecentList();

	bool					isItemsFreeOfFriends(const uuid_vec_t& uuids);

	void					updateButtons();
	const std::string&      getActiveTabName() const;
	LLUUID					getCurrentItemID() const;
	void					getCurrentItemIDs(uuid_vec_t& selected_uuids) const;
	void					setSortOrder(LLAvatarList* list, ESortOrder order, bool save = true);

	// UI callbacks
	void					onFilterEdit(const std::string& search_string);
	void					onGroupLimitInfo();
	void					onTabSelected(const LLSD& param);
	void					onAddFriendButtonClicked();
	void					onAddFriendWizButtonClicked();
	void					onDeleteFriendButtonClicked();
	void					onChatButtonClicked();
	void					onGearButtonClicked(LLUICtrl* btn);
	void					onImButtonClicked();
	void					onMoreButtonClicked();
	void					onAvatarListDoubleClicked(LLUICtrl* ctrl);
	void					onAvatarListCommitted(LLAvatarList* list);
	bool					onGroupPlusButtonValidate();
	void					onGroupMinusButtonClicked();
	void					onGroupPlusMenuItemClicked(const LLSD& userdata);

	void					onFriendsViewSortMenuItemClicked(const LLSD& userdata);
	void					onNearbyViewSortMenuItemClicked(const LLSD& userdata);
	void					onGroupsViewSortMenuItemClicked(const LLSD& userdata);
	void					onRecentViewSortMenuItemClicked(const LLSD& userdata);

	bool					onFriendsViewSortMenuItemCheck(const LLSD& userdata);
	bool					onRecentViewSortMenuItemCheck(const LLSD& userdata);
	bool					onNearbyViewSortMenuItemCheck(const LLSD& userdata);

	bool					onMenuVisibilityCheck(const LLSD& userdata);

	// misc callbacks
	static void				onAvatarPicked(const uuid_vec_t& ids, const std::vector<LLAvatarName> names);

	//BD
	void					onBlockedPlusMenuItemClicked(const LLSD& userdata);
	void					onBlockedViewSortMenuItemClicked(const LLSD& userdata);
	bool					onBlockedViewSortMenuItemCheck(const LLSD& userdata);
	void					blockResidentByName();
	void					blockObjectByName();
	void					callbackBlockPicked(const uuid_vec_t& ids, const std::vector<LLAvatarName> names);
	static void				callbackBlockByName(const std::string& text);

	std::string             getAvatarInformation(const LLUUID& avatar);


	LLTabContainer*			mTabContainer;

	LLAvatarList*			mAllFriendList;
	LLAvatarList*			mNearbyList;
	LLAvatarList*			mRecentList;
	LLGroupList*			mGroupList;

	LLTextBox*				mGroupCount = nullptr;
	LLUICtrl*				mGroupMinusBtn = nullptr;
	LLUICtrl*				mFriendGearBtn = nullptr;
	LLUICtrl*				mNearbyGearBtn = nullptr;
	LLUICtrl*				mRecentGearBtn = nullptr;
	LLUICtrl*				mBlockedGearBtn = nullptr;

	LLNetMap*				mMiniMap;

	std::vector<std::string> mSavedOriginalFilters;
	std::vector<std::string> mSavedFilters;

	Updater*				mFriendListUpdater;
	Updater*				mNearbyListUpdater;
	Updater*				mRecentListUpdater;
	Updater*				mButtonsUpdater;
    LLHandle< LLFloater >	mPicker;

	//BD
	LLBlockList*			mBlockedList = nullptr;
	LLTextBox*				mFriendCount = nullptr;
	LLTextBox*				mBlockCount = nullptr;

	LLTextBox*				mLabelNoFriends = nullptr;

	LLMenuItemBranchGL*		mMenuFilters;
	LLMenuItemBranchGL*		mMenuEdit;
};

#endif //LL_LLPANELPEOPLE_H
