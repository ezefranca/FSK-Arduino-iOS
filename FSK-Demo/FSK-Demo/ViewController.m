//
//  ViewController.m
//  FSK-Demo
//
//  Created by Ezequiel Franca dos Santos on 20/04/14.
//  Copyright (c) 2014 Ezequiel Franca dos Santos. All rights reserved.
//

#import "ViewController.h"

@interface NSString (NSStringHexToBytes)
-(NSData*) hexToBytes ;
@end

@implementation NSString (NSStringHexToBytes)
-(NSData*) hexToBytes {
    NSMutableData* data = [NSMutableData data];
    int idx;
    for (idx = 0; idx+2 <= self.length; idx+=2) {
        NSRange range = NSMakeRange(idx, 2);
        NSString* hexStr = [self substringWithRange:range];
        NSScanner* scanner = [NSScanner scannerWithString:hexStr];
        unsigned int intValue;
        [scanner scanHexInt:&intValue];
        [data appendBytes:&intValue length:1];
    }
    return data;
}
@end

@interface ViewController ()

@end

@implementation ViewController

- (void)viewDidLoad
{
    [super viewDidLoad];
	// Do any additional setup after loading the view, typically from a nib.
}

- (void)didReceiveMemoryWarning
{
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}

#pragma mark - read Arduino (send to arduino 0x00 and wait the response)
//For more details see the arduino firmware

- (BOOL)readArduino{
    for (int i =0; i < 15; i++) // Just for me dont need do 15 clicks
        [[[AudioDemo shared] generator] writeByte:0x00];
    
    NSString *s = [NSString stringWithFormat:@"%d",[[AudioDemo shared] returnChar]];
    self.valueLabel.text = s;
    return YES;
}

#pragma mark - write Arduino (send to arduino 0xFF)
//For more details see the arduino firmware

- (BOOL)writeArduino{
        [[[AudioDemo shared] generator] writeByte:0xFF];
    return YES;
}

#pragma mark - buttons

- (IBAction)sendButton:(id)sender {
    [self writeArduino];
    
}

- (IBAction)receiveButton:(id)sender {
    [self readArduino];
}



@end
