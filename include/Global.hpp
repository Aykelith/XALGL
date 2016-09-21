#ifndef GLOBAL_HPP
#define GLOBAL_HPP

typedef unsigned int uint;

/** For all the entities with the same model draw the same mesh to all entities
  * Example:
  *     meshes = model.meshesCount()
  *     for (mesh : meshes) {
  *         for (entity : entities) {
  *             entity.draw(mesh)
  *         }
  *     }
  * Bad:
  *     - I need to set the position for every mesh
  */
// #define EXPERIMENTAL_DRAW_SAME_MESH

#define MODEL_CONSOLE_INFO
#ifdef MODEL_CONSOLE_INFO
    //#define MODEL_CONSOLE_MESH_INFO
    //#define MODEL_CONSOLE_TEX_INFO
    #define MODEL_CONSOLE_MORE_INFO
#endif
#define LIGHT_BOXES
#define SHADER_OUTPUT
// #define NDEBUG

#define GLM_FORCE_CXX11 

#include <cassert>

#endif