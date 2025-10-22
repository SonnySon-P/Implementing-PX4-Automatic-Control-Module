#include <px4_platform_common/log.h>
#include <uORB/uORB.h>
#include <uORB/topics/vehicle_command.h>
#include <uORB/topics/offboard_control_mode.h>
#include <uORB/topics/trajectory_setpoint.h>
#include <drivers/drv_hrt.h>
#include <px4_platform_common/px4_config.h>
#include <px4_platform_common/tasks.h>
#include <px4_platform_common/posix.h>
#include <unistd.h>
#include <math.h>

// 解鎖無人機
void armVehicle(void) {
    // 發送解鎖命令的提示訊息
    PX4_INFO("Arming vehicle...");

    struct vehicle_command_s command = {};
    command.timestamp = hrt_absolute_time();
    command.command = 400;  // ARM的命令編號是400 (MAV_CMD_COMPONENT_ARM_DISARM)
    command.param1 = 1;     // 1代表解鎖
    command.target_system = 1;
    command.target_component = 1;
    command.source_system = 1;
    command.source_component = 1;
    command.from_external = true;

    // 發送命令
    orb_advert_t command_pub = orb_advertise(ORB_ID(vehicle_command), &command);
    if (command_pub == NULL) {
        // 無法發布vehicle_command主題的錯誤訊息
        PX4_ERR("Failed to advertise vehicle_command topic");
        return;
    }

    // 解鎖命令已發出的提示訊息
    PX4_INFO("Vehicle armed");
}

// 上鎖無人機
void disarmVehicle(void) {
    // 發送上鎖命令的提示訊息
    PX4_INFO("Disarming vehicle...");

    struct vehicle_command_s command = {};
    command.timestamp = hrt_absolute_time();
    command.command = 400;  // ARM的命令編號是400 (MAV_CMD_COMPONENT_ARM_DISARM)
    command.param1 = 0;     // 0代表上鎖 (disarm)
    command.target_system = 1;
    command.target_component = 1;
    command.source_system = 1;
    command.source_component = 1;
    command.from_external = true;

    // 發送命令
    orb_advert_t command_pub = orb_advertise(ORB_ID(vehicle_command), &command);
    if (command_pub == NULL) {
        // 無法發布vehicle_command主題的錯誤訊息
        PX4_ERR("Failed to advertise vehicle_command topic");
        return;
    }
    
    // 上鎖命令已發出的提示訊息
    PX4_INFO("Vehicle disarmed");
}

// 發送Offboard setpoint
void publishOffboardSetpoints(float x, float y, float z) {
    // uORB發佈器
    static orb_advert_t offboard_publish = NULL;
    static orb_advert_t trajectory_publish = NULL;

    // 建立Offboard控制模式結構
    struct offboard_control_mode_s ocm = {};
    
    // 建立trajectory setpoint結構
    struct trajectory_setpoint_s ts = {};

    // 設定Offboard控制模式的時間戳
    ocm.timestamp = hrt_absolute_time();
    
    // 啟用位置控制，表示我要用位置setpoint來控制
    ocm.position = true;

    // 設定軌跡setpoint的時間戳
    ts.timestamp = hrt_absolute_time();
    
    // 指定目標位置 (NED座標：x前後、y左右、z向下為正；要上升要給負值)
    ts.position[0] = x;  // X座標（前/後）
    ts.position[1] = y;  // Y座標（左/右）
    ts.position[2] = z;  // Z座標（高度，要上升要給負值）
    ts.yaw = NAN;        // 航向設定為NAN，表示保持目前航向不變

    // 發佈Offboard控制模式ocm
    if (offboard_publish == NULL) {
        // 第一次發佈需要advertise以建立publisher
        offboard_publish = orb_advertise(ORB_ID(offboard_control_mode), &ocm);
    } else {
        // 之後使用orb_publish持續更新資料
        orb_publish(ORB_ID(offboard_control_mode), offboard_publish, &ocm);
    }

    // 發佈 Trajectory Setpoint ts
    if (trajectory_publish == NULL) {
        // 第一次發佈需要advertise以建立publisher
        trajectory_publish = orb_advertise(ORB_ID(trajectory_setpoint), &ts);
    } else {
        // 之後使用orb_publish持續更新資料
        orb_publish(ORB_ID(trajectory_setpoint), trajectory_publish, &ts);
    }
}

// 切換到Offboard模式
void setOffboardMode(void)
{
    // 發出Offboard模式命令的提示訊息
    PX4_INFO("Switching to Offboard mode...");
    
    // 建構一個Offboard模式切換的命令
    struct vehicle_command_s command = {};
    command.timestamp = hrt_absolute_time();
    command.command = 176;   // VEHICLE_CMD_DO_SET_MODE的命令編號是176
    command.param1 = 1;      // base mode (MAV_MODE_FLAG) 這裡給1占位即可
    command.param2 = 6;      // PX4_CUSTOM_MAIN_MODE_OFFBOARD = 6，正確切到Offboard模式
    command.target_system = 1;
    command.target_component = 1;
    command.source_system = 1;
    command.source_component = 1;
    command.from_external = true;

    // 發送命令
    orb_advert_t command_publish = orb_advertise(ORB_ID(vehicle_command), &command);
    if (command_publish == NULL) {
        // 無法發布vehicle_command主題的錯誤訊息
        PX4_ERR("Failed to advertise vehicle_command topic");
        return;
    }

    // Offboard模式命令已發出的提示訊息
    PX4_INFO("Offboard mode set");
}

// 主程式
__EXPORT int px4_simple_app_main(int argc, char *argv[])
{
    // 開始程式訊息
    PX4_INFO("Start program");

    // 先解鎖
    armVehicle();
    
    // 等待2sec，確保已解鎖
    px4_usleep(2000000);
    
    // 切換到Offboard模式前，先送一點setpoint（避免PX4因未連續收到setpoint而拒絕進入Offboard）
    for (int i = 0; i < 50; i++) {
        publishOffboardSetpoints(0.0f, 0.0f, -3.0f); // 起飛目標高度3公尺（NED z=-3）
        px4_usleep(50000); // 20Hz
    }

    // 切換到 Offboard 模式（使用PX4自訂主模式=Offboard）
    setOffboardMode();

    // 起飛：持續在(0,0,-3)懸停一段時間，確保穩定
    for (int i = 0; i < 50; i++) {
        publishOffboardSetpoints(0.0f, 0.0f, -3.0f);
        px4_usleep(50000);
    }

    // 向前飛5公尺（保持高度不變）
    for (int i = 0; i < 50; i++) {
        publishOffboardSetpoints(5.0f, 0.0f, -3.0f); // X=5m，Z=-3m
        px4_usleep(50000);
    }

    // 降落到地面（Z=0表示地面；保持X=5m位置）
    for (int i = 0; i < 200; i++) {
        publishOffboardSetpoints(5.0f, 0.0f, 0.0f); // Z=0降落
        px4_usleep(50000);
    }
    
    // 自動上鎖
    disarmVehicle();
    
    // 結束程式訊息
    PX4_INFO("End program");
    
    return 0;   
}
