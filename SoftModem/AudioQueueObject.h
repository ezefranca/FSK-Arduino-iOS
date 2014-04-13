//
//  AudioQueueObject.h
//  iNfrared
//
//  Created by George Dean on 11/28/08.
//  Culled from SpeakHere sample code.
//  Copyright 2008 Perceptive Development. All rights reserved.
//

#import <UIKit/UIKit.h>
#include <AudioToolbox/AudioToolbox.h>

#define kNumberAudioDataBuffers	3

@interface AudioQueueObject : NSObject {
	AudioQueueRef					queueObject;					// the audio queue object being used for playback
	AudioStreamBasicDescription		audioFormat;
}

@property (readwrite)			AudioQueueRef				queueObject;
@property (readwrite)			AudioStreamBasicDescription	audioFormat;

- (BOOL) isRunning;

@end
