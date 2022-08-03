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

#ifndef  _TCC_AUDIO_RENDER_H_
#define  _TCC_AUDIO_RENDER_H_

class TCCAudioOut
{
public:
	virtual ~TCCAudioOut() {};

	virtual int set(int iSampleRate, int iChannels, int iBitPerSample, int iEndian, int iFrameSize, int iOutputMode, void *pParam, void *DemuxApp) = 0;
	virtual void close() = 0;
	virtual void stop() = 0;
	virtual int write(unsigned char *data, int data_size, long long llPTS, int dev_id) = 0;
	virtual int setVolume(float left, float right) = 0;
	virtual int latency() = 0;
	virtual int writable(int data_size) = 0;

	int getOutputLatency() { return mLatency; };
	int initCheck() { return mInitError; };

protected:
	int mLatency;
	int mInitError;
	pthread_mutex_t mLock;
};

class TCCAudioRender
{
public:
	enum
	{
		OUTPUT_PCM,
		OUTPUT_HDMI_PASSTHRU_AC3,
		OUTPUT_HDMI_PASSTHRU_DDP,
		OUTPUT_SPDIF_PASSTHRU_AC3,
		OUTPUT_SPDIF_PASSTHRU_DDP,
	};

	TCCAudioRender(int iTrackCount);
	~TCCAudioRender();

	int error_check(int iIdx);
	int set(int iIdx, int iSampleRate, int iChannels, int iBitPerSample, int iEndian, int iFrameSize, int iOutputMode, void *pParam, void *DemuxApp);
	void close(int iIdx);
	void stop(int iIdx);
	int write(int iIdx, unsigned char *data, int data_size, long long llPTS);
	int setVolume(int iIdx, float left, float right);
	int mute(bool bMute);
	int latency(int iIdx);
	int getOutputLatency(int iIdx);
	int writable(int iIdx, int data_size);
	int latency_ex();
	int setCurrentIdx(int iIdx);

private:
	TCCAudioOut *getOut(int iIdx);

#if defined(HAVE_LINUX_PLATFORM)
	int mCurrentIdx;
	TCCAudioOut *mAlsa;
#endif
};

#endif//_TCC_AUDIO_RENDER_H_
