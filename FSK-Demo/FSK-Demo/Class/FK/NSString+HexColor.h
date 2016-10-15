//
//  NSStringColor.h
//  PhoneGap
//
//  Created by Michael Nachbaur on 08/05/09.
//  Copyright 2009 Decaf Ninja Software. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface NSString (HexColor)

- (UIColor*)colorFromHex;

@end
