//
//  HDMProjector.h
//  hdm-mapcore-lib
//
//  Created by Stadin Benjamin on 30.03.17.
//
//

#import <Foundation/Foundation.h>
#import <HDMMapCore/HDMMapTypes.h>

/**
 * The elevation mode to be used by the API. When HDMElevationModeLocal is used,
 * the z value of all coordinates provided to and returned from the API is based
 * on a local reference. Ususally this means like all features on the map lie on a
 * flat plane, no geoid or sea level is considered.
 * E.g. the ground floor of a building may be at 0m elevation when local reference is used.
 *
 * When HDMElevationModeWGS84 is used the elevation is the ellipsoidal height based on WGS84.
 *
 * Regardless of the elevation mode, values are always in meters.
 */
typedef enum : NSUInteger {
    HDMElevationModeLocal,
    HDMElevationModeWGS84
} HDMElevationMode;

@interface HDMProjector : NSObject

- (instancetype)initWithApiCRS:(NSString*)apiCRS displayCRS:(NSString*)displayCRS elevationMode:(HDMElevationMode)elevationMode;

/**
 * Sets the coordinate reference system (CRS) to be used for all data and values passed to and returned by the map.
 * If individual map sources use a CRS other than the one specified, the coordinates are converted internally before.
 * Note: This does not change the projection for *displaying* the map, but only affects the API.
 * By default WGS84 is used, which is common and most convenient since it’s applicable for the whole earth
 * most commonly used e.g. as GPS location.
 *
 * The default is EPSG:4362 used by GPS (also known as WGS84).
 *
 @param crs     A valid CRS: http://spatialreference.org
 @return        NO if CRS is not valid or not supported otherwise, YES on success
 *
 @discussion This does *not* change the CRS used to render the map. See setDisplayCRS to change the
 projection used for rendering.
 *
 */
- (BOOL)setCRS:(NSString*)crs;

/**
 * Return the CRS used by the API.
 */
- (NSString*)CRS;

/**
 * Set’s the projection to be used to *display* the map. If no display CRS is set,
 * the CRS of the main map data source is used (commonly some UTM projection for indoor applications).
 *
 @param crs     A valid CRS: http://spatialreference.org
 @return        NO if CRS is not valid or not supported otherwise, YES on success
 *
 @discussion
 * The rational to have separate CRS for displaying the map and for the API is that
 * most location services (GPS, Beacons) today use WGS84 which is applicable for the whole globe.
 * For rendering indoor maps a map projection (usually a UTM zone) is used, which is not applicable
 * for the whole world.
 *
 * Put simple, commonly users don't need to care about map projections but can just use the
 * common world-wide applicable WGS84 ("GPS") for the API and rely on that the map translates all
 * API calls and renders the map using the best fitting projection for rendering.
 *
 * Note: This does *not* change the CRS used by the API. See setCRS to change the API CRS.
 */
- (BOOL)setDisplayCRS:(NSString*)crs;

/**
 * Return the CRS used by the API.
 */
- (NSString*)displayCRS;

/**
 * The elevation mode to be used by the API. See the description
 * about elevation modes in the typedef of HDMElevationMode.
 */
@property (nonatomic, assign) HDMElevationMode elevationMode;

/**
 * Project a coordinate in API CRS to a coordinate in display CRS
 */
- (HDMMapCoordinate)projectAPIToDisplayCoordinate:(HDMMapCoordinate)apiCoordinate;

/**
 * Project a coordinate in display CRS to a coordinate in API CRS
 */
- (HDMMapCoordinate)projectDisplayToAPICoordinate:(HDMMapCoordinate)displayCoordinate;

/**
 Return YES if the projection was initialized successfully.
 */
- (BOOL)isInitialized;

@end
