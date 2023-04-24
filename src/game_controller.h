#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#import <GameController/GameController.h>

@interface InputController : NSObject
@property (nonatomic, strong) NSMutableSet<NSNumber *> *keysPressed;
+ (instancetype)controller;
@end

#endif
