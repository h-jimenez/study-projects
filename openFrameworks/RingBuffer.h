#pragma once

class RingBuffer
{
public:
	float* m_pBuffer;
	int m_writeIndex = 0;
	int m_size;

	~RingBuffer();
	RingBuffer(int size);

	void feed(float currentSample);
	float getSample(int delay);
};
