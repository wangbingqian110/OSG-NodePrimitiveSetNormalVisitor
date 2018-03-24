#pragma once
#include <osgViewer/Viewer>

#include <osg/Node>
#include <osg/Geode>
#include <osg/Group>

#include <osg/TriangleFunctor>

#include <iostream>

using namespace std;
using namespace osg;

/// �����ڵ��Ի������������ķ�����
class NodePrimitiveSetNormalVisitor :public osg::NodeVisitor {
public:
	osg::Vec3Array* extracted_normal; // �洢��ȡ�ķ�����
	osg::Group* groupNormalLine = new osg::Group(); // �洢�������ڵ㣬����չʾ
	int count = 1;
	NodePrimitiveSetNormalVisitor();
	//����apply����
	void apply(osg::Geode& geode);
};

/// ������������
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
