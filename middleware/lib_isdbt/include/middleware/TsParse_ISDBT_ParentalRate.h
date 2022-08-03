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

#include <ISDBT_Common.h>

#define AGE_RATE_ER		AGE_RATE_MAX
#define AGE_RATE_L		AGE_RATE_MAX

#define ISDBT_PARENT_RATE_MAX_CONTENT 	(7)
#define ISDBT_PASSWD_MAX_LEN          		(4)

typedef enum
{
	AGE_RATE_10,	// Block if the age will be less than 10 years old
	AGE_RATE_12,	// Block if the age will be less than 12 years old
	AGE_RATE_14,	// Block if the age will be less than 14 years old
	AGE_RATE_16,	// Block if the age will be less than 16 years old
	AGE_RATE_18,	// Block if the age will be less than 18 years old
	AGE_RATE_MAX
} E_ISDBT_VIEWER_RATE;

typedef struct
{
	unsigned char				AgeClass;
	E_ISDBT_VIEWER_RATE	Rate;
	char 					*RateString;
} ISDBT_PR_AGE_CLASS;

typedef struct
{
	unsigned char	Desc;
	char 		*DescString;
} ISDBT_PR_CONTENT_DESC;

extern void ISDBT_AccessCtrl_ProcessParentalRating(void);
extern unsigned char ISDBT_AccessCtrl_ExtractContentDesc(unsigned char ratingField);
extern unsigned char ISDBT_AccessCtrl_ExtractAgeRate(unsigned char ratingField);
extern E_ISDBT_VIEWER_RATE ISDBT_AccessCtrl_GetAgeRate(void);
extern void ISDBT_AccessCtrl_SetAgeRate(unsigned char user_setting_age_from_UI);
extern char * ISDBT_AccessCtrl_GetParentAgeRateString(void);
extern char * ISDBT_AccessCtrl_GetTSAgeRateString(unsigned char ageClass);
extern E_ISDBT_VIEWER_RATE ISDBT_AccessCtrl_GetRateClass(unsigned char ucAgeClass);
extern void ISDBT_AccessCtrl_ProcessParentalRating(void);

