//
//  HDMGPSLocationProvider.h
//  hdm-mapcore-lib
//
//  Created by Stadin Benjamin on 27.03.17.
//
//

#import <Foundation/Foundation.h>
#import <CoreLocation/CoreLocation.h>
#import "HDMLocationProvider.h"

@interface HDMGPSLocationProvider : NSObject <HDMLocationProvider, CLLocationManagerDelegate>

/**
 Define whether a heading calibration alert should be shown when necessary.
 Default is `YES`.
 */
@property (nonatomic, assign) BOOL displayHeadingCalibration;

/**
 The receiver’s delegate.
 
 A map view sends messages to its delegate to notify it of changes to its
 contents or the viewpoint. The delegate also provides information about
 annotations displayed on the map, such as the styles to apply to individual
 annotations.
 */
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

/**
 Indicates that the app is currently asking for CoreLocation permissions.
 */
-(BOOL)isRequestingPermissions;

/**
 Indicates whether the location provider is currently simulating location updates.
 */
-(BOOL)isSimulating;

/**
 Simulates location updates using provided location array and time interval for the updates.
 GPS updates are ignored in simulation mode.
 */
-(void)simulateLocationUpdates:(NSArray<HDMLocation*>*_Nonnull)locations withInterval:(NSTimeInterval)interval;

/**
 Stops simulating location updates
 */

-(void)stopSimulatingLocationUpdates;

@end
