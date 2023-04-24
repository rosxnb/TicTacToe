#import "game_controller.h"

@implementation InputController
+ (instancetype)controller {
    static InputController *inputController = nil;
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        inputController = [[InputController alloc] init];
    });
    return inputController;
}

- (instancetype)init {
    if (self = [super init]) {
        self.keysPressed = [[NSMutableSet alloc] init];

        NSNotificationCenter *center = [NSNotificationCenter defaultCenter];

        [center addObserverForName:GCKeyboardDidConnectNotification object:nil queue:nil usingBlock:^(NSNotification *notification) {
            GCKeyboard *keyboard = (GCKeyboard *)notification.object;
            keyboard.keyboardInput.keyChangedHandler = ^(GCKeyboardInput *keyboard, GCControllerButtonInput *key, GCKeyCode keyCode, BOOL pressed) {
                if (pressed) {
                    [self.keysPressed addObject:@(keyCode)];
                } else {
                    [self.keysPressed removeObject:@(keyCode)];
                }
            };
        }];
    }

    return self;
}
@end
