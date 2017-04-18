FSK-Arduino-iOS 
========
[![Build Status](http://img.shields.io/travis/ezefranca/FSK-Arduino-iOS.svg?style=flat)](https://travis-ci.org/ezefranca/FSK-Arduino-iOS)
[![Badge w/ Version](https://cocoapod-badges.herokuapp.com/v/FSK-Arduino-iOS/badge.png)](http://cocoadocs.org/docsets/FSK-Arduino-iOS)
[![Badge w/ Platform](https://cocoapod-badges.herokuapp.com/p/FSK-Arduino-iOS/badge.svg)](http://cocoadocs.org/docsets/FSK-Arduino-iOS)
[![wercker status](https://app.wercker.com/status/7d5a33ceed1006aae64e3937f40d2bf1/s "wercker status")](https://app.wercker.com/project/bykey/7d5a33ceed1006aae64e3937f40d2bf1)


Dependencies for iOS Development, using [Sofmodem Arduino library](https://code.google.com/p/arms22/downloads/detail?name=SoftModem-005.zip&can=2&q=), with FSK communication.

[![imagem](https://raw.githubusercontent.com/ezefranca/FSK-Arduino-iOS7/master/FSK-Demo/image.png)](http://ironbark.xtelco.com.au/subjects/DC/lectures/7/)

=====

How to use
====

This libraries have a propouse work with an Arduino using a Sofmodem Shield* to communicate with iOS using FSK. Currently, the source code of the SoftModem is not made as a framework. If you want to use SoftModem in your project, you have two ways:

## Adding FSK-Arduino-iOS to your project

### CocoaPods

[CocoaPods](http://cocoapods.org) is the recommended way to add FSK-Arduino-iOS to your project.

1. Add a pod entry for FSK-Arduino-iOS to your Podfile `pod 'FSK-Arduino-iOS', '~> 0.0.2'`
2. Install the pod(s) by running `pod install`.

### Source files

Alternatively you can directly adding source files to your project.

1. Download the [latest code version](https://github.com/ezefranca/FSK-Arduino-iOS/archive/master.zip) or add the repository as a git submodule to your git-tracked project. 
2. The Folder /FSK must be copied from the source code of your project. The following is the list of source code related to FSK. Please copy these to the project source code.

```objectivec
* AudioQueueObject.h
* AudioQueueObject.m
* AudioSignalAnalyzer.h
* AudioSignalAnalyzer.m
* AudioSignalGenerator.h
* AudioSignalGenerator.m
* CharReceiver.h
* FSKModemConfig.h
* FSKByteQueue.h
* FSKRecognizer.h
* FSKRecognizer.mm
* FSKSerialGenerator.h
* FSKSerialGenerator.m
* lockfree.h
* MultiDelegate.h
* MultiDelegate.m
* PatternRecognizer.h
```

SoftModem uses the following two framework for audio input and output. Please add them to your project.

![Image](https://raw.githubusercontent.com/ezefranca/FSK-Arduino-iOS7/master/FSK-Demo/FSK-Demo/framework.png)

```objectivec
* AudioToolbox.framework
* AVFoundation.framework
```


Initialization
=====

First, set the category of application with AVAudioSession class. To do voice recording and playback, AVAudioSessionCategoryPlayAndRecord need to be set.

```objectivec
AVAudioSession *session = [AVAudioSession sharedInstance];   
[[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(interruption:) name:
AVAudioSessionInterruptionNotification object:nil];

[session setCategory:AVAudioSessionCategoryPlayAndRecord error:nil];
[session setCategory:AVAudioSessionCategoryPlayback error:nil];
[session setActive:YES error:nil];
```

interruption selector method

```objectivec
- (void) interruption:(NSNotification*)notification
{
    NSDictionary *interuptionDict = notification.userInfo;
    NSUInteger interuptionType = (NSUInteger)[interuptionDict valueForKey:AVAudioSessionInterruptionTypeKey];
    
    if (interuptionType == AVAudioSessionInterruptionTypeBegan)
        [self beginInterruption];
# if __CC_PLATFORM_IOS >= 40000
    else if (interuptionType == AVAudioSessionInterruptionTypeEnded)
        [self endInterruptionWithFlags:(NSUInteger)[interuptionDict valueForKey:AVAudioSessionInterruptionOptionKey]];
# else
    else if (interuptionType == AVAudioSessionInterruptionTypeEnded)
        [self endInterruption];
# endif
}
```

Next, for analysis of the voice, make instance of class AudioSignalAnalyzer, FSKRecognizer and AudioSignalAnalyzer parses the input waveform from the microphone to detect the falling and rising edge of the waveform. FSKRecognizer restores the data bits based on the results of the analysis of AudioSignalAnalyzer.

```objectivec
        recognizer = [[FSKRecognizer alloc] init];     
        [recognizer addReceiver:self];
        analyzer = [[AudioSignalAnalyzer alloc] init]; 
        [analyzer addRecognizer:recognizer];
```

Then create an instance of a class FSKSerialGenerator for sound output. FSKSerialGenerator converts the data bits to audio signal and output.

```objectivec
        generator = [[FSKSerialGenerator alloc] init]; 
        [generator play];
````
Receiving
=====

Register the class that implements the CharReceiver protocol to the FSKRecognizer class, and AVAudioSessionDelegate.

```objectivec
@interface YourClass : NSObject <AVAudioSessionDelegate, CharReceiver>
````
Register FSKRecognizer class at initialization.

```objectivec
YourClass *yourClassInstance;
[recognizer addReceiver: yourClassInstance];
```
receivedChar: method is called　when one byte of data is received.

```objectivec
- (void) receivedChar: (char) input
{
     // Receive handling
}
```
Sending
=====

Sending data is much easier than receiving data. FSKSerialGenerator class's writeByte: method to sends a single byte.

```objectivec
[generator writeByte: 0xff];
```

Links and Credits
=====
[arms22](http://arms22.blog91.fc2.com/) - Creator of Softmodem hardware, libraries for Arduino and ARC version lib for iOS.

[Arduino Libraries](https://code.google.com/p/arms22/downloads/detail?name=SoftModem-005.zip&can=2&q=)

[iOS 4/5 ARC version](https://github.com/9labco/IR-Remote)

[FSK Wikipedia](http://en.wikipedia.org/wiki/Frequency-shift_keying)

[FSK Explanation](http://ironbark.xtelco.com.au/subjects/DC/lectures/7/)

License
=====
This code is distributed under the terms and conditions of the [MIT license](LICENSE). 


