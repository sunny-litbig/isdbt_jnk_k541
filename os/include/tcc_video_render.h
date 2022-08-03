/****************************************************************************

Copyright (C) 2013 Telechips Inc.


Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at
http://www.apache.org/licenses/LICENSE-2.0
Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions 
andlimitations under the License.

****************************************************************************/

#ifndef _TCC_VIDEO_RENDER_H_
#define _TCC_VIDEO_RENDER_H_

class TCCVideoRender
{
public:
	TCCVideoRender(int flag);
	~TCCVideoRender();
	int SetVideoSurface(void *native);
	int ReleaseVideoSurface(void);
	int SetNativeWindow(int width, int height, int format, int outmode);
	int dequeueBuffer(unsigned char **buf, int width, int height, int pts);
	void enqueueBuffer(unsigned char *buf, int width, int height, int pts);
	int IsVSyncEnabled();
	void setCrop(int left, int top, int width, int height);
	void *GetPrivateAddr(int fd_val, int width, int height);
	int SetDisplaySize(void);
	int getfromsysfs(const char *sysfspath, char *val);


private:
	class CDisplayPos
	{
	public:
		friend class TCCVideoRender;
		CDisplayPos() { m_iWidth = 1024; m_iHeight = 600;};
		virtual ~CDisplayPos() {};

	private:
		int m_iWidth;
		int m_iHeight;
	};

private:
	CDisplayPos m_clDisplayPos;

};

#endif//_TCC_VIDEO_RENDER_H_
