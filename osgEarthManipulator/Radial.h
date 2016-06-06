#pragma once
#include <osg/Vec3>
#include <osg/Plane>

	class Radial3
	{
	public:
		Radial3(void);
		Radial3(const osg::Vec3 &ptOrigin, const osg::Vec3 &vecDirection);
		virtual ~Radial3(void);

		void setOrigin(const osg::Vec3 &ptOrigin);
		const osg::Vec3 &getOrigin(void) const;

		void setDirection(const osg::Vec3 &vecDir);
		const osg::Vec3 &getDirection(void) const;

		osg::Vec3 getPoint(osg::Vec3::value_type t) const;
		osg::Vec3 operator*(osg::Vec3::value_type t) const;

		osg::Vec3::value_type IntersectPlane(const osg::Plane &plane) const;

	protected:
		osg::Vec3	m_ptOrigin;
		osg::Vec3	m_vecDirection;
	};



