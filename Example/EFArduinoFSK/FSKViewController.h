//
//  FSKViewController.h
//  EFArduinoFSK
//
//  Created by ezefranca on 06/28/2016.
//  Copyright (c) 2016 ezefranca. All rights reserved.
//

@import UIKit;
#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "CharReceiver.h"
#import "AudioSignalAnalyzer.h"
#import "FSKSerialGenerator.h"
#import "FSKRecognizer.h"
#import "FSKAudioDemo.h"

@interface FSKViewController : UIViewController <AVAudioSessionDelegate>

@property (strong, nonatomic) IBOutlet UILabel *valueLabel;

- (BOOL)readArduino;
- (BOOL)writeArduino;

- (IBAction)sendButton:(id)sender;
- (IBAction)receiveButton:(id)sender;

@end

