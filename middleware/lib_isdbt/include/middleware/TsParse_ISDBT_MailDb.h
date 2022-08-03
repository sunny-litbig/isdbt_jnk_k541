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


#ifndef _TCC_MAIL_DB_H_
#define	_TCC_MAIL_DB_H_

#include "sqlite3.h"


#ifdef __cplusplus
extern "C" {
#endif



/******************************************************************************
* defines 
******************************************************************************/
#define ISDBT_MAIL_MAX_CNT		13
#define ISDB_MAX_MAIL_SIZE		800

/******************************************************************************
* typedefs & structure
******************************************************************************/


/******************************************************************************
* globals
******************************************************************************/

/******************************************************************************
* locals
******************************************************************************/

/******************************************************************************
* declarations
******************************************************************************/
extern int TCCISDBT_MailDB_Update(
	sqlite3 *sqlhandle,
	int uiCA_system_id, 
	int uiMessage_ID, 
	int uiYear,
	int uiMonth,
	int uiDay,
	int uiMail_length,
	char *ucMailpayload,
	int uiUserView_status,
	int uiDelete_status
);
extern int TCCISDBT_MailDB_DeleteStatus_Update(sqlite3 *sqlhandle, int uiMessageID, int uiDeleteStatus);
extern int TCCISDBT_MailDB_UserViewStatus_Update(sqlite3 *sqlhandle, int uiMessageID, int uiUserView_status);


#ifdef __cplusplus
}
#endif

#endif

