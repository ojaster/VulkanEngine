//
//  physics.cpp
//  FIrst_Project
//
//  Created by Данил on 4.9.2021.
//  Copyright © 2021 Daniil. All rights reserved.
//

#include "physics.hpp"
Rect::Rect(std::vector<Vertex> &vertices){
    pos.x = vertices[0].pos.x;
    pos.y = vertices[0].pos.y;
    size.x = vertices[0].pos.x - vertices[1].pos.x;
    size.y = vertices[0].pos.y - vertices[2].pos.y;
    if(size.x < 0){
        size.x = -size.x;
    }
    if(size.y < 0){
        size.y = -size.y;
    }
}
Rect::Rect(std::vector<Vertex2> &vertices){
    set(vertices);
}
Rect::Rect(){}

void Rect::set(std::vector<Vertex2> &vertices){
    pos.x = vertices[0].pos.x;
    pos.y = vertices[0].pos.y;
    size.x = vertices[0].pos.x - vertices[1].pos.x;
    size.y = vertices[0].pos.y - vertices[2].pos.y;
    if(size.x < 0){
        size.x = -size.x;
    }
    if(size.y < 0){
        size.y = -size.y;
    }
    vel = {0,0};
}

bool pointVsRect(const glm::vec2 &point,const Rect &r){
    return (point.x >= r.pos.x && point.y <= r.pos.y && point.x <= r.pos.x + r.size.x && point.y >= r.pos.y - r.size.y);
}

bool rectCol(Rect &rect1, Rect &rect2){
    if(rect1.pos.x < rect2.pos.x + rect2.size.x && // left
       rect1.pos.x + rect1.size.x > rect2.pos.x && // right
       rect1.pos.y > rect2.pos.y - rect2.size.y &&// top
       rect1.pos.y - rect1.size.y < rect2.pos.y){// bot
        return true;
    }
    return false;
}
bool rectColTopLeftMinusMinus(Rect &rect1, Rect &rect2){
    if(rect1.pos.x < rect2.pos.x + rect2.size.x && // left
       rect1.pos.x + rect1.size.x > rect2.pos.x && // right
       rect1.pos.y < rect2.pos.y + rect2.size.y &&// top
       rect1.pos.y + rect1.size.y > rect2.pos.y){// bot
        return true;
    }
    return false;
}
void colResolve(Rect &wallRect, Rect &player,bool &notRight, bool &right, bool &down, bool &notDown,float &up, float &left,float offsetX, float offsetY){
        if(notRight && (pointVsRect({player.pos.x-0.01,player.pos.y-0.01}, wallRect) || pointVsRect({player.pos.x-0.01,player.pos.y-player.size.y+0.01},wallRect))){
            left = wallRect.pos.x+offsetX + wallRect.size.x;
//            notRight = false;
//            return;
        }
        if(right && (pointVsRect({player.pos.x+player.size.x+0.01,player.pos.y-0.01}, wallRect) || pointVsRect({player.pos.x+player.size.x+0.01,player.pos.y-player.size.y+0.01},wallRect))){
            left = wallRect.pos.x+offsetX-player.size.x;
//            right = false;
//            return;
        }
        if(down && (pointVsRect({player.pos.x+0.01,player.pos.y-player.size.y -0.01}, wallRect) || pointVsRect({player.pos.x+player.size.x-0.01,player.pos.y-player.size.y-0.01},wallRect))){
            up = wallRect.pos.y+offsetY+player.size.y;
//            down = false;
//            return;
        }
        if(notDown && (pointVsRect({player.pos.x+0.01,player.pos.y+0.01}, wallRect) || pointVsRect({player.pos.x+player.size.x-0.01,player.pos.y+0.01},wallRect))){
            up = wallRect.pos.y+offsetY-wallRect.size.y;
//            notDown = false;
//            return;
        }
}
void colResolve(Rect &wallRect, Rect &player,float &up, float &left,float offsetX, float offsetY){
    bool x = false;
    bool y = false;
        if(player.vel.x < 0 && (pointVsRect({player.pos.x-0.01,player.pos.y-0.01}, wallRect) || pointVsRect({player.pos.x-0.01,player.pos.y-player.size.y+0.01},wallRect))){
            left = wallRect.pos.x+offsetX + wallRect.size.x + 0.001;
//            return;
            if(x){
                return;
            }
            x = true;
        }
        if(player.vel.x > 0 && (pointVsRect({player.pos.x+player.size.x+0.01,player.pos.y-0.01}, wallRect) || pointVsRect({player.pos.x+player.size.x+0.01,player.pos.y-player.size.y+0.01},wallRect))){
            if(x){
                return;
            }
            x = true;
            left = wallRect.pos.x+offsetX-player.size.x - 0.001;
//            return;
        }
        if(player.vel.y < 0 && (pointVsRect({player.pos.x+0.01,player.pos.y-player.size.y -0.01}, wallRect) || pointVsRect({player.pos.x+player.size.x-0.01,player.pos.y-player.size.y-0.01},wallRect))){
            if(y){
                return;
            }
            y = true;
            up = wallRect.pos.y+offsetY+player.size.y;
//            return;
        }
        if(player.vel.y > 0 && (pointVsRect({player.pos.x+0.01,player.pos.y+0.01}, wallRect) || pointVsRect({player.pos.x+player.size.x-0.01,player.pos.y+0.01},wallRect))){
            if(y){
                return;
            }
            y = true;
            up = wallRect.pos.y+offsetY-wallRect.size.y ;
//            return;
        }
}
void minerResolve(Rect &wallRect, Rect &player,bool &notRight, bool &right, bool &down, bool &notDown,float &up, float &left,float offsetX, float offsetY){
        if(notRight){
            left = wallRect.pos.x+offsetX + wallRect.size.x;
            notRight = false;
            return;
        }
        if(right){
            left = wallRect.pos.x+offsetX-player.size.x;
            right = false;
            return;
        }
        if(down){
            up = wallRect.pos.y+offsetY+player.size.y;
            down = false;
            return;
        }
        if(notDown){
            up = wallRect.pos.y+offsetY-wallRect.size.y;
            notDown = false;
            return;
        }
}

float SweptAABB(Rect &b1, Rect b2, float& normalx, float& normaly, float& vx, float & vy){
    float xInvEntry, yInvEntry;
    float xInvExit, yInvExit;

    // find the distance between the objects on the near and far sides for both x and y
    if (vx > 0.0f)
    {
      xInvEntry = b2.pos.x - (b1.pos.x + b1.size.x);
      xInvExit = (b2.pos.x + b2.size.x) - b1.pos.x;
    }
    else
    {
      xInvEntry = (b2.pos.x + b2.size.x) - b1.pos.x;
      xInvExit = b2.pos.x - (b1.pos.x + b1.size.x);
    }

    if (vy > 0.0f)
    {
      yInvEntry = b2.pos.y - (b1.pos.y + b1.size.y);
      yInvExit = (b2.pos.y + b2.size.y) - b1.pos.y;
    }
    else
    {
      yInvEntry = (b2.pos.y + b2.size.y) - b1.pos.y;
      yInvExit = b2.pos.y - (b1.pos.y + b1.size.y);
    }
    
    // find time of collision and time of leaving for each axis (if statement is to prevent divide by zero)
    float xEntry, yEntry;
    float xExit, yExit;

    if (vx == 0.0f)
    {
        xEntry = -UINT32_MAX;
        xExit = UINT32_MAX;
    }
    else
    {
      xEntry = xInvEntry / vx;
      xExit = xInvExit / vx;
    }

    if (vy == 0.0f)
    {
        yEntry = -UINT32_MAX;
        yExit = UINT32_MAX;
    }
    else
    {
      yEntry = yInvEntry / vy;
      yExit = yInvExit / vy;
    }
    
    
    // find the earliest/latest times of collisionfloat
    float entryTime = std::max(xEntry, yEntry);
    float exitTime = std::min(xExit, yExit);
    
    // if there was no collision
    if (entryTime > exitTime || xEntry < 0.0f && yEntry < 0.0f || xEntry > 1.0f || yEntry > 1.0f)
    {
      normalx = 0.0f;
      normaly = 0.0f;
      return 1.0f;
    }else // if there was a collision
    {
      // calculate normal of collided surface
      if (xEntry > yEntry)
      {
        if (xInvEntry < 0.0f)
        {
          normalx = 1.0f;
          normaly = 0.0f;
        }
        else
        {
          normalx = -1.0f;
          normaly = 0.0f;
        }
      }
      else
      {
        if (yInvEntry < 0.0f)
        {
          normalx = 0.0f;
          normaly = 1.0f;
        }
        else
        {
          normalx = 0.0f;
          normaly = -1.0f;
        }
      } // return the time of collision
        return entryTime;
    }
    
}

bool DynamicRectVsRect(const Rect& r_dynamic, const float fTimeStep, const Rect & r_static,
                       glm::vec2& contact_point, glm::vec2& contact_normal, float& contact_time){
    // Check if dynamic rectangle is actually moving - we assume rectangles are NOT in collision to start
    if (r_dynamic.vel.x == 0 && r_dynamic.vel.y == 0)
        return false;

    // Expand target rectangle by source dimensions
    Rect expanded_target;
    expanded_target.pos = r_static.pos - r_dynamic.size / 2.f;
    expanded_target.size = r_static.size + r_dynamic.size;

    glm::vec2 ray_origin = r_dynamic.pos + r_dynamic.size / 2.f;
    glm::vec2 ray_dir = r_dynamic.vel * fTimeStep;
    if (RayVsRect(ray_origin, ray_dir, expanded_target, contact_point, contact_normal, contact_time))
        return (contact_time < 1.0f);
    else
        return false;
}



bool ResolveDynamicRectVsRect(Rect& r_dynamic, const float fTimeStep, Rect& r_static){
    glm::vec2 contact_point, contact_normal;
    float contact_time = 0.0f;
    if (DynamicRectVsRect(r_dynamic, fTimeStep, r_static, contact_point, contact_normal, contact_time))
    {
        if (contact_normal.y > 0) r_dynamic.contact[0] = &r_static; else nullptr;
        if (contact_normal.x < 0) r_dynamic.contact[1] = &r_static; else nullptr;
        if (contact_normal.y < 0) r_dynamic.contact[2] = &r_static; else nullptr;
        if (contact_normal.x > 0) r_dynamic.contact[3] = &r_static; else nullptr;

        r_dynamic.vel += contact_normal * glm::vec2(std::abs(r_dynamic.vel.x), std::abs(r_dynamic.vel.y)) * (1 - contact_time);
        return true;
    }

    return false;
}
bool RayVsRect(const glm::vec2 ray_origin, const glm::vec2 ray_dir, const Rect target, glm::vec2& contact_point, glm::vec2& contact_normal, float& t_hit_near){
            contact_normal = { 0,0 };
            contact_point = { 0,0 };

            // Cache division
            glm::vec2 invdir = 1.0f / ray_dir;

            // Calculate intersections with rectangle bounding axes
            glm::vec2 t_near = (target.pos - ray_origin) * invdir;
            glm::vec2 t_far = (target.pos + target.size - ray_origin) * invdir;

            if (std::isnan(t_far.y) || std::isnan(t_far.x)) return false;
            if (std::isnan(t_near.y) || std::isnan(t_near.x)) return false;

            // Sort distances
            if (t_near.x > t_far.x) std::swap(t_near.x, t_far.x);
            if (t_near.y > t_far.y) std::swap(t_near.y, t_far.y);

            // Early rejection
            if (t_near.x > t_far.y || t_near.y > t_far.x) return false;

            // Closest 'time' will be the first contact
            t_hit_near = std::max(t_near.x, t_near.y);

            // Furthest 'time' is contact on opposite side of target
            float t_hit_far = std::min(t_far.x, t_far.y);

            // Reject if ray direction is pointing away from object
            if (t_hit_far < 0)
                return false;

            // Contact point of collision from parametric line equation
            contact_point = ray_origin + t_hit_near * ray_dir;

            if (t_near.x > t_near.y)
                if (invdir.x < 0)
                    contact_normal = { 1, 0 };
                else
                    contact_normal = { -1, 0 };
            else if (t_near.x < t_near.y)
                if (invdir.y < 0)
                    contact_normal = { 0, 1 };
                else
                    contact_normal = { 0, -1 };

            // Note if t_near == t_far, collision is principly in a diagonal
            // so pointless to resolve. By returning a CN={0,0} even though its
            // considered a hit, the resolver wont change anything.
            return true;
        }
