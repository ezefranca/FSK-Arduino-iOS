//
//  FSKRecognizer.h
//  FSK Terminal
//
//  Created by George Dean on 12/22/08.
//  Copyright 2008 Perceptive Development. All rights reserved.
//
//  Edited by Ezequiel Franca on 20/04/14

#import "PatternRecognizer.h"

#define FSK_SMOOTH 3

typedef enum
{
	FSKStart,
	FSKBits,
	FSKSuccess,
	FSKFail
} FSKRecState;

@class MultiDelegate;

@protocol CharReceiver;

struct FSKByteQueue;

@interface FSKRecognizer : NSObject <PatternRecognizer> {
	unsigned recentLows;
	unsigned recentHighs;
	unsigned halfWaveHistory[FSK_SMOOTH];
	unsigned bitPosition;
	unsigned recentWidth;
	unsigned recentAvrWidth;
	char bits;
	FSKRecState state;
	MultiDelegate<CharReceiver>* receivers;
	struct FSKByteQueue* byteQueue;
}

- (void) addReceiver:(id<CharReceiver>)receiver;

- (void) edge: (int)height width:(UInt64)nsWidth interval:(UInt64)nsInterval;
- (void) idle: (UInt64)nsInterval;
- (void) reset;

@end
