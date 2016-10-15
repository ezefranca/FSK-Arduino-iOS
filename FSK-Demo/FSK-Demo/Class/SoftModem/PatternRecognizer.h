//
//  PatternRecognizer.h
//  iNfrared
//
//  Created by George Dean on 12/22/08.
//  Copyright 2008 Perceptive Development. All rights reserved.
//

#import <Foundation/Foundation.h>


@protocol PatternRecognizer <NSObject>

- (void) edge: (int)height width:(UInt64)nsWidth interval:(UInt64)nsInterval;
- (void) idle: (UInt64)nsInterval;
- (void) reset;

@end
