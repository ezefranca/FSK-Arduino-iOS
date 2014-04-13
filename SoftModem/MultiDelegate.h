//
//  MultiDelegate.h
//  iNfrared
//
//  Created by George Dean on 1/19/09.
//  Copyright 2009 Perceptive Development. All rights reserved.
//

#import <Foundation/Foundation.h>


@interface MultiDelegate : NSObject {
	NSMutableSet* delegateSet;
	Protocol* proto;
}

- (id) initWithProtocol:(Protocol*)p;
- (void) addDelegate:(id)delegate;

@end
