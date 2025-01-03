#include <mutex>
#include "inputs.hpp"
#include "mouse.hpp"
std::mutex fuck;

uintptr_t NetConnection = 0x520; // https://dumpspace.spuckwaffel.com/Games/?hash=6b77eceb&type=classes&idx=APlayerController&member=NetConnection
uintptr_t RotationInput = NetConnection + 0x8; // no change, this is just the sizeof netconnection ^ get it from there


bool write_memory(fvector new_rotation)
{
    write<fvector>(Globals.PlayerController + RotationInput, new_rotation);
    return true;
}

class mouse
{
public:

    inline void move(fvector2d Head2D) {
        float x = Head2D.x; float y = Head2D.y;
        float AimSpeed = g_smooth;

        fvector2d ScreenCenter = { (double)sw / 2 , (double)sh / 2 };
        fvector2d Target;


        if (x != 0)
        {
            if (x > ScreenCenter.x)
            {
                Target.x = -(ScreenCenter.x - x);
                Target.x /= AimSpeed;
                if (Target.x + ScreenCenter.x > ScreenCenter.x * 2) Target.x = 0;
            }

            if (x < ScreenCenter.x)
            {
                Target.x = x - ScreenCenter.x;
                Target.x /= AimSpeed;
                if (Target.x + ScreenCenter.x < 0) Target.x = 0;
            }
        }
        if (y != 0)
        {
            if (y > ScreenCenter.y)
            {
                Target.y = -(ScreenCenter.y - y);
                Target.y /= AimSpeed;
                if (Target.y + ScreenCenter.y > ScreenCenter.y * 2) Target.y = 0;
            }

            if (y < ScreenCenter.y)
            {
                Target.y = y - ScreenCenter.y;
                Target.y /= AimSpeed;
                if (Target.y + ScreenCenter.y < 0) Target.y = 0;
            }
        }
        if ((int)Target.x != 0 or (int)Target.y != 0);

        if (g_memory && g_aimbot) {
            write_memory(fvector(-Target.y / 5, Target.x / 5, 0));
        }
        else if (g_aimbot && !g_memory) {
            //Driver::MoveMouse((int)Target.x, (int)Target.y);
            SonyDriverHelper::api::MouseMove(static_cast<int>(Target.x), static_cast<int>(Target.y));
           // input->MoveMouse(static_cast<int>(Target.x), static_cast<int>(Target.y));
        }
        else if (!g_aimbot && g_memory) {
            write_memory(fvector(-Target.y / 5, Target.x / 5, 0));
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

};  static mouse* input = new mouse();

fvector PredictLocation(fvector target, fvector targetVelocity, float projectileSpeed, float projectileGravityScale, float distance)
{
    float horizontalTime = distance / projectileSpeed;
    float verticalTime = distance / projectileSpeed;

    target.x += targetVelocity.x * horizontalTime;
    target.y += targetVelocity.y * horizontalTime;
    target.z += targetVelocity.z * verticalTime +
        abs(-980 * projectileGravityScale) * 0.5f * (verticalTime * verticalTime);

    return target;
}


