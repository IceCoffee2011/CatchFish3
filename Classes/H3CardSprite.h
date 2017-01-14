#ifndef CardFlip_h
#define CardFlip_h

class MyScaleTo : public CCActionInterval
{
public:
    /** initializes the action with the same scale factor for X and Y */
    bool initWithDuration(float duration, float s);
    
    /** initializes the action with and X factor and a Y factor */
    bool initWithDuration(float duration, float sx, float sy);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual CCObject* copyWithZone(CCZone* pZone);
    virtual void startWithTarget(CCNode *pTarget);
    virtual void update(float time);
    
public:
    
    /** creates the action with the same scale factor for X and Y */
    static MyScaleTo* create(float duration, float s);
    
    /** creates the action with and X factor and a Y factor */
    static MyScaleTo* create(float duration, float sx, float sy);
protected:
    float m_fScaleX;
    float m_fScaleY;
    float m_fStartScaleX;
    float m_fStartScaleY;
    float m_fEndScaleX;
    float m_fEndScaleY;
    float m_fDeltaX;
    float m_fDeltaY;
};

class CardSprite : public CCSprite
{
public:
    CardSprite();
    ~CardSprite();
    static CardSprite* create(const char* inCardImageName, const char* outCardImageName, BYTE cbCardData, float duration);
    virtual bool init(const char* inCardImageName, const char* outCardImageName, BYTE cbCardData, float duration);
    
    //获取数值
    BYTE GetCardValue(BYTE cbCardData) { return cbCardData & LOGIC_MASK_VALUE; }
    //获取颜色
    BYTE GetCardColor(BYTE cbCardData) { return cbCardData & LOGIC_MASK_COLOR; }
private:
    bool m_bOpened;
    CCActionInterval* m_openAnimIn;
    CCActionInterval* m_openAnimOut;
    
    void initData(const char* inCardImageName, const char* outCardImageName, BYTE cbCardData, float duration);
public:
    void openCard();
};

#endif
