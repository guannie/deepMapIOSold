//
//  HDMMapDownloader.h
//  HDMMapCoreDemo
//
//  Created by Mikalai Lasitsa on 9/15/15.
//  Copyright (c) 2015 Heidelberg mobil International GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "DeepMap.h"

#define kHDMUpdateBaseUrl @"https://update.deepmap.de/map/v2"

/**
 *  Map Data update helper
 */
@interface HDMMapDownloader : NSObject

/** @name Properties **/
/**
*  Customer name
*/
@property (nonatomic, readonly) NSString *customerName;

/**
 *  Project name
 */
@property (nonatomic, copy) NSString *projectName;

/**
 *   Update server url
 */
@property (nonatomic, readonly) NSString *baseUrl;

/**
 *   No if downloading map is test version
 */
@property (nonatomic, readonly) BOOL isLive;

/** @name  Creating **/
/**
 *   Initialize without a project name to fetch all map projects of the specified customer.
 *
 *  @param baseUrl      Update server url
 *  @param customerName customer name
 *  @param isLive       No if downloading map is test version
 *
 *  @return HDMMapDownloader instance
 */
- (instancetype)initWithBaseUrl:(NSString *)baseUrl customerName:(NSString *)customerName live:(BOOL)isLive;

/**
 *  Initialize with a project name only fetch and check updates for this particlular map project
 *
 *  @param baseUrl      Update server url
 *  @param customerName Customer name
 *  @param projectName  Project name
 *  @param isLive       No if downloading map is test version
 *
 *  @return HDMMapDownloader instance
 *  @discussion update will fecth only projectName for customerName
 */
- (instancetype)initWithBaseUrl:(NSString *)baseUrl customerName:(NSString *)customerName projectName:(NSString*)projectName live:(BOOL)isLive;

/** @name  Tasks **/
/**
 *   Downloads a list of available maps for current customer.
 *
 *  @param block completion block call after finishing downloading processes
 *  @discussion Works asynchronously, i.e. doesn't lock main thread
 */
- (void)fetchAvailableMaps:(void (^)(NSArray<NSArray<DeepMap*>*> *availableMaps, NSError *error))block;

/**
 *  Checks if there is a new version for a particular map.
 *
 *  @param map   map that will be checked for update
 *  @param block completion block call after finishing checking processes
 *  @discussion Works asynchronously, i.e. doesn't lock main thread
 */
-(void)checkForUpdate:(DeepMap*)map completion:(void (^)(DeepMap *newMap))block;

/**
 *  Checks if updates are available for installed maps.
 *
 *  @param block completion block call after finishing checking processes
 *  @discussion Works asynchronously, i.e. doesn't lock main thread
 */
- (void)checkForUpdates:(void (^)(NSArray<DeepMap*> *availableMaps, NSError *error))block;

/**
 *  Downloads and installs a map.
 *
 *  @param map   map for downloading
 *  @param block completion block call after finishing download&install processes
 *  @discussion Works asynchronously, i.e. doesn't lock main thread
 */
- (void)getInstallMap:(DeepMap*)map completion:(void (^)(BOOL success))block;

/**
 *  Returns true if a map is currently being downloaded. This is a convenience methods, useful for e.g. listing and downloading multiple maps, like it's done in the update demo.
 *
 *  @param map Checking map
 *
 *  @return Yes if specified map is downloading now.
 */
- (BOOL)isMapDowloading:(DeepMap*)map;

/**
 *  Fetch names of all projects of specified customer
 *
 *  @param customerName Customer Name
 *  @param block        Completion block call after finishing fetching processes
 *  @discussion         Base URL for this method is kHDMUpdateBaseUrl.
 */
+ (void)getProjectsForCustomer:(NSString*)customerName completion:(void (^)(NSArray<DeepMapProject*> *projectsList, NSError *error))block;


@end
