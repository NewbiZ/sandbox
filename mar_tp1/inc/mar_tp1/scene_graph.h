/**
 * \file
 * \brief Declaration of the scene graph related classes: SceneGraph, Node, Light and Geometry
 */

#ifndef SCENE_GRAPH
#define SCENE_GRAPH

#include "mar_tp1/color.h"
#include "mar_tp1/model.h"
#include "mar_tp1/transform.h"

namespace mar
{
  class Node;
  class Geometry;
  class Light;
  
  /**
   * \brief The class is a node of a scene graph. It contains geometry, light, transform,
   *        and possibly children.
   * \see SceneGraph
   */
  class Node
  {
  public:
    /**
     * \brief Type of the children container.
     */
    typedef std::vector<Node*> node_list;
    
    /**
     * \brief Type of an iterator to the children container.
     */
    typedef node_list::iterator node_iterator;
    
    /**
     * \brief Type of a constant iterator to the children container.
     */
    typedef node_list::const_iterator const_node_iterator;
    
    /**
     * \brief Type of the geometry container.
     */
    typedef std::vector<Geometry*> geometry_list;
    
    /**
     * \brief Type of an iterator to the geometry container.
     */
    typedef geometry_list::iterator geometry_iterator;
    
    /**
     * \brief Type of a constant iterator to the geometry container.
     */
    typedef geometry_list::const_iterator const_geometry_iterator;
    
    /**
     * \brief Type of the light container.
     */
    typedef std::vector<Light*> light_list;
    
    /**
     * \brief Type of an iterator to the light container.
     */
    typedef light_list::iterator light_iterator;
    
    /**
     * \brief Type of a constant iterator to the light container.
     */
    typedef light_list::const_iterator const_light_iterator;
    
  public:
    /**
     * \brief Create a node.
     */
    Node();
    
    /**
     * \brief Destruct a node.
     */
    ~Node();
    
  public:
    /**
     * \brief Render a node.
     */
    void render() const;
    
  public: // transformation
   /**
    * \brief Retrieve the transform of the node.
    */
    Transform& transform();
    
    /**
     * \brief Retrieve the transform of the node.
     */
    const Transform& transform() const;
    
  public:
    /**
     * \brief Add a children node to the current node.
     */
    Node& operator<<( Node* );
    
    /**
     * \brief Add a sibling geometry to the current node.
     */
    Node& operator<<( Geometry* );
    
    /**
     * \brief Add a sibling light to the current node.
     */
    Node& operator<<( Light* );
    
  public: // nodes
   /**
    * \brief Iterator the beginning of the children container.
    */
    node_iterator node_begin();
    
    /**
     * \brief Constant iterator the beginning of the children container.
     */
    const_node_iterator node_begin() const;
    
    /**
     * \brief Iterator the end of the children container.
     */
    node_iterator node_end();
    
    /**
     * \brief Constant iterator the end of the children container.
     */
    const_node_iterator node_end() const;
    
    /**
     * \brief Number of children.
     */
    size_t node_size() const;
    
    /**
     * \brief Is it a leaf node?
     */
    bool node_empty() const;
  
  public: // geometry
   /**
    * \brief Iterator the beginning of the geometry container.
    */
    geometry_iterator geometry_begin();
    
    /**
     * \brief Constant iterator the beginning of the geometry container.
     */
    const_geometry_iterator geometry_begin() const;

    /**
     * \brief Iterator the end of the geometry container.
     */
    geometry_iterator geometry_end();
    
    /**
     * \brief Constant iterator the end of the geometry container.
     */
    const_geometry_iterator geometry_end() const;

    /**
     * \brief Number of geometries.
     */
    size_t geometry_size() const;
    
    /**
     * \brief Are there any geometry?
     */
    bool geometry_empty() const;
    
  public: // lights
    /**
     * \brief Iterator the beginning of the light container.
     */
    light_iterator light_begin();
    
    /**
     * \brief Constant iterator the beginning of the light container.
     */
    const_light_iterator light_begin() const;
    
    /**
     * \brief Iterator the end of the light container.
     */
    light_iterator light_end();
    
    /**
     * \brief Constant iterator the end of the light container.
     */
    const_light_iterator light_end() const;
    
    /**
     * \brief Number of lights.
     */
    size_t light_size() const;
    
    /**
     * \brief Are there any lights?
     */
    bool light_empty() const;
    
  private:
    /**
     * \brief The transform of the current node.
     */
    Transform transform_;
    
    /**
     * \brief The children container.
     */
    node_list nodes_;
    
    /**
     * \brief The geometry container.
     */
    geometry_list geometries_;
    
    /**
     * \brief The light container.
     */
    light_list lights_;
  };
  
  /**
   * \brief The class handles a scene graph, using nodes.
   * \see Node
   */
  class SceneGraph
  {
  public:
    /**
     * \brief Construct a SceneGraph.
     */
    SceneGraph();
    
    /**
     * \brief Destruct a SceneGraph.
     */
    ~SceneGraph();
    
  public:
    /**
     * \brief Retrieve the root node of the scene graph.
     */
    Node& root();
    
    /**
     * \brief Retrieve the root node of the scene graph.
     */
    const Node& root() const;
    
  public:
    /**
     * \brief Render the scene graph, depth-first-traversal from the root calling render on Nodes.
     */
    void render() const;
    
  private:
    /**
     * \brief The root node of the scene graph.
     */
    Node root_;
  };
  
  /**
   * \brief This class handles geometry. At the moment, this is just a wrapper for
   *        the Model class.
   */
  class Geometry
  {
  public:
    /**
     * \brief Construct a Geometry with an attached model.
     * \param m Model to be attached. (Default to 0)
     */
    Geometry( Model* m=0 );
    
  public:
    /**
     * \brief Render the geometry (calling render on the model).
     */
    void render() const;
    
  public:
    /**
     * \brief Retrieve the model.
     */
    const Model* model() const;
    
    /**
     * \brief Retrieve the model.
     */
    void model( Model* model );
    
  private:
    /**
     * \brief Model of the Geometry.
     */
    Model* model_;
  };
  
  /**
   * \brief This class represents a simple OpenGL light with ambient, diffuse and specular components.
   */
  class Light
  {
  public:
    /**
     * \brief Create a Light with its components.
     * \param a Ambient color of the light. Default to White.
     * \param d Diffuse color of the light. Default to White.
     * \param s Specular color of the light. Default to White.
     */
    Light( Color a=Color::White, Color d=Color::White, Color s=Color::White );
    
  public:
    /**
     * \brief Render the light. (Updating components)
     */
    void render() const;
    
  public:
    /**
     * \brief Retrieve the ambient component of the light.
     */
    Color& ambient();
    
    /**
     * \brief Retrieve the diffuse component of the light.
     */
    Color& diffuse ();
    
    /**
     * \brief Retrieve the specular component of the light.
     */
    Color& specular();
    
    /**
     * \brief Retrieve the ambient component of the light.
     */
    const Color& ambient() const;
    
    /**
     * \brief Retrieve the diffuse component of the light.
     */
    const Color& diffuse() const;
    
    /**
     * \brief Retrieve the specular component of the light.
     */
    const Color& specular() const;
    
  private:
    /**
     * \brief Ambient component of the light.
     */
    Color ambient_;
    
    /**
     * \brief Diffuse component of the light.
     */
    Color diffuse_;
    
    /**
     * \brief Specular component of the light.
     */
    Color specular_;
  };
}

#endif // SCENE_GRAPH
