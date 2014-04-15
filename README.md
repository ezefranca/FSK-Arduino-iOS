FSK-iOS7
========

Dependencies for iOS7 Development Non-ARC, using [Sofmodem Arduino library](https://code.google.com/p/arms22/downloads/detail?name=SoftModem-005.zip&can=2&q=), with FSK communication.

=====

How to use
====

This libraries have a propouse work with an Arduino using a Sofmodem Shield* to communicate with iOS using FSK. Currently, the source code of the SoftModem is not made as a framework. If you want to use SoftModem in your project, the source code related to the SoftModem must be copied from the source code of the SoftModemTerminal. The following is the list of source code related to SoftModem. Please copy these to the project source code.

```c
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

```
* AudioToolbox.framework
* AVFoundation.framework
```
Initialization
=====

First, set the category of application with AVAudioSession class. To do voice recording and playback, AVAudioSessionCategoryPlayAndRecord need to be set.

```objectivec
AVAudioSession * session = [AVAudioSession sharedInstance];
session.delegate = self;
[Session setCategory: AVAudioSessionCategoryPlayAndRecord error: nil];
[Session setActive: YES error: nil];
```
Next, for analysis of the voice, make instance of class AudioSignalAnalyzer, FSKRecognizer and AudioSignalAnalyzer parses the input waveform from the microphone to detect the falling and rising edge of the waveform. FSKRecognizer restores the data bits based on the results of the analysis of AudioSignalAnalyzer.

```objectivec
recognizer = [[FSKRecognizer alloc] init];
analyzer = [[AudioSignalAnalyzer alloc] init];
[Analyzer addRecognizer: recognizer]; // set recognizer to analyzer
[analyzer record]; // start analyzing
```

Then create an instance of a class FSKSerialGenerator for sound output. FSKSerialGenerator converts the data bits to audio signal and output.

```objectivec
generator = [[FSKSerialGenerator alloc] init];
[Generator play]; // audio output starts
````
Receiving
=====

Register the class that implements the CharReceiver protocol to the FSKRecognizer class.
```objectivec
@ Interface MainViewController: UIViewController <CharReceiver>
````
Register FSKRecognizer class at initialization.
```objectivec
MainViewController * mainViewController;
[Recognizer addReceiver: mainViewController];
```
receivedChar: method is called　when one byte of data is received.
```objectivec
- (Void) receivedChar: (char) input
{
     // Receive handling
}
```
Sending
=====

Sending data is much easier than receiving data. FSKSerialGenerator class's writeByte: method to sends a single byte.
```objectivec
[Generator writeByte: 0xff];
```

Links and Credits
=====
[arms22](http://arms22.blog91.fc2.com/) - creator of Softmodem hardware, libraries for Arduino and ARC version lib for iOS.

[Arduino Libraries](https://code.google.com/p/arms22/downloads/detail?name=SoftModem-005.zip&can=2&q=)

[iOS 4/5 ARC version]()

[FSK Wikipedia]()


