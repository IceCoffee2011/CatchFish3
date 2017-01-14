//
//  CPicturesEditView.h
//  CatchFish
//
//  Created by 河北腾游 on 15/6/24.
//
//



#import <UIKit/UIKit.h>
#import <AssetsLibrary/AssetsLibrary.h>

#import "CPicturesLayer.h"


@interface PicturesEditView : NSObject<UIImagePickerControllerDelegate, UINavigationControllerDelegate>
{
    UIImagePickerController* pickerController;
    UIImageView* imageview;
    UIView    *mView;
    PicturesLayer * pLayerPictures;
    bool m_bEditPictures;
}
@property(nonatomic,retain)UIImagePickerController* pickerController;
@property(nonatomic,retain)UIImageView* imageview;

- (UIImage*)scaleFromImage:(UIImage*)image scaledToSize:(CGSize)newSize;

-(void)addLocationPicture: (PicturesLayer*) picturesLayer;

-(void) backClicked;

-(void) EditPictures;

-(void) uploadPictures;

@end



