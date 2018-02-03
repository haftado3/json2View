//addItem2Tree.h
#pragma once
#include "resource.h"

HTREEITEM AddItemToTree(HWND hwndTree, LPCWSTR text, int nLevel)
{
	TVITEM tvi;
	TVINSERTSTRUCT tvins;
	static HTREEITEM hPrev = (HTREEITEM)TVI_FIRST;
	static HTREEITEM hRootItem = NULL;
	static HTREEITEM hPrevLev2Item = NULL;
	//tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_PARAM;
	tvi.mask = TVIF_TEXT | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIS_STATEIMAGEMASK;

	//icon load fail need to solve this
	tvi.iImage = IDB_BITMAP1;
	tvi.iSelectedImage = tvi.iImage;
	tvi.pszText =LPWSTR( text);
	tvins.hInsertAfter = hPrev;
	tvins.item = tvi;

	if (nLevel == 1)
	{
		tvins.hParent = TVI_ROOT;
	}
	else if (nLevel == 2)
	{
		tvins.hParent = hRootItem;
	}
	else
	{
		TVITEM tviSetup;
		tviSetup.hItem = hPrev;
		tviSetup.mask = TVIF_PARAM;
		TVITEM * tviLocal = &tviSetup;
		TreeView_GetItem(hwndTree, tviLocal);

		if (nLevel > tviLocal->lParam)
		{
			tvins.hParent = hPrev;
		}
		else
		{
			HTREEITEM hPrevLocal = TreeView_GetParent(hwndTree, hPrev);
			tviLocal->hItem = hPrevLocal;
			TreeView_GetItem(hwndTree, tviLocal);
			for (int i = nLevel; i <= tviLocal->lParam;)
			{
				HTREEITEM hPrevLocalTemp = TreeView_GetParent(hwndTree, hPrevLocal);
				hPrevLocal = hPrevLocalTemp;
				tviLocal->hItem = hPrevLocal;
				TreeView_GetItem(hwndTree, tviLocal);
			}
			tviLocal->mask = TVIF_TEXT;
			TreeView_GetItem(hwndTree, tviLocal);
			tvins.hParent = hPrevLocal;

		}
	}

	hPrev = (HTREEITEM)SendMessage(hwndTree, TVM_INSERTITEM, 0, (LPARAM)(LPTVINSERTSTRUCT)&tvins);

	if (hPrev == 0)
	{
		return false;
	}
	if (nLevel == 1)
	{
		hRootItem = hPrev;
	}

	return hPrev;
}

// Extracts heading text and heading levels from a global 
// array and passes them to a function that adds them as
// parent and child items to a tree-view control. 
// Returns TRUE if successful, or FALSE otherwise. 
// hwndTV - handle to the tree-view control. 

BOOL InitTreeViewItems(HWND hwndTV)
{
	HTREEITEM hti;

	// g_rgDocHeadings is an application-defined global array of 
	// the following structures: 
	//     typedef struct 
	//       { 
	//         TCHAR tchHeading[MAX_HEADING_LEN]; 
	//         int tchLevel; 
	//     } Heading; 
	for (int i = 0; i < 100; i++)
	{
		// Add the item to the tree-view control. 
		hti = AddItemToTree(hwndTV, L"item", 1);

		if (hti == NULL)
			return FALSE;
	}

	return TRUE;
}
