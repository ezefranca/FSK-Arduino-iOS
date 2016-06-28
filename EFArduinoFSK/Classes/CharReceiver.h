//
//  CharReceiver.h
//  FSK Terminal
//
//  Created by George Dean on 1/18/09.
//  Copyright 2009 Perceptive Development. All rights reserved.
//
//  Edited by Ezequiel Franca on 20/04/14

#import <Foundation/Foundation.h>


@protocol CharReceiver

- (void) receivedChar:(char)input;

@end
