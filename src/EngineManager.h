#pragma once

#include <filesystem>
#include <future>

#include <juce_gui_extra/juce_gui_extra.h>

#include "BoardComponent.h"
#include "EngineBridge.h"
#include "GameState.h"

class EngineManager {
public:
    EngineManager(const std::string &path)
        : engine(path) {
    }

    void generateMove() {
        Chess::Game &game = AppState::getInstance().getGame();
        // Wait for engine to pick a move in a separate thread,
        // to avoid blocking the UI.
        task = std::async(std::launch::async, [this, &game] {
            Chess::Move move = engine.analyze(game).bestMove;
            if (onMove) onMove(move);
        });
    }

    std::function<void (Chess::Move)> onMove;

private:
    std::future<void> task;
    Chess::Engine engine;
};
