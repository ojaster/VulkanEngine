//
//  physics.hpp
//  FIrst_Project
//
//  Created by Данил on 4.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#ifndef physics_hpp
#define physics_hpp

#include <stdio.h>
#include "box2d.h"
#include "b2_polygon_shape.h"
#ifndef helperFunctions_hpp
    #include "helperFunctions.hpp"
#endif
struct Rect{
    glm::vec2 pos;
    glm::vec2 size;
    glm::vec2 vel;
    std::array<Rect*, 4> contact;
    
    Rect(std::vector<Vertex> &vertices);
    Rect(std::vector<Vertex2> &vertices);
    Rect();
    void set(std::vector<Vertex2> &vertices);
};
bool pointVsRect(const glm::vec2 &point,const Rect &r);
bool rectCol(Rect &rect1, Rect &rect2);
void colResolve(Rect &wallRect, Rect &player,bool &notRight, bool &right, bool &down, bool &notDown,float &up, float &left,float offsetX, float offsetY);
void colResolve(Rect &wallRect, Rect &player,float &up, float &left,float offsetX, float offsetY);
void minerResolve(Rect &wallRect, Rect &player,bool &notRight, bool &right, bool &down, bool &notDown,float &up, float &left,float offsetX, float offsetY);

float SweptAABB(Rect &b1, Rect b2, float& normalx, float& normaly, float& vx, float & vy);



bool DynamicRectVsRect(const Rect& r_dynamic, const float fTimeStep, const Rect & r_static,
                       glm::vec2& contact_point, glm::vec2& contact_normal, float& contact_time);
bool ResolveDynamicRectVsRect(Rect& r_dynamic, const float fTimeStep, Rect& r_static);
bool RayVsRect(const glm::vec2 ray_origin, const glm::vec2 ray_dir, const Rect target, glm::vec2& contact_point, glm::vec2& contact_normal, float& t_hit_near);
bool rectColTopLeftMinusMinus(Rect &rect1, Rect &rect2);

#endif /* physics_hpp */
