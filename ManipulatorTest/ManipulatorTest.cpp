// ManipulatorTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <osgViewer\Viewer>
#include <osgDB\ReadFile>
#include <EarthManipulator.h>
#include <osgEarth/MapNode>
#include <osgEarthUtil/SkyNode>
#include <osgga/TrackballManipulator>
#include "FPSManipulator.h"
#include <iostream>
#include <fstream>
#include <osgViewer/ViewerEventHandlers>
using namespace std;
using namespace osgEarth;
using namespace osgEarth::Util;
int main(int argc, char** argv)
{
	osgViewer::Viewer view;
	//osgEarth::Util::EarthManipulator *pManipulator = new osgEarth::Util::EarthManipulator();
	//osgGA::TrackballManipulator* pManipulator = new osgGA::TrackballManipulator;
	//osg::Node* pNode = osgDB::readNodeFile("D:\\osgEarth\\globe\\globe.ive");
	osg::Group* root = new osg::Group();
	root->addChild(osgDB::readNodeFile("d:\\cad\\010-A-1177 - Rev_0.dwg"));
	root->addChild(osgDB::readNodeFile("d:\\cad\\010-A-1178 - Rev_0.dwg"));
	root->addChild(osgDB::readNodeFile("d:\\cad\\010-A-1179 - Rev_0.dwg"));
	view.setSceneData(root);
	view.addEventHandler(new osgViewer::StatsHandler());
	view.addEventHandler(new osgViewer::WindowSizeHandler());
	//view.setCameraManipulator(pManipulator);
	view.run();
	return 0;
}

