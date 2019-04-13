#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    shaking = false;
    nodding = false;

    myVideoGrabber.initGrabber(640, 480);
    if(!face_cascade.load("/Users/frederikcalsius/Documents/OpenFrameworks/of_v20181102_osx_release/apps/myApps/A3_Question3/bin/data/haarcascade_frontalface_default.xml")){
        cout<<"Error loading face cascade"<<endl;
    }
}

void ofApp::initPointsToTrack(cv::Size subPixWinSize, TermCriteria termcrit){
    
    face_cascade.detectMultiScale(matCamGrey, faces, 1.1, 2, 0 | CV_HAAR_SCALE_IMAGE, cv::Size(80, 80));
    
    cv::Rect face = faces.front();
    facePoints.emplace_back(face.x, face.y);
    facePoints.emplace_back(face.x + face.width, face.y);
    facePoints.emplace_back(face.x + face.width, face.y + face.height);
    facePoints.emplace_back(face.x, face.y + face.height);
    
    vector<cv::Point2f> faceFrame;
    goodFeaturesToTrack(matCamGrey, faceFrame, MAX_COUNT, 0.01, 10, cv::Mat(), 3, 0, 0.04);
    cornerSubPix(matCamGrey, faceFrame, subPixWinSize, cv::Size(-1, -1), termcrit);
    
    if (!faceFrame.empty()) {
        std::vector<cv::Point2f>::iterator it = faceFrame.begin();
        int added = 0;
        for (;it != faceFrame.end(); it++)
        {
            int x = it->x;
            int y = it->y;
            
            // Check if points are inside the face and add it
            if (x < facePoints[1].x && x > facePoints[0].x
                && y < facePoints[2].y && y > facePoints[1].y)
            {
                curPoints.emplace_back(it->x, it->y);
                added++;
            }
        }
    }
}

void ofApp::trackKeyPoints(cv::Size winSize){
    
    vector<uchar> status;
    vector<float> err;
    if(matCamPreGrey.empty()){
        matCamGrey.copyTo(matCamPreGrey);
    }
    calcOpticalFlowPyrLK(matCamPreGrey, matCamGrey, prePoints, curPoints, status, err, winSize);
}

void ofApp::getDirections(){
    for(int i = 0; i < curPoints.size(); i++){
        Point2f dir = Point2f(curPoints.at(i).x-prePoints.at(i).x,curPoints.at(i).y-prePoints.at(i).y);
        directions.push_back(dir);
    }
}

Point2f ofApp::getAverageDirection(){
    Point2f averageDir(0,0);
    for(int i = 0; i < directions.size(); i++){
        averageDir.x += directions.at(i).x;
        averageDir.y += directions.at(i).y;
    }
    if(directions.size()!=0){
        averageDir.x /= directions.size();
        averageDir.y /= directions.size();
    }
    return averageDir;
}


//--------------------------------------------------------------
void ofApp::update(){
    myVideoGrabber.update();
    
    TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
    cv::Size subPixWinSize(10, 10),winSize(31,31);
    
    
    // If the grabber indeed has fresh data,
    if (myVideoGrabber.isFrameNew()) {
        // Obtain a pointer to the grabber's image data.
        imgCam.setFromPixels(myVideoGrabber.getPixels());
        matCam = toCv(imgCam);
        cvtColor(matCam, matCamGrey, CV_BGR2GRAY);
        
        
        if(needToInit){
            initPointsToTrack(subPixWinSize, termcrit);
            needToInit = false;
        }
        else if(!prePoints.empty()){
            trackKeyPoints(winSize);
            getDirections();
            Point2f averageDir = getAverageDirection();
            
            //the threshold needs to be adjusted
            if(averageDir.x > 0.1 || averageDir.x < -0.1){
                shaking = true;
                nodding = false;
            }
            else if(averageDir.y > 0.05 || averageDir.y < -0.05){
                nodding = true;
                shaking = false;
            }
            else{
                nodding = false;
                shaking = false;
            }
            
        }
        

        
        std::swap(curPoints, prePoints);
        cv::swap(matCamPreGrey, matCamGrey);
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofSetColor(255,255,255);
    drawMat(matCam, 0, 0);
    ofDrawRectangle(0, 0, 200, 30);
    ofSetColor(255, 0, 0);
    
    if(shaking == true){
        ofDrawBitmapString("Shaking", 100, 15);
    }
    if(nodding == true){
        ofDrawBitmapString("Nodding", 100, 15);
    }
    if(shaking == false && nodding == false){
        ofDrawBitmapString("Still", 100, 15);
    }
    
    ofSetColor(255,255,255);
    
    if(!prePoints.empty()){
        for(int i = 0; i < curPoints.size(); i++ )
        {
            ofSetColor(0,255,0);
            ofDrawCircle(curPoints[i].x, curPoints[i].y, 3);
            ofSetColor(255,0,0);
            ofDrawLine(prePoints[i].x, prePoints[i].y, curPoints[i].x, curPoints[i].y);
        }
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    switch(key){
            case 'c':
                cout<<"clear points"<<endl;
                prePoints.clear();
                curPoints.clear();
                facePoints.clear();
                break;
            case 'r':
                cout<<"re-initialize"<<endl;
                needToInit = true;
                break;
    }

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
