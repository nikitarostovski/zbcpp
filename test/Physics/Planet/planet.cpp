//#include "planet.hpp"
//#include "CDT.h"
//#include "clipper.h"
//#include "polygon.hpp"
//
//using namespace PolygonUtils;
//
//Planet::Planet(PhysicsWorld *world, float x, float y, float radius, float gravity_radius_)
//{
//    gravity_radius = gravity_radius_;
//    gravity_force = 0.01;
//    this->world = world;
//    center = b2Vec2(x, y);
//    
//    Polygon contour = Polygon::makeCircle(center, radius, 9, MaterialType::red, false);
//    
//    auto chunks = contour.split(50.0f);
//    
////    std::vector<Polygon> chunks;
////    chunks.push_back(contour);
//    
//    std::map<MaterialType, std::vector<Polygon>> chunksByMaterial;
//    std::vector<MaterialType> allMaterials = {MaterialType::red, MaterialType::yellow};
//    
//    for (auto &chunk : chunks) {
//        float dist = (center - chunk.center).Length();
//        
//        if (dist <= radius * 0.75) {
//            chunk.isDynamic = false;
//            chunk.material = MaterialType::yellow;
//        } else {
//            chunk.isDynamic = false;
//            chunk.material = MaterialType::red;
//        }
//        chunksByMaterial[chunk.material.type].push_back(chunk);
//    }
//    
//    for (auto m : allMaterials) {
//        for (auto chunk: chunksByMaterial[m]) {
//            SolidBlockDef chunkDef{chunk, chunk.isDynamic};
////            SolidBlock block{chunkDef};
//        }
////        auto c = Polygon::combine(chunksByMaterial[m]);
////        for (auto chunk : c) {
////            auto chunks = chunk.triangulate();//.splitIntoSubpolygons();
////            for (auto cc : chunks) {
////                cc.isDynamic = true;//chunk.isDynamic;
////                SolidBlockDef chunkDef{cc, false};
////                m_world->instantinateBlock(chunkDef);
////            }
////        }
//    }
//}
