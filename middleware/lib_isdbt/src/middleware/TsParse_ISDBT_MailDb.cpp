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
#define LOG_TAG "TSPARSE_MAIL_DB"
#include <utils/Log.h>
#endif

#ifdef HAVE_LINUX_PLATFORM
#include <string.h> /* for memset(), memcpy(), strlen()*/
#include <unistd.h> /* for usleep()*/
#endif

#include "TsParse_ISDBT_MailDb.h"
#include "tcc_msg.h"

#define ERR_MSG	ALOGE
#define DBG_MSG //ALOGD

int TCCISDBT_MailDB_Update(
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
)
{
	int err =SQLITE_ERROR;
	char szSQL[4096];
	sqlite3_stmt *stmt;
	int	i=0, rowID=-1, listCnt=0;
	unsigned short uiMail[ISDB_MAX_MAIL_SIZE];

	ALOGE("%s:%d mail_length = %d\n", __func__, __LINE__, uiMail_length);

	memset(uiMail, 0, sizeof(uiMail));

	if(uiMail_length>0 && uiMail_length<ISDB_MAX_MAIL_SIZE*2)
		memcpy((short*)uiMail, (short*)ucMailpayload, uiMail_length);
	else if(uiMail_length>ISDB_MAX_MAIL_SIZE*2)
		memcpy((short*)uiMail, (short*)ucMailpayload, ISDB_MAX_MAIL_SIZE);


	if (sqlhandle != NULL) {
		/* quary uiMessage_ID */
		sprintf(szSQL, "SELECT _id FROM Mail WHERE Message_ID=%d", uiMessage_ID);
		err = sqlite3_prepare_v2(sqlhandle, szSQL, -1, &stmt, NULL);
		if (err == SQLITE_OK) {
#if 1
			while(sqlite3_step(stmt) == SQLITE_ROW)
			{
				rowID = sqlite3_column_int(stmt, 0);
			}
#else
			err = sqlite3_step(stmt);
			if (err == SQLITE_ROW) {
				rowID = sqlite3_column_int(stmt, 0);
			}
#endif
		}
		if(stmt){
			sqlite3_finalize(stmt);
		}

		ALOGE("%s %d err = %d, rowID = %d \n", __func__, __LINE__, err, rowID);
		ALOGE("%s %d uiCA_system_id = %d, uiMessage_ID = %d, uiYear =%d, uiMonth = %d, uiDay =%d\n", __func__, __LINE__, uiCA_system_id, uiMessage_ID, uiYear, uiMonth, uiDay);
		ALOGE("%s %d uiMail_length = %d, uiUserView_status = %d, uiDelete_status=%d uiMail_length =%d \n", __func__, __LINE__, uiMail_length, uiUserView_status, uiDelete_status, uiMail_length);

		sprintf(szSQL, "SELECT COUNT(*) FROM Mail");
		if (sqlite3_prepare_v2(sqlhandle, szSQL, -1, &stmt, NULL) == SQLITE_OK) {
			err = sqlite3_step(stmt);
			if (err == SQLITE_ROW) {
				listCnt = sqlite3_column_int(stmt, 0);
			}
		}
		if (stmt) {
			sqlite3_finalize(stmt);
			stmt = NULL;
		}
		ALOGE("%s %d listCnt = %d \n", __func__, __LINE__, listCnt);


		if(listCnt < ISDBT_MAIL_MAX_CNT)
		{
			if(rowID == -1)
			{
				/* no Mail */
				sprintf(szSQL, "INSERT INTO Mail (CA_system_id, Message_ID, Year, Month, Day, Mail_length, UserView_status, Delete_status, Mail) \
								VALUES(%d, %d, %d, %d, %d, %d, %d, %d,  ?)",
							uiCA_system_id,
							uiMessage_ID,
							uiYear,
							uiMonth,
							uiDay,
							uiMail_length,
							uiUserView_status,
							uiDelete_status);
				if (sqlite3_prepare_v2(sqlhandle, szSQL, strlen(szSQL), &stmt, NULL) == SQLITE_OK) {
					sqlite3_reset (stmt);
					err = sqlite3_bind_text16(stmt, 1, uiMail, -1, SQLITE_STATIC);
					if (err != SQLITE_OK)
						ALOGE("[%s:%d] ERROR : %d\n", __func__, __LINE__, err);
					err = sqlite3_step(stmt);
					if ((err != SQLITE_ROW) && (err != SQLITE_DONE))
						ALOGE("[%s:%d] ERROR : %d\n", __func__, __LINE__, err);
				}
				if (stmt) {
					sqlite3_finalize(stmt);
					stmt = NULL;
				}
			}
		}
		else
		{
	 			sprintf(szSQL, "UPDATE Mail SET CA_system_id=%d, \
							Year = %d,\
							Month = %d,\
							Day = %d,\
							Mail_length = %d,\
							UserView_status = %d,\
							Delete_status = %d,\
							Mail = ?\
							WHERE Message_ID = %d ",
							uiCA_system_id,
							uiYear,
							uiMonth,
							uiDay,
							uiMail_length,
							uiUserView_status,
							uiDelete_status,
							uiMessage_ID);
				if (sqlite3_prepare_v2(sqlhandle, szSQL, strlen(szSQL), &stmt, NULL) == SQLITE_OK) {
					sqlite3_reset (stmt);
					err = sqlite3_bind_text16(stmt, 1, uiMail, -1, SQLITE_STATIC);
					if (err != SQLITE_OK)
						ALOGE("[%s:%d] ERROR : %d\n", __func__, __LINE__, err);
					err = sqlite3_step(stmt);
					if ((err != SQLITE_ROW) && (err != SQLITE_DONE))
						ALOGE("[%s:%d] ERROR : %d\n", __func__, __LINE__, err);
				}
				if (stmt) {
					sqlite3_finalize(stmt);
					stmt = NULL;
				}
		}
	}
	return err;
}

int TCCISDBT_MailDB_Deletedata(sqlite3 *sqlhandle, int uiRowID)
{
	int err = SQLITE_ERROR;
	char szSQL[4096];
	sqlite3_stmt *stmt;
	char *errMsg;

	if (sqlhandle != NULL) {
		sprintf(szSQL, "delete _id FROM Mail WHERE _id =%d", uiRowID);
		if (sqlite3_prepare_v2(sqlhandle, szSQL, strlen(szSQL), &stmt, NULL) == SQLITE_OK) {
			err = sqlite3_exec(sqlhandle, szSQL, NULL, NULL, &errMsg);
			if(err != SQLITE_OK)
			{
				ALOGE("%s %d DB Delete error : Msg=%s\n", __func__,  __LINE__, errMsg);
			}
		}
		if (stmt) {
			sqlite3_finalize(stmt);
			stmt = NULL;
		}
	}

	return err;
}

int TCCISDBT_MailDB_DeleteStatus_Update(sqlite3 *sqlhandle, int uiMessageID, int uiDeleteStatus)
{
	int err =SQLITE_ERROR;
	char szSQL[4096];
	sqlite3_stmt *stmt;
	int	i=0;

	if (sqlhandle != NULL) {
		/* quary uiMessage_ID */
		sprintf(szSQL, "SELECT _id FROM Mail WHERE Message_ID=%d", uiMessageID);
		if (sqlite3_prepare_v2(sqlhandle, szSQL, strlen(szSQL), &stmt, NULL) == SQLITE_OK) {
			for(i=0; i<ISDBT_MAIL_MAX_CNT; i++)
			{
				err = sqlite3_step(stmt);
				if (err == SQLITE_ROW) {
					err = sqlite3_column_int(stmt, 0);
				}
				if (err == SQLITE_DONE || err == SQLITE_ERROR)
					break;
				usleep(5000);
			}
		}
		if (stmt) {
			sqlite3_finalize(stmt);
			stmt = NULL;
		}

 		if (err == SQLITE_ROW){
			/* found Mail */
 			sprintf(szSQL, "UPDATE Mail SET Delete_status = %d", uiDeleteStatus);
			if (sqlite3_prepare_v2(sqlhandle, szSQL, strlen(szSQL), &stmt, NULL) == SQLITE_OK) {
				sqlite3_reset (stmt);
				err = sqlite3_step(stmt);
				if ((err != SQLITE_ROW) && (err != SQLITE_DONE))
					ALOGE("[%s:%d] ERROR : %d\n", __func__, __LINE__, err);
			}
			if (stmt) {
				sqlite3_finalize(stmt);
				stmt = NULL;
			}
 		} else {
			/* error */
			err = SQLITE_ERROR;
		}
	}
	return err;
}

int TCCISDBT_MailDB_UserViewStatus_Update(sqlite3 *sqlhandle, int uiMessageID, int uiUserView_status)
{
	int err =SQLITE_ERROR;
	char szSQL[4096];
	sqlite3_stmt *stmt;
	int	i=0;

	if (sqlhandle != NULL) {
		/* quary uiMessage_ID */
		sprintf(szSQL, "SELECT _id FROM Mail WHERE Message_ID=%d", uiMessageID);
		if (sqlite3_prepare_v2(sqlhandle, szSQL, strlen(szSQL), &stmt, NULL) == SQLITE_OK) {
			for(i=0; i<ISDBT_MAIL_MAX_CNT; i++)
			{
				err = sqlite3_step(stmt);
				if (err == SQLITE_ROW) {
					err = sqlite3_column_int(stmt, 0);
				}
				if (err == SQLITE_DONE || err == SQLITE_ERROR)
					break;
				usleep(5000);
			}
		}
		if (stmt) {
			sqlite3_finalize(stmt);
			stmt = NULL;
		}

 		if (err == SQLITE_ROW){
			/* found Mail */
 			sprintf(szSQL, "UPDATE Mail SET UserView_status = %d", uiUserView_status);
			if (sqlite3_prepare_v2(sqlhandle, szSQL, strlen(szSQL), &stmt, NULL) == SQLITE_OK) {
				sqlite3_reset (stmt);
				err = sqlite3_step(stmt);
				if ((err != SQLITE_ROW) && (err != SQLITE_DONE))
					ALOGE("[%s:%d] ERROR : %d\n", __func__, __LINE__, err);
			}
			if (stmt) {
				sqlite3_finalize(stmt);
				stmt = NULL;
			}
 		} else {
			/* error */
			err = SQLITE_ERROR;
		}
	}
	return err;
}



