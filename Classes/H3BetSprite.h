#ifndef BetSprite_h
#define BetSprite_h

class BetSprite : public CCSprite
{
public:
    BetSprite();
    ~BetSprite();
    static BetSprite* create(SCORE nBet, int nChairID);
    virtual bool init(SCORE nBet, int nChairID);
    
    CC_SYNTHESIZE(int, m_nChairID, ChairID);
    
};

#endif
