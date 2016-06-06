
#include "ModelViewer.h"
#include <string>
namespace ViewerExs{
public ref class ViewerEx
	{
	public:
		ViewerEx(void);
		~ViewerEx(void);

		void init(System::IntPtr hWnd, unsigned nWndWidth, unsigned nWndHeight);
		void doFrame();
		void resetCamera();
		void clearModel();
		void setScene(System::String^ fileName);
		void setManipulator(System::String^ manipulatorName);
		void setMouseSensitivity(double sense);
		void setViewpoint(float x, float y, float z);
		void setSpeedFactor(float fac);
		void setRotateSpeed(float fac);
		void setDateTime(float hour);
	private: 
		ModelViewer* m_pModelViewer;
	};
}