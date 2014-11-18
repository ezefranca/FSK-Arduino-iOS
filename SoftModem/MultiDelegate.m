//
//  MultiDelegate.m
//  iNfrared
//
//  Created by George Dean on 1/19/09.
//  Copyright 2009 Perceptive Development. All rights reserved.
//

#import "MultiDelegate.h"
#import <objc/runtime.h>

@implementation MultiDelegate

- (id) initWithProtocol:(Protocol*)p
{
	if(self = [super init])
	{
		proto = p;
	}

	return self;
}

- (void) addDelegate:(id)delegate
{
	if(!delegateSet)
		delegateSet = [[NSMutableSet alloc] init];
	[delegateSet addObject:delegate];
}

- (NSMethodSignature*) methodSignatureForSelector:(SEL)selector
{
	if([delegateSet count])
		return [[delegateSet anyObject] methodSignatureForSelector:selector];

	if(proto)
	{
		struct objc_method_description desc = protocol_getMethodDescription(proto, selector, YES, YES);
		return [NSMethodSignature signatureWithObjCTypes:desc.types];
	}

	// return void-void signature by default
	return nil;
    //return [self methodSignatureForSelector:@selector(void)];
}

- (void) forwardInvocation:(NSInvocation*)invocation
{
	if(delegateSet) for (id delegate in delegateSet) {
		if([delegate respondsToSelector:[invocation selector]])
			[invocation invokeWithTarget:delegate];
	}
}

//- (void) receivedChar:(char)input{
//    //
//}

@end
