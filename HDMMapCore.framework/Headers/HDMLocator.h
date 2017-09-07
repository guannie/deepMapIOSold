//
//  HDMLocator.h
//  HDMMapCore
//
//  Created by Mikalai Lasitsa on 9/27/16.
//  Copyright © 2016 Heidelberg mobil International GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <HDMMapCore/HDMMapTypes.h>
#import <HDMMapCore/HDMFeature.h>
#import <HDMMapCore/HDMProjector.h>
#import <CoreLocation/CoreLocation.h>

@class HDMProjector;


@interface HDMLocator : NSObject

/**
 Init HDMLocator instance

 @param path Full path to a Deep Map database
 @param apiCRS CRS in use by the API
 @param displayCRS CRS used by the renderer
 @param displayCRS CRS used by the renderer
 @param elevationMode The elevation mode used by the API CRS

 @return HDMLocator instance
 */
- (nonnull instancetype)initWithWithDb:(nonnull NSString*)path apiCRS:(nonnull NSString*)apiCRS displayCRS:(nonnull NSString*)displayCRS elevationMode:(HDMElevationMode)elevationMode;
- (nonnull instancetype)initWithWithDb:(nonnull NSString*)path projector:(nonnull HDMProjector*)projector;
- (nonnull instancetype)initWithProjector:(nonnull HDMProjector*)projector;

/**
 Sets the database that is used for all lookups.
 
 @param path Full path to a Deep Map database
 */
- (void)setDB:(nonnull NSString*)path;

/**
 Fetch a feature object from the database by it's "original serial" (aka customer identifier).
 
 @param originalSerial The original serial (customer identifier) of the feature which is searched for.
 @return HDMFeeature instance if found. Returns nil if the object was not found.
 */
- (nullable HDMFeature*)getFeatureByOriginalSerial:(nonnull NSString*)originalSerial;

/**
 Fetch a feature object from the database by featureId.
 
 @param featureId The database ID of a feature
 @return HDMFeeature instance if found. Returns nil if the object was not found.
 */
- (nonnull HDMFeature*)getFeatureById:(uint64_t)featureId;

/**
 Fetch an array of features from an array of featureIds.
 
 @param featureIds The database IDs for features to retrieve from the db
 @return Array of HDMFeeature instances if found. Returns an empty array if no obect was found.
 */
- (nonnull NSArray<HDMFeature*>*)getFeaturesByIds:(nonnull NSArray<NSNumber*>*)featureIds NS_REFINED_FOR_SWIFT;

/**
 Fetch an array of features by SQL statement.
 
 @param sql The SQL query for feature lookup.
 @return Array of HDMFeeature instances if found. Returns an empty array if no obect was found.
 
 @discussion The SQL query must return feature IDs via a column object_id. After the query executed, 
 full features are created and returned for each unqiue featureId occurring in the result set of the query.
 
 The keys and values of features are stored in the 'tags' table.
 
 Example:
 "SELECT object_id from tags WHERE key = 'level' AND value = '4'"
 */
- (nonnull NSArray<HDMFeature*>*)getFeaturesWithSQL:(nonnull NSString*)sql;

/**
 * Returns the maximum level/floor of an feature on the map
 */
- (float)getMaximumLevel;
/**
 * Returns the minimum level/floor of an feature on the map
 */
- (float)getMinimumLevel;

/**
 Get the ID for a feature by it's original serial.
 
 @param originalSerial The original serial (customer identifier) of the feature which is searched for.
 @return NSNumber of featureId if found. Nil if not found.
 */
- (uint64_t)getFeatureIdByOriginalSerial:(nonnull NSString*)originalSerial;

/**
 *  Get the localized floor name for a given floor and locale.
 
     Example return values (long form):
     
  en:
     1st <basement> floor
     2nd <basement> floor
     ...
     11th <basement> floor
     ...
     20th <basement> floor
     21st <basement> floor
     ...
     
     Mezzanine 1.5
     
  de:
     ...
     1. Untergeschoss
     Erdgeschoss
     1. Obergeschoss
     2. Obergeschoss
     1.5 Zwischengeschoss
     
     
     Example return values (short form):
     
  en:
     1st
     2nd
     ...
     11th
     ...
     20th
     21st
     ...
     
     1.5
     
  de:
     ...
     1. UG
     EG
     1. OG
     2. OG
     1.5 ZG
 */
- (nonnull NSString*)getLocalizedFloorName:(float)floor withLocale:(NSString*_Nonnull)localeStr shortName:(BOOL)shortName;

/**
 *  Get the localized numeric floor name for a given floor and locale.
 
     Example return values:
     
  en:
     -2
     -1
     1
     1.5
     2
     3
     ...
     
  de:
     -1
     0
     1
     2
     ...
 */
- (nonnull NSString*)getLocalizedNumericFloorName:(float)floor withLocale:(NSString*_Nonnull)localeStr;

- (nonnull NSArray<HDMFeature*>*)floors;

@end
