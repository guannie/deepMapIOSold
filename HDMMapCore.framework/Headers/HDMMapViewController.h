//
//  HDMMapViewController.h
//  HDMMapCore
//
//  Created by Mikalai Lasitsa on 1/29/16.
//  Copyright © 2016 Heidelberg mobil International GmbH. All rights reserved.
//

#import <UIKit/UIKit.h>

#import <UIKit/UIKit.h>
#import <HDMMapCore/HDMMapView.h>
#import <HDMMapCore/DeepMap.h>
#import <HDMMapCore/HDMAnnotation.h>
#import <HDMMapCore/HDMMapRouting.h>
#import <HDMMapCore/HDMFeature.h>
#import "HDMMapVersion.h"

@class HDMMapViewController;

/*!
 *  The HDMMapViewDelegate protocol defines a set of optional methods that you can use to receive map-related update messages. You can to consider HDMMapViewControllerDelegate as a wrapper over HDMMapViewDelegate
 */
@protocol HDMMapViewControllerDelegate <NSObject>
@optional

/**
 *  Tells the delegate that the specified map view successfully started.
 *
 *  @param controller The controller whose map view started
 *  @discussion Before this call no API calls are accepted by the map.
 */
- (void)mapViewControllerDidStart:(nonnull HDMMapViewController*)controller error:(nullable NSError*)error;

/**
 *  Tells the delegate that the map view of the map view controller is will start loading the map.
 *
 *  @param controller The map view controller for which the map is about to initialize
 *  @discussion This delegate is mainly used for loading additional resources.
 */
- (void)mapViewControllerWillStart:(nonnull HDMMapViewController*)controller;

/**
 *  Tells the delegate that the camera of the map view was changed
 *
 *  @param controller The controller containing the map view where the camera was changed
 *  @discussion Called each time the user pinchs the map, pans etc.
 */
- (void)mapViewControllerCameraDidChange:(nonnull HDMMapViewController*)controller;

/**
 *  Tells the delegate all visible features at the real world coordinate where the tap occured
 *
 *  @param controller   The map view controller whose map was tapped
 *  @param coordinate   The real world coordinate where the tap occured
 *  @param ids          Array of feature references of visible features at the tapped location, sorted bottom to top
 *                      (topmost visible object is at array index n-1).
 *  @discussion         Called when the map is tapped
 */
- (void)mapViewController:(nonnull HDMMapViewController *)controller tappedAtCoordinate:(HDMMapCoordinate)coordinate features:(nonnull NSArray<HDMFeature *> *)features;

/**
 *  Tells the delegate all visible objects at the real world coordinate where the long-press occured
 *
 *  @param controller   The map view controller where map was long-pressed
 *  @param point        The real world coordinate where the long-press occured
 *  @param geoIds       Array of feature references of visible features at the tapped location, sorted bottom to top
 *                      (topmost visible feature is at array index n-1).
 *  @discussion         Called when the map is tapped at a location for more than 1 second
 */
- (void)mapViewController:(nonnull HDMMapViewController *)controller longPressedAtCoordinate:(HDMMapCoordinate)coordinate features:(nonnull NSArray<HDMFeature *> *)features;

/**
 *  Tells the delegate that the user location was updated on the map to a new location.
 *
 *  @param controller   The map view that was updated with a new user location
 *  @param point        The real world coordinate where the long-press occured
 *  @param geoIds       Array of object ids of the visible objects at the long-press location, sorted bottom to top
 *                      (topmost visible object is at array index n-1).
 *  @discussion         Called when the map is tapped at a location for more than 1 second
 */
- (void)mapViewController:(nonnull HDMMapViewController*)controller didUpdateUserLocation:(nullable HDMUserLocation*)userLocation;

/**
 *  Tells the delegate that the map was rotated
 *
 *  @param controller   The map view that was rotated
 *  @discussion         Called after the map was rotated
 */
- (void)mapViewControllerDidRotate:(nonnull HDMMapViewController*)controller;

/**
 *  Tells the delegate that the map was panned
 *
 *  @param controller   The map view that was rotated
 *  @discussion         Called after the map was panned
 */
- (void)mapViewControllerDidPan:(nonnull HDMMapViewController*)controller;

/**
 *  Tells the delegate that the map was zoomed
 *
 *  @param controller   The map view that was zoomed
 *  @discussion         Called after the map was zoomed
 */
- (void)mapViewControllerDidZoom:(nonnull HDMMapViewController*)controller;

/**
 *  Tells the delegate that the map's userTrackingMode was chagned
 *
 *  @param mapViewController              The Controller where userTrackingMode was chagned
 *  @param didChangeUserTrackingMode      Old userTrackingMode
 *  @param toUserTrackingMode             New userTrackingMode
 *  @discussion         Called after userTrackingMode was chagned
 */
- (void)mapViewController:(nonnull HDMMapViewController*)controller didChangeUserTrackingMode:(HDMUserTrackingMode)oldUserTrackingMode toUserTrackingMode:(HDMUserTrackingMode)newUserTrackingMode;

@end

/**
 *  The HDMMapViewController class creates a controller object that manages a map view
 */
@interface HDMMapViewController : UIViewController

/**
 * @name  Properties
 */

/**
 *  Displaying map view
 */
@property (nonatomic, readonly) HDMMapView * _Nonnull mapView;

/**
 *  Displaying map data package
 */
@property (nonatomic, strong, nullable) DeepMap *map;

/**
 *  The receiver’s delegate.
 *  @discussion A map view controller sends messages to its delegate regarding the loading of map data, tapping features or change point of view.
 *
 *  The delegate should implement the methods of the HDMMapViewControllerDelegate protocol.
 */
@property (nonatomic, assign, nullable) id <HDMMapViewControllerDelegate> delegate;

/**
 *  Instance of HDMMapRouting which provides routing API
 *  @discussion routing is lazy-loading property, i.e. it's not initialized while you don't touch it.
 *  If routing graph is very large it's a good practice to load (touch this property) in advance.
 */
@property (nonatomic, readonly, nullable) HDMMapRouting *routing;

/**
 *  Map Label locale
 */
@property (nonatomic, readonly, nullable) NSString *locale;

/**
 * @name  Initialization
 */

/**
 *  Returns a newly initialized map view controller with default map
 *
 *  @return initialized map view controller
 *  @discussion Default map is a map from DeepMap.zip from Main Bundle
 */
- (_Nonnull instancetype)init;

/**
 *  Returns a newly initialized map view controller with specified map
 *
 *  @param map Map Package for dispaying
 *
 *  @return initialized map view controller
 */
- (_Nonnull instancetype)initWithMap:(nullable DeepMap*)map;

/**
 *  Returns a newly initialized map view controller with specified map
 *
 *  @param map Map Package for dispaying
*  @param locale
 *
 *  @return initialized map view controller
 */
- (_Nonnull instancetype)initWithMap:(nonnull DeepMap*)map locale:(nonnull NSString*)locale;

/**
 *  Check if update for current map is available
 *
 *  @param completion Callback function that provide result of checking for update.
 */
- (void)checkMapUpdate:(void (^ _Nonnull)(BOOL isUpdateAvailable, NSError * _Nullable error))completion;

/**
 *  Download and install map update.
 *
 *  @param completion Callback function that provide result of map data updating.
 *  @discussion Method download the latest map available on the update server and install it
 */
- (void)installMapUpdate:(void (^ _Nonnull)(BOOL success, NSError * _Nullable error))completion;

@end
