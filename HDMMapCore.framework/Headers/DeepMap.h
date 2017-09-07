//
//  DeepMap.h
//  HDMMapCoreDemo
//
//  Created by Benjamin Stadin on 16.11.15.
//  Copyright © 2015 Heidelberg mobil International GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIView.h>
#define MAPS_CONFIG_FILENAME @"maps.json"
#define MAPS_CONFIG_TEST @"testing"
#define MAPS_CONFIG_LIVE @"live"
#define MAPS_PROJECT_LIST_FILENAME @"customer_projects.json"

#define kDeepMapPackage @"DeepMapPackage"

typedef NS_ENUM(NSInteger, DeepmapError) {
    DeepmapErrorNetwork,
    DeepmapErrorSameMap,
    DeepmapErrorUpdateFileCorrupted,
    DeepmapErrorMapPackageCorrupted
};

/**
 *  Class helps to store and fetch all map related resources
 */
@interface MapResources : NSObject

/**
 * @name  Properties
 */

/**
 *  Path to folder with cells
 */
@property (weak, nonatomic, readonly, nullable) NSString  *cellPath;

/**
 *  Path to atlas file
 */
@property (weak, nonatomic, readonly, nullable) NSString  *atlasPath;

/**
 *  Paths to icons file
 */
@property (weak, nonatomic, readonly, nullable) NSString  *iconsPath;

/**
 *  Path to font
 */
@property (weak, nonatomic, readonly, nullable) NSString  *fontPath;

/**
 *  Path to Deep Map Style file
 */
@property (weak, nonatomic, readonly, nullable) NSString  *stylePath;

/**
 *  Path to Deep Map Rule file
 */
@property (weak, nonatomic, readonly, nullable) NSString *rulePath;

/**
 *  Path to settings file
 */
@property (weak, nonatomic, readonly, nullable) NSString  *settingsPath;

/**
 *  Path to sqlite3 data base which provide some information about map's object
 */
@property (weak, nonatomic, readonly, nullable) NSString  *databasePath;

/**
 * @name  Creating
 */

/**
 *  MapResources constructor
 *
 *  @param path Path to map resources file (map_resources.plist)
 *
 *  @return MapResources instance
 */
- (nonnull instancetype)initWithMapPath:(NSString* _Nonnull)path;

@end

/**
 *  DeepMap reperesent map package properties
 */
@interface DeepMap : NSObject
/**
 * @name  Properties
 */

/**
 *  Customer of map package
 */
@property (nonatomic, readonly, nullable) NSString    *customerName;

/**
 *  Project of map package
 */
@property (nonatomic, readonly, nullable) NSString    *projectName;

/**
 *  Map package's comment
 */
@property (nonatomic, readonly, nullable) NSString    *mapComment;

/**
 *  Date of map package creation
 */
@property (nonatomic, readonly, nullable) NSDate      *creationDate;

/**
 *  Remote url where map package can be downloaded
 */
@property (nonatomic, readonly, nullable) NSString    *remotePath;

/**
 *  Remote url where available maps can be found (maps.json)
 */
@property (nonatomic, readonly, nullable) NSString    *availableRemoteMaps;

/**
 *  Local path of installed map package
 */
@property (nonatomic, readonly, nullable) NSString    *installedPath;

/**
 *  Map resources provider
 */
@property (nonatomic, readonly, nonnull) MapResources *mapResources;

/**
 *  Yes if map packege is live, No if it's test version
 */
@property (nonatomic, readonly) BOOL        isLive;

/**
 *  Yes is map package already installed
 */
@property (nonatomic, readonly) BOOL        isInstalled;

/**
 *  Yes is scene wasn't created
 */
@property (nonatomic, readonly) BOOL        shouldCreateScene;

/**
 * @name  Init
 */

/**
 *  Create DeepMap instance based on input parameters
 *
 *  @param mapDict      Dictionary that provides NSStrings for folowing keys: current_map_file project_name current_map_comment current_map_updated
 *  @param customerName Customer name
 *  @param isLive       No if map is test version
 *
 *  @return DeepMap instance
 */
- (nonnull instancetype)initWithDictionary:(NSDictionary* _Nonnull)mapDict customerName:(NSString* _Nonnull)customerName live:(BOOL)isLive;

/**
 *  Convenient contructor. Create DeepMap instance based on sourcePath/map.json
 *
 *  @param sourcePath Path to folder where map.json is located
 *
 *  @return DeepMap instance
 */
+ (nullable instancetype)mapWithPath:(NSString* _Nonnull)sourcePath;

/**
 *  Convenient contructor. Create DeepMap instance based on sourcePath file (maps.json)
 *
 *  @param sourcePath Path to folder where map.json is located
 *
 *  @return DeepMap instance
 */
+ (nullable instancetype)mapWithFile:(NSString* _Nonnull)sourcePath;

/**
 *  Convenient contructor. Create DeepMap instance based on sourcePath file (map.json)
 *
 *  @param sourcePath Path to folder where map.json is located
 *  @param projectName Required project name
 *
 *  @return DeepMap instance
 *
 */
+ (nullable instancetype)mapWithFile:(NSString* _Nonnull)sourcePath projectName:(NSString* _Nullable)projectName;

/**
 *  Convenient contructor. Create DeepMap instance based on zipped map package
 *
 *  @param zipPath Path to  zipped map package
 *
 *  @return DeepMap instance
 */
+ (nullable instancetype)mapWithPackage:(NSString* _Nonnull)zipPath;

/**
 *  Convenient contructor. Create DeepMap instance based on unzipped map package
 *
 *  @param sourcePath Path to unzipped map package
 *
 *  @return DeepMap instance
 */
+ (nullable instancetype)mapWithPackageInfo:(NSString* _Nonnull)sourcePath;

/**
 * @name  Tasks
 */
/**
 Adds the map to the local repository.
 */
- (BOOL)installMap;

/**
 Removes the map from the local repository.
 */
- (BOOL)uninstallMap;

/**
 *  Compares if two map packages are equal (same creation date, customer, project and live/test)
 *
 *  @param object The map package to compare with the receiver.
 *
 *  @return Yes if the object is an DeepMap object and is exactly equal to the receiver, otherwise NO.
 */
- (BOOL)isEqual:(id _Nullable)object;

/**
 *  Compares if two map packages provides data for the same project (customer, project and live/test are equal)
 *
 *  @param object The map package to compare with the receiver.
 *
 *  @return Yes if the object is an DeepMap object and customer/project/live values are equal to the receiver, otherwise NO.
 */
- (BOOL)isMapSame:(id _Nonnull)object;

/**
 Set's the generated scene's map creation date to the current map data date.
 */
- (void)setSceneUpdated;

/**
 *   Clones all values from another map instance. Used e.g. to transform a fetched map that was not downloaded into a full map instance after download.
 *
 *  @param map source DeepMap instance
 */
-(void)clone:(DeepMap* _Nonnull)map;

/**
 *   Loads a default DeepMap.zip from the app's bundle.
 *
 *  @return DeepMap instance with default data
 */
+ (nullable instancetype)defaultMap;

/**
 *  Curently installed maps.
 *
 *  @return Array of curently installed maps.
 */
+ (nonnull NSArray<DeepMap*>*)installedMaps;

/**
 Removes all maps form the local repository. Use with care, e.g. clear the MapView instance
 before removing currently used files. 
 */
+ (void)cleanup;

/**
 *  Compare date of map creations.
 *
 *  @param otherMap The DeepMap object to compare to the data object’s value.
 *
 *  @return If:
 *  The receiver and anotherDate are exactly equal to each other, NSOrderedSame
 *  The receiver is later in time than anotherDate, NSOrderedDescending
 *  The receiver is earlier in time than anotherDate, NSOrderedAscending..
 */
- (NSComparisonResult)compareMapDate:(DeepMap* _Nonnull)otherMap;


/**
 *  Check if update for current map is available
 *
 *  @param completion Callback function
 */
- (void)checkForMapUpdate:(nullable void (^)(BOOL isUpdateAvailable, NSError* _Nullable error))completion;


/**
 *  Get last available map update for current map;
 *
 *  @param completion Callback function
 */
- (void)getMapUpdate:(nullable void (^)(DeepMap* _Nonnull map, NSError* _Nullable error))completion;

@end

@interface DeepMapProject : NSObject

/**
 *  Customer of map package
 */
@property (nonatomic, copy, nullable) NSString *customerName;

/**
 *  Project of map package
 */
@property (nonatomic, copy, nullable) NSString    *projectName;

/**
 *  Date of map package creation
 */
@property (nonatomic, retain, nullable) NSDate      *lastUpdate;

/**
 *  Yes if map packege is live, No if it's test version
 */
@property (nonatomic, assign) BOOL        isLive;

@end
