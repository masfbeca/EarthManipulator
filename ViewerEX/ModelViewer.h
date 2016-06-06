#pragma once

#include "string"
#include "EarthManipulator.h"
#include "SkyNode.h"
#include <osg/MatrixTransform>
#include <osgEarth/Viewpoint>
#include <osgViewer/Viewer>
#include <osgGA/TrackballManipulator>
#include <osgGA/CameraManipulator>
using namespace std;
class ModelViewer 
{
public:
    ModelViewer(void);
    virtual ~ModelViewer(void);

	virtual bool     Initialize(void *hWnd, unsigned nWndWidth, unsigned nWndHeight);
    virtual void     UnInitialize(void);
	virtual void     ClearModel(void);
	virtual void     UpdateModel(const std::string ID);
	virtual void     DoFrame(void);
	virtual void     ResetCamera(void);

	void setScene(char* fileName);
	void setManipulator(char* manipulatorName);
	void setMouseSensitivity(double sens);
	void setKeyboardSensitivity(double sens);
	void setSrollSensitivity(double sens);
	void setViewpoint(float x, float y, float z);
	void setSpeedFactor(float fac);
	void setRotateFactor(float fac);
	void setDateTime(float hour);
protected:
    osg::ref_ptr<osg::Group>                  m_pSceneRootNode;
	osg::ref_ptr<osgViewer::Viewer>           m_pViewer;
	osg::ref_ptr<osgGA::CameraManipulator>    m_pManipulator;
	osgEarth::Util::SkyNode*                  m_skynode;
	osg::Node*                                m_globe;
	osg::Node*                                m_zzu;
	//osg::ref_ptr<IModelBase>					m_pModelBase;
};