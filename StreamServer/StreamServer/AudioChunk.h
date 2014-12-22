#pragma once
struct AudioChunk
{
	int sampleRate;
	int channels;
	int frameCount;
	void* frames;
};
