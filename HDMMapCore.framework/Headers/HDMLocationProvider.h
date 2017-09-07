//
//  HDMLocationProvider.h
//  hdm-mapcore-lib
//
//  Created by Stadin Benjamin on 27.03.17.
//
//

#import <Foundation/Foundation.h>
#import <HDMMapCore/HDMUserLocation.h>

NS_ASSUME_NONNULL_BEGIN

@protocol HDMLocationProvider;


@protocol HDMLocationProviderDelegate <NSObject>

@optional

/**
 Tells the delegate that the location provider will begin tracking the user’s location.
 
 @param locationProvider The location provider that is tracking the user’s location.
 */
- (void)locationProviderWillStartLocatingUser:(id<HDMLocationProvider>)locationProvider;

/**
 Tells the delegate that the location provider  has stopped tracking the user’s location.

 @param locationProvider The location provider that is tracking the user’s location.
 */
- (void)locationProviderDidStopLocatingUser:(id<HDMLocationProvider>)locationProvider;

/**
 Tells the delegate that the location of the user was updated.
 
 This method is not called if the application is currently running in the
 background. If you want to receive location updates while running in the
 background, you must use the Core Location framework.
 
 @param locationProvider The location provider that is tracking the user’s location.
 @param userLocation The location object representing the user’s latest
 location. This property may be `nil`.
 */
- (void)locationProvider:(id<HDMLocationProvider>)locationProvider didUpdateUserLocation:(nullable HDMUserLocation*)userLocation;

/**
 Tells the delegate that an attempt to locate the user’s position failed.
 
 @param locationProvider The location provider that is tracking the user’s location.
 @param error An error object containing the reason why location tracking
 failed.
 */
- (void)locationProvider:(id<HDMLocationProvider>)locationProvider didFailToLocateUserWithError:(NSError *)error;

@end

/**
 A location provider can be any implementation. The only requirement is that it implements
 the delegate which updates the user location */
@protocol HDMLocationProvider <NSObject>

@required
@property(nonatomic, weak, nullable) id<HDMLocationProviderDelegate> delegate;

/**
 Start updating the user location.
 */
- (void)startUpdating;

/**
 Stop updating the user location.
 */
- (void)stopUpdating;

/**
 Indicates whether the location provider is currently tracking the position
 */
-(BOOL)isUpdating;

@optional
/**
 Indicates whether the location provider is currently requesting permissions. 
 This may be useful to e.g. indicate that a modal system view is currently visible, asking a user for app permissions.
 */
-(BOOL)isRequestingPermissions;

@end

NS_ASSUME_NONNULL_END
