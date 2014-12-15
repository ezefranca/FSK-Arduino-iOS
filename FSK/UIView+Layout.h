//
//  UIView+UIFont+Layout.h
//  Labour
//
//  Created by Bret Cheng on 29/12/2010.
//  Copyright 2010 __MyCompanyName__. All rights reserved.
//

#import <Foundation/Foundation.h>

@interface UIView (Layout)

@property(nonatomic, readonly)CGFloat width;
@property(nonatomic, readonly)CGFloat height;
@property(nonatomic, readonly)CGFloat top;
@property(nonatomic, readonly)CGFloat bottom;
@property(nonatomic, readonly)CGFloat left;
@property(nonatomic, readonly)CGFloat right;

@end

@implementation UIView (Layout)

- (CGFloat)width {
	return self.frame.size.width;
}

- (CGFloat)height {
	return self.frame.size.height;
}

- (CGFloat)top {
	return self.frame.origin.y;
}

- (CGFloat)bottom {
	return self.frame.origin.y + self.frame.size.height;
}

- (CGFloat)left {
	return self.frame.origin.x;
}

- (CGFloat)right {
	return self.frame.origin.x + self.frame.size.width;
}
@end

