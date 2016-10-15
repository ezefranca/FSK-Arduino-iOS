//
//  ViewController.h
//  FSK-Demo
//
//  Created by Ezequiel Franca dos Santos on 20/04/14.
//  Copyright (c) 2014 Ezequiel Franca dos Santos. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "CharReceiver.h"
#import "AudioSignalAnalyzer.h"
#import "FSKSerialGenerator.h"
#import "FSKRecognizer.h"
#import "AudioDemo.h"

@interface ViewController : UIViewController <AVAudioSessionDelegate>

@property (strong, nonatomic) IBOutlet UILabel *valueLabel;

- (BOOL)readArduino;
- (BOOL)writeArduino;

- (IBAction)sendButton:(id)sender;
- (IBAction)receiveButton:(id)sender;


@end
