#include "DragonBall.h"


DragonBall::DragonBall(QPointF pos,QPointF Tower_c,qreal attack_range)
    :Projectile(pos,Tower_c,attack_range)
{
    src=":/img/asset/2.png";
    type=4;
    damage=50;
}
