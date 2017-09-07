//
//  HDMUserLocation.h
//  hdm-mapcore-lib
//
//  Created by Stadin Benjamin on 26.03.17.
//
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import <HDMMapCore/HDMLocation.h>
#import <UIKit/UIKit.h>


typedef NS_ENUM(NSInteger, HDMUserLocationMapBounds) {
     HDMUserLocationMapBoundsUnknown = 0,
     HDMUserLocationMapBoundsOutside = 1,
     HDMUserLocationMapBoundsInside = 2
};

/**
 The HDMUserLocation class defines a specific type of annotation that identifies
 the user’s current location. You do not create instances of this class
 directly. Instead, you retrieve an existing HDMUserLocation object from the
 `userLocation` property of the map view displayed in your application.
 */
@interface HDMUserLocation : NSObject <NSCopying>

-(_Nonnull instancetype)initWithLocation:(HDMLocation* _Nonnull)location
                       updating:(BOOL)updating
                        heading:(CLHeading* _Nullable)heading;

/**
 The current location of the device. (read-only)
 
 This property contains `nil` if the map view is not currently showing the user
 location or if the user’s location has not yet been determined.
 */
@property (nonatomic, readonly, nullable) HDMLocation *location;

/**
 A Boolean value indicating whether the user’s location is currently being
 updated. (read-only)
 */
@property (nonatomic, readonly, getter=isUpdating) BOOL updating;

/**
 The heading of the user location. (read-only)
 
 This property is `nil` if the user location tracking mode is not
 `HDMUserTrackingModeFollowWithHeading`.
 */
@property (nonatomic, readonly, nullable) CLHeading *heading;


/**
    Property shows if location is inside or outside of current map.
 */
@property (nonatomic, readonly) HDMUserLocationMapBounds locationToMapState;

@end
