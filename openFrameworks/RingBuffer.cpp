#include "RingBuffer.h"

RingBuffer::~RingBuffer() {
	delete[] m_pBuffer;
}
RingBuffer::RingBuffer(int size) {
	m_pBuffer = new float[size];
	m_size = size;
}
void RingBuffer::feed(float currentSample) {
	m_pBuffer[m_writeIndex] = currentSample;
	m_writeIndex++;
	if (m_writeIndex >= m_size) m_writeIndex = 0;
}
float RingBuffer::getSample(int delay) {
	int readIndex = m_writeIndex - delay - 1;
	if (readIndex < 0) readIndex = m_size + readIndex;
	return m_pBuffer[readIndex];
}

