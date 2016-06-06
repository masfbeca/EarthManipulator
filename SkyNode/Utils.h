
#ifndef OSGEARTH_UTILS_H
#define OSGEARTH_UTILS_H 1

#include "windows.h"
#include <osg/Vec3f>
#include <osg/AutoTransform>
#include <osgGA/GUIEventHandler>
#include <osgViewer/View>
#include <osgUtil/CullVisitor>
#include <osgUtil/RenderBin>
#include "osg/Group"
#include <string>
#include <list>
#include <map>

namespace osg
{
    class EllipsoidModel;
}

namespace osgEarth
{    
	template<typename T> struct TraverseFunctor {
		T* _target;
		TraverseFunctor(T* target) : _target(target) { }
		void operator()(osg::NodeVisitor& nv) { _target->T::traverse(nv); }
	};

	// utility: node that traverses another node via a functor
	template<typename T>
	struct TraverseNode : public osg::Node {
		TraverseFunctor<T> _tf;
		TraverseNode(TraverseFunctor<T>& tf) : _tf(tf) { }
		TraverseNode(T* target) : _tf(TraverseFunctor<T>(target)) { }
		void traverse(osg::NodeVisitor& nv) { _tf(nv); }
		osg::BoundingSphere computeBound() const { return _tf._target->getBound(); }
	};

	// utility: cull callback that traverses another node
	struct TraverseNodeCallback : public osg::NodeCallback {
		osg::ref_ptr<osg::Node> _node;
		TraverseNodeCallback(osg::Node* node) : _node(node) { }
		void operator()(osg::Node* node, osg::NodeVisitor* nv) {
			_node->accept(*nv);
			traverse(node, nv);
		}
	};
}

#endif // OSGEARTH_UTILS_H
