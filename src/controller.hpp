#import <GameController/GameController.h>

@interface InputController : NSObject

@property (nonatomic, strong) NSMutableSet<NSNumber *> *keysPressed;
+ (instancetype)controller;

@end

