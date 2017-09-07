//
//  HDMMapView.h
//  HDMMapCore
//
//
//  Copyright (c) 2017 Heidelberg mobil International GmbH. All rights reserved.
//

#import <UIKit/UIKit.h>
#import <GLKit/GLKit.h>
#import <HDMMapCore/HDMAnnotation.h>
#import <HDMMapCore/DeepMap.h>
#import <HDMMapCore/HDMFeature.h>
#import <HDMMapCore/HDMRoutingPathFeature.h>
#import <HDMMapCore/HDMMapTypes.h>
#import <HDMMapCore/HDMUserLocation.h>
#import <HDMMapCore/HDMProjector.h>
#import <HDMMapCore/HDMLocator.h>
#import <HDMMapCore/HDMMapCamera.h>
#import <HDMMapCore/HDMMapRouting.h>


#include <CoreLocation/CoreLocation.h>

#import <HDMMapCore/HDMGPSLocationProvider.h>

@protocol HDMMapViewAnnotationProtocol <NSObject>

/**
 * Annotating the Map
 *
 */

/** The complete list of annotations associated with the receiver. (read-only)
 *
 * If no annotations are associated with the map view, the value of this property is `nil`. */
@property (nonatomic, readonly, nonnull) NSArray<HDMAnnotation*>* annotations;

/** Adds the specified annotation to the map view.
 *   @param annotation The annotation object to add to the receiver. This object must conform
 * to the HDMAnnotation protocol. The map view retains the specified annotation. */
- (void)addAnnotation:(HDMAnnotation * _Nullable)annotation;

- (void)addAnnotation:(HDMAnnotation * _Nullable)annotation toCoordinate:(HDMMapCoordinate)coordinate;
- (void)addAnnotation:(HDMAnnotation * _Nullable)annotation toEntity:(uint64_t)featureId;
- (void)addAnnotation:(HDMAnnotation * _Nullable)annotation toFeature:(HDMFeature* _Nonnull)feature;

/** Adds an array of annotations to the map view.
 *   @param annotations An array of annotation objects. Each object in the array must
 * conform to the HDMAnnotation protocol. The map view retains the individual annotation objects. */
- (void)addAnnotations:(NSArray<HDMAnnotation*>* _Nullable)annotations;

/** Removes the specified annotation object from the map view.
 *
 *   Removing an annotation object disassociates it from the map view entirely, preventing it from being displayed on the map. Thus, you would typically call this method only when you want to hide or delete a given annotation.
 *
 *   @param annotation The annotation object to remove. This object must conform to the HDMAnnotation protocol. */
- (void)removeAnnotation:(HDMAnnotation * _Nullable)annotation;

/** Removes an array of annotation objects from the map view.
 *
 *   Removing annotation objects disassociates them from the map view entirely, preventing them from being displayed on the map. Thus, you would typically call this method only when you want to hide or delete the specified annotations.
 *
 *   @param annotations The array of annotations to remove. Objects in the array must conform to the HDMAnnotation protocol. */
- (void)removeAnnotations:(NSArray<HDMAnnotation*>* _Nullable)annotations;


/** Move camera to region which consist passed annotations
 *
 *  @param annotations Annotations you want to move to
 *  @param animated If YES, animate Map moving to required annotations If NO, show required map region without any animations.
 */
- (void)moveToAnnotations:(NSArray<HDMAnnotation*> * _Nullable)annotations animated:(BOOL)animated;

@end

/**
 The mode used to track the user's location on the map. Used by
 `HDMMapView.userTrackingMode`.
 */
typedef enum : NSUInteger
{
    /** The map does not follow the user location. */
    HDMUserTrackingModeNone =0,
    /** The map follows the user location. This tracking mode falls back
     to `HDMUserTrackingModeNone` if the user pans the map view. */
    HDMUserTrackingModeFollow =1,
    /**
     The map follows the user location and rotates when the heading changes.
     The default user location annotation displays a fan-shaped indicator with
     the current heading. The heading indicator represents the direction the
     device is facing, which is sized according to the reported accuracy.
     
     This tracking mode is disabled if the user pans the map view, but
     remains enabled if the user zooms in. If the user rotates the map
     view, this tracking mode will fall back to `HDMUserTrackingModeFollow`.
     */
    HDMUserTrackingModeFollowWithHeading =2,
    /**
     The map follows the user location and rotates when the course changes.
     Course represents the direction in which the device is traveling.
     The default user location annotation shows a puck-shaped indicator
     that rotates as the course changes.
     
     This tracking mode is disabled if the user pans the map view, but
     remains enabled if the user zooms in. If the user rotates the map view,
     this tracking mode will fall back to `HDMUserTrackingModeFollow`.
     */
    HDMUserTrackingModeFollowWithCourse =3
} HDMUserTrackingMode;

/**
 An interactive, customizable map view with an interface similar to the one
 provided by Apple’s MapKit.
 
 HDMMapView can be embedded inside a view, and allow users to
 manipulate it with standard gestures, animate the map between different
 viewpoints, and present information in the form of annotations and overlays.
 
 The map can be styled with a style conforming to the
 <a href="https://sdk.deep-map.com/style-spec">Deep Map Style specification</a>.

 The map view loads vector cells (aka "tiles") that conform to the HSG (Heidelberg Sinusoidal Grid)
 format. 
 
 The map view also initializes the "locator". Locator is Deep Map's data query engine,
 allowing a user to e.g. query for features of a certain type or within vicinity. It also allows
 to query for features on the map by a foreign ID ("original serial"). See the locator
 class for further documentation on locator.
 
 */
@class HDMMapView;
/*!
 *  The HDMMapViewDelegate protocol defines a set of optional methods that you can use to receive map-related update messages.
 */
@protocol HDMMapViewDelegate <NSObject>
@optional

/**
 *  Tells the delegate that the specified map view is done with
 *  internal initialization and about to prepare data.
 *
 *  @param mapView The map view that will start
 *  @discussion This delegate is mainly used for loading internal resources.
 */
- (void)hdmMapViewWillStart:(HDMMapView * _Nonnull)mapView;

/**
 *  Tells the delegate that the specified map view successfully started.
 *
 *  @param mapView The map view that started
 *  @discussion Before this call no API call to the map is accepted
 */
- (void)hdmMapViewDidStart:(HDMMapView * _Nonnull)mapView error:(NSError* _Nullable)error;

/**
 *  Tells the delegate that camera of the specified map view was changed
 *
 *  @param mapView The map view of which the camera was moved
 *  @discussion Calls each time the user pans the map, pinchs, etc.
 */
- (void)hdmMapViewCameraDidChange:(HDMMapView * _Nonnull)mapView;

/**
 *  Tells the delegate all visible objects at the real world coordinate where the tap occured
 *
 *  @param controller   The map view that was tapped
 *  @param coordinate   The real world coordinate where the tap occured
 *  @param ids          Array of object ids of the visible objects at the tapped location, sorted bottom to top
 *                      (topmost visible object is at array index n-1).
 *  @discussion         Called when the map is tapped
 */
- (void)hdmMapView:(HDMMapView * _Nonnull)mapView tappedAtCoordinate:(HDMMapCoordinate)coordinate features:(NSArray<HDMFeature*>* _Nonnull)features;

/**
 *  Tells the delegate all visible objects at the real world coordinate where the long-press occured
 *
 *  @param controller   The map view that was long-pressed
 *  @param point        The real world coordinate where the long-press occured
 *  @param geoIds       Array of object ids of the visible objects at the long-press location, sorted bottom to top
 *                      (topmost visible object is at array index n-1).
 *  @discussion         Called when the map is tapped at a location for more than 1 second
 */
- (void)hdmMapView:(HDMMapView * _Nonnull)mapView longPressedAtCoordinate:(HDMMapCoordinate)coordinate features:(NSArray<HDMFeature*>* _Nonnull)features;

/**
 *  Tells the delegate that the user location was updated on the map to a new location.
 *
 *  @param controller   The map view that was updated with a new user location
 *  @param point        The real world coordinate where the long-press occured
 *  @param geoIds       Array of object ids of the visible objects at the long-press location, sorted bottom to top
 *                      (topmost visible object is at array index n-1).
 *  @discussion         Called when the map is tapped at a location for more than 1 second
 */
- (void)hdmMapView:(HDMMapView * _Nonnull)mapView didUpdateUserLocation:(nullable HDMUserLocation*)userLocation;

/**
 *  Tells the delegate that the map was rotated
 *
 *  @param mapView   The map view that was rotated
 *  @discussion         Called after the map was rotated
 */
- (void)hdmMapViewDidRotate:(HDMMapView * _Nonnull)mapView;

/**
 *  Tells the delegate that the map was panned
 *
 *  @param mapView   The map view that was rotated
 *  @discussion         Called after the map was panned
 */
- (void)hdmMapViewDidPan:(HDMMapView * _Nonnull)mapView;

/**
 *  Tells the delegate that the map was zoomed
 *
 *  @param mapView   The map view that was zoomed
 *  @discussion         Called after the map was zoomed
 */
- (void)hdmMapViewDidZoom:(HDMMapView * _Nonnull)mapView;

/**
 *  Tells the delegate that the map's userTrackingMode was chagned
 *
 *  @param mapView      The map view where userTrackingMode was chagned
 *  @param didChangeUserTrackingMode      Old userTrackingMode
 *  @param toUserTrackingMode             New userTrackingMode
 *  @discussion         Called after userTrackingMode was chagned
 */
- (void)hdmMapView:(HDMMapView * _Nonnull)mapView didChangeUserTrackingMode:(HDMUserTrackingMode)oldUserTrackingMode toUserTrackingMode:(HDMUserTrackingMode)newUserTrackingMode;

@end

@interface HDMMapView : UIView <HDMMapViewAnnotationProtocol, HDMLocationProviderDelegate>

#pragma mark -
#pragma mark Creating a HDMMapView
#pragma mark -

/**
 *  Create a map view and load the specified Deep Map package.
 */
- (instancetype _Nonnull)initWithDeepMap:(DeepMap* _Nonnull)deepMap;

/**
 *  Create a map view loading specified map resources.
 */
- (instancetype _Nonnull)initWithCells:(NSString* _Nonnull)cellFolder
                             atlasFile:(NSString* _Nonnull)atlasFile
                              fontFile:(NSString* _Nonnull)fontFile
                              iconFile:(NSString* _Nonnull)iconFile
                             styleFile:(NSString* _Nonnull)styleFile
                            configFile:(NSString* _Nonnull)configFile
                                dbPath:(NSString* _Nonnull)dbPath;

/**
 *  Create a map view loading specified map resources.
 */
- (instancetype _Nonnull)initWithCells:(NSString* _Nonnull)cellFolder
                             atlasFile:(NSString* _Nonnull)atlasFile
                              fontFile:(NSString* _Nonnull)fontFile
                              iconFile:(NSString* _Nonnull)iconFile
                             styleFile:(NSString* _Nonnull)styleFile
                              ruleFile:(NSString* _Nonnull)ruleFile
                            configFile:(NSString* _Nonnull)configFile
                                dbPath:(NSString* _Nonnull)dbPath;

#pragma mark -
#pragma mark Accessing the map view's delegate
#pragma mark -

/**
 *  The receiver’s delegate.
 *  @discussion A map view sends messages to its delegate regarding the loading of map data, tapping features on the map or change point of view.
 *
 *  The delegate should implement the methods of the HDMMapViewDelegate protocol.
 */
@property (nonatomic, assign, nullable) IBOutlet id<HDMMapViewDelegate> delegate;

#pragma mark -
#pragma mark Configuring the map and it's appearance
#pragma mark -

/**
 * Set and load a new map using provided resource info. 
 * Note: The Map will be reloaded even if the passed map
 * is equal to an already installed map.
 *
 * @param deepMap DeepMap resource instance
 */
- (void)setMap:(DeepMap * _Nonnull)deepMap;

/**
 *  Load and configure a map by loading individual map resource files.
 */
- (void)configureMap:(NSString* _Nonnull)cellFolder
           atlasFile:(NSString* _Nonnull)atlasFile
            fontFile:(NSString* _Nonnull)fontFile
            iconFile:(NSString* _Nonnull)iconFile
           styleFile:(NSString* _Nonnull)styleFile
          configFile:(NSString* _Nonnull)configFile
              dbPath:(NSString* _Nonnull)dbPath;

/**
 *  @returns if the map has been configured with resource files already
 */
-(bool)isMapConfigured;

/**
 *  Load and configure a map by loading individual map resource files.
 */
- (void)configureMap:(NSString* _Nonnull)cellFolder
           atlasFile:(NSString* _Nonnull)atlasFile
            fontFile:(NSString* _Nonnull)fontFile
            iconFile:(NSString* _Nonnull)iconFile
           styleFile:(NSString* _Nonnull)styleFile
            ruleFile:(NSString* _Nonnull)ruleFile
          configFile:(NSString* _Nonnull)configFile
              dbPath:(NSString* _Nonnull)dbPath;

/**
 The camera used to show the currently visible portion of the map.
 */
@property (nonatomic, readwrite, nonnull) HDMMapCamera* camera;

/**
 Changes the camera used to show the map, with optional animation.
 */
- (void)setCamera:(HDMMapCamera* _Nonnull)camera animated:(BOOL)animated;



/**
 *  Toggle between 2D and 3D mode. Note: This method uses default values for the rotation angle of the camera (top: 0, bottom: (3.0f * M_PI / 10.0f)).
 *  You can customize the rotation angle, and set setRotateYTop and setRotateYBottom directly instead of using this method.
 *
 *  @param enable3D  enable or disable 3D mode
 *  @param animated If YES, animate Map moving to required point If NO, show required map region without any animations.
 */
- (void)set3DMode:(BOOL)enable3D animated:(BOOL)animated;

/**
 The Deep Map logo, positioned in the lower left corner of the view.
 @note The Deep Map terms of service requires most Deep Map
 customers to display the Deep Map logo. If this applies to you, do not
 hide this view or change its contents.
 */
@property (nonatomic, readonly, nullable) UIImageView *logoView;

/**
 * Path to a Deep Map Style file.
 *
 @brief setter: Sets and loads a Deep Map Style file
 getter: Returns the path to the current Deep Map Style file
 @discussion Setting a different style forces the map to reinitialize.
 */
@property (nonatomic, copy, nonnull) NSString *style;


/**
 * Path to a Deep Map Rule file.
 *
 @brief setter: Sets and loads a Deep Map Rule file
 getter: Returns the path to the current Deep Map Rule file
 @discussion Setting different rules forces the map to reinitialize.
 */
@property (nonatomic, copy, nonnull) NSString *ruleFile;
    
@property (nonatomic, assign) BOOL showsCompass;

@property (nonatomic, strong, nonnull) UIView *compassView;

#pragma mark -
#pragma mark Manipulating the map projection
#pragma mark -

/**
 The projector convertes between coordinates within the 
 CRS (coordinate reference system) used by the API and
 the CRS used by the renderer. 
 By default the API uses WGS84 CRS (same as what GPS uses)
 while the map is rendered commonly in a certain UTM 
 zone (depending on the loaded map).
 */
@property (nonatomic, readonly, nullable) HDMProjector *projector;

#pragma mark -
#pragma mark Geocoding and data querying
#pragma mark -

/**
 Locator allows to query for map features by attribute or original serial.
 An original serial is the object identifier imported from private map data.
 */
@property (nonatomic, readonly, nonnull) HDMLocator *locator;


#pragma mark -
#pragma mark Displaying the user's location
#pragma mark -

/**
 A Boolean value indicating whether the map may display the user location.
 
 As long as this property is `YES`, the
 map view continues to track the user’s location and update it as location
 information is updated.
 
 This property does not indicate whether the user’s position is actually visible
 on the map, only whether the map view is allowed to display it. To determine
 whether the user’s position is visible, use the `userLocationVisible` property.
 The default value of this property is `NO`.
 
 When setting this property to true and the locationProvider is set to
 HDMDefaultLocationProvider (default), the map view will use the Core Location
 framework to find the current location.
 When the locationProvider is set to HDMCustomLocationProvider updateUserLocation()
 must be called whenever the external location provider wisches to update the 
 user's current location.
 
 Your app must define a value for `NSLocationWhenInUseUsageDescription` or
 `NSLocationAlwaysUsageDescription` in the `Info.plist` to satisfy the
 requirements of the underlying Core Location framework when enabling this
 property.
 */
@property (nonatomic, assign) BOOL showsUserLocation;

/**
 A Boolean value indicating whether the device’s current location is visible in
 the map view.
 
 Use `showsUserLocation` to control the visibility of the displayed user location.
 */
@property (nonatomic, assign, readonly, getter=isUserLocationVisible) BOOL userLocationVisible;

/**
 * Returns if a given HDMLocation is inside map bounds
 */
- (BOOL)isInsideMap:(HDMLocation* _Nonnull)location;

/**
 Returns the user’s current location.
 */
@property (nonatomic, readonly, nullable) HDMUserLocation *userLocation;

/**
 The mode used to track the user's location on the map. The default value is
 `HDMUserTrackingModeNone`.
 
 Changing the value of this property updates the map view with an animated
 transition. If you don’t want to animate the change, use the
 `-setUserTrackingMode:animated:` method instead.
 */
@property (nonatomic, assign) HDMUserTrackingMode userTrackingMode;

/**
 Set an alternative location provider to be used by the map. 
 The default location provider is HDMGPSLocationProvider which uses
 CoreLocation to find a user's current position.
 */
@property (nonatomic, readwrite, nonnull) id<HDMLocationProvider> locationProvider;

#pragma mark -
#pragma mark Manipulating the visible area of the map
#pragma mark -

/* The currently visible region of the map. The region is described by a center point and a coordinate span.
   Span is given in the delta of longitude and latitude of coordinates.
 */
@property (nonatomic, readonly) HDMMapCoordinateRegion region;

-(void)setRegion:(HDMMapCoordinateRegion)region animated:(BOOL)animated;

/**
 The coordinate of the map center. Returns center of available map.
 */
@property (nonatomic, readonly) HDMMapCoordinate mapCenter;

/*!
 @brief setter: Moves the map to the specified center coordinate
 getter: Get coordinates of curren map view center
 @discussion Doesn't affect map if coordinates out of map bounds or if projection source is unknown
 */
@property (nonatomic, assign) HDMMapCoordinate centerCoordinate;

/**
 *  Moves the map to the specified center coordinate with animation if needed
 *
 *  @param coordinate WGS84 coordinates (latitude, longitude)
 *  @param animated If YES, animate Map moving to required coordinate If NO, show required map region without any animations.
 *  @discussion Doesn't affect map if coordinates out of map bounds or if projection source is unknown
 */
- (void)setCenterCoordinate:(HDMMapCoordinate)coordinate animated:(BOOL)animated;

/**
 *  Set current Map Level
 *
 *  @param level    Required level
 *  @param animated If YES, animate the setting Map level If NO, set required without any animations.
 */
- (void)setLevel:(float)level animated:(BOOL)animated;

/**
 *  Current map's level (also know as floor)
 */
@property (nonatomic, readonly) float currentLevel;

/*
 * minimum distance of the camera to currently selected floor.
 */
@property (nonatomic) float minFloorDistance;

/*
 * Get the maximum level of an feature on the loaded map
 */
- (int)getMaximumLevel;

/*
 * Get the minimum level of an feature on the loaded map
 */
- (int)getMinimumLevel;

/**
 * @returns coordinates of the given feature.
 */
- (HDMMapCoordinate)getCoordinateForFeatureWithId:(uint64_t) featureId;

/**
 * @returns The featuretype of the feature with the given featureId.
 */
- (nullable NSString*)getFeatureTypeById:(uint64_t)featureId;

/**
 * @returns the feature to a given featureId
 */
- (HDMFeature*_Nullable)getFeatureById:(uint64_t)featureId;

/**
 *  Heading of the map (in degrees)
 *
 *  @param direction direction in degrees from 0 to 359.9
 *  @param animated If YES, animate the heading setting  If NO, set specified heading without any animations.
 */
- (void)setDirection:(CLLocationDirection)direction animated:(BOOL)animated;

/**
 *  Reset heading of the map
 *  @discussion Equal direction = 0.0
 */
- (void)resetNorth;

/**
 * Ask map to show region with point in the center
 *
 *  @param coordinate Required coordinates in WGS84 (lat, long)
 *  @param animated If YES, animate Map moving to required point If NO, show required map region without any animations.*
 */
-(void)moveToCoordinate:(HDMMapCoordinate)coordinate animated:(BOOL)animated;

/**
 * Ask map to show region with point in the center
 *
 *  @param coordinate Required coordinates in WGS84 (lat, long)
 *  @param radius   Required radius in meters
 *  @param animated If YES, animate Map moving to required point If NO, show required map region without any animations.*
 */
-(void)moveToCoordinate:(HDMMapCoordinate)coordinate radius:(double)radius animated:(BOOL)animated;

/**
 *  Ask map to show region that incudes specified object
 *
 *  @param featureId    Feature's featureId
 *  @param animated If YES, animate Map moving to required obejct If NO, show required map region without any animations.
 *
 *  @discussion Doesn't affect map if featureId wasn't found. Feature's featureId comes in map DB
 */
- (void)moveToFeatureWithId:(uint64_t)featureId animated:(BOOL)animated;

/**
 *  Ask map to show region that incudes specified object
 *
 *  @param featureId    Feature's original serial
 *  @param animated     If YES, animate Map moving to required obejct If NO, show required map region without any animations.
 *
 *  @discussion Doesn't affect map if featureId wasn't found. Feature's featureId comes in map DB
 */
- (void)moveToFeatureWithOriginalSerial:(NSString* _Nonnull)originalSerial animated:(BOOL)animated;

/**
 *  Ask map to show region that incudes specified objects
 *
 *  @param ids      Set of feature's featureIds
 *  @param animated If YES, animate Map moving to required obejct If NO, show required map region without any animations.
 *  @discussion Doesn't affect map if featureIds weren't found.
 */
- (void)moveToFeaturesWithIds:(NSArray<NSNumber*>* _Nonnull)ids animated:(BOOL)animated;

/**
 *  Ask map to show region that fits line with required ID
 *
 *  @param line     Line's ID
 *  @param animated Move to line with animation
 */
- (void)moveToLine:(uint64_t)lineID animated:(BOOL)animated;

/**
 *  Ask map to show region that incudes specified objects
 *
 *  @param ids      Set of feature's original serials
 *  @param animated If YES, animate Map moving to required obejct If NO, show required map region without any animations.
 *  @discussion Doesn't affect map if featureIds weren't found.
 */
- (void)moveToFeaturesWithOriginalSerials:(NSArray<NSString*>* _Nonnull)originalSerials animated:(BOOL)animated;

/**
 *  Stops all currently running camera animations.
 */
-(void)stopAllCameraAnimations;

#pragma mark -
#pragma mark Interact with and act on features on the map
#pragma mark -

/**
 *  Select the feature with specified featureId
 *
 *  @param featureId    Feature's featureId that should be selected
 *
 *  @discussion Doesn't affect map if featureId is not available. Feature's featureId come in map DB. Also please note that selection just change state of specified feature, but not color or other style parameters. If you want to have special style for selected feature make sure that you have coresponding map materiels for selected stаte.
 *  @see setMaterialsConfig:
 */
- (void)selectFeatureWithId:(uint64_t)featureId;

/**
 *  Select the feature with specified original serial
 *
 *  @param originalSerial    Feature's featureId that should be selected
 *
 *  @discussion Doesn't affect map if featureId is not available. Feature's featureId come in map DB. Also please note that selection just change state of specified feature, but not color or other style parameters. If you want to have special style for selected feature make sure that you have coresponding map materiels for selected stаte.
 *  @see setMaterialsConfig:
 */
- (void)selectFeatureWithOriginalSerial:(NSString* _Nonnull)originalSerial;

 /**
  * Deselect the feature with specified id
  *
  * @param featureId    The Id of the feaute that should be deselected
  */
- (void)deselectFeatureWithId:(uint64_t)featureId;

/**
 * Deselect the feature with original serial
 *
 * @param originalSerial    The original serial of the feaute that should be deselected
 */
- (void)deselectFeatureWithOriginalSerial:(NSString* _Nonnull)originalSerial;

/**
 *  Highlight the feature with specified featureId
 *
 *  @param featureId    Feature's featureId that should be highlighted
 *
 *  @discussion Doesn't affect map if featureId is not available. Feature's featureId come in map DB. Also please note that selection just change state of specified feature, but not color or other style parameters. If you want to have special style for selected feature make sure that you have coresponding map materiels for selected stаte.
 *  @see setMaterialsConfig:
 */
- (void)highlightFeatureWithId:(uint64_t)featureId;

/**
 *  Highlight the feature with specified original serial
 *
 *  @param originalSerial    Feature's featureId that should be highlighted
 *
 *  @discussion Doesn't affect map if featureId is not available. Feature's featureId come in map DB. Also please note that selection just change state of specified feature, but not color or other style parameters. If you want to have special style for selected feature make sure that you have coresponding map materiels for selected stаte.
 *  @see setMaterialsConfig:
 */
- (void)highlightFeatureWithOriginalSerial:(NSString* _Nonnull)originalSerial;

/**
 * Unhighlight the feature with specified id
 *
 * @param featureId    The Id of the feaute that should be unhighlighted
 */
- (void)unhighlightFeatureWithId:(uint64_t)featureId;

/**
 * Unhighlight the feature with original serial
 *
 * @param originalSerial    The original serial of the feaute that should be unhighlighted
 */
- (void)unhighlightFeatureWithOriginalSerial:(NSString* _Nonnull)originalSerial;

#pragma mark -
#pragma mark Adding custom data to the map
#pragma mark -

/*
 * Add a 3d object to the map. Only .obj files are supported for now.
 */
- (uint64_t)createMeshFeature:(HDMMapCoordinate)position withType:(NSString* _Nonnull)ftypename meshFile:(NSString* _Nonnull)meshfile animated:(BOOL)animated;

#pragma mark -
#pragma mark Navigating from a start point to a destination
#pragma mark -

/*
 Navigate from a start location to a destination. If the location has a floor level set for start or destination location,
 the floor level is considered when the route is calculated.
 This method drwas the routing path on the map, and also shows and updates the user's current position ("blue dot") if the tracking mode is not none.
 This method causes the locationProvider to start updating a user's location (e.g. GPS).
 */
- (void)navigateWithPath:(HDMRoutingPathFeature* _Nonnull)path usingTrackingMode:(HDMUserTrackingMode)trackingMode;

/**
 Cancels the current navigation, and removes the routing line and blue dot position from the map
 */
-(void)cancelNavigation;

#pragma mark -
#pragma mark Showing custom map content
#pragma mark -

/**
 Adds a feature (e.g. a routing line feature) to the map and draws it.
 */
- (void)addFeature:(HDMFeature* _Nonnull)feature;

/**
 Removes a feature from the map.
 */
- (void)removeFeature:(HDMFeature* _Nonnull)feature;

#pragma mark -
#pragma mark Manipulating dynamic style properties
#pragma mark -

/**
 *  Shows all objects of a given featuretype. Forces and immediate update of the scene
 *
 *  @param featureName  the name of a feature type
 */
- (void)showFeatures:(NSString* _Nonnull)featureName;

/**
 *  Hides all objects of a given featuretype. Forces and immediate update of the scene
 *
 *  @param featureName  the name of a feature type
 */
- (void)hideFeatures:(NSString* _Nonnull)featureName;

/**
 *  Shows all objects of a given featuretype. Choose if an update should be forced
 *
 *  @param featureName  the name of a feature type
 *  @param update       If YES, update the scene immediatly. If NO, update when reloadStyle or setFeatureStyle with update is called
 */
- (void)showFeatures:(NSString* _Nonnull)featureName update:(BOOL)update;

/**
 *  Hides all objects of a given featuretype. Choose if an update should be forced
 *
 *  @param featureName  the name of a feature type
 *  @param update       If YES, update the scene immediatly. If NO, update when reloadStyle or setFeatureStyle with update is called
 */
- (void)hideFeatures:(NSString* _Nonnull)featureName update:(BOOL)update;

/**
 * Changes property value of a featureStyle. Forces an immediate update of the scene
 * The values of this function correspond to the DeepMap Style definitions
 *
 * @param featureName   the name of the featureStyle
 * @param propertyName  the name of the property to be changed
 * @param value         the new value for the property
 */
- (BOOL)setFeatureStyle:(NSString* _Nonnull)featureName propertyName:(NSString* _Nonnull)propertyName value:(NSString* _Nonnull)value;

/**
 * Changes property value of a featureStyle. Choose if an update should be forced
 * The values of this function correspond to the DeepMap Style definitions
 *
 * @param featureName   the name of the featureStyle
 * @param propertyName  the name of the property to be changed
 * @param value         the new value for the property
 * @param update        If YES, update the scene immediatly. If NO, update when reloadStyle or setFeatureStyle/showFeature with update is called
 */
- (BOOL)setFeatureStyle:(NSString* _Nonnull)featureName propertyName:(NSString* _Nonnull)propertyName value:(NSString* _Nonnull)value update:(BOOL)update;

/**
 *  Forces the engine to reload the style materials
 *  Should be called if multiple featureStyles have been changed whithout forcing an update
 */
- (void)reloadStyle;

/**
 * Swaps out the current Deep Map Style file and Deep Map Rule file
 * @param styleFile the name of the new Deep Map Style file
 * @param ruleFile  the name of the new Deep Map Rule file
 */

- (void)switchStyles:(NSString* _Nonnull)styleFile ruleFile:(NSString* _Nonnull)ruleFile;

#pragma mark -
#pragma mark Assigning data to map rules
#pragma mark -

/**
 *  Sets custom Attribute for a feature, which can be queried by the Style-Rules
 *  
 *  @param key              the name of the attribute
 *  @param value            the value of the attribute
 *  @param originalSerial   the serial name of the feature
 */
- (void)setFeatureAttribute:(NSString* _Nonnull)key value:(NSString* _Nonnull)value withOriginalSerial:(NSString* _Nonnull)originalSerial;


/**
 *  Removes custom Attribute for a feature
 *
 *  @param key              the name of the attribute
 *  @param featureId         the id of the entity
 */
- (void)removeFeatureAttribute:(NSString* _Nonnull)key withOriginalSerial:(NSString* _Nonnull)originalSerial;

/**
 *  Sets custom Attribute for a feature, which can be queried by the Style-Rules
 *
 *  @param key              the name of the attribute
 *  @param value            the value of the attribute
 *  @param featureId         the id of the entity
 */
- (void)setFeatureAttribute:(NSString* _Nonnull)key value:(NSString* _Nonnull)value withFeatureId:(uint64_t)featureId;


/**
 *  Removes custom Attribute for a feature
 *
 *  @param key              the name of the attribute
 *  @param featureId         the id of the entity
 */
- (void)removeFeatureAttribute:(NSString* _Nonnull)key withFeatureId:(uint64_t)featureId;

/**
 *  Sets a global Attribute which can be queried by the all Style-Rules
 *
 *  @param key              the name of the attribute
 *  @param value            the value of the attribute
 */
- (void)setGlobalAttribute:(NSString* _Nonnull)key value:(NSString* _Nonnull)value;

/**
 * Removes a global Attribute
 *
 * @param key   the name of the attribute
 */
- (void)removeGlobalAtribute:(NSString* _Nonnull)key;


#pragma mark -
#pragma mark Enabling and disabling map interaction
#pragma mark -

/*!
 @brief Enable or disable zooming
 @discussion Default value is YES (zoom is enabled)
 */
@property (nonatomic, assign) BOOL zoomEnabled;

/**
 *  Enable or disable scrolling(map moving)
 *   @discussion Default value is YES (scrolling is enabled)
 */
@property (nonatomic, assign) BOOL scrollEnabled;

/**
 *  Enable or disable rotation
 *  @discussion Default value is YES (rotation is enabled)
 */
@property (nonatomic, assign) BOOL rotateEnabled;



/**
 *  Enable or disable tap
 *  @discussion Default value is YES (tap is enabled)
 */
@property (nonatomic, assign) BOOL tapEnabled;
/**
 *  Enable or disable long press
 *  @discussion Default value is YES (long press is enabled)
 */
@property (nonatomic, assign) BOOL longPressEnabled;

/**
 *  Enable or disable tilt gesture (two finger pan for changing camera angel)
 *  @discussion Default value is NO (Tilt is disabled)
 */
@property (nonatomic, assign) BOOL tiltEnabled;

@end
