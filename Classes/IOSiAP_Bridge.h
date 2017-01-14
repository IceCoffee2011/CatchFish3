//
//  IOSiAP_Bridge.h
//  LongChengDaRen
//
//  Created by 白白 on 14-11-11.
//
//

#ifndef __LongChengDaRen__IOSiAP_Bridge__
#define __LongChengDaRen__IOSiAP_Bridge__

#import "IOSiAP.h"
class IOSiAP_Bridge : public IOSiAPDelegate, public CCNode
{
public:
    IOSiAP_Bridge(CMD_GPR_IAPProductList IAPProductList);
    ~IOSiAP_Bridge();
    IOSiAP *iap;
    std::string productID;
    //int BuyBarrelID;//购买炮的ID，不能根据价格来，因为价格都一样
    void requestProducts(std::string);
    virtual void onRequestProductsFinish(void);
    virtual void onRequestProductsError(int code);
    virtual void onPaymentEvent(std::string &identifier, IOSiAPPaymentEvent event, int quantity);
    void onHttpChargeComplete(CCHttpClient* sender, CCHttpResponse* response);
    CMD_GPR_IAPProductList m_IAPProductList;
    
    void receiveHallLink2(float dt);
    void requestMail();
};
#endif /* defined(__LongChengDaRen__IOSiAP_Bridge__) */
