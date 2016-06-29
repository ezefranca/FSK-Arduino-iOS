#
# Be sure to run `pod lib lint EFArduinoFSK.podspec' to ensure this is a
# valid spec before submitting.
#
# Any lines starting with a # are optional, but their use is encouraged
# To learn more about a Podspec see http://guides.cocoapods.org/syntax/podspec.html
#

Pod::Spec.new do |s|
  s.name             = 'EFArduinoFSK'
  s.version          = '0.3.0'
  s.summary          = 'A short description of EFArduinoFSK.'
  s.description      = <<-DESC
TODO: Add long description of the pod here.
                       DESC

  s.homepage         = 'https://github.com/ezefranca/EFArduinoFSK'
  # s.screenshots     = 'www.example.com/screenshots_1', 'www.example.com/screenshots_2'
  s.license          = { :type => 'MIT', :file => 'LICENSE' }
  s.author           = { 'ezefranca' => 'ezequiel.ifsp@gmail.com' }
  s.source           = { :git => 'https://github.com/ezefranca/EFArduinoFSK.git', :tag => '0.0.5' }
  s.social_media_url = 'https://twitter.com/ezefranca'

  s.ios.deployment_target = '8.0'
s.platform     = :ios, '7.0' 
# s.platform     = :ios
  s.source_files = 'EFArduinoFSK/Classes/**/*'
s.exclude_files = "LICENSE, README.md, EFArduinoFSK-iOS.podspec"
 #s.exclude_files ='EFArduinoFSK/Classes/lockfree.h'	  
  s.requires_arc = false
  s.frameworks   = 'AudioToolbox', 'AVFoundation'

  # s.resource_bundles = {
  #   'EFArduinoFSK' => ['EFArduinoFSK/Assets/*.png']
  # }

  # s.public_header_files = 'Pod/Classes/**/*.h'
  # s.frameworks = 'UIKit', 'MapKit'
  # s.dependency 'AFNetworking', '~> 2.3'
end
