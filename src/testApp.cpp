#include "testApp.h"

//--------------------------------------------------------------
void testApp::setup(){
    ofEnableAlphaBlending(); 
    
    
   if(ass.loadFile("SVG.xml"))
   {
       cout<<"Arena mapping file loaded"<<endl;
       
       string data;
       ofBuffer dataBuffer;
       
       //set header for svg
       data = "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<!-- Generator: Resolume Arena 4.1.6, SVG Export Plug-In)  -->\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org Graphics/SVG/1.1/DTD/svg11.dtd\">\n <svg version=\"1.1\" \n xmlns=\"http://www.w3.org/2000/svg\" xmlns:xlink=\"http://www.w3.org/1999/xlink\" x=\"0px\" y=\"0px\" width=\"1920px\" height=\"1080px\" viewBox=\"0 0 1920 1080\" enable-background=\"new 0 0 1920 1080\" xml:space=\"preserve\">\n";
       
       //get number of slices
       ass.pushTag("preset");
       ass.pushTag("screen");
       ass.pushTag("slices");
       int numSlice = ass.getNumTags("Slice");
       cout << numSlice << endl;
       
       
       //iterate through slices
       for ( int i = 0; i < numSlice; i++ )
       {
           //get slice name and use it as path name
           string name = ass.getAttribute("Slice:name", "value", "Layer "+ofToString(i), i);
           cout<<name<<endl;
           data += "<polygon id=\""+name+"\"";
           
           //get beziermode
           int bezierMode = ass.getAttribute("Slice:warper:bezierMode", "value", 0, i);
           
           //get and write point data
           string slicePoints = ass.getValue("Slice:warper:verts", "nothing found?", i);
           //cout<<slicePoints<<endl;
           
           //convert normalized data to pixel data based on width
           vector<string> pointsIn = ofSplitString(slicePoints, " ");
           vector<int> ps;
           ps.clear();
           vector<string> pointsOut;
           pointsOut.clear();
           
           for(int i = 0; i < pointsIn.size(); i ++)
           {
               ps.push_back(1080*ofToFloat(pointsIn[i]));
               pointsOut.push_back(ofToString(ps[i]));
           }
           
           //to do: figure out which points are the actual corner points
           //for now we are just hardcoding the four corner points
           int w = ass.getAttribute("Slice:warper", "width", 4, i);
           int h = ass.getAttribute("Slice:warper", "height", 4, i);
           
           
           data += " fill=\"none\" stroke=\"#000000\" stroke-miterlimit=\"10\" points=\"";
           data += pointsOut[0]+","+pointsOut[1]+" "+pointsOut[6]+","+pointsOut[7]+" "+pointsOut[30]+","+pointsOut[31]+" "+pointsOut[24]+","+pointsOut[25]+"\"/>\n";
           
           
       }
       ass.popTag();
       ass.popTag();
       ass.popTag();
       
       //close the svg file
       data += "</svg>";
       dataBuffer = data;
       ofBufferToFile("ass.svg", dataBuffer);
   }
    
    //load the output file
    assIO.loadFile("RamBam.xml");
    
    if(!assIO.tagExists("preset"))
    {
        assIO.addTag("preset");
        assIO.addAttribute("preset", "name", "TempTest", 0);
        ofSeedRandom(ofGetSystemTime());
        assIO.addAttribute("preset", "uniquePresetId", ofRandom(9999999), 0);
    }
    
    assIO.pushTag("preset");
    
    if(!assIO.tagExists("versionInfo"))
    {
        assIO.addTag("versionInfo");
        assIO.addAttribute("versionInfo", "name", "Resolume Arena", 0);
        assIO.addAttribute("versionInfo", "majorVersion", 4, 0);
        assIO.addAttribute("versionInfo", "minorVersion", 1, 0);
        assIO.addAttribute("versionInfo", "microVersion", 6, 0);
        assIO.addAttribute("versionInfo", "revision", "5380", 0);
        assIO.addAttribute("versionInfo", "id", 1, 0);
    }
    
    if(!assIO.tagExists("screen"))
    {
        assIO.addTag("screen");
        assIO.addAttribute("screen", "outputDeviceName", "Temp Display", 0);
        assIO.addAttribute("screen", "outputDeviceID", 0, 0 );
        assIO.addAttribute("screen","enabled", 0, 0);
        assIO.addAttribute("screen", "fullscreen", 1, 0);
    }
    
    assIO.pushTag("screen");
    
    if(!assIO.tagExists("name"))
    {
        assIO.addTag("name");
        assIO.addAttribute("name", "value", "Screen 1", 0);
    }
    
    if(!assIO.tagExists("slices"))
        assIO.addTag("slices");
    
    assIO.popTag(); //out of screen
    assIO.popTag(); //out of preset

    


    
    
    
                          

}

//--------------------------------------------------------------
void testApp::update(){

}

//--------------------------------------------------------------
void testApp::draw(){
    ofBackground(32);
    
    ofSetColor(255);
    ofLine(ofGetWidth()/2,0,ofGetWidth()/2,ofGetHeight()/2);
    ofLine(0, ofGetHeight()/2,ofGetWidth(), ofGetHeight()/2);
    
    assIO.pushTag("preset");
    assIO.pushTag("screen");
    assIO.pushTag("slices");
    
    int numberOfSlicesInXml = assIO.getNumTags("Slice");
    ofColor color;
       
    //if we already have slices made
    for(int k = 0; k < numberOfSlicesInXml; k++)
    {
        float colorSteps = 255 / numberOfSlicesInXml;
        color.setHsb(colorSteps*k, 255, 128);
        
        assIO.pushTag("Slice", k);
        //and they have input rects
        if(assIO.tagExists("rect"))
        {
            //get the data and draw it
            float l = assIO.getAttribute("rect", "l", 0.0) * ofGetWidth()/2;
            float t = assIO.getAttribute("rect", "t", 0.0) * ofGetHeight()/2;
            float r = assIO.getAttribute("rect", "r", 1.0) * ofGetWidth()/2;
            float b = assIO.getAttribute("rect", "b", 1.0) * ofGetHeight()/2;
            
            ofFill();
            ofSetColor(color,64);
            ofRect(l, t, r-l, b-t);
            ofNoFill();
            ofSetColor(color);
            ofRect(l, t, r-l, b-t);
        }
        
        if(assIO.tagExists("warper"))
        {
            assIO.pushTag("warper");
            string s = assIO.getValue ("verts", "");
            vector<string> pointsIn = ofSplitString(s, " ");
            vector<int> ps;
            ps.clear();
            for(int i = 0; i < pointsIn.size(); i ++)
            {
                ps.push_back(ofGetHeight()/2*ofToFloat(pointsIn[i]));
            }
            
            float x = ps[0]+ofGetWidth()/2;
            float y = ps[1];
            float w = ps[30] - ps[0];
            float h = ps[31] - ps[1];
            
            ofFill();
            ofSetColor(color,64);
            ofRect(x,y,w,h);
            ofNoFill();
            ofSetColor(color);
            ofRect(x,y,w,h);
            
            assIO.popTag();
            
        }
        assIO.popTag();//out of Slice
    }
    
    assIO.popTag(); //out of slices
    assIO.popTag(); //out of name
    assIO.popTag(); //out of screen
    assIO.popTag(); //out of preset
    
}

//--------------------------------------------------------------
void testApp::keyPressed(int key){
    

    assIO.pushTag("preset");
    assIO.pushTag("screen");
    assIO.pushTag("slices");
    
    if(key == 'i')
    {

        if(svgInputFile.loadFile("inputrects.svg"))
        {
            cout<<"svgInputFile loaded"<<endl;
    
            
            //get the doc width and height       
            string docWidthString = svgInputFile.getAttribute("svg", "width", "640px");
            string docHeightString = svgInputFile.getAttribute("svg", "height", "480px");
            
            //svg files store the resolution with 'px' behind it, so reformat the w & h data to remove the px and convert to int
            ofStringReplace(docWidthString, "px", "");
            docWidth = ofToInt(docWidthString);
            ofStringReplace(docHeightString, "px", "");
            docHeight = ofToInt(docHeightString);
            
                        
            //move into the svg tag
            svgInputFile.pushTag("svg");
            
            //if we are working with layers, scrape each layer, if not , just scrape once
            int numLayers = svgInputFile.getNumTags("g");
            if (numLayers == 0)
                numLayers = 1;
            //cout << numLayers << endl;
            
            for(int i = 0; i < numLayers; i++)
            {
                
                //push into the layer where necessary
                if(svgInputFile.getNumTags("g")>0)
                    svgInputFile.pushTag("g", i);
            
                //get the amount of rects in this layer
                int numRects = svgInputFile.getNumTags("rect");
                //cout<<numRects<<endl;
                
                for ( int j = 0; j < numRects; j++ )
                {
                    //do a check for rotated or otherwise transformed rects
                    if(svgInputFile.attributeExists("rect", "transform", j))
                    {
                        cout<<"Transformed rect skipped!"<<endl;
                        continue;
                    }
                    
                    //get the name of the rect
                    string name = svgInputFile.getAttribute("rect", "id", "Slice "+ofToString(j+1), j);
                    int haveMatch = -1;
                    
                    //get number of slice already present in the xml
                    int numberOfSlicesInXml = assIO.getNumTags("Slice");
                    
                    //if we already have slices made from loading an output file
                    for(int k = 0; k < numberOfSlicesInXml; k++)
                    {
                        //pop into the current slice
                        assIO.pushTag("Slice", k);
                        
                        //if names of the input rects and the current slice math
                        
                        if(name == assIO.getAttribute("name", "value", ""))
                        {
                            //setInputData(j);
                            haveMatch = k;
                        }
                        //pop out of the current slice
                        assIO.popTag();
                        
                    }
                    
                    
                    //to do: check this, it feels kind of hacky... 
                    if( haveMatch < 0 )
                    {
                        int sliceCount = numberOfSlicesInXml;
                        //add a slice tag
                        assIO.addTag("Slice");
                        assIO.addAttribute("Slice", "isEnabled", 1, sliceCount);
                        
                        //move in to the slice tag
                        assIO.pushTag("Slice", sliceCount);
                        
                        //write name
                        assIO.addTag("name");
                        assIO.addAttribute("name", "value", name, 0);
                        
                        setInputData(j);
                        
                        //move out of the slice tag
                        assIO.popTag();
                    
                    }
                    else
                    {
                        assIO.pushTag("Slice", haveMatch);
                        setInputData(j);
                        assIO.popTag();
                    }
                }
                
                //pop out of the current layer
                svgInputFile.popTag();
            }
            
        }
        else
        {
            cout<<"Input file loading failed"<<endl;
        }
        
    }
    
    if(key == 'o')
    {
        if(svgOutputFile.loadFile("outputrects.svg"))
        {
            cout<<"svgOutputFile loaded"<<endl;
            
            //get the doc width and height       
            string docWidthString = svgOutputFile.getAttribute("svg", "width", "640px");
            string docHeightString = svgOutputFile.getAttribute("svg", "height", "480px");
            
            //svg files store the resolution with 'px' behind it, so reformat the w & h data to remove the px and convert to int
            ofStringReplace(docWidthString, "px", "");
            docWidth = ofToInt(docWidthString);
            ofStringReplace(docHeightString, "px", "");
            docHeight = ofToInt(docHeightString);
            
            
            //move into the svg tag
            svgOutputFile.pushTag("svg");
            
            //if we are working with layers, scrape each layer, if not , just scrape once
            int numLayers = svgOutputFile.getNumTags("g");
            if (numLayers == 0)
                numLayers = 1;
            
            for(int i = 0; i < numLayers; i++)
            {
                
                //push into the layer where necessary
                if(svgOutputFile.getNumTags("g")>0)
                    svgOutputFile.pushTag("g", i);
                
                //get the amount of rects in this layer
                int numRects = svgOutputFile.getNumTags("rect");
                
                for ( int j = 0; j < numRects; j++ )
                {
                    //do a check for rotated or otherwise transformed rects
                    if(svgOutputFile.attributeExists("rect", "transform", j))
                    {
                        cout<<"Transformed rect skipped!"<<endl;
                        continue;
                    }
                    
                    //get the name of the rect
                    string name = svgOutputFile.getAttribute("rect", "id", "Slice "+ofToString(j+1), j);
                    int haveMatch = -1;
                    
                    //get number of slice already present in the xml
                    int numberOfSlicesInXml = assIO.getNumTags("Slice");
                    
                    //if we already have slices made from loading an output file
                    for(int k = 0; k < numberOfSlicesInXml; k++)
                    {
                        //pop into the current slice
                        assIO.pushTag("Slice", k);
                        
                        //if names of the input rects and the current slice math
                        
                        if(name == assIO.getAttribute("name", "value", ""))
                        {
                            haveMatch = k;
                        }
                        //pop out of the current slice
                        assIO.popTag();
                        
                    }
                    
                    
                    //to do: check this, it feels kind of hacky... 
                    if( haveMatch < 0 )
                    {
                        int sliceCount = numberOfSlicesInXml;
                        //add a slice tag
                        assIO.addTag("Slice");
                        assIO.addAttribute("Slice", "isEnabled", 1, sliceCount);
                        
                        //move in to the slice tag
                        assIO.pushTag("Slice", sliceCount);
                        
                        //write name
                        assIO.addTag("name");
                        assIO.addAttribute("name", "value", name, 0);
                        
                        setOutputData(j);
                        
                        //move out of the slice tag
                        assIO.popTag();
                        
                    }
                    else
                    {
                        assIO.pushTag("Slice", haveMatch);
                        setOutputData(j);
                        assIO.popTag();
                    }
                }
                
                //pop out of the current layer
                svgOutputFile.popTag();
            }
            
        }
        else
        {
            cout<<"No output file loaded!"<<endl;
        }
        
    }
    
    
    //if we're completely done with slices
    assIO.popTag(); //out of slices
    assIO.popTag(); //out of screen
    assIO.popTag(); //out of preset
    
    

}

void testApp::setOutputData(int j)
{
    //write input data
    //get the input rectangle data
    float x1 = svgOutputFile.getAttribute("rect", "x", 0, j);
    float y1 = svgOutputFile.getAttribute("rect", "y", 0, j);
    float x2 = x1 + svgOutputFile.getAttribute("rect", "width", 0, j);
    float y2 = y1 + svgOutputFile.getAttribute("rect", "height", 0, j);
    
    //normalize based on height of the document, this is how the ASS works as well
    x1 = x1 / docHeight;
    y1 = y1 / docHeight;
    x2 = x2 / docHeight;
    y2 = y2 / docHeight;
    
    //add output warper data
    if(!assIO.tagExists("warper"))
        assIO.addTag("warper");
    assIO.addAttribute("warper", "width", 4, 0);
    assIO.addAttribute("warper", "height", 4, 0);
    assIO.addAttribute("warper", "angle", 0, 0);
    assIO.addAttribute("warper", "subdivision", 21, 0);
    
    assIO.pushTag("warper");
    
    assIO.addTag("verts");
    string vertData;
    for(int y = 0; y < 4; y++)
    {
        for(int x = 0; x < 4; x++)
        {
            float xPos = x1 + (x2 - x1) / 3 * x;
            float yPos = y1 + (y2 - y1) / 3 * y;
            vertData+=ofToString(xPos)+" "+ofToString(yPos)+" ";
            
        }
    }
    assIO.setValue("verts", vertData);
    
    assIO.popTag(); //out of warper
    
}

void testApp::setInputData(int j)
{
    //write output data
    //get the input rectangle data
    float x = svgInputFile.getAttribute("rect", "x", 0, j);
    float y = svgInputFile.getAttribute("rect", "y", 0, j);
    float w = x + svgInputFile.getAttribute("rect", "width", 0, j);
    float h = y + svgInputFile.getAttribute("rect", "height", 0, j);
    
    //normalize
    x = x / docWidth;
    y = y / docHeight;
    w = w / docWidth;
    h = h / docHeight;
    
    if(!assIO.tagExists("rect"))
        assIO.addTag("rect");
    assIO.addAttribute("rect", "l", x, 0);
    assIO.addAttribute("rect", "t", y, 0);
    assIO.addAttribute("rect", "r", w, 0);
    assIO.addAttribute("rect", "b", h, 0);
    
}

void testApp::openInput()
{
    ofFileDialogResult inputFile = ofSystemLoadDialog("Select file for Input Selection");
    string fileName = inputFile.getName();
    if(fileName != "")
    {
        if(svgInputFile.loadFile(inputFile.getPath()))
        {
            cout << "loaded!" <<endl;
            
            //get the doc width and height       
            string docWidthString = svgInputFile.getAttribute("svg", "width", "640px");
            string docHeightString = svgInputFile.getAttribute("svg", "height", "480px");
            
            //svg files store the resolution with 'px' behind it, so reformat the w & h data to remove the px and convert to int
            ofStringReplace(docWidthString, "px", "");
            docWidth = ofToInt(docWidthString);
            ofStringReplace(docHeightString, "px", "");
            docHeight = ofToInt(docHeightString);
            
            
            //move into the svg tag
            svgInputFile.pushTag("svg");
            
            //if we are working with layers, scrape each layer, if not , just scrape once
            int numLayers = svgInputFile.getNumTags("g");
            if (numLayers == 0)
                numLayers = 1;
            //cout << numLayers << endl;
            
            for(int i = 0; i < numLayers; i++)
            {
                
                //push into the layer where necessary
                if(svgInputFile.getNumTags("g")>0)
                    svgInputFile.pushTag("g", i);
                
                //get the amount of rects in this layer
                int numRects = svgInputFile.getNumTags("rect");
                //cout<<numRects<<endl;
                
                for ( int j = 0; j < numRects; j++ )
                {
                    //do a check for rotated or otherwise transformed rects
                    if(svgInputFile.attributeExists("rect", "transform", j))
                    {
                        cout<<"Transformed rect skipped!"<<endl;
                        continue;
                    }
                    
                    //get the name of the rect
                    string name = svgInputFile.getAttribute("rect", "id", "Slice "+ofToString(j+1), j);
                    int haveMatch = -1;
                    
                    //get number of slice already present in the xml
                    int numberOfSlicesInXml = assIO.getNumTags("Slice");
                    
                    //if we already have slices made from loading an output file
                    for(int k = 0; k < numberOfSlicesInXml; k++)
                    {
                        //pop into the current slice
                        assIO.pushTag("Slice", k);
                        
                        //if names of the input rects and the current slice math
                        
                        if(name == assIO.getAttribute("name", "value", ""))
                        {
                            //setInputData(j);
                            haveMatch = k;
                        }
                        //pop out of the current slice
                        assIO.popTag();
                        
                    }
                    
                    
                    //to do: check this, it feels kind of hacky... 
                    if( haveMatch < 0 )
                    {
                        int sliceCount = numberOfSlicesInXml;
                        //add a slice tag
                        assIO.addTag("Slice");
                        assIO.addAttribute("Slice", "isEnabled", 1, sliceCount);
                        
                        //move in to the slice tag
                        assIO.pushTag("Slice", sliceCount);
                        
                        //write name
                        assIO.addTag("name");
                        assIO.addAttribute("name", "value", name, 0);
                        
                        setInputData(j);
                        
                        //move out of the slice tag
                        assIO.popTag();
                        
                    }
                    else
                    {
                        assIO.pushTag("Slice", haveMatch);
                        setInputData(j);
                        assIO.popTag();
                    }
                }
                
                //pop out of the current layer
                svgInputFile.popTag();
            }

        }
        else
        {
            ofSystemAlertDialog("This is not a valid SVG file...");
        }
    }
        
}

void testApp::openOutput()
{
    ofFileDialogResult outputFile = ofSystemLoadDialog("Select file for Output Transformation");
    string fileName = outputFile.getName();
    if(fileName != "")
    {
        if(svgOutputFile.loadFile(outputFile.getPath()))
        {
            cout<<"svgOutputFile loaded"<<endl;
            
            //get the doc width and height       
            string docWidthString = svgOutputFile.getAttribute("svg", "width", "640px");
            string docHeightString = svgOutputFile.getAttribute("svg", "height", "480px");
            
            //svg files store the resolution with 'px' behind it, so reformat the w & h data to remove the px and convert to int
            ofStringReplace(docWidthString, "px", "");
            docWidth = ofToInt(docWidthString);
            ofStringReplace(docHeightString, "px", "");
            docHeight = ofToInt(docHeightString);
            
            //store the default resolution values in the xml
            assIO.popTag(); //pop out of slices into screen
            if(!assIO.tagExists("defaultWidth"))
                assIO.addTag("defaultWidth");
            if(!assIO.tagExists("defaultHeight"))
                assIO.addTag("defaultHeight");
            assIO.addAttribute("defaultWidth", "value" , docWidth, 0);
            assIO.addAttribute("defaultHeight", "value" , docHeight, 0);
            assIO.pushTag("slices"); //push back up to slices
            
            //move into the svg tag
            svgOutputFile.pushTag("svg");
            
            //if we are working with layers, scrape each layer, if not , just scrape once
            int numLayers = svgOutputFile.getNumTags("g");
            if (numLayers == 0)
                numLayers = 1;
            
            for(int i = 0; i < numLayers; i++)
            {
                
                //push into the layer where necessary
                if(svgOutputFile.getNumTags("g")>0)
                    svgOutputFile.pushTag("g", i);
                
                //get the amount of rects in this layer
                int numRects = svgOutputFile.getNumTags("rect");
                
                for ( int j = 0; j < numRects; j++ )
                {
                    //do a check for rotated or otherwise transformed rects
                    if(svgOutputFile.attributeExists("rect", "transform", j))
                    {
                        cout<<"Transformed rect skipped!"<<endl;
                        continue;
                    }
                    
                    //get the name of the rect
                    string name = svgOutputFile.getAttribute("rect", "id", "Slice "+ofToString(j+1), j);
                    int haveMatch = -1;
                    
                    //get number of slice already present in the xml
                    int numberOfSlicesInXml = assIO.getNumTags("Slice");
                    
                    //if we already have slices made from loading an output file
                    for(int k = 0; k < numberOfSlicesInXml; k++)
                    {
                        //pop into the current slice
                        assIO.pushTag("Slice", k);
                        
                        //if names of the input rects and the current slice math
                        
                        if(name == assIO.getAttribute("name", "value", ""))
                        {
                            haveMatch = k;
                        }
                        //pop out of the current slice
                        assIO.popTag();
                        
                    }
                    
                    
                    //to do: check this, it feels kind of hacky... 
                    if( haveMatch < 0 )
                    {
                        int sliceCount = numberOfSlicesInXml;
                        //add a slice tag
                        assIO.addTag("Slice");
                        assIO.addAttribute("Slice", "isEnabled", 1, sliceCount);
                        
                        //move in to the slice tag
                        assIO.pushTag("Slice", sliceCount);
                        
                        //write name
                        assIO.addTag("name");
                        assIO.addAttribute("name", "value", name, 0);
                        
                        setOutputData(j);
                        
                        //move out of the slice tag
                        assIO.popTag();
                        
                    }
                    else
                    {
                        assIO.pushTag("Slice", haveMatch);
                        setOutputData(j);
                        assIO.popTag();
                    }
                }
                
                //pop out of the current layer
                svgOutputFile.popTag();
            }

        }
        
        else
        {
            ofSystemAlertDialog("This is not a valid SVG file...");
        }
    }

}

void testApp::saveXML()
{
    ofFileDialogResult saveFile = ofSystemSaveDialog("ASS.xml", "Save as...");
    string fileName = saveFile.getName();
    if(fileName != "")
    {
        assIO.setAttribute("preset", "name", saveFile.getName(),0);
        assIO.saveFile(saveFile.getPath()); //and save the file
    }
}

//--------------------------------------------------------------
void testApp::keyReleased(int key){

}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y){

}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button){
    
    assIO.pushTag("preset");
    assIO.pushTag("screen");
    assIO.pushTag("slices");
    
    if(x < ofGetWidth() / 2 && y < ofGetHeight() / 2 )
        openInput();
    
    if(x > ofGetWidth() / 2 && y < ofGetHeight() / 2 )
        openOutput();
    

    
    //if we're completely done with slices
    assIO.popTag(); //out of slices
    assIO.popTag(); //out of screen
    assIO.popTag(); //out of preset
    
    if(y > ofGetHeight() / 2)
        saveXML();

}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void testApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void testApp::dragEvent(ofDragInfo dragInfo){ 

}