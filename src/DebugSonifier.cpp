//
// Created by Rose Sun on 2/17/22.
//

#include "DebugSonifier.h"

DebugSonifier::DebugSonifier (){
    srand(time(0));
}

DebugSonifier::~DebugSonifier() {
}

void DebugSonifier::sonifyPiece(Chess::Square const& square, Chess::Piece const& piece) {
    float frequencies[8] = {262, 294.8, 327.5, 349.3, 393, 436.7, 491.2, 524};
    float gains[8] = {1, 0.8, 0.6, 0.4, 0.4, 0.6, 0.8, 1};
    float pans[8] = { 0.0, 0.2, 0.4, 0.5, 0.5, 0.6, 0.8, 1.0 };
    int freqIdx = static_cast<int>(square.file);
    int gainIdx = static_cast<int>(square.rank);
    int panIdx = rand() % 8;

    if (piece.color == Chess::Color::Black && piece.type == Chess::Piece::Type::Pawn){
        oscillators.emplace_back(sine, frequencies[freqIdx], gains[gainIdx],44100);
    }
    else if (piece.color == Chess::Color::Black && piece.type != Chess::Piece::Type::Pawn){
        oscillators.emplace_back(sine, frequencies[freqIdx], gains[gainIdx],44100);
    }
    else if (piece.color == Chess::Color::White && piece.type == Chess::Piece::Type::Pawn){
        oscillators.emplace_back(sine, frequencies[freqIdx], gains[gainIdx],44100);
    }
    else if (piece.color == Chess::Color::White && piece.type != Chess::Piece::Type::Pawn) {
        oscillators.emplace_back(sine, frequencies[freqIdx], gains[gainIdx], 44100);
    } else {
        assert(false);
    }
    oscillators.back().noteOn();
    oscillators.back().setADSRParameters(2,0,1,2);
    oscillators.back().setPan(pans[panIdx]);
    m_mainProcessor.addInstRef(oscillators.back());
}

void DebugSonifier::process(float **ppfOutBuffer, int iNumChannels, int iNumFrames) {

    m_mainProcessor.process(ppfOutBuffer, iNumChannels, iNumFrames);
}


void DebugSonifier::prepareToPlay(int iExpectedBlockSize, double fsampleRate){

    m_fSampleRate = fsampleRate;
    m_mainProcessor.setSampleRate(fsampleRate);
    m_mainProcessor.setGain(0.25);
    m_mainProcessor.setADSRParameters(4,0,1,2);
    m_fExpectedBlockSize = iExpectedBlockSize;

};


void DebugSonifier::releaseResources(){

};


Error_t DebugSonifier::onMove(Chess::Board &board) {
    auto it = oscillators.begin();
    while (it != oscillators.end()){
        if (it->isActive()){
            it->noteOff();
            it++;
        }
        else {
            m_mainProcessor.removeInstRef(*it);
            it = oscillators.erase(it);
        }
    }
    for( const auto [square, piece] : board.getPieceMap() ) {
        sonifyPiece(square, piece);
    }
    return Error_t::kNoError;
}
