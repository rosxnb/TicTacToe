#include <iostream>
#import "game_controller.hpp"
#import <GameController/GameController.h>
#import <Foundation/Foundation.h>

// void onButtonPressedCallback(GCControllerButtonInput *button, float value, BOOL pressed) 
// {
//     if (pressed) 
//     {
//         GameController gameController;
//         gameController.onButtonPressed(button.buttonIndex);
//     }
// }
// 
// void onButtonPressedCallback(GCControllerButtonInput *button, float value, BOOL pressed) {
//     if (pressed) {
//         GameController gameController;
//         if (button == button.controller.extendedGamepad.buttonA) {
//             gameController.onButtonPressed(0);
//         } else if (button == button.controller.extendedGamepad.buttonB) {
//             gameController.onButtonPressed(1);
//         } else if (button == button.controller.extendedGamepad.buttonX) {
//             gameController.onButtonPressed(2);
//         } else if (button == button.controller.extendedGamepad.buttonY) {
//             gameController.onButtonPressed(3);
//         }
//         // Add additional button mappings as needed
//     }
// }

// void onButtonPressedCallback(GCControllerButtonInput* button, float value, bool pressed) {
//     __builtin_printf("Keypress detected. Received code value: %f", value);
//     if (pressed) {
//         GameController gameController;
//         gameController.onButtonPressed(value);
//     }
// }

void installInputCallbacks() 
{
    NSArray<GCController *> *controller = [GCController controllers];
    NSLog(@"%@", controller);
    // GCControllerButtonInput *button = controller.extendedGamepad.buttonA;
    // [button setValueChangedHandler:^(GCControllerButtonInput *button, float value, BOOL pressed) {
    //     onButtonPressedCallback(button, value, pressed);
    // }];
}
