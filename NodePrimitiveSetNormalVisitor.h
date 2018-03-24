#pragma once
#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osg/TriangleFunctor>

#include <iostream>

using namespace std;
using namespace osg;

/// 遍历节点以获得所有三角面的法向量
class NodePrimitiveSetNormalVisitor :public osg::NodeVisitor {
public:
	osg::Vec3Array* extracted_normal; // 存储提取的法向量
	osg::Group* groupNormalLine = new osg::Group(); // 存储法向量节点，用于展示
	int count = 1;
	NodePrimitiveSetNormalVisitor();
	//重载apply方法
	void apply(osg::Geode& geode);
};

/// 三角网访问器
class TriangleVisitor {
public:
	static void TriangleNormal(osg::Geometry& geom, osg::Vec3Array*& normals, osg::Group*& group);
};

/// TriangleNormalFunctor
struct TriangleNormalFunctor
{

	osg::Vec3Array* _normalBase;
	osg::Group* _group;

	void set(int noVertices, osg::Vec3Array *nb, osg::Group*& group);
	inline void operator() (const osg::Vec3 &v1, const osg::Vec3 &v2, const osg::Vec3 &v3, bool treatVertexDataAsTemporary);
};
