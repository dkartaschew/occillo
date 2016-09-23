/*
   Copyright (C) 2014 Darran Kartaschew

   This file is part of Occillo.

    Occillo is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Occillo is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Occillo.  If not, see <http://www.gnu.org/licenses/>.  */

#include "BrickDestructionAnimation.h"

BrickDestructionAnimation::BrickDestructionAnimation(Texture *texture, int x, int y, uint32_t animationTime, uint32_t gravity, bool small) :
texture(texture), x(x), y(y), animationTime(animationTime), gravity(gravity), small(small) {
    this->startTime = SDL_GetTicks();
    for (int i = 0; i < 16; i++) {
        double j = (i < 8) ? 2 : 5;
        deltas[3 * i] = ((double) std::rand() / (double) RAND_MAX); // x velocity
        deltas[3 * i + 1] = ((double) std::rand() / (double) RAND_MAX) + 1; // y velocity
        deltas[3 * i + 2] = (((double) std::rand() / (double) RAND_MAX) * j) + j; // gravity
    }
}

BrickDestructionAnimation::~BrickDestructionAnimation() {
    // texture is owned by original brick, so don't destroy.
}

bool BrickDestructionAnimation::isComplete() {
    return ((SDL_GetTicks() - startTime) > animationTime);
}

bool BrickDestructionAnimation::reset() {
    this->startTime = SDL_GetTicks();
    return true;
}

void BrickDestructionAnimation::render(SDL_Renderer* renderer) {
    float timeDelta = (float) (SDL_GetTicks() - startTime);
    float alpha = 255.0f - (timeDelta / (float) animationTime * 255.0f);
    texture->setAlpha((Uint8) alpha);
    if (small) {
        texture->render(renderer, x, (y + (timeDelta / 1000.0f * gravity)));
    } else {

        int tWidth = texture->getWidth();
        int tHeight = texture->getHeight();
        
        // fragment height/width.
        int th2 = tHeight / 2;
        int tw8 = tWidth / 8;

        // As duration increases reduce width of fragment.
        float subWidth = (float) tw8 - (timeDelta / (float) animationTime * (float) tw8);

        // Determine locations of fragments.
        Point p0 = getPoint(timeDelta, x, y, -tWidth * deltas[0], -tHeight * deltas[1], gravity * deltas[2]);
        Point p1 = getPoint(timeDelta, x + tw8, y, -tWidth * deltas[3], -tHeight * deltas[4], gravity * deltas[5]);
        Point p2 = getPoint(timeDelta, x + tw8 * 2, y, tWidth * deltas[6], -tHeight * deltas[7], gravity * deltas[8]);
        Point p3 = getPoint(timeDelta, x + tw8 * 3, y, -tWidth * deltas[9], -tHeight * deltas[10], gravity * deltas[11]);
        Point p4 = getPoint(timeDelta, x + tw8 * 4, y, tWidth * deltas[12], -tHeight * deltas[13], gravity * deltas[14]);
        Point p5 = getPoint(timeDelta, x + tw8 * 5, y, -tWidth * deltas[15], -tHeight * deltas[16], gravity * deltas[17]);
        Point p6 = getPoint(timeDelta, x + tw8 * 6, y, tWidth * deltas[18], -tHeight * deltas[19], gravity * deltas[20]);
        Point p7 = getPoint(timeDelta, x + tw8 * 7, y, tWidth * deltas[21], -tHeight * deltas[22], gravity * deltas[23]);

        Point p8 = getPoint(timeDelta, x, y + th2, -tWidth * deltas[24], -tHeight * deltas[25], gravity * deltas[26]);
        Point p9 = getPoint(timeDelta, x + tw8, y + th2, -tWidth * deltas[27], -tHeight * deltas[28], gravity * deltas[29]);
        Point p10 = getPoint(timeDelta, x + tw8 * 2, y + th2, -tWidth * deltas[30], -tHeight * deltas[31], gravity * deltas[32]);
        Point p11 = getPoint(timeDelta, x + tw8 * 3, y + th2, tWidth * deltas[33], -tHeight * deltas[34], gravity * deltas[35]);
        Point p12 = getPoint(timeDelta, x + tw8 * 4, y + th2, -tWidth * deltas[36], -tHeight * deltas[37], gravity * deltas[38]);
        Point p13 = getPoint(timeDelta, x + tw8 * 5, y + th2, tWidth * deltas[39], -tHeight * deltas[40], gravity * deltas[41]);
        Point p14 = getPoint(timeDelta, x + tw8 * 6, y + th2, tWidth * deltas[42], -tHeight * deltas[43], gravity * deltas[44]);
        Point p15 = getPoint(timeDelta, x + tw8 * 7, y + th2, tWidth * deltas[45], -tHeight * deltas[46], gravity * deltas[47]);

        // Render fragments
        texture->render(renderer, p0.x, p0.y, 0, subWidth, 0, th2);
        texture->render(renderer, p1.x, p1.y, tw8, subWidth, 0, th2);
        texture->render(renderer, p2.x, p2.y, tw8 * 2, subWidth, 0, th2);
        texture->render(renderer, p3.x, p3.y, tw8 * 3, subWidth, 0, th2);
        texture->render(renderer, p4.x, p4.y, tw8 * 4, subWidth, 0, th2);
        texture->render(renderer, p5.x, p5.y, tw8 * 5, subWidth, 0, th2);
        texture->render(renderer, p6.x, p6.y, tw8 * 6, subWidth, 0, th2);
        texture->render(renderer, p7.x, p7.y, tw8 * 7, subWidth, 0, th2);

        texture->render(renderer, p8.x, p8.y, 0, subWidth, th2, th2);
        texture->render(renderer, p9.x, p9.y, tw8, subWidth, th2, th2);
        texture->render(renderer, p10.x, p10.y, tw8 * 2, subWidth, th2, th2);
        texture->render(renderer, p11.x, p11.y, tw8 * 3, subWidth, th2, th2);
        texture->render(renderer, p12.x, p12.y, tw8 * 4, subWidth, th2, th2);
        texture->render(renderer, p13.x, p13.y, tw8 * 5, subWidth, th2, th2);
        texture->render(renderer, p14.x, p14.y, tw8 * 6, subWidth, th2, th2);
        texture->render(renderer, p15.x, p15.y, tw8 * 7, subWidth, th2, th2);
    }
    texture->setAlpha((Uint8) 255);
}

Point BrickDestructionAnimation::getPoint(float timeDelta, int x, int y, double gravityX, double velocityY, double gravityY) {
    double tm = timeDelta / 1000.0f;
    double sx = x + (tm * gravityX); // x displacement is simple
    double sy = y + (velocityY * tm) + (0.5 * gravityY * tm * tm); // y displacement = v t + 1/2 a t^2
    return Point(sx, sy);
}