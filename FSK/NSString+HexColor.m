//
//  NSString+HexColor.m
//  PhoneGap
//
//  Created by Michael Nachbaur on 08/05/09.
//  Copyright 2009 Decaf Ninja Software. All rights reserved.
//

#import "NSString+HexColor.h"

@implementation NSString (HexColor)

- (UIColor*)colorFromHex
{
    NSString *hexColor = [[self stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]] uppercaseString];  

    if ([hexColor length] < 6)
        return [UIColor blackColor];
    if ([hexColor hasPrefix:@"#"])
        hexColor = [hexColor substringFromIndex:1];  
    if ([hexColor length] != 6 && [hexColor length] != 8)
        return [UIColor blackColor];
    
    NSRange range;  
    range.location = 0;  
    range.length = 2; 
    
    NSString *rString = [hexColor substringWithRange:range];  
    
    range.location = 2;  
    NSString *gString = [hexColor substringWithRange:range];  
    
    range.location = 4;  
    NSString *bString = [hexColor substringWithRange:range];  
    
    range.location = 6;
    NSString *aString = @"FF";
    if ([hexColor length] == 8)
        aString = [hexColor substringWithRange:range];

    // Scan values  
    unsigned int r, g, b, a;  
    [[NSScanner scannerWithString:rString] scanHexInt:&r];  
    [[NSScanner scannerWithString:gString] scanHexInt:&g];  
    [[NSScanner scannerWithString:bString] scanHexInt:&b];  
    [[NSScanner scannerWithString:aString] scanHexInt:&a];  
    
    return [UIColor colorWithRed:((float) r / 255.0f)  
                           green:((float) g / 255.0f)  
                            blue:((float) b / 255.0f)  
                           alpha:((float) a / 255.0f)];
}

@end
