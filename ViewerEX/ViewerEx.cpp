#include "ViewerEx.h"

namespace ViewerExs{
	ViewerEx::ViewerEx(void)
	{
		m_pModelViewer = new ModelViewer();	
	}
	ViewerEx::~ViewerEx(void)
	{
		delete m_pModelViewer;
	}

	void ViewerEx::init(System::IntPtr hWnd, unsigned nWndWidth, unsigned nWndHeight)
	{
		m_pModelViewer->Initialize((void*)hWnd, nWndWidth, nWndHeight);
	}

	void ViewerEx::doFrame()
	{
		m_pModelViewer->DoFrame();
	}

	void ViewerEx::setScene(System::String^ fileName)
	{
		char* name = (char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(fileName); 
		m_pModelViewer->setScene(name);
	}
	
	void ViewerEx::setManipulator(System::String^ manipulatorName)
	{
		m_pModelViewer->setManipulator((char*)(void*)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(manipulatorName)); 
	}

	void ViewerEx::resetCamera()
	{
		m_pModelViewer->ResetCamera();
	}

	void ViewerEx::setMouseSensitivity(double sense)
	{
		m_pModelViewer->setMouseSensitivity(sense);
	}

	void ViewerEx::setViewpoint(float x, float y, float z)
	{
		m_pModelViewer->setViewpoint(x, y, z);
	}

	void ViewerEx::clearModel()
	{
		m_pModelViewer->ClearModel();
	}

	void ViewerEx::setSpeedFactor(float fac)
	{
		m_pModelViewer->setSpeedFactor(fac);
	}

	void ViewerEx::setRotateSpeed(float fac)
	{
		m_pModelViewer->setRotateFactor(fac);
	}

	void ViewerEx::setDateTime(float hour)
	{
		m_pModelViewer->setDateTime(hour);
	}
}