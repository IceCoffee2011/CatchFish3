//
//  CPicturesEditView.cpp
//  CatchFish
//
//  Created by 河北腾游 on 15/6/24.
//
//

#import "CPicturesEditView.h"
#import "EAGLView.h"
#import "string"

extern std::string TouXiangName;
extern std::string UserIDStr;
std::string eVersionStr = "";
extern long long nOwnUserVersion;

@implementation PicturesEditView

@synthesize pickerController,imageview;

- (id)init{
    
    self = [super init];
    
    if (self) {
        
        m_bEditPictures = false;
        
    }
    
    return self;
    
}

- (void)dealloc
{
    [pickerController release];
    [imageview release];
    [mView release];
    [super dealloc];
}

-(void)addLocationPicture: (PicturesLayer*) picturesLayer
{
    pLayerPictures = picturesLayer;
    mView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, 1280 , 720)];
    [[EAGLView sharedEGLView] addSubview:mView];
    
    imageview = [[UIImageView alloc] initWithImage:[UIImage imageNamed:@""]];
    [mView addSubview:imageview];
    [imageview setFrame:CGRectMake(167, 143, 45, 45)];
}

-(void) EditPictures
{
    pickerController = [[UIImagePickerController alloc] init];
    pickerController.sourceType = UIImagePickerControllerSourceTypePhotoLibrary;
    pickerController.allowsEditing = YES;
    pickerController.delegate = self;
    [mView addSubview:pickerController.view];
}

-(void) uploadPictures
{
    if(!m_bEditPictures)
    {
        UIAlertView* alertView = [[[UIAlertView alloc] initWithTitle:@"提示" message:@"请先选择图片" delegate:self cancelButtonTitle:@"确定" otherButtonTitles:nil] autorelease];
        [alertView show];
        pLayerPictures->m_Menu->setTouchEnabled(true);
    }
    else
    {
        //先在沙盒目录下读取名为UID的txt文件，如果没有，则创建，如果有，先读取内容，然后将头像文件删除，再创建新的头像文件
        NSFileManager* fileManager=[NSFileManager defaultManager];
        NSArray* myPaths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask, YES);
        NSString* myDocPaths = [myPaths objectAtIndex:0];
        NSString* nameStr = [NSString stringWithFormat:@"%s.txt", UserIDStr.c_str()];
        NSString* filename = [myDocPaths stringByAppendingPathComponent:nameStr];
        if(TouXiangName != "")//说明没有使用默人的头像，则沙盒下有记录，那么要将原头像文件删除
        {
            NSString* str = [NSString stringWithUTF8String:TouXiangName.c_str()];
            [fileManager removeItemAtPath:str error:nil];
        }
        else
        {
            [fileManager createFileAtPath:filename contents:nil attributes:nil];
        }
        unsigned int time = [NSDate timeIntervalSinceReferenceDate];
        NSString* timeDataStr = [NSString stringWithFormat:@"%d",time];
        eVersionStr = [timeDataStr UTF8String];
        nOwnUserVersion = atoll(eVersionStr.c_str());
        [timeDataStr writeToFile:filename atomically:YES encoding:NSUTF8StringEncoding error:nil];
        //保存图片到沙盒目录下
        NSArray* array = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
        NSData* imageData = UIImagePNGRepresentation(self.imageview.image);
        NSString* fileName = [NSString stringWithFormat:@"%s_%s.png", UserIDStr.c_str(), eVersionStr.c_str()];
        NSLog(@"保存图片到沙盒目录下 上传 2222222    %@", fileName);
        NSString *filePath2 = [[array objectAtIndex:0] stringByAppendingPathComponent:fileName];   // 保存文件的名称
        [imageData writeToFile:filePath2 atomically:YES];
        TouXiangName = [filePath2 UTF8String];
        
        pLayerPictures->uploadImage();
        
    }
}

- (UIImage*)scaleFromImage:(UIImage*)image scaledToSize:(CGSize)newSize
{
    CGSize imageSize = image.size;
    CGFloat width = imageSize.width;
    CGFloat height = imageSize.height;
    
    if (width <= newSize.width && height <= newSize.height){
        return image;
    }
    
    if (width == 0 || height == 0){
        return image;
    }
    
    CGFloat widthFactor = newSize.width / width;
    CGFloat heightFactor = newSize.height / height;
    CGFloat scaleFactor = (widthFactor<heightFactor?widthFactor:heightFactor);
    
    CGFloat scaledWidth = width * scaleFactor;
    CGFloat scaledHeight = height * scaleFactor;
    //CGSize targetSize = CGSizeMake(scaledWidth,scaledHeight);
    CGSize targetSize = CGSizeMake(100,100);
    UIGraphicsBeginImageContext(targetSize);
    [image drawInRect:CGRectMake(0,0,100,100)];
    UIImage* newImage = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    
    NSLog(@"imageSize.width is %f, imageSize.height is %f", imageSize.width, imageSize.height);
    NSLog(@"newSize.width is %f, newSize.height is %f", newSize.width, newSize.height);
    NSLog(@"scaleWidth is %f, scaleHeight is %f", scaledWidth, scaledHeight);
    
    return newImage;
}


- (void)imagePickerController:(UIImagePickerController *)picker didFinishPickingMediaWithInfo:(NSDictionary *)info
{
    NSLog(@"%@",info);
    UIImage* iamge1 = [info objectForKey:UIImagePickerControllerOriginalImage];
    
    imageview.image = [self scaleFromImage:iamge1 scaledToSize:CGSizeMake(100.0f, 100.0f)];
    //imageview.image = [info objectForKey:UIImagePickerControllerOriginalImage];
    
    [pickerController.view removeFromSuperview];

    CCImage *imf =new CCImage();
    NSData *imgData = UIImagePNGRepresentation(imageview.image);
    NSUInteger len = [imgData length];
    Byte *byteData = (Byte*)malloc(len);
    memcpy(byteData, [imgData bytes], len);
    imf->initWithImageData(byteData,(int)imgData.length);
    imf->autorelease();
    CCTexture2D* pTexture = new CCTexture2D();
    pTexture->initWithImage(imf);
    pTexture->autorelease();
    CCSprite* pHead = (CCSprite*)pLayerPictures->m_pClippingNode->getChildByTag(100);
    if (pHead != NULL) {
        pHead->removeFromParentAndCleanup(true);
    }
    CCSprite* pHeadNew = CCSprite::createWithTexture(pTexture);
    pHeadNew->setScale(2.7f);
    pHeadNew->setPosition(ccp(194, pLayerPictures->m_pSpBg->getContentSize().height/2-10));
    pLayerPictures->m_pClippingNode->addChild(pHeadNew, 100);
    
    [mView removeFromSuperview];
    
    //    NSArray *fileNameList=[[NSFileManager defaultManager] subpathsAtPath:myDocPath];
    //    for(int i = 0; i < [fileNameList count]; i++)
    //    {
    //        NSString* uniquePath = [fileNameList objectAtIndex:i];
    //        NSString *filePath2 = [myDocPath stringByAppendingPathComponent:uniquePath];
    //        [fileManager removeItemAtPath:filePath2 error:nil];
    //    }
    
    
    
    NSURL* url = [info objectForKey:UIImagePickerControllerReferenceURL];
    ALAssetsLibraryAssetForURLResultBlock resultBlock = ^(ALAsset* asset)
    {
//        ALAssetRepresentation* representation = [asset defaultRepresentation];
//        NSString* filePictureName = [representation filename];
//        NSLog(@"1111111  %@",filePictureName);
        m_bEditPictures = true;
    };
    ALAssetsLibrary* assetslibrary = [[[ALAssetsLibrary alloc] init] autorelease];
    [assetslibrary assetForURL:url resultBlock:resultBlock failureBlock:nil];
    
    
}

- (void)imagePickerControllerDidCancel:(UIImagePickerController *)picker
{
    [pickerController.view removeFromSuperview];
}

-(void) backClicked
{
    [mView removeFromSuperview];
}

@end