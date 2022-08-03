/*
 * Copyright (C) 2013 Telechips, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#if defined(HAVE_LINUX_PLATFORM)
#define LOG_NDEBUG 0
#define LOG_TAG	"TsParser_EPG"
#include <utils/Log.h>
#endif

#include <stdio.h>
#include <string.h>
#include "ISDBT_EPG.h"

#define PRINTF		//ALOGE
#define LOGE		ALOGE

L_EIT_EPG_LIST_h *	g_pEventList = NULL;

int TC_Deallocate_Memory(void *p)
{
	tcc_mw_free(__FUNCTION__, __LINE__, p);
	return 1;
}

void ISDBT_EPG_FreeEventList_h(L_EIT_EPG_LIST_h * pEpgList_h)
{
	L_EIT_EPG_NODE* pEpgNode = NULL;
	unsigned short usCurEventID = 0;
	unsigned char ucCurVersion = 0;
	short sLEITEventCnt = MAX_L_EIT_NUM;

	if (pEpgList_h == NULL) {
		LOGE("[%s] Cannot find a pointer of EPG LIST Root !!\n", __func__);
		return;
	}

	ucCurVersion = pEpgList_h->version;

	while (pEpgList_h->pHead != NULL)
	{
		pEpgNode = pEpgList_h->pHead;
		usCurEventID = pEpgNode->eventID;

		pEpgList_h->pHead = pEpgList_h->pHead->pNext;

		if (pEpgNode->pEventName)
		{
			TC_Deallocate_Memory(pEpgNode->pEventName);
			pEpgNode->pEventName = NULL;
		}

		if (pEpgNode->pEventText)
		{
			TC_Deallocate_Memory(pEpgNode->pEventText);
			pEpgNode->pEventText = NULL;
		}

		TC_Deallocate_Memory(pEpgNode);
		pEpgNode = NULL;

		/* If loop is executed over the 10 times, escape the loop */
		if (sLEITEventCnt-- == 0)
			break;
	}

	/* mem free to EPG service name */
	TC_Deallocate_Memory(pEpgList_h->pServiceName);
	pEpgList_h->pServiceName = NULL;

	/* mem free to EPG TS name */
	TC_Deallocate_Memory(pEpgList_h->pTSName);
	pEpgList_h->pTSName = NULL;

	/* mem free to EPG root pointer */
	TC_Deallocate_Memory(pEpgList_h);

	PRINTF("[EPG version:0x%02x] mem free pEpgList_h\n", ucCurVersion);
}

void * ISDBT_EPG_GetEventDBRoot(void)
{
	return (void *)g_pEventList;
}

void ISDBT_TIME_GetRealDate(DATE_TIME_T *pRealDate, unsigned int  MJD)
{
	int y_dash, m_dash, k;

	y_dash = (MJD * 100 - 1507820) / 36525;
	m_dash = (MJD * 10000 - 149561000 - INT(y_dash * 3652500, 10000)) / 306001;
	pRealDate->day = (MJD * 10000 - 149560000 - INT(y_dash * 3652500, 10000) - INT(m_dash * 306001, 10000)) / 10000;

	if (m_dash == 14 || m_dash == 15)
		k = 1;
	else
		k = 0;

	pRealDate->year = y_dash + k + 1900;
	pRealDate->month = m_dash - 1 - k * 12;
	pRealDate->weekday = ((MJD + 2) % 7) + 1;
}

/* End of "ISDBT_EPG.c" File */

