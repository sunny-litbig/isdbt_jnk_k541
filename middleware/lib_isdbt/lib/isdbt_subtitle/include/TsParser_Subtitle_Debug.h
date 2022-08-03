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

#define DBG_ERR			0x00000001
#define DBG_WARN		0x00000002
#define DBG_INFO		0x00000004

#define DBG_C0			0x00000010
#define DBG_C1			0x00000020
#define DBG_CSI			0x00000040
#define DBG_GRP			0x00000080

#define DBG_ALL			0xFFFFFFFF

extern void LIB_DBG(const int level, const char *format, ...);
