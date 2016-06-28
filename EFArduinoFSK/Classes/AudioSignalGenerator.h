//
//  AudioSignalGenerator.h
//  FSK Terminal
//
//  Created by George Dean on 1/6/09.
//  Copyright 2009 Perceptive Development. All rights reserved.
//
//  Edited by Ezequiel Franca on 20/04/14

#import "AudioQueueObject.h"


@interface AudioSignalGenerator : AudioQueueObject {

	AudioQueueBufferRef				buffers[kNumberAudioDataBuffers];	// the audio queue buffers for the audio queue

	UInt32							bufferByteSize;						// the number of bytes to use in each audio queue buffer
	UInt32							bufferPacketCount;

	AudioStreamPacketDescription	*packetDescriptions;

	BOOL							stopped;
	BOOL							audioPlayerShouldStopImmediately;
}

@property (readwrite) AudioStreamPacketDescription	*packetDescriptions;
@property (readwrite) BOOL							stopped, audioPlayerShouldStopImmediately;
@property (readwrite) UInt32						bufferByteSize;
@property (readwrite) UInt32						bufferPacketCount;

- (void) setupAudioFormat;
- (void) setupPlaybackAudioQueueObject;
- (void) setupAudioQueueBuffers;

- (void) play;
- (void) stop;
- (void) pause;
- (void) resume;

- (void) fillBuffer:(void*)buffer;

@end
