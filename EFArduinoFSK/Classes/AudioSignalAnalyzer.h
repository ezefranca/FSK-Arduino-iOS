//
//  AudioSignalAnalyzer.h
//  iNfrared
//
//  Created by George Dean on 11/28/08.
//  Copyright 2008 Perceptive Development. All rights reserved.
//
//  Edited by Ezequiel Franca on 20/04/14

#import "AudioQueueObject.h"
#import "PatternRecognizer.h"

//#define DETAILED_ANALYSIS

typedef struct
{
	int			lastFrame;
	int			lastEdgeSign;
	unsigned	lastEdgeWidth;
	int			edgeSign;
	int			edgeDiff;
	unsigned	edgeWidth;
	unsigned	plateauWidth;
#ifdef DETAILED_ANALYSIS
	SInt64		edgeSum;
	int			edgeMin;
	int			edgeMax;
	SInt64		plateauSum;
	int			plateauMin;
	int			plateauMax;
#endif
}
analyzerData;

@interface AudioSignalAnalyzer : AudioQueueObject {
	BOOL	stopping;
	analyzerData pulseData;
	NSMutableArray* recognizers;
}

@property (readwrite) BOOL	stopping;
@property (readonly) analyzerData* pulseData;

- (void) addRecognizer: (id<PatternRecognizer>)recognizer;

- (void) setupRecording;

- (void) record;
- (void) stop;

- (void) edge: (int)height width:(unsigned)width interval:(unsigned)interval;
- (void) idle: (unsigned)samples;
- (void) reset;

@end
