//
//  ChargeWebView.cpp
//  CatchFish
//
//  Created by 河北腾游 on 15/3/7.
//
//

#include "RootViewController.h"
#include "ChargeWebView.h"
#include "EncryptMD5.h"
#include <string>
#include "../tinyxml/tinyxml.h"
#include "Order.h"
#include "APAuthV2Info.h"
#include "Util/DataSigner.h"
#include <AlipaySDK/AlipaySDK.h>
#include "WXApiObject.h"
#include "WXApi.h"
#import "sys/utsname.h"
#include <sys/socket.h> // Per msqr
#import <AdSupport/AdSupport.h>

//#import <TencentOpenAPI/QQApi.h>
#import <TencentOpenAPI/QQApiInterface.h>

#include <sys/sysctl.h>
#include <net/if_dl.h>
#import <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>
#import <ifaddrs.h>
#import <dlfcn.h>

//#include "ChargeLayer.h"
#include "Reachability.h"
#include "header.h"

#define OFFICIAL_URL    "http://lb.66y.com/"

extern std::string TouXiangName;
extern bool bDownLoadOwnUserCompleted;
extern int OwnUserID;
extern int ChargeSuccess;
extern std::string AlipayChargeSuccess;
extern std::string Charge1URLstr;

int newVersion = 0;
int newUserID = 0;
int oldVersion = 0;

ChargeWebView::ChargeWebView()
{
    sFileName = "";
    bDownloadComplete = false;
}

void ChargeWebView::linkToFull(const char* gameId)
{
    char temp[256];
    char temp2[32];
    sprintf(temp2, "%sbuyu2015",gameId);
    char szMD5Result[128];
    CEncryptMD5::toString32(temp2,szMD5Result);
    sprintf(temp, "%s/pay.aspx?uid=%s&num=%s", OFFICIAL_URL, gameId, szMD5Result);
    NSString* str = [NSString stringWithUTF8String:temp];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
}

void ChargeWebView::lickToFeedback(const char* gameId, const char* account)
{
    char temp[256];
    sprintf(temp, "%sFeedback.aspx?gameid=%s&accname=%s",OFFICIAL_URL, gameId,account);
    NSString* str = [NSString stringWithUTF8String:temp];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
}

void ChargeWebView::linkToDownLoad()
{
    NSString* str = @"http://lb.66y.com";
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
}

void ChargeWebView::linkToNiuDownLoad()
{
    NSString* str = @"http://ln.66y.com";
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:str]];
}

void ChargeWebView::linkToMoreGamesDownLoad(std::string str)
{
    NSString* strURL = [NSString stringWithFormat:@"%s", str.c_str()];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:strURL]];
}

bool ChargeWebView::canExitFishGame(std::string str)
{
    NSString* strURL = [NSString stringWithFormat:@"%s", str.c_str()];
    return [[UIApplication sharedApplication] canOpenURL:[NSURL URLWithString:strURL]];
}

void ChargeWebView::openFishGame(std::string str)
{
    NSString* strURL = [NSString stringWithFormat:@"%s", str.c_str()];
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:strURL]];
}

std::string ChargeWebView::getUIDevice()
{
    NSString *identifierForVendor = [[UIDevice currentDevice].identifierForVendor UUIDString];
    std::string str = [identifierForVendor UTF8String];//9AF7108C-4EA6-407D-ABF5-2CF678EE5BE3
    std::string::iterator   it;
    for (it =str.begin(); it != str.end(); ++it)
    {
        if ( *it == '-')
        {
            str.erase(it);
        }
    
    }
   
    return str;
}

std::string ChargeWebView::getIOSIDFA()
{
    NSString *adid = [[[ASIdentifierManager sharedManager] advertisingIdentifier] UUIDString];
    return [adid UTF8String];
}

std::string ChargeWebView::getIOSMAC()
{
    int                 mib[6];
    size_t              len;
    char                *buf;
    unsigned char       *ptr;
    struct if_msghdr    *ifm;
    struct sockaddr_dl  *sdl;
    
    mib[0] = CTL_NET;
    mib[1] = AF_ROUTE;
    mib[2] = 0;
    mib[3] = AF_LINK;
    mib[4] = NET_RT_IFLIST;
    
    if ((mib[5] = if_nametoindex("en0")) == 0) {
        printf("Error: if_nametoindex error/n");
        return "";
    }
    
    if (sysctl(mib, 6, NULL, &len, NULL, 0) < 0) {
        printf("Error: sysctl, take 1/n");
        return "";
    }
    
    if ((buf = (char*)malloc(len)) == NULL) {
        printf("Could not allocate memory. error!/n");
        return "";
    }
    
    if (sysctl(mib, 6, buf, &len, NULL, 0) < 0) {
        printf("Error: sysctl, take 2");
        return "";
    }
    
    ifm = (struct if_msghdr *)buf;
    sdl = (struct sockaddr_dl *)(ifm + 1);
    ptr = (unsigned char *)LLADDR(sdl);
    NSString *outstring = [NSString stringWithFormat:@"%02x:%02x:%02x:%02x:%02x:%02x", *ptr, *(ptr+1), *(ptr+2), *(ptr+3), *(ptr+4), *(ptr+5)];
    
    free(buf);
    
    return [[outstring uppercaseString] UTF8String];
}

std::string ChargeWebView::getIOSUUID()
{
    CFUUIDRef puuid = CFUUIDCreate( nil );
    CFStringRef uuidString = CFUUIDCreateString( nil, puuid );
    NSString * result = (NSString *)CFStringCreateCopy( NULL, uuidString);
    CFRelease(puuid);
    CFRelease(uuidString);
    return [[result autorelease] UTF8String];
}

std::string ChargeWebView::getDeviceInfo(int num)
{
    struct utsname systemInfo;
    uname(&systemInfo);
    
    if(num == 0)
    {
        return [[[UIDevice currentDevice] systemVersion] UTF8String];
    }
    else if(num == 1)
    {
        return [[NSString stringWithCString:systemInfo.nodename encoding:NSUTF8StringEncoding] UTF8String];//用户名
    }
    else if(num == 2)//操作系统名称
    {
        return [[NSString stringWithCString:systemInfo.sysname encoding:NSUTF8StringEncoding] UTF8String];
    }
    else if(num == 3)
    {
        return [[NSString stringWithCString:systemInfo.release encoding:NSUTF8StringEncoding] UTF8String];//操作系统版本号
    }
    else if(num == 4)
    {
        return [[NSString stringWithCString:systemInfo.version encoding:NSUTF8StringEncoding] UTF8String];
    }
    else if(num == 5)//手机型号
    {
        return [[NSString stringWithCString:systemInfo.machine encoding:NSUTF8StringEncoding] UTF8String];
    }
    else if(num == 6)//内网IP
    {
        
        /*
        char baseHostName[256]; // Thanks, Gunnar Larisch
        int success = gethostname(baseHostName, 255);
        if (success != 0) return "";
        baseHostName[255] = '/0';
        
#if TARGET_IPHONE_SIMULATOR
        NSString* hostName = [NSString stringWithFormat:@"%s", baseHostName];
#else
        NSString* hostName = [NSString stringWithFormat:@"%s.local", baseHostName];
#endif
        struct hostent *host = gethostbyname([hostName UTF8String]);
        if (!host) {herror("resolv"); return "";}
        struct in_addr **list = (struct in_addr **)host->h_addr_list;
        return [[NSString stringWithCString:inet_ntoa(*list[0]) encoding:NSUTF8StringEncoding] UTF8String];
         */
    }
    
    return "";
}


//支付宝充值 第三个参数为1代表金币回调，为2代表武器回到，为3代表技能回调
void ChargeWebView::alipayCharge(const char* orderStr, int prise, int nBuyGameType)
{
    // {"body":"支付测试","buyer_email":"583002506@qq.com","buyer_id":"2088902019697751","discount":"0.00","gmt_create":"2015-06-05 15:44:11","is_total_fee_adjust":"Y","notify_id":"0d491326310b950d335bc9331945552866","notify_time":"2015-06-05 15:44:11","notify_type":"trade_status_sync","out_trade_no":"alipay14334902441511","payment_type":"1","price":"10.00","quantity":"1","seller_email":"2011037576@qq.com","seller_id":"2088911718169890","sign":"OxWMipWp+mvAY1zM9VFmjtQJScDYOjpPQqO2vGis+iQG3taUB/8eoc5xaRGn6/ZiD6p7a0nQDN3BWl0IY69vuHVDp8UQIghZMVlaL0O5z0LmtwtqxfIhadUoVlfoxlF2CNhAEBrZ7H/8c+9LFTrOXQaTBXnzfBkIC3PmzFQz+dE=","sign_type":"RSA","subject":"支付详情","total_fee":"10.00","trade_no":"2015060500001000750063727868","trade_status":"WAIT_BUYER_PAY","use_coupon":"N"}
    NSString *partner = @"2088911718169890";
    NSString *seller = @"2011037576@qq.com";
    NSString *privateKey =@"MIICdgIBADANBgkqhkiG9w0BAQEFAASCAmAwggJcAgEAAoGBANsMwKfhvmdeW+1NXvlP89ntchrDszLkY/8dJDl/wqMIoDp3K4tDoTOXFmpirv+/1d8RqXVyMRfWHbO/tjTm1IekQPRDG4PhC/27attLeM0uHmYitpZoncVIit0hvutPgjd8w/HlBFZCNbH6Eg3cm9tvBQWHufJCcJVeoW46JytlAgMBAAECgYAnPwutiJaysodJ5xjjFl4cZ5yQCwwqTB1WcgQxgSCAUWVT1FIq630QXzuW6ANusxJAm0t/TWxaVvSPZgPhSpI9Pl2KO2Rrd52i2xHkK5MRMAabM5dNX0XtJW4FMVsh+XXy+9fdtZAkYFVvwzOQ2XnMJDIuqNR6yoBMYpF8C+PM6QJBAPQO5kB1BFM1W1mka4mo1weO8mRiCAsYofyG4j9HCNv3puujjGyyyCq2FUdUm2WU90+aTv8OWEMUyWBJ3rwENBsCQQDlxJhjvhzrsbD5U6/d6PlELA0upMRKhiGylze53aCu7ooTDGROHO0YSZWe09vKNaSinVzGQhZ3SjR+aQhKNRZ/AkBVL5oop29yB66x9FTgwMEFT1zFsv3xuZBYz0/LHGU4OhHWL8jE7AY3+kQlGKAA7eUAsPCf0Ab4qxm69S28XI8/AkBYzTecbRCeY0mjwx1YZb9ZArGBknqu1tJVRK5r7mU41saUgBiQPAvCdEYYYYczyrG7i5gco9Qs/If3Rz7F92ifAkEApGmkAU7fDWxL2gYUJvX8D6wOM0Cp4rlFdgK6dDJ1wyYzm8gzBcWhPmS5YGEkiGGwSk+jq5ZXEo0gmD9y5PWdvw==";
    /*============================================================================*/
    /*============================================================================*/
    /*============================================================================*/
    
    //partner和seller获取失败,提示
    if ([partner length] == 0 ||
        [seller length] == 0 ||
        [privateKey length] == 0)
    {
        /*UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"提示"
                                                        message:@"缺少partner或者seller或者私钥。"
                                                       delegate:self
                                              cancelButtonTitle:@"确定"
                                              otherButtonTitles:nil];
        [alert show];*/
        return;
    }
    
#pragma mark   ==============产生随机订单号==============
    static int kNumber = 15;
    NSString *sourceStr = @"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    NSMutableString *resultStr = [[NSMutableString alloc] init];
    srand(time(0));
    for (int i = 0; i < kNumber; i++)
    {
        unsigned index = rand() % [sourceStr length];
        NSString *oneStr = [sourceStr substringWithRange:NSMakeRange(index, 1)];
        [resultStr appendString:oneStr];
    }

    
    /*
     *生成订单信息及签名
     */
    //将商品信息赋予AlixPayOrder的成员变量
    Order *order = [[Order alloc] init];
    order.partner = partner;
    order.seller = seller;
    
    //order.tradeNO = resultStr; //订单ID（由商家自行制定）
    order.tradeNO = [NSString stringWithFormat:@"%s",orderStr];
    NSLog(@"ordre.tradeNO is %@", order.tradeNO);
    order.productName = @"道具"; //商品标题
    order.productDescription = @"支付测试"; //商品描述
    order.amount = [NSString stringWithFormat:@"%d",prise]; //商品价格
    //金币回调
    if(nBuyGameType == 1)
    {
        if(Charge1URLstr == "")
        {
            order.notifyURL = @"http://lelepay.66y.com/Alipay20151028/alipaynotify.aspx";
        }
        else
        {
            order.notifyURL =  [NSString stringWithFormat:@"%s/Alipay20151028/alipaynotify.aspx", Charge1URLstr.c_str()]; //回调URL
        }
    }
    else if(nBuyGameType == 2 || nBuyGameType == 3)//武器回调和技能回调
    {
        if(Charge1URLstr == "")
        {
            order.notifyURL = @"http://lelepay.66y.com/skpay/alipay/alipayNotify.aspx";
        }
        else
        {
            order.notifyURL =  [NSString stringWithFormat:@"%s/skpay/alipay/alipayNotify.aspx", Charge1URLstr.c_str()]; //回调URL
        }
    }
    
    order.service = @"mobile.securitypay.pay";
    order.paymentType = @"1";
    order.inputCharset = @"utf-8";
    order.itBPay = @"30m";
    order.showUrl = @"m.alipay.com";
    
    //应用注册scheme,在AlixPayDemo-Info.plist定义URL types
    NSString *appScheme = @"alisdklelebuyu";
    
    //将商品信息拼接成字符串
    NSString *orderSpec = [order description];
    NSLog(@"orderSpec = %@",orderSpec);
    
    //获取私钥并将商户信息签名,外部商户可以根据情况存放私钥和签名,只需要遵循RSA签名规范,并将签名字符串base64编码和UrlEncode
    id<DataSigner> signer = CreateRSADataSigner(privateKey);
    NSString *signedString = [signer signString:orderSpec];
    
    //将签名成功字符串格式化为订单字符串,请严格按照该格式
    NSString *orderString = nil;
    if (signedString != nil) {
        orderString = [NSString stringWithFormat:@"%@&sign=\"%@\"&sign_type=\"%@\"",
                       orderSpec, signedString, @"RSA"];
        
        [[AlipaySDK defaultService] payOrder:orderString fromScheme:appScheme callback:^(NSDictionary *resultDic) {
            NSLog(@"reslut1111 = %@",resultDic);
            NSString* resultStatusStr = resultDic[@"resultStatus"];
            
            if([resultStatusStr isEqualToString:@"9000"])
            {
                //UIAlertView* alertView = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"支付成功，金币将发放到银行中" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil] autorelease];
                //[alertView show];
                NSLog(@"支付成功");
                NSString* signStr = resultDic[@"result"];
                AlipayChargeSuccess = [signStr UTF8String];
                ChargeSuccess = 1;
            }
            else
            {
                ChargeSuccess = 2;
                NSLog(@"支付取消或者失败\n");
                //UIAlertView* alertView = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"支付失败" delegate:nil cancelButtonTitle:@"确定" otherButtonTitles:nil] autorelease];
                //[alertView show];
                //CMainLogic::sharedMainLogic()->ShowMessage("支付取消", eMBOK);
                
            }
            if(CMainLogic::sharedMainLogic()->m_bFirstChargeUIStatus)
            {
                CMainLogic::sharedMainLogic()->notifyIOSAlipayCompleted();
                return ;
            }
            
            if(CMainLogic::sharedMainLogic()->m_bGameChargeStatus)//如果在游戏中充值的话
            {
                //返回到大厅
                CMainLogic::sharedMainLogic()->switchLayer(eHallLayer, false, false, 1, 1);
            }
            else
            {
                ChargeLayer* pChargeLayer = (ChargeLayer*)CCDirector::sharedDirector()->getRunningScene()->getChildByTag(20000);
                if(pChargeLayer != NULL)
                {
                    pChargeLayer -> notifyIOSAlipayCompleted();
                }
            }
        }];
    }
}

void ChargeWebView::wxCharge(std::string appid, std::string partnerId,std::string prepayId,std::string nonceStr,int timeStamp,std::string package,std::string sign)
{
    printf("appid is %s\n", appid.c_str());
    PayReq* req = [[PayReq alloc] init];
    req.openID  = [NSString stringWithUTF8String:appid.c_str()];
    req.partnerId = [NSString stringWithUTF8String:partnerId.c_str()];
    req.prepayId  = [NSString stringWithUTF8String:prepayId.c_str()];
    req.nonceStr = [NSString stringWithUTF8String:nonceStr.c_str()];
    req.timeStamp = timeStamp;
    req.package = @"Sign=WXPay";
    req.sign = [NSString stringWithUTF8String:sign.c_str()];
    [WXApi sendReq:req];
    
}

void ChargeWebView::openAppStore()
{
    RootViewController * ctrol= (RootViewController * )[UIApplication sharedApplication].keyWindow.rootViewController;
    
    SKStoreProductViewController *storeProductViewController = [[SKStoreProductViewController alloc] init];
    
    [storeProductViewController setDelegate:ctrol];
    [storeProductViewController loadProductWithParameters:@{SKStoreProductParameterITunesItemIdentifier : @"1024870573"} completionBlock:^(BOOL result, NSError *error) {
        if (error) {
        } else {
            [ctrol presentViewController:storeProductViewController animated:YES completion:nil];
        }
    }];
}

std::string ChargeWebView::OnInternet()
{
    NetworkStatus internetStatus = [[Reachability reachabilityWithHostName:@"www.apple.com"] currentReachabilityStatus];
    bool isHaveNet = false;
    std::string sType = "";
    switch (internetStatus)
    {
        case NotReachable:
        {
            isHaveNet = false;
            break;
        }
        case ReachableViaWiFi:
        {
            isHaveNet = true;
            sType = "0";
            break;
        }
        case ReachableViaWWAN:
        {
            isHaveNet = true;
            sType = "1";
            break;
        }
    }
    return sType;
}

long long ChargeWebView::getCurrentTime()
{
    return [[NSDate date] timeIntervalSince1970]*1000;
}

std::string ChargeWebView::getCurrentTimes()
{
    NSDateFormatter* formatter = [[[NSDateFormatter alloc] init] autorelease];
    [formatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    return [[formatter stringFromDate:[NSDate date]] UTF8String];
}

bool ChargeWebView::checkWX()
{
    return [WXApi isWXAppInstalled];
}

void ChargeWebView::openIOSShare(std::string str1, std::string str2, int nType)
{
    std::string str3 = "hall/Icon-120.png";
    //创建发送对象实例
    SendMessageToWXReq *sendReq = [[SendMessageToWXReq alloc] init];
    sendReq.bText = NO;//不使用文本信息
    //sendReq.scene = 1;//0 = 好友列表 1 = 朋友圈 2 = 收藏
    sendReq.scene = nType;//0 = 好友列表 1 = 朋友圈 2 = 收藏
    //创建分享内容对象
    WXMediaMessage *urlMessage = [WXMediaMessage message];
    if(nType == 0)
    {
        urlMessage.title = [NSString stringWithUTF8String:"乐乐捕鱼"];//分享标题
        urlMessage.description = [NSString stringWithUTF8String:str2.c_str()];//分享描述
    }
    else
    {
        urlMessage.title = [NSString stringWithUTF8String:str2.c_str()];//分享标题
    }
    
    [urlMessage setThumbImage:[UIImage imageNamed:[NSString stringWithUTF8String:str3.c_str()]]];//分享图片,使用SDK的setThumbImage方法可压缩图片大小
    //创建多媒体对象
    WXWebpageObject *webObj = [WXWebpageObject object];
    webObj.webpageUrl = [NSString stringWithFormat:@"%s", str1.c_str()];//分享链接
    //完成发送对象实例
    urlMessage.mediaObject = webObj;
    sendReq.message = urlMessage;
    
    //发送分享信息
    [WXApi sendReq:sendReq];
}

bool ChargeWebView::checkQQ()
{
    return [QQApiInterface isQQInstalled];
}

void ChargeWebView::openQQShare(std::string str1, std::string str2, int nType)
{
    //用于分享图片内容的对象
    printf("str1 is %s, str2 is %s\n", str1.c_str(), str2.c_str());
    std::string str3 = str1 + "/images/Icon-122.png";//hall/Icon-120.png
    
    //分享图预览图URL地址
    QQApiNewsObject *newsObj = [QQApiNewsObject objectWithURL:[NSURL URLWithString:[NSString stringWithUTF8String:str1.c_str()]] title: @"乐乐捕鱼" description:[NSString stringWithUTF8String:str2.c_str()] previewImageURL:[NSURL URLWithString:[NSString stringWithUTF8String:str3.c_str()]]];
    SendMessageToQQReq *req = [SendMessageToQQReq reqWithContent:newsObj];
    if(nType == 0)//将内容分享到qq
    {
        QQApiSendResultCode sent = [QQApiInterface sendReq:req];
        printf("sent is %d\n", sent);
    }
    else//将内容分享到QQ空间
    {
        QQApiSendResultCode sent = [QQApiInterface SendReqToQZone:req];
        printf("sent is %d\n", sent);
    }
}

bool ChargeWebView::findUserIDFile(const char* UserID)
{
    //读取沙盒下的文件
    NSArray* myPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString* myDocPaths = [myPaths objectAtIndex:0];
    NSString* nameStr = [NSString stringWithFormat:@"%s.txt", UserID];
    NSString* filename = [myDocPaths stringByAppendingPathComponent:nameStr];
    if([[NSFileManager defaultManager] fileExistsAtPath:filename])
    {
        //文本文件下版本号为dataStr
        NSString* dataStr = [NSString stringWithContentsOfFile:filename encoding:NSUTF8StringEncoding error:nil];
        NSString* filePictureName = [NSString stringWithFormat:@"/%s_%@.png",UserID,dataStr];
        NSString* filePictureStr = [myDocPaths stringByAppendingString:filePictureName];
        //然后获取本地图片
        if([[NSFileManager defaultManager] fileExistsAtPath:filePictureStr])
        {
            TouXiangName = [filePictureStr UTF8String];
            return true;
        }
        else
        {
            TouXiangName = "";
        }
    }
    else
    {
        TouXiangName = "";
    }
    
    NSLog(@"TouXiangName is %s", TouXiangName.c_str());
    return false;
}

std::string ChargeWebView::findUserVersionFile(int UserID, int nVersion, int TableID, int SeatID)
{
    newUserID = UserID;
    newVersion = nVersion;
    wTableID = TableID;
    wSeatID = SeatID;
    m_nUserID = UserID;
    bool bSame = true;
    NSFileManager* fileManager = [NSFileManager defaultManager];
    NSArray* myPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString* myDocPaths = [myPaths objectAtIndex:0];
    NSString* nameStr = [NSString stringWithFormat:@"%d.txt", UserID];
    NSString* filename = [myDocPaths stringByAppendingPathComponent:nameStr];
    if([fileManager fileExistsAtPath:filename])
    {
        //读取内容，判断是否和nVersion是否一致。如果一致， 则返回0；如果不一致，那么替换文件，下载新的头像并保存，返回这个头像的路径，然后将以前的头像文件删除
        NSString* str = [NSString stringWithContentsOfFile:filename encoding:NSUTF8StringEncoding error:nil];
        NSNumberFormatter* formatter = [[[NSNumberFormatter alloc] init] autorelease];
        int version = [[formatter numberFromString:str] intValue];//以前的版本
        oldVersion = version;
        printf("nVersion is %d, version is %d\n", nVersion, version);
        if(nVersion != version)//不一致，说明有更新
        {
            printf("version no same\n");
            NSString* timeDataStr = [NSString stringWithFormat:@"%d",nVersion];
            [timeDataStr writeToFile:filename atomically:YES encoding:NSUTF8StringEncoding error:nil];//覆盖版本文件
            bSame = false;
            printf("nVersion is %d, version is %d\n", nVersion, version);
        }
        else
        {
            printf("version same\n");
            bSame = true;
            //检查是否存在头像文件
            NSString* filePath = [NSString stringWithFormat:@"/%d_%d.png", UserID, nVersion];
            NSString* filePath2 = [myDocPaths stringByAppendingString:filePath];
            if(![fileManager fileExistsAtPath:filePath2])
            {
                bSame = false;
                printf("文件不存在\n");
            }
            else
            {
                sFileName = [filePath2 UTF8String];
                bDownloadComplete = true;
                printf("文件存在 1\n");
            }
        }
    }
    else//创建文件，保存nVersion，下载头像并保存，返回头像的路径
    {
        [fileManager createFileAtPath:filename contents:nil attributes:nil];
        NSString* timeDataStr = [NSString stringWithFormat:@"%d",nVersion];
        [timeDataStr writeToFile:filename atomically:YES encoding:NSUTF8StringEncoding error:nil];//保存版本文件
        bSame = false;
        
    }
    //访问网站，获取图片地址，然后下载，最后保存到沙盒目录下
    if(!bSame)
    {
        char urlStr[256];
        sprintf(urlStr, "http://%s/GetFaceUrl.aspx?uid=%d", NORMAL_URL,UserID);
        CCHttpRequest* request = new CCHttpRequest();
        request -> setUrl(urlStr);
        printf("url Str is %s\n", urlStr);
        request -> setRequestType(CCHttpRequest::kHttpGet);
        char tempUserId[24];
        sprintf(tempUserId, "%d_%d", UserID, newVersion);
        request -> setTag(tempUserId);
        request -> setResponseCallback(this, httpresponse_selector(ChargeWebView::onHttpUserIDComplete));
        CCHttpClient::getInstance()->send(request);
        //    request -> release();
        CC_SAFE_RELEASE(request);
        request = NULL;
    }
    
    
    NSString* filePath = [NSString stringWithFormat:@"/%d_%d.png", UserID, nVersion];
    NSString* filePath2 = [myDocPaths stringByAppendingString:filePath];
    sFileName = [filePath2 UTF8String];
    printf("filePath2 is %s\n", [filePath2 UTF8String]);
    return [filePath2 UTF8String];
}

void ChargeWebView::onHttpUserIDComplete(CCHttpClient* sender, CCHttpResponse* response)
{
    if (!response)
    {
        return;
    }
    if (!response->isSucceed())
    {
        return;
    }
    std::string tag = response->getHttpRequest()->getTag();
    std::string str1 = "";
    std::string str2 = "";
    int size = tag.find("_");
    if(size >= 0)
    {
        str1 = tag.substr(0, size);
        str2 = tag.substr(size+1);
        printf("str1 is %s, str2 is %s\n", str1.c_str(), str2.c_str());
    }
    int userIDTag = atoi(str1.c_str());
    int nVersion2 = atoi(str2.c_str());
    printf("userIDTag is %d, nVersioin is %d\n", userIDTag, nVersion2);
    printf("222222\n");
    //if(userIDTag == newUserID)
    //if(1)
    {
        std::vector<char> *buffer = response->getResponseData();
        std::string str;
        for (unsigned int i = 0; i < buffer->size(); i++)
        {
            char a = (*buffer)[i];
            str.append(1,a);
        }
        //printf("url str is %s\n", str.c_str());
        std::string urlStr = "";
        TiXmlDocument* document = new TiXmlDocument();
        int len = (int)strlen(str.c_str());
        if(document->LoadFile2((unsigned char *)str.c_str(), len))
        {
            TiXmlElement* rootElement = document->RootElement();
            std::string nodename=rootElement->Value();
            if (0!=nodename.compare("accountsInface"))
            {
                return;
            }
            TiXmlElement * pCurrentElement=rootElement->FirstChildElement();
            while (NULL!=pCurrentElement)
            {
                const char * pValue=pCurrentElement->Value();
                if (!strcmp(pValue,"facelist"))
                {
                    TiXmlElement* firstElement = pCurrentElement -> FirstChildElement();
                    const char * valueId=firstElement->Value();
                    if (!strcmp(valueId,"FaceUrl"))
                    {
                        urlStr = firstElement->GetText();
                    }
                }
                pCurrentElement=pCurrentElement->NextSiblingElement();
            }
        }
        delete document;
        NSString* imageUrl = [NSString stringWithFormat:@"http://%s%s", NORMAL_URL,urlStr.c_str()];
        printf("urlStr is %s\n", urlStr.c_str());
        NSData* imagedata = [NSData dataWithContentsOfURL:[NSURL URLWithString:imageUrl]];
        NSArray* myPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
        NSString* myDocPaths = [myPaths objectAtIndex:0];
        
//        //将以前的头像文件删除
//        NSString* oldFileStr = [NSString stringWithFormat:@"%d_%d.png", userIDTag, oldVersion];
//        NSString* oldFilePath = [myDocPaths stringByAppendingString:oldFileStr];
//        CCLOG("nVersion2 = %d, %d",nVersion2,oldVersion);
//        if([[NSFileManager defaultManager] fileExistsAtPath:oldFilePath])
//        {
//            [[NSFileManager defaultManager] removeItemAtPath:oldFilePath error:nil];
//        }
        
        NSString* nameStr = [NSString stringWithFormat:@"%d_%d.png", userIDTag, nVersion2];
        NSString* filename = [myDocPaths stringByAppendingPathComponent:nameStr];
        NSLog(@"filename is %@\n", filename);
        
        [imagedata writeToFile:filename atomically:YES];
        CCSprite* sprite = CCSprite::create([filename UTF8String]);
        if(sprite == NULL)
        {
            CCLOG("sprite == NULL");
        }
    
        
        bDownloadComplete = true;
        printf("OwnuserId is %d, userIDTag is %d\n", OwnUserID, userIDTag);
        if(OwnUserID == userIDTag)
        {
            bDownLoadOwnUserCompleted = true;
            TouXiangName = [filename UTF8String];
            printf("TouXiangName 1111 is %s\n", TouXiangName.c_str());
        }
    }
}


std::string SandBox::findUserFiles(int UserID)
{
    NSFileManager* fileManager = [NSFileManager defaultManager];
    NSArray* myPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
    NSString* myDocPaths = [myPaths objectAtIndex:0];
    NSString* nameStr = [NSString stringWithFormat:@"%d.txt", UserID];
    NSString* filename = [myDocPaths stringByAppendingPathComponent:nameStr];
    if([fileManager fileExistsAtPath:filename])
    {
        NSString* str = [NSString stringWithContentsOfFile:filename encoding:NSUTF8StringEncoding error:nil];
        NSNumberFormatter* formatter = [[[NSNumberFormatter alloc] init] autorelease];
        int version = [[formatter numberFromString:str] intValue];
        NSString* oldFileStr = [NSString stringWithFormat:@"/%d_%d.png", UserID, version];
        NSString* oldFilePath = [myDocPaths stringByAppendingString:oldFileStr];
        if([fileManager fileExistsAtPath:oldFilePath])
        {
            return [oldFilePath UTF8String];
        }
        else
        {
            return "";
        }
    }
    else
    {
        return "";
    }
    
}

void ChargeWebView::copy(std::string str) {
    //把string类型转换成为char*
    char*p=(char*)str.data();
    
    //把char*转换成OC的NSString
    NSString *nsMessage= [[NSString alloc] initWithCString:p encoding:NSUTF8StringEncoding];
    
    //获得iOS的剪切板
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    
    //改变剪切板的内容
    pasteboard.string = nsMessage;
}

std::string ChargeWebView::checkPaste()
{
    //获得iOS的剪切板
    UIPasteboard *pasteboard = [UIPasteboard generalPasteboard];
    if(pasteboard.string == nil)
    {
        return "";
    }
    else
    {
        std::string str = [pasteboard.string UTF8String];
        return str;
    }
}
