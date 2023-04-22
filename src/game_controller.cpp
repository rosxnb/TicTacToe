#include "game_controller.hpp"

void GameController::onButtonPressed(float buttonIndex)
{
    __builtin_printf("Keypress detected. Received code value: %f", buttonIndex);
}
