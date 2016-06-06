#include "stdafx.h"
#include "Radial.h"
#include <assert.h>

///////////////////////////////////////////////////////////////////////////////
Radial3::Radial3(void)
	   : m_ptOrigin(0.0f, 0.0f, 0.0f),
	     m_vecDirection(0.0f, 0.0f, 0.0f)
{
}


Radial3::Radial3(const osg::Vec3 &ptOrigin, const osg::Vec3 &vecDirection)
{
	m_ptOrigin = ptOrigin;
	m_vecDirection = vecDirection;
	const osg::Vec3::value_type fltLen = m_vecDirection.normalize();
	assert(fltLen > FLT_EPSILON);
}


Radial3::~Radial3(void)
{
}


void Radial3::setOrigin(const osg::Vec3 &ptOrigin)
{
	m_ptOrigin = ptOrigin;
}


const osg::Vec3 &Radial3::getOrigin(void) const
{
	return m_ptOrigin;
}


void Radial3::setDirection(const osg::Vec3 &vecDir)
{
	m_vecDirection = vecDir;
	const osg::Vec3::value_type fltLen = m_vecDirection.normalize();
	assert(fltLen > FLT_EPSILON);
}


const osg::Vec3 &Radial3::getDirection(void) const
{
	return m_vecDirection;
}


osg::Vec3 Radial3::getPoint(osg::Vec3::value_type t) const 
{ 
	return osg::Vec3(m_ptOrigin + (m_vecDirection * t));
}


osg::Vec3 Radial3::operator*(osg::Vec3::value_type t) const
{ 
	return getPoint(t);
}

osg::Vec3::value_type Radial3::IntersectPlane(const osg::Plane &plane) const
{
	const osg::Vec3 vecPlaneNormal = plane.getNormal();
	const osg::Vec3::value_type fltCosTheta = vecPlaneNormal * m_vecDirection;
	if ((fabs(fltCosTheta - 0.0f) < FLT_EPSILON))
	{
		return FLT_MAX;
	}

	const osg::Vec3::value_type fltDistance = plane.distance(m_ptOrigin);
	const osg::Vec3::value_type fltRatio = -(fltDistance / fltCosTheta);
	return fltRatio;
}
