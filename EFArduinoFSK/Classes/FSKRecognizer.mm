
//
//  FSKRecognizer.mm
//  FSK Terminal
//
//  Created by George Dean on 12/22/08.
//  Copyright 2008 Perceptive Development. All rights reserved.
//
//  Edited by Ezequiel Franca on 20/04/14

#import "FSKModemConfig.h"
#import "FSKRecognizer.h"
#import "FSKByteQueue.h"
#import "MultiDelegate.h"
#import "CharReceiver.h"

#define WL_HIGH (1000000000 / FREQ_HIGH)
#define WL_LOW  (1000000000 / FREQ_LOW)
#define HWL_HIGH (500000000 / FREQ_HIGH)
#define HWL_LOW  (500000000 / FREQ_LOW)

#define SMOOTHER_COUNT (FSK_SMOOTH * (FSK_SMOOTH + 1) / 2)

#define DISCRIMINATOR (SMOOTHER_COUNT * (WL_HIGH + WL_LOW) / 4)

#define BIT_PERIOD     (1000000000 / BAUD)
#define HALF_BIT_PERIOD (500000000 / BAUD)

@implementation FSKRecognizer

- (id) init
{
	if(self = [super init])
	{
		receivers = [[MultiDelegate alloc] init];
		byteQueue = new FSKByteQueue();
		[self reset];
	}

	return self;
}

- (void) addReceiver:(id<CharReceiver>)receiver
{
	[receivers addDelegate:receiver];
}

- (void) commitBytes
{
	char input;
	while (byteQueue->get(input))
	{
		//NSLog(@"Byte: %c (%02x)", input, (unsigned char)input);
		[receivers receivedChar:input];
	}
}

- (void) dataBit:(BOOL)one
{
	if(one)
		bits |= (1 << bitPosition);
	bitPosition++;
}

- (void) freqRegion:(unsigned)length high:(BOOL)high
{
	FSKRecState newState = FSKFail;
	switch (state) {
		case FSKStart:
			if(!high) // Start Bit
			{
				//NSLog(@"Start bit: %c %d", high?'H':'L', length);
				newState = FSKBits;
				bits = 0;
				bitPosition = 0;
			}
			else
				newState = FSKStart;
			break;
		case FSKBits:
			//NSLog(@"Bit: %c %d", high?'H':'L', length);
			if((bitPosition <= 7)){ //Data Bits
				newState = FSKBits;
				[self dataBit:high];
			}
			else if(bitPosition == 8){ // Stop Bit
				newState = FSKStart;
				byteQueue->put(bits);
				[self performSelectorOnMainThread:@selector(commitBytes)
									   withObject:nil
									waitUntilDone:NO];
				bits = 0;
				bitPosition = 0;
			}
			break;
        case FSKSuccess:
            NSLog(@"FSKSucess");
            break;
        case FSKFail:
            NSLog(@"FSKFail");
            break;
	}
	state = newState;
}

- (void) halfWave:(unsigned)width
{
	for (int i = FSK_SMOOTH - 2; i >= 0; i--) {
		halfWaveHistory[i+1] = halfWaveHistory[i];
	}
	halfWaveHistory[0] = width;

	unsigned waveSum = 0;
	for(int i=0; i<FSK_SMOOTH; ++i)
	{
		waveSum += halfWaveHistory[i] * (FSK_SMOOTH - i);
	}

	BOOL high = (waveSum < DISCRIMINATOR);
	unsigned avgWidth = waveSum / SMOOTHER_COUNT;

	recentWidth += width;
	recentAvrWidth += avgWidth;

//	NSLog(@"high %d, width %u(avg %u), low %u, high %u, w %u,a %u",high, width/1000, avgWidth/1000, recentLows/1000, recentHighs/1000,recentWidth/1000,recentAvrWidth/1000);


	if (state == FSKStart)
	{
		if(!high)
		{
			recentLows += avgWidth;
		}
		else if(recentLows)
		{
			recentHighs += avgWidth;
			if(recentHighs > recentLows)
			{
//				NSLog(@"False start: %d", recentLows);
				recentLows = recentHighs = 0;
			}
		}

		if(recentLows + recentHighs >= BIT_PERIOD)
		{
			[self freqRegion:recentLows high:FALSE];
			recentWidth = recentAvrWidth = 0;
			if(recentLows < BIT_PERIOD)
			{
				recentLows = 0;
			}
			else
			{
				recentLows -= BIT_PERIOD;
			}
			if(!high)
				recentHighs = 0;
		}
	}
	else
	{
		if(high)
			recentHighs += avgWidth;
		else
			recentLows += avgWidth;

		if(recentLows + recentHighs >= BIT_PERIOD)
		{
			BOOL regionHigh = (recentHighs > recentLows);
			[self freqRegion:regionHigh?recentHighs:recentLows high:regionHigh];

			recentWidth -= BIT_PERIOD;
			recentAvrWidth -= BIT_PERIOD;

			if(state == FSKStart)
			{
				// The byte ended, reset the accumulators
				recentLows = recentHighs = 0;
				return;
			}

			unsigned* matched = regionHigh?&recentHighs:&recentLows;
			unsigned* unmatched = regionHigh?&recentLows:&recentHighs;
			if(*matched < BIT_PERIOD)
			{
				*matched = 0;
			}
			else
			{
				*matched -= BIT_PERIOD;
			}
			if(high == regionHigh)
				*unmatched = 0;
		}
	}
}

- (void) edge: (int)height width:(UInt64)nsWidth interval:(UInt64)nsInterval
{
	if(nsInterval <= HWL_LOW + HWL_HIGH)
		[self halfWave:(unsigned)nsInterval];
	else {
//		NSLog(@"skip");
	}

}

- (void) idle: (UInt64)nsInterval
{
	[self reset];
}

- (void) reset
{
	bits = 0;
	bitPosition = 0;
	state = FSKStart;
	for (int i = 0; i < FSK_SMOOTH; i++) {
		halfWaveHistory[i] = (WL_HIGH + WL_LOW) / 4;
	}
	recentLows = recentHighs = 0;
}

@end
