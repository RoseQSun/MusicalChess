#ifndef COMMENTARY_SONIFIER_H
#define COMMENTARY_SONIFIER_H

#include "Sonifier.h"

class CommentarySonifier: public Sonifier {
  public:
    CommentarySonifier(float sampleRate);
  protected:
    void onMove(Chess::Game &game) override;

  private:
    double audioSampleRate;
    juce::AudioSampleBuffer ranks[8];
    juce::AudioSampleBuffer files[8];
    std::unordered_map<Chess::Piece::Type, std::unique_ptr<juce::AudioSampleBuffer>> pieces;
    std::unordered_map<Chess::Square, std::unique_ptr<juce::AudioSampleBuffer>> squares;
    juce::AudioSampleBuffer equals, takes, check, castleShort, castleLong;
};

#endif // COMMENTARY_SONIFIER_H
