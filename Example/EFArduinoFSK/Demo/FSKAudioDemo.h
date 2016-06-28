//
//  FSKAudioDemo.h
//  EFArduinoFSK
//
//  Created by techthings on 6/28/16.
//  Copyright © 2016 ezefranca. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <AVFoundation/AVFoundation.h>
#import <AudioToolbox/AudioToolbox.h>
#import "CharReceiver.h"

@class AudioSignalAnalyzer, FSKSerialGenerator;

@interface FSKAudioDemo : NSObject <AVAudioSessionDelegate, CharReceiver>{
    char local_input;
}

@property (strong, nonatomic) AudioSignalAnalyzer* analyzer;
@property (strong, nonatomic) FSKSerialGenerator* generator;

+ (id)shared;

- (void)signalArduino:(BOOL)on;
- (char)returnChar;

@end
