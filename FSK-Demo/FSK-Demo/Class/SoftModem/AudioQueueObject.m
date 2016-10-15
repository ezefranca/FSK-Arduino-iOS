//
//  AudioQueueObject.m
//  iNfrared
//
//  Created by George Dean on 11/28/08.
//  Culled from SpeakHere sample code.
//  Copyright 2008 Perceptive Development. All rights reserved.
//
//  Edited by Ezequiel Franca on 20/04/14

#import "AudioQueueObject.h"


@implementation AudioQueueObject

@synthesize queueObject;
@synthesize audioFormat;

- (BOOL) isRunning {

	UInt32		isRunning;
	UInt32		propertySize = sizeof (UInt32);
	OSStatus	result;

	result =	AudioQueueGetProperty (
									   queueObject,
									   kAudioQueueProperty_IsRunning,
									   &isRunning,
									   &propertySize
									   );

	if (result != noErr) {
		return false;
	} else {
		return isRunning;
	}
}


@end
