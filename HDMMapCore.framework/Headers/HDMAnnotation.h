//
//  HDMAnnotation.h
//  HDMMapCoreDemo
//
//  Created by Mikalai Lasitsa on 29.10.15.
//  Copyright (c) 2015 Heidelberg mobil International GmbH. All rights reserved.
//

#import <Foundation/Foundation.h>
#import <HDMMapCore/HDMMapTypes.h>
#import <UIKit/UIKit.h>

@class HDMAnnotation;

/**
 * The HDMAnnotationDelegate protocol defines a set of optional methods that you can use to receive annotation-related update messages
 */
@protocol HDMAnnotationDelegate <NSObject>

@optional
/**
 @brief Method called if annotation view was tapped.
 @param annotation Annotation was tapped
 */
-(void)annotationTapped:(HDMAnnotation * _Nonnull)annotation;
-(void)leftAnnotationAccessoryTapped:(HDMAnnotation * _Nonnull)annotation;
-(void)rightAnnotationAccessoryTapped:(HDMAnnotation * _Nonnull)annotation;

@end

/**
 The HDMAnnotation class defines a annotation object located at a specified point. You can use this class for showing title, subtitle and two images.
 */
@interface HDMAnnotation : NSObject

/**
 *  @name Properties
 */

/**
 @brief The coordinate of the annotation.
 @discussion Returns coordinates of annotation in world coordinates (WGS84)
 */
@property (nonatomic, readonly) HDMMapCoordinate coordinate;

/**
 @brief Read-only selfgenerated annotation view.
 @discussion Generated from title & subtitle, left & right accessory views if available. Do not recommended use it for adding extra subviews.
 */
@property (nonatomic, readonly, nullable) UIView *anotationView;

/**
 @brief Custom view for annotation can be set by passing new view to this property.
 @discussion By default passed custom view is clipped by default annotation border, which can be turn off by passing YES to defaultBorderHidden property. Normalazed ancor point for view is (0.5,1)
 */
@property (strong, nonatomic, nullable) UIView *contentView;

/**
 @brief By default annotation view is clipped by annotation border, in case another layout is required border can be turn off by passing YES.
 @discussion For hidding default border value YES should be passed BEFORE adding annotation to the map.
 */
@property (assign, nonatomic) BOOL defaultBorderHidden;

/*!
 @brief Annotation title.
 @see subtitle
 */
@property (nonatomic, copy, nullable) NSString *title;

/**
 @brief Annotation subtitle
 @see title
 */
@property (nonatomic, copy, nullable) NSString *subtitle;

/**
 @brief Left Accessory view
 @discussion Can be used for dispaying extra annotation icons. size is 30x30px
 */
@property (nonatomic, strong, nonnull) UIView *leftCalloutAccessoryView;

/**
 @brief Right Accessory view
 @discussion Can be used for dispaying extra annotation icons like size is 30x30px
 */
@property (nonatomic, strong, nonnull) UIView *rightCalloutAccessoryView;

/**
 @brief Annotation Delegate
 @discussion See HDMAnnotationDelegate.
 */
@property (nonatomic, assign, nonnull) id<HDMAnnotationDelegate> delegate;

/**
 *  @name Creating
 */
/*!
 @brief Contructor
 @param coordinate The center point (specified as a UTM map coordinate) of the annotation

 @discussion Returns an initialized HDMAnnotation object that located in specified coordinate

     HDMAnnotation *annotation = [[HDMAnnotation alloc] initWithCoordinate:_tapPoint];
     annotation.title = @"test annotation"
     [self.mapViewController.mapView addAnnotation:annotation];
 */
- (instancetype _Nonnull)initWithCoordinate:(HDMMapCoordinate)coordinate;


/**
 Change coordinate of exist annotation

 @param coordinate New anotation's coordinate
 @param animated If YES, animate annotation moving to required coordinate.
 */
- (void)updateCoordinate:(HDMMapCoordinate)coordinate animated:(BOOL)animated;

@end

/*!
 The HDMPinAnnotation class defines a annotation Pin object located at a specified point.
 */
@interface HDMPinAnnotation : HDMAnnotation

/*!
 @brief YES if want to show callout by tap to pin
 @discussion Default value is NO
 */
@property (nonatomic, assign) BOOL canShowCallout;

/*!
 @brief Set color for head of pin.
 @discussion Default value is Red
 */
@property (nonatomic, retain, nonnull) UIColor *pinColor;

@end

@interface HDMUserLocationAnnotation : HDMAnnotation

/*!
 @brief Set color for user location.
 @discussion Default value is Blue
 */
@property (nonatomic, retain, nonnull) UIColor *tintColor;

/*!
 @brief Set accuracy radius
 */
@property (nonatomic, assign) float accuracyRadius;

@end



