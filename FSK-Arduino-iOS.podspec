Pod::Spec.new do |s|
  s.name         = "FSK-Arduino-iOS"
  s.version      = "0.0.1"
  s.summary      = "FSK Library for iOS interface with Arduino Development."
  s.homepage     = "http://github.com/ezefranca/FSK-Arduino-iOS"
  # s.screenshots  = "www.example.com/screenshots_1.gif", "www.example.com/screenshots_2.gif"
  s.license      = { :type => "MIT", :file => "LICENSE" }
  s.author             = { "ezefranca" => "ezequiel.ifsp@gmail.com" }
  s.social_media_url   = "http://twitter.com/ezefranca"
  s.platform     = :ios
  s.source       = { :git => "https://github.com/ezefranca/FSK-Arduino-iOS.git", :tag => "0.0.1" }
  #s.source_files = "../SoftModem.{h,m}, ../FK.{h,m}"
  s.source_files = "*"
  s.exclude_files = "Arduino-SoftModem"
  s.requires_arc = false
end
