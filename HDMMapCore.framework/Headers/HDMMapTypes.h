//
//  HDMMapTypes.h
//  HDMMapCore
//
//  Copyright (c) 2017 Heidelberg mobil International GmbH. All rights reserved.
//

#ifndef HDMMapCore_HDMMapTypes_h
#define HDMMapCore_HDMMapTypes_h

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*!
 
 @typedef HDMMapEasingData
 @brief User data for easing functions
 
 */
typedef void* HDMMapEasingData;

/*!
 
 @typedef HDMMapEasingFunction1f
 @brief Function that describes the value of a property given a percentage of completeness.
 
 @param time        Current time in a range [0, duration]
 @param source      Start value
 @param change      Change in value (eg. destination - source)
 @param duration    Duration
 @param data        Any user provided data
 
 */
typedef float (*HDMMapEasingFunction1f)(double time, float source, float change, double duration, HDMMapEasingData data);

/*!
 
 @typedef HDMMapCoordinate
 @brief A structure that contains a point in a three-dimensional coordinate system.
 
 @field x Point on x-axis.
 @field y Point on y-axis.
 @field z Point on z-axis.
 
 */
typedef struct {
    double x;
    double y;
    double z;
} HDMMapCoordinate;

/*!
 
 @function HDMMapCoordinateMake
 @brief Returns a point with the specified coordinates.
 
 @param x The x-coordinate of the point to construct.
 @param y The y-coordinate of the point to construct.
 @param z The z-coordinate of the point to construct.
 
 */
extern HDMMapCoordinate HDMMapCoordinateMake(double x, double y, double z);
  
extern const HDMMapCoordinate HDMMapCoordinateZero;

typedef struct {
    HDMMapCoordinate point1;
    HDMMapCoordinate point2;
    HDMMapCoordinate point3;
    HDMMapCoordinate point4;
} HDMMapRegion;
    
extern HDMMapRegion HDMMapRegionMake(double point1X, double point1Y, double point1Z,
                                                         double point2X, double point2Y, double point2Z,
                                                         double point3X, double point3Y, double point3Z,
                                                         double point4X, double point4Y, double point4Z);

typedef struct{
    double width;
    double height;
} HDMMapSize;

extern HDMMapSize HDMMapSizeMake(double width, double height);
    
typedef struct{
    HDMMapCoordinate origin;
    HDMMapSize size;
} HDMMapRect;

extern HDMMapRect HDMMapRectMake(double x, double y, double z, double width, double height);

/*!
 
 @typedef HDMMapCoordinateRegion
 @brief A structure that defines a region of coordinates.
 
 @field center coordinate of the center of the region.
 @field span The size of the region given in the span of latitude and longitude.

 */
typedef struct{
    HDMMapCoordinate center;
    struct HDMMapCoordinateSpan{
        double latitudeDelta;
        double longitudeDelta;
    } span;
} HDMMapCoordinateRegion;

/*!
 @brief Returns a HDMMapCoordinateRegion at the given center point with given coordinate-span.
 
 @param x The x-coordinate of the center.
 @param y The y-coordinate of the center.
 @param z The z-coordinate of the center.
 @param latitudeDelta The size of the region in direction of latitude.
 @param longitudeDelta The size of the region in direction of logitude.
 */
extern HDMMapCoordinateRegion HDMMapCoordinateRegionMake(double x, double y, double z, double latitudeDelta, double longitudeDelta);
    
#ifdef __cplusplus
}
#endif

#endif
