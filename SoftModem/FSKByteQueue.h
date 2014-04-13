/*
 *  FSKByteQueue.h
 *  SoftModemTerminal
 *
 *  Created by arms22 on 10/07/22.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */

#import "lockfree.h"

struct FSKByteQueue: public lock_free::queue<char> {
	FSKByteQueue(): lock_free::queue<char>(256){};
};
