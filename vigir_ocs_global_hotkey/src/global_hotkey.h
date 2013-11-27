#include <ros/ros.h>

#include <flor_ocs_msgs/OCSKeyEvent.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

namespace vigir_ocs
{
    class GlobalHotkey
    {
      public:
        GlobalHotkey();
        ~GlobalHotkey();
        
        void onUpdate();

        void calculateCursorPosition(int &x, int &y);

        void publishKeyPressed(int k, int x, int y);
        void publishKeyReleased(int k, int x, int y);

        bool isActive();

      private:
        ros::NodeHandle n_;
        
        ros::Publisher key_event_pub_;

        Display *display_name_;
        
        bool old_keys_[4096];
        bool registering_keystrokes_;
    };
}