//
//  FSKSerialGenerator.h
//  FSK Terminal
//
//  Created by George Dean on 1/7/09.
//  Copyright 2009 Perceptive Development. All rights reserved.
//
//  Edited by Ezequiel Franca on 20/04/14

#import "AudioSignalGenerator.h"

struct FSKByteQueue;

@interface FSKSerialGenerator : AudioSignalGenerator {
	float nsBitProgress;
	unsigned sineTableIndex;

	unsigned bitCount;
	UInt16 bits;

	BOOL byteUnderflow;
	BOOL sendCarrier;

	struct FSKByteQueue* byteQueue;
}

- (void) writeByte:(UInt8)byte;
- (void) writeBytes:(const UInt8 *)bytes length:(int)length;

@end
