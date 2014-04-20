//
//  AudioDemo.h
//  FSK-Demo
//
//  Created by Ezequiel Franca dos Santos on 20/04/14.
//  Copyright (c) 2014 Ezequiel Franca dos Santos. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>

#import "CharReceiver.h"

@class AudioSignalAnalyzer, FSKSerialGenerator;

@interface AudioDemo : NSObject <AVAudioSessionDelegate, CharReceiver>{
    char local_input;
}
@property (strong, nonatomic) AudioSignalAnalyzer* analyzer;
@property (strong, nonatomic) FSKSerialGenerator* generator;

+ (id)shared;

- (void)signalArduino:(BOOL)on;
- (char)returnChar;
@end