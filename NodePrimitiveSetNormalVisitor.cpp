#include "NodePrimitiveSetNormalVisitor.h"

NodePrimitiveSetNormalVisitor::NodePrimitiveSetNormalVisitor() :osg::NodeVisitor(osg::NodeVisitor::TRAVERSE_ACTIVE_CHILDREN)
{
	extracted_normal = new osg::Vec3Array();
}

void NodePrimitiveSetNormalVisitor::apply(osg::Geode & geode)
{
	{
		std::cout << count++ << std::endl;
		for (unsigned int i = 0; i < geode.getNumDrawables(); ++i)
		{
			osg::Geometry* geom = dynamic_cast<osg::Geometry*>(geode.getDrawable(i));
			if (!geom)
			{
				std::cout << "几何体错误！" << std::endl;
				continue;
			}
			//得到定点数组
			//osg::Vec3Array* verts = dynamic_cast<osg::Vec3Array*>(geom->getVertexArray());

			osg::Vec3Array *normals;
			TriangleVisitor::TriangleNormal(*geom, normals, groupNormalLine);//自动生成法线
																			
			if (!normals) {
				std::cout << "法向量获取错误" << std::endl;
				continue;
			}
			//添加到extracted_verts
			extracted_normal->insert(extracted_normal->end(), normals->begin(), normals->end());
		}
	}
}

void TriangleVisitor::TriangleNormal(osg::Geometry & geom, osg::Vec3Array *& normals, osg::Group *& group)
{
	{
		Geometry::PrimitiveSetList& primitives = geom.getPrimitiveSetList();
		Geometry::PrimitiveSetList::iterator itr;
		unsigned int numSurfacePrimitives = 0;
		for (itr = primitives.begin();
			itr != primitives.end();
			++itr)
		{
			switch ((*itr)->getMode())
			{
			case(PrimitiveSet::TRIANGLES):
			case(PrimitiveSet::TRIANGLE_STRIP):
			case(PrimitiveSet::TRIANGLE_FAN):
			case(PrimitiveSet::QUADS):
			case(PrimitiveSet::QUAD_STRIP):
			case(PrimitiveSet::POLYGON):
				++numSurfacePrimitives;
				break;
			default:
				break;
			}
		}

		if (!numSurfacePrimitives) return;

		osg::Vec3Array *coords = dynamic_cast<osg::Vec3Array*>(geom.getVertexArray());
		if (!coords || !coords->size()) return;

		osg::Vec3Array *_normals = new osg::Vec3Array();

		osg::Vec3Array::iterator nitr;

		TriangleFunctor<TriangleNormalFunctor> stf;
		stf.set(coords->size(), _normals, group);

		geom.accept(stf);

		for (nitr = _normals->begin();
			nitr != _normals->end();
			++nitr)
		{
			nitr->normalize();
		}
		normals = _normals;
	}
}

void TriangleNormalFunctor::set(int noVertices, osg::Vec3Array * nb, osg::Group *& group)
{
	{
		_normalBase = nb;
		_group = group;
	}

}

inline void TriangleNormalFunctor::operator()(const osg::Vec3 & v1, const osg::Vec3 & v2, const osg::Vec3 & v3, bool treatVertexDataAsTemporary)
{
	{
		if (!treatVertexDataAsTemporary)
		{
			// calc orientation of triangle.
			osg::Vec3 normal = (v2 - v1) ^ (v3 - v1);
			_normalBase->push_back(normal);

			// 绘制线段
			osg::Geode* geodeLine = new osg::Geode();
			osg::Geometry* geometryLine = new osg::Geometry();
			osg::Vec3Array * coordsLine = new osg::Vec3Array();
			//三角面的重心的空间坐标
			osg::Vec3 center((v1[0] + v2[0] + v3[0]) / 3, (v1[1] + v2[1] + v3[1]) / 3, (v1[2] + v2[2] + v3[2]) / 3);
			coordsLine->push_back(center);
			coordsLine->push_back(normal + center);
			geometryLine->setVertexArray(coordsLine);
			geometryLine->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES, 0, 2));
			geodeLine->addDrawable(geometryLine);
			_group->addChild(geodeLine);
		}

	}
}
