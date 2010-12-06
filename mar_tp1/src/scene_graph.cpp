#include "mar_tp1/scene_graph.h"

namespace mar
{
  //===========================================================================
  // NODE
  //===========================================================================
  Node::Node()
  {
  }
  
  Node::~Node()
  {
  }
  
  Transform& Node::transform()
  {
    return transform_;
  }
  
  const Transform& Node::transform() const
  {
    return transform_;
  }
  
  Node::node_iterator Node::node_begin()
  {
    return nodes_.begin();
  }
  
  Node::const_node_iterator Node::node_begin() const
  {
    return nodes_.begin();
  }
  
  Node::node_iterator Node::node_end()
  {
    return nodes_.end();
  }
  
  Node::const_node_iterator Node::node_end() const
  {
    return nodes_.end();
  }
  
  size_t Node::node_size() const
  {
    return nodes_.size();
  }
  
  bool Node::node_empty() const
  {
    return nodes_.empty();
  }
  
  Node::geometry_iterator Node::geometry_begin()
  {
    return geometries_.begin();
  }
  
  Node::const_geometry_iterator Node::geometry_begin() const
  {
    return geometries_.begin();
  }
  
  Node::geometry_iterator Node::geometry_end()
  {
    return geometries_.end();
  }
  
  Node::const_geometry_iterator Node::geometry_end() const
  {
    return geometries_.end();
  }
  
  size_t Node::geometry_size() const
  {
    return geometries_.size();
  }
  
  bool Node::geometry_empty() const
  {
    return geometries_.empty();
  }
  
  Node::light_iterator Node::light_begin()
  {
    return lights_.begin();
  }
  
  Node::const_light_iterator Node::light_begin() const
  {
    return lights_.begin();
  }
  
  Node::light_iterator Node::light_end()
  {
    return lights_.end();
  }
  
  Node::const_light_iterator Node::light_end() const
  {
    return lights_.end();
  }
  
  size_t Node::light_size() const
  {
    return lights_.size();
  }
  
  bool Node::light_empty() const
  {
    return lights_.empty();
  }
  
  //===========================================================================
  // SCENE GRAPH
  //===========================================================================
  SceneGraph::SceneGraph()
  {
  }
  
  SceneGraph::~SceneGraph()
  {
  }
  
  //===========================================================================
  // GEOMETRY
  //===========================================================================
  Geometry::Geometry( Model* m )
    : model_(m)
  {
  }
  
  const Model* Geometry::model() const
  {
    return model_;
  }
  
  void Geometry::model( Model* model )
  {
    model_ = model;
  }
  
  //===========================================================================
  // LIGHT
  //===========================================================================
  Light::Light( Color a, Color d, Color s )
    : ambient_(a), diffuse_(d), specular_(s)
  {
  }
  
  Color& Light::ambient()
  {
    return ambient_;
  }
  
  const Color& Light::ambient() const
  {
    return ambient_;
  }
  
  Color& Light::diffuse()
  {
    return diffuse_;
  }
  
  const Color& Light::diffuse() const
  {
    return diffuse_;
  }
  
  Color& Light::specular()
  {
    return specular_;
  }
  
  const Color& Light::specular() const
  {
    return specular_;
  }
}
