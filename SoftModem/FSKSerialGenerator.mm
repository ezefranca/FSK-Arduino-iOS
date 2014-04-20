//
//  FSKSerialGenerator.m
//  FSK Terminal
//
//  Created by George Dean on 1/7/09.
//  Copyright 2009 Perceptive Development. All rights reserved.
//
//  Edited by Ezequiel Franca on 20/04/14

#import "FSKModemConfig.h"
#import "FSKSerialGenerator.h"
#import "FSKByteQueue.h"

#define SAMPLE_RATE  44100
#define SAMPLE       SInt16
#define SAMPLE_MAX   32767
#define NUM_CHANNELS 1

#define BITS_PER_CHANNEL (sizeof(SAMPLE) * 8)
#define BYTES_PER_FRAME  (NUM_CHANNELS * sizeof(SAMPLE))

#define SAMPLE_DURATION  (1000000000 / SAMPLE_RATE)
#define SAMPLES_TO_NS(__samples__) (((UInt64)(__samples__) * 1000000000) / SAMPLE_RATE)
#define NS_TO_SAMPLES(__nanosec__) (unsigned)(((UInt64)(__nanosec__) * SAMPLE_RATE) / 1000000000)
#define US_TO_SAMPLES(__microsec__) (unsigned)(((UInt64)(__microsec__) * SAMPLE_RATE) / 1000000)
#define MS_TO_SAMPLES(__millisec__) (unsigned)(((UInt64)(__millisec__) * SAMPLE_RATE) / 1000)

#define WL_HIGH (1000000000 / FREQ_HIGH)
#define WL_LOW  (1000000000 / FREQ_LOW)
#define HWL_HIGH (500000000 / FREQ_HIGH)
#define HWL_LOW  (500000000 / FREQ_LOW)

#define BIT_PERIOD     (1000000000 / BAUD)

#define SINE_TABLE_LENGTH 441

#define PRE_CARRIER_BITS	  ((40000000+BIT_PERIOD)/BIT_PERIOD)
#define POST_CARRIER_BITS	  (( 5000000+BIT_PERIOD)/BIT_PERIOD)

// TABLE_JUMP = phase_per_sample / phase_per_entry
// phase_per_sample = 2pi * time_per_sample / time_per_wave
// phase_per_entry = 2pi / SINE_TABLE_LENGTH
// TABLE_JUMP = time_per_sample / time_per_wave * SINE_TABLE_LENGTH
// time_per_sample = 1000000000 / SAMPLE_RATE
// time_per_wave = 1000000000 / FREQ
// TABLE_JUMP = FREQ / SAMPLE_RATE * SINE_TABLE_LENGTH
#define TABLE_JUMP_HIGH (FREQ_HIGH * SINE_TABLE_LENGTH / SAMPLE_RATE)
#define TABLE_JUMP_LOW  (FREQ_LOW  * SINE_TABLE_LENGTH / SAMPLE_RATE)

SAMPLE sineTable[SINE_TABLE_LENGTH];

@implementation FSKSerialGenerator

- (id) init
{
	if (self = [super init])
	{
		byteQueue = new FSKByteQueue();
		byteUnderflow = YES;
		for(int i=0; i<SINE_TABLE_LENGTH; ++i)
		{
			sineTable[i] = (SAMPLE)(sin(i * 2 * 3.14159 / SINE_TABLE_LENGTH) * SAMPLE_MAX);
		}
	}

	return self;
}

- (void) setupAudioFormat
{
	audioFormat.mSampleRate			= SAMPLE_RATE;
	audioFormat.mFormatID			= kAudioFormatLinearPCM;
	audioFormat.mFormatFlags		= kAudioFormatFlagIsSignedInteger | kAudioFormatFlagIsPacked;
	audioFormat.mFramesPerPacket	= 1;
	audioFormat.mChannelsPerFrame	= NUM_CHANNELS;
	audioFormat.mBitsPerChannel		= BITS_PER_CHANNEL;
	audioFormat.mBytesPerPacket		= BYTES_PER_FRAME;
	audioFormat.mBytesPerFrame		= BYTES_PER_FRAME;

	bufferByteSize = 0x400;
}

- (BOOL) getNextByte
{
	char byte;
	if(byteUnderflow)
	{
		if(!byteQueue->empty())
		{
			bits = 1;
			bitCount = PRE_CARRIER_BITS;
			sendCarrier = YES;
			byteUnderflow = NO;
			return YES;
		}
	}
	else
	{
		if(byteQueue->get(byte))
		{
			bits = ((UInt16)byte << 1) | (0x0200);
			bitCount = 10;
			sendCarrier = NO;
			return YES;
		}
		else
		{
			bits = 1;
			bitCount = POST_CARRIER_BITS;
			sendCarrier = YES;
			byteUnderflow = YES;
			return YES;
		}
	}
	bits = 1;	// Make sure the output is HIGH when there is no data
	return NO;
}

- (void) fillBuffer:(void*)buffer
{
	SAMPLE* sample = (SAMPLE*)buffer;
	BOOL underflow = NO;

	if(!bitCount)
		underflow = ![self getNextByte];

	for(int i=0; i<bufferByteSize; i += BYTES_PER_FRAME, sample++)
	{
		if(nsBitProgress >= BIT_PERIOD)
		{
			if(bitCount)
			{
				--bitCount;
				if(!sendCarrier)
					bits >>= 1;
			}
			nsBitProgress -= BIT_PERIOD;
			if(!bitCount)
				underflow = ![self getNextByte];
		}

		if(underflow){
			*sample = 0;
		}
		else{
			sineTableIndex += (bits & 1)?TABLE_JUMP_HIGH:TABLE_JUMP_LOW;
			if(sineTableIndex >= SINE_TABLE_LENGTH)
				sineTableIndex -= SINE_TABLE_LENGTH;
			*sample = sineTable[sineTableIndex];
		}

		if(bitCount)
			nsBitProgress += SAMPLE_DURATION;
	}
}

- (void) writeByte:(UInt8)byte
{
	byteQueue->put((char)byte);
}

- (void) writeBytes:(const UInt8 *)bytes length:(int)length
{
	for(int i = 0; i<length; i++){
		byteQueue->put((char)*bytes++);
	}
}

@end
