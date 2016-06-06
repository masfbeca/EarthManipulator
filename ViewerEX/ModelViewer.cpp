
#include "ModelViewer.h"
#include "FPSManipulator.h"
#include <osgDB/ReadFile>
#include <osgDB/Registry>
#include <osgViewer/api/Win32/GraphicsWindowWin32>
#include <osgViewer/ViewerEventHandlers>


ModelViewer::ModelViewer(void)
{
	m_pViewer = new osgViewer::Viewer;
}

ModelViewer::~ModelViewer(void)
{
	UnInitialize();
}

bool ModelViewer::Initialize(void *hWnd, unsigned nWndWidth, unsigned nWndHeight)
{
    if(hWnd == NULL) return false;
	//初始化窗口
	osg::ref_ptr<osg::GraphicsContext::Traits> pTraits = new osg::GraphicsContext::Traits;
    pTraits->x                             = 0;
    pTraits->y                             = 0;
    pTraits->width                         = osg::clampAbove(nWndWidth, 8u);
    pTraits->height                        = osg::clampAbove(nWndWidth, 8u);
    pTraits->windowDecoration              = false;
    pTraits->doubleBuffer                  = true;
    pTraits->sharedContext                 = 0;
    pTraits->setInheritedWindowPixelFormat = true;
    pTraits->stencil                       = 0u;
	pTraits->inheritedWindowData           = new osgViewer::GraphicsWindowWin32::WindowData((HWND)hWnd);

	//初始化相机
	osg::Camera *pCamera = m_pViewer->getCamera();
	osg::GraphicsContext *pGraphicsContext = osg::GraphicsContext::createGraphicsContext(pTraits.get());   
    pGraphicsContext->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	pCamera->setGraphicsContext(pGraphicsContext);
    pCamera->setViewport(pTraits->x, pTraits->y, pTraits->width, pTraits->height);
	pCamera->setProjectionMatrixAsPerspective(45.0, pTraits->width/(double)pTraits->height, 0.1, 100000);

	//初始化场景数据
	m_pSceneRootNode = new osg::Group;
	

    //初始化漫游器
	m_pManipulator = new osgEarth::Util::EarthManipulator();
	m_pViewer->setCameraManipulator(m_pManipulator.get());

	m_pViewer->setSceneData(m_pSceneRootNode);
	m_pViewer->addEventHandler(new osgViewer::StatsHandler);
    m_pViewer->realize();

    return true;
}

void ModelViewer::UnInitialize(void)
{
	osgDB::Registry::instance()->setReadFileCallback(NULL);
}

void ModelViewer::ClearModel(void)
{
	unsigned int nNum = m_pSceneRootNode->getNumChildren();
    for(int i = nNum - 1; i >= 0; i--)
	{
		m_pSceneRootNode->removeChild(i);
	}
}

void ModelViewer::UpdateModel(const std::string ID)
{
	//ClearModel();

	osg::ref_ptr<osg::Node> pNode = osgDB::readNodeFile(ID);
	m_pSceneRootNode->addChild(pNode.get());

	ResetCamera();
}

void ModelViewer::DoFrame(void)
{
	if(m_pViewer.valid())
	{
		m_pViewer->frame();
	}
}


void ModelViewer::ResetCamera(void)
{
	osgGA::CameraManipulator* manipulator = m_pViewer->getCameraManipulator();
	m_pViewer->getCameraManipulator()->home(0.0);
}

void ModelViewer::setScene(char* fileName)
{
	ClearModel();
	string name = fileName;
	osg::Node* node = osgDB::readNodeFile(name);
	m_skynode = new osgEarth::Util::SkyNode();
	m_skynode->setDateTime(2013,6,2,12);
	m_skynode->attach(m_pViewer);
	m_pSceneRootNode->addChild(m_skynode);
	m_pSceneRootNode->addChild(node);
	m_pViewer->getCameraManipulator()->home(0.0);
}

void ModelViewer::setManipulator(char* manipulatorName)
{
	ClearModel();
	osgGA::CameraManipulator* manipulator;
	string name = manipulatorName;
	if (name=="EarthManipulator")
	{
		manipulator = new osgEarth::Util::EarthManipulator();
	}
	else if (name=="FPSManipulator")
	{
		manipulator = new FPSManipulator();
	}
	else
	{

	}
	
	m_pViewer->setCameraManipulator(manipulator);
	
}

void ModelViewer::setMouseSensitivity(double sens)
{
	string name = m_pViewer->getCameraManipulator()->className();
	if ( name== "EarthManipulator")
	{
		osgEarth::Util::EarthManipulator* manipulator = dynamic_cast<osgEarth::Util::EarthManipulator*> (m_pViewer->getCameraManipulator());

		manipulator->getSettings()->setMouseSensitivity(sens);
	}
}
void ModelViewer::setDateTime(float hour)
{
	string name = m_pViewer->getCameraManipulator()->className();
	if ( name== "EarthManipulator")
	{
		osgEarth::Util::EarthManipulator* manipulator = dynamic_cast<osgEarth::Util::EarthManipulator*> (m_pViewer->getCameraManipulator());
		osgEarth::Util::SkyNode* skynode = dynamic_cast<osgEarth::Util::SkyNode*>(m_pSceneRootNode->getChild(0));
		skynode->setDateTime(2013,6,3,hour);
	}
}
void ModelViewer::setSpeedFactor(float fac)
{
	string name = m_pViewer->getCameraManipulator()->className();
	if ( name== "FPSManipulator")
	{
		FPSManipulator* manipulator = dynamic_cast<FPSManipulator*> (m_pViewer->getCameraManipulator());

		manipulator->setSpeedFactor(fac);
	}
}
void ModelViewer::setRotateFactor(float fac)
{
	string name = m_pViewer->getCameraManipulator()->className();
	if ( name== "FPSManipulator")
	{
		FPSManipulator* manipulator = dynamic_cast<FPSManipulator*> (m_pViewer->getCameraManipulator());

		manipulator->setRotateSpeed(fac);
	}
}

void ModelViewer::setViewpoint(float x, float y, float z)
{
	osgEarth::Util::EarthManipulator* manipulator = dynamic_cast<osgEarth::Util::EarthManipulator*> (m_pViewer->getCameraManipulator());
	osgEarth::Util::Viewpoint point = manipulator->getViewpoint();
	manipulator->setViewpoint(osgEarth::Util::Viewpoint( x, y, z, 0.0, -90.0, 2e6),4.0);
}