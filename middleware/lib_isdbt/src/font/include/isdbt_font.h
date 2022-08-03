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


/*
 *                ***** NOTE *****
 * All inforamtion of this file should be consistent with an internal configuration of middleware.
 * Please don't modify anything.
 */

/*----- code table of character code set ------*/
#define	UNIPAGE_CODENUM		7808
	/* total row = 84 (0x21 ~ 0x74)
	 * cell per each row = 94
	 * cells for row84 = 6
	 * => (total_row - 1) * (cell per each row) + (cells for row84) */

#define USE_EUCJPMAP_FOR_LATINEXT_CODEMAPPING
#define USE_EUCJPMAP_FOR_SPECIAL_CODEMAPPING


/*----- vertical direction in caption & superimpose -----*/
enum {
	CS_NONE,
	CS_KANJI,
	CS_ALPHANUMERIC,
	CS_LATIN_EXTENSION,
	CS_SPECIAL,
	CS_HIRAGANA,
	CS_KATAKANA
};
typedef struct _VerRotCode_ {
	unsigned short usIn;	/* code values of character */ 
	unsigned short usOut;	/* mapping to font file */
} VerRotCode;

struct _VerRotTable_ {
	unsigned char	char_set;
	int total_no;
	VerRotCode *pVerRotCode;
};

