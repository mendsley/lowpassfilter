/**
 * Copyright 2011-2015 Matthew Endsley
 * All rights reserved
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted providing that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <span>
#include <string.h>

constexpr int MaxChannels = 2;

struct LowPassFilterState
{
	float cutoffFrequency = 1250.0f;
	float samplingRate = 48000.0f;

	float channelHistory[MaxChannels] = {};
};


static void lpfInit(LowPassFilterState* state, float cutoffFrequency, float samplingRate)
{
	for (float& history : channelHistory)
	{
		history = 0.0f;
	}
}

static void lpfFilter(
	  LowPassFilterState* state
	, std::span<float> output
	, std::span<const float> input
	, const int numSamples
	, const int numChannels
)
{
	assert(numChannels > 0);
	assert(numChannels <= MaxChannels);

	float yk[MaxChannels];
	memcpy(yk, state->channelHistory, sizeof(yk));

	const float alpha = state->cutoffFrequency / state->samplingRate;

	for (int ii = 0; ii != numSamples; ++ii)
	{
		for (int channel = 0; channel != numChannels; ++channel)
		{
			yk[channel] += alpha * (input[ii*numChannels + channel] - yk[channel]);
			output[ii*numChannels + channel] = yk[channel];
		}
	}

	memcpy(state->channelHistory, yk, sizeof(yk));
}
