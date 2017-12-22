//
//  ViewController.swift
//  DeepMapTestIOS
//
//  Created by Lee Kuan Xin on 05.09.17.
//  Copyright © 2017 Lee Kuan Xin. All rights reserved.
//

import HDMMapCore
import HDMMapCore.HDMAnnotation
class ViewController: HDMMapViewController, HDMMapViewControllerDelegate {
   
    //MARK: DeepMap function
    
    @IBOutlet weak var test: UIView!
    @IBOutlet weak var Food: UIImageView!
    var startPoint : HDMMapCoordinate?
    var endPoint : HDMMapCoordinate?
    
    func mapViewController(_ controller: HDMMapViewController, longPressedAt coordinate: HDMMapCoordinate, features: [HDMFeature]) {
        print("Set routing start point!")
        self.startPoint = coordinate
    }
    
    func mapViewController(_ controller: HDMMapViewController, tappedAt coordinate: HDMMapCoordinate, features: [HDMFeature]) {
        
        self.endPoint = coordinate
        guard let feature = features.first else {return}
        print("Selecting object with ID \(feature.featureId)")
        
        // tell the map to select the object that has been touched
        self.mapView.selectFeature(withId: feature.featureId)
        
        //remove all previously added annotations
        self.mapView.remove(self.mapView.annotations)
        
        //create a new annotation and add it to the map
        let annotation = HDMAnnotation(coordinate: coordinate)
        annotation.title = "ObjectID \(feature.featureId)"
        annotation.subtitle = "This is subtitle"
        annotation.leftCalloutAccessoryView = Food
        annotation.rightCalloutAccessoryView = test
        self.mapView.add(annotation)

        
        
        guard let startPoint = self.startPoint else {return}
        //1
        guard let routing = controller.routing else {return}
        //2
        
        guard let route = routing.calculateRoute(from: startPoint, destinationPoint: coordinate) else {return}
        self.mapView.navigate(withPath: route, using: HDMUserTrackingModeNone)
        // HDMUserTrackingMode
        // older sdk
        //self.mapView.navigate(withPath: route, using: HDMUserTrackingModeNone)
        
        //sdk2.0.2pre 1 sdk version and above
        
        // guard let route = routing.calculateRoute(from: startPoint, to: coordinate) else {return}
        //self.mapView.navigate(withPath: route, using: HDMUserTrackingMode(rawValue: 0)!)
        // or
        //self.mapView.navigate(withPath: route, using: HDMUserTrackingMode.none)
    
    }
    override func viewDidLoad() {
        super.viewDidLoad()
        
//        self.mapView.switchStyles(Bundle.main.path(forResource: "custom", ofType: "mapStyle"), ruleFile: Bundle.main.path(forResource: "custom", ofType: "mapRule"));
                self.delegate = self
        
                let switchToggle = UISwitch(frame: CGRect(origin:CGPoint(x:0.0, y:0.0), size:CGSize(width:100.0,height:10.0) ) );
        
                //place switch bottom right
                switchToggle.frame = CGRect(origin: CGPoint(x: self.view.frame.size.width - switchToggle.frame.size.width,
                                                            y: self.view.frame.size.height - switchToggle.frame.size.height),
                                            size:   CGSize(width:switchToggle.frame.size.width, height:switchToggle.frame.size.height));
        
                switchToggle.addTarget(self, action: #selector(test(sender:)), for: .valueChanged)
//                switchToggle.addTarget(self, action: #selector(flip(sender:)), for: .valueChanged);
//                switchToggle.setOn(true, animated: true);
        
        
                self.view.addSubview(switchToggle);
        
        
    }
    
    @objc func test(sender: UISwitch!) {
        if(sender.isOn == false)
        {
            self.mapView.set3DMode(true, animated: true)
        }
        else
        {
            self.mapView.set3DMode(false, animated: true)
        }

    }
    
    func flip(sender: UISwitch!){
        
        if(sender.isOn == true)
        {
            self.mapView.setFeatureStyle("building", propertyName: "fill-color", value: "#0000ff", update: false);
            self.mapView.setFeatureStyle("stand_tables", propertyName: "visibility", value: "visible", update: false);
            self.mapView.setFeatureStyle("stand_tables", propertyName: "text-visibility", value: "visible", update: true);
        }
        else
        {
            self.mapView.setFeatureStyle("building", propertyName: "fill-color", value: "#ff0000", update: false);
            self.mapView.setFeatureStyle("stand_tables", propertyName: "visibility", value: "none", update: false);
            self.mapView.setFeatureStyle("stand_tables", propertyName: "text-visibility", value: "none", update: true);
        }
    }

    
//        override func didReceiveMemoryWarning() {
//            super.didReceiveMemoryWarning()
//            // Dispose of any resources that can be recreated.
//        }
    
    

   
}

